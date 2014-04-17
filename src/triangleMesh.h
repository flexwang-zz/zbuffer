#pragma once

#ifndef ZBUFFER_TRIANGLEMESH_H
#define	ZBUFFER_TRIANGLEMESH_H

#include "zbuffer.h"
#include "geometry.h"

class Triangle {
public:
	Triangle(){}
    Triangle(TriangleMesh* m,
             int v0, int v1, int v2,
             int n0=0, int n1=0, int n2=0,
             int t0=0, int t1=0): mesh(m) {
		v[0] = v0;
		v[1] = v1;
		v[2] = v2;

        n[0] = n0;
        n[1] = n1;
        n[2] = n2;

        t[0] = t0;
        t[1] = t1;
	}

	TriangleMesh *mesh;
	int v[3];
    int n[3];
    int t[2];
};

class TriangleMesh {
public:
	TriangleMesh(map<string, vector<string>> &infos) {
        //store the vertex in .obj
        vector<string> vs = infos["v"];

		for (int i=0, size=vs.size(); i<size; i++) {
			istringstream is(vs[i]);
			float x, y, z;
			is >>x >>y >>z;
			v.push_back(Point(x,y,z));
		}

        //store the normals in .obj
        vector<string> ns = infos["vn"];
        for (int i=0, size=ns.size(); i<size; i++) {
            istringstream is(ns[i]);
            float x, y, z;
            is >>x >>y >>z;
            n.push_back(Vector(x,y,z));
        }

		vector<string> fs = infos["f"];
		for (int i=0, size=fs.size(); i<size; i++) {
            string faceinfo = fs[i];
            int countOfSlash = 0;
            for (int j=0, length=faceinfo.length(); j<length; j++) {
                if (faceinfo[j] == '/') {
                    countOfSlash ++;
                    faceinfo[j] = ' ';
                }
            }

            if (countOfSlash == 0) {  //no other info, only vertex index
                int cur_num_of_normals = n.size();
                istringstream is(faceinfo);
                int x, y, z;
                is >>x >>y >>z;
                Vector nn = Normalize(Cross((v[x-1]-v[y-1]), (v[x-1]-v[z-1])));
                n.push_back(nn);
                f.push_back(Triangle(this, x-1, y-1, z-1, cur_num_of_normals,cur_num_of_normals,cur_num_of_normals));
            }
            else if (countOfSlash == 3) { //normals are supplied
                istringstream is(faceinfo);
                int vv[3], nn[3];

                for (int j=0; j<3; j++) {
                    is >> vv[j] >> nn[j];
                }
                f.push_back(Triangle(this, vv[0]-1, vv[1]-1, vv[2]-1, nn[0]-1, nn[1]-1, nn[2]-1));
            }
		}

	}


	vector<Point> v;
    vector<Vector> n;
    vector<Point> t;
	vector<Triangle> f;
};

#endif
