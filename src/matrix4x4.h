#pragma once
#ifndef ZBUFFER_MATRIX4X4_H
#define ZBUFFER_MATRIX4X4_H

#include "zbuffer.h"
#include "geometry.h"

class Matrix4x4 {
public:
	Matrix4x4(){}
	
	Matrix4x4(float mat[4][4]) {
		memcpy(m, mat, 16*sizeof(float));
	}

	Matrix4x4(float t00, float t01, float t02, float t03,
                     float t10, float t11, float t12, float t13,
                     float t20, float t21, float t22, float t23,
                     float t30, float t31, float t32, float t33) {
		m[0][0] = t00; m[0][1] = t01; m[0][2] = t02; m[0][3] = t03;
		m[1][0] = t10; m[1][1] = t11; m[1][2] = t12; m[1][3] = t13;
		m[2][0] = t20; m[2][1] = t21; m[2][2] = t22; m[2][3] = t23;
		m[3][0] = t30; m[3][1] = t31; m[3][2] = t32; m[3][3] = t33;
	}

	 Matrix4x4 operator*(const Matrix4x4 &m2) const{
        Matrix4x4 r;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                r.m[i][j] = m[i][0] * m2.m[0][j] +
                            m[i][1] * m2.m[1][j] +
                            m[i][2] * m2.m[2][j] +
                            m[i][3] * m2.m[3][j];
        return r;
    }

	inline Vector operator()(const Vector &v) const {
	    float x = v.x, y = v.y, z = v.z;
		float xp = m[0][0]*x + m[0][1]*y + m[0][2]*z + m[0][3];
		float yp = m[1][0]*x + m[1][1]*y + m[1][2]*z + m[1][3];
		float zp = m[2][0]*x + m[2][1]*y + m[2][2]*z + m[2][3];
		float wp = m[3][0]*x + m[3][1]*y + m[3][2]*z + m[3][3];

		if (wp == 1.f) return Vector(xp, yp, zp);
		else          return Vector(xp, yp, zp)/wp;
	}

	float m[4][4];
};

inline Matrix4x4 Inverse(const Matrix4x4 &m) {
    int indxc[4], indxr[4];
    int ipiv[4] = { 0, 0, 0, 0 };
    float minv[4][4];
    memcpy(minv, m.m, 4*4*sizeof(float));
    for (int i = 0; i < 4; i++) {
        int irow = -1, icol = -1;
        float big = 0.;
        // Choose pivot
        for (int j = 0; j < 4; j++) {
            if (ipiv[j] != 1) {
                for (int k = 0; k < 4; k++) {
                    if (ipiv[k] == 0) {
                        if (fabsf(minv[j][k]) >= big) {
                            big = float(fabsf(minv[j][k]));
                            irow = j;
                            icol = k;
                        }
                    }
                    else if (ipiv[k] > 1)
                        fprintf(stderr,"Error: Singular matrix in MatrixInvert");
                }
            }
        }
        ++ipiv[icol];
        // Swap rows _irow_ and _icol_ for pivot
        if (irow != icol) {
            for (int k = 0; k < 4; ++k) {
				float tmp = minv[irow][k];
				minv[irow][k] = minv[icol][k];
				minv[icol][k] = tmp;
			}
        }
        indxr[i] = irow;
        indxc[i] = icol;
        if (minv[icol][icol] == 0.)
			fprintf(stderr,"Error: Singular matrix in MatrixInvert");

        // Set $m[icol][icol]$ to one by scaling row _icol_ appropriately
        float pivinv = 1.f / minv[icol][icol];
        minv[icol][icol] = 1.f;
        for (int j = 0; j < 4; j++)
            minv[icol][j] *= pivinv;

        // Subtract this row from others to zero out their columns
        for (int j = 0; j < 4; j++) {
            if (j != icol) {
                float save = minv[j][icol];
                minv[j][icol] = 0;
                for (int k = 0; k < 4; k++)
                    minv[j][k] -= minv[icol][k]*save;
            }
        }
    }
    // Swap columns to reflect permutation
    for (int j = 3; j >= 0; j--) {
        if (indxr[j] != indxc[j]) {
            for (int k = 0; k < 4; k++) {
				float tmp = minv[k][indxr[j]];
				minv[k][indxr[j]] = minv[k][indxc[j]];
				minv[k][indxc[j]] = tmp;
			}
        }
    }
    return Matrix4x4(minv);
}

inline Matrix4x4 LookAt(const Point &pos, const Point &look, const Vector &up) {
    float m[4][4];
    // Initialize fourth column of viewing matrix
    m[0][3] = pos.x;
    m[1][3] = pos.y;
    m[2][3] = pos.z;
    m[3][3] = 1;

    // Initialize first three columns of viewing matrix
    Vector dir = Normalize(look - pos);
    Vector left = Normalize(Cross(Normalize(up), dir));
    Vector newUp = Cross(dir, left);
    m[0][0] = left.x;
    m[1][0] = left.y;
    m[2][0] = left.z;
    m[3][0] = 0.;
    m[0][1] = newUp.x;
    m[1][1] = newUp.y;
    m[2][1] = newUp.z;
    m[3][1] = 0.;
    m[0][2] = dir.x;
    m[1][2] = dir.y;
    m[2][2] = dir.z;
    m[3][2] = 0.;

    return Inverse(Matrix4x4(m));
}

inline Matrix4x4 Scale(float x, float y, float z) {
    return Matrix4x4(x, 0, 0, 0,
                0, y, 0, 0,
                0, 0, z, 0,
                0, 0, 0, 1);
}

inline Matrix4x4 Rotate(float angle, const Vector &axis) {
    Vector a = Normalize(axis);
    float s = sinf(Radians(angle));
    float c = cosf(Radians(angle));
    float m[4][4];

    m[0][0] = a.x * a.x + (1.f - a.x * a.x) * c;
    m[0][1] = a.x * a.y * (1.f - c) - a.z * s;
    m[0][2] = a.x * a.z * (1.f - c) + a.y * s;
    m[0][3] = 0;

    m[1][0] = a.x * a.y * (1.f - c) + a.z * s;
    m[1][1] = a.y * a.y + (1.f - a.y * a.y) * c;
    m[1][2] = a.y * a.z * (1.f - c) - a.x * s;
    m[1][3] = 0;

    m[2][0] = a.x * a.z * (1.f - c) - a.y * s;
    m[2][1] = a.y * a.z * (1.f - c) + a.x * s;
    m[2][2] = a.z * a.z + (1.f - a.z * a.z) * c;
    m[2][3] = 0;

    m[3][0] = 0;
    m[3][1] = 0;
    m[3][2] = 0;
    m[3][3] = 1;

    
    return Matrix4x4(m);;
}


inline Matrix4x4 Perspective(float fov, float n, float f) {
    // Perform projective divide
    Matrix4x4 persp = Matrix4x4(1, 0,           0,              0,
                                0, 1,           0,              0,
                                0, 0, f / (f - n), -f*n / (f - n),
                                0, 0,           1,              0);

    // Scale to canonical viewing volume
    float invTanAng = 1.f / tanf(Radians(fov) / 2.f);
    return Scale(invTanAng, invTanAng, 1) * persp;
}
#endif