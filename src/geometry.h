#pragma once

#ifndef ZBUFFER_GEOMETRY_H
#define ZBUFFER_GEOMETRY_H

#include "zbuffer.h"

class Vector {
public:
    // Vector Public Methods
    Vector() { x = y = z = 0.f; }
    Vector(float xx, float yy, float zz)
        : x(xx), y(yy), z(zz) {
    }
    explicit Vector(const Point &p);
    Vector(const Vector &v) {
        x = v.x; y = v.y; z = v.z;
    }
    
    Vector &operator=(const Vector &v) {
        x = v.x; y = v.y; z = v.z;
        return *this;
    }

    Vector operator+(const Vector &v) const {
        return Vector(x + v.x, y + v.y, z + v.z);
    }
    
    Vector& operator+=(const Vector &v) {
        x += v.x; y += v.y; z += v.z;
        return *this;
    }
    Vector operator-(const Vector &v) const {
        return Vector(x - v.x, y - v.y, z - v.z);
    }
    
    Vector& operator-=(const Vector &v) {
        x -= v.x; y -= v.y; z -= v.z;
        return *this;
    }
    Vector operator*(float f) const { return Vector(f*x, f*y, f*z); }
    
    Vector &operator*=(float f) {
        x *= f; y *= f; z *= f;
        return *this;
    }
    Vector operator/(float f) const {
        float inv = 1.f / f;
        return Vector(x * inv, y * inv, z * inv);
    }
    
    Vector &operator/=(float f) {
        float inv = 1.f / f;
        x *= inv; y *= inv; z *= inv;
        return *this;
    }
    Vector operator-() const { return Vector(-x, -y, -z); }
    float operator[](int i) const {
        
        return (&x)[i];
    }
    
    float &operator[](int i) {
        
        return (&x)[i];
    }
    float LengthSquared() const { return x*x + y*y + z*z; }
    float Length() const { return sqrtf(LengthSquared()); }

    bool operator==(const Vector &v) const {
        return x == v.x && y == v.y && z == v.z;
    }
    bool operator!=(const Vector &v) const {
        return x != v.x || y != v.y || z != v.z;
    }

    // Vector Public Data
    float x, y, z;
};


class Point {
public:
    // Point Public Methods
    Point() { x = y = z = 0.f; }
    Point(float xx, float yy, float zz)
        : x(xx), y(yy), z(zz) {
    }
    Point(const Point &p) {
        x = p.x; y = p.y; z = p.z;
    }
    
    Point &operator=(const Point &p) {
        x = p.x; y = p.y; z = p.z;
        return *this;
    }

    Point operator+(const Vector &v) const {
        return Point(x + v.x, y + v.y, z + v.z);
    }
    
    Point &operator+=(const Vector &v) {
        x += v.x; y += v.y; z += v.z;
        return *this;
    }
    Vector operator-(const Point &p) const {
        return Vector(x - p.x, y - p.y, z - p.z);
    }
    
    Point operator-(const Vector &v) const {
        return Point(x - v.x, y - v.y, z - v.z);
    }
    
    Point &operator-=(const Vector &v) {
        x -= v.x; y -= v.y; z -= v.z;
        return *this;
    }
    Point &operator+=(const Point &p) {
        x += p.x; y += p.y; z += p.z;
        return *this;
    }
    Point operator+(const Point &p) const {
        return Point(x + p.x, y + p.y, z + p.z);
    }
    Point operator* (float f) const {
        return Point(f*x, f*y, f*z);
    }
    Point &operator*=(float f) {
        x *= f; y *= f; z *= f;
        return *this;
    }
    Point operator/ (float f) const {
        float inv = 1.f/f;
        return Point(inv*x, inv*y, inv*z);
    }
    Point &operator/=(float f) {
        float inv = 1.f/f;
        x *= inv; y *= inv; z *= inv;
        return *this;
    }
    float operator[](int i) const { 
        return (&x)[i];
    }
    
    float &operator[](int i) {        
        return (&x)[i];
    }

    bool operator==(const Point &p) const {
        return x == p.x && y == p.y && z == p.z;
    }
    bool operator!=(const Point &p) const {
        return x != p.x || y != p.y || z != p.z;
    }

    // Point Public Data
    float x, y, z;
};

inline Vector Cross(const Vector &v1, const Vector &v2) {
    float v1x = v1.x, v1y = v1.y, v1z = v1.z;
    float v2x = v2.x, v2y = v2.y, v2z = v2.z;
    return Vector((v1y * v2z) - (v1z * v2y),
                  (v1z * v2x) - (v1x * v2z),
                  (v1x * v2y) - (v1y * v2x));
}

inline Vector Normalize(const Vector &v) { return v / v.Length(); }

inline float AbsDot(const Vector &v1, const Vector &n2) {
    return fabsf(v1.x * n2.x + v1.y * n2.y + v1.z * n2.z);
}

inline float AbsCos(const Vector &v1, const Vector &n2) {
    return AbsDot(Normalize(v1), Normalize(n2));
}

#endif
