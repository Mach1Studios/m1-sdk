//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#pragma once

#include <cmath>

#ifndef DEG_TO_RAD
#    define DEG_TO_RAD (PI / 180.0f)
#endif

#ifndef PI
#    define PI 3.14159265358979323846f
#endif

typedef struct Mach1Point3D {
    float x, y, z;
    
    bool operator==(const Mach1Point3D &p2) {
        return (this->x == p2.x && this->y == p2.y && this->z == p2.z);
    }

    bool operator!=(const Mach1Point3D &p2) {
        return !(*this == p2);
    }
    
    float length() const {
        return (float)sqrtf(x * x + y * y + z * z);
    }

    float operator[](int index) {
        float arr[3] = {x, y, z};
        return arr[index];
    }
    
    Mach1Point3D operator+(const Mach1Point3D &pnt) {
        Mach1Point3D p;
        p.x = this->x + pnt.x;
        p.y = this->y + pnt.y;
        p.z = this->z + pnt.z;
        return p;
    }

    Mach1Point3D operator*(const float f) {
        Mach1Point3D p;
        p.x = this->x * f;
        p.y = this->y * f;
        p.z = this->z * f;
        return p;
    }

    Mach1Point3D operator*(const Mach1Point3D &pnt) {
        Mach1Point3D p;
        p.x = this->x * pnt.x;
        p.y = this->y * pnt.y;
        p.z = this->z * pnt.z;
        return p;
    }

    Mach1Point3D operator-(const Mach1Point3D &pnt) {
        Mach1Point3D p;
        p.x = this->x - pnt.x;
        p.y = this->y - pnt.y;
        p.z = this->z - pnt.z;
        return p;
    }

    Mach1Point3D operator/(float f) {
        return {this->x / f, this->y / f, this->z / f};
    }

    Mach1Point3D operator-() {
        return {-this->x, -this->y, -this->z};
    }

    Mach1Point3D rotate(float angle, const Mach1Point3D &axis) {
        Mach1Point3D ax = axis.getNormalized();
        float a = (float)(angle * DEG_TO_RAD);
        float sina = sinf(a);
        float cosa = cosf(a);
        float cosb = 1.0f - cosa;

        Mach1Point3D p;
        float nx = this->x * (ax.x * ax.x * cosb + cosa) + this->y * (ax.x * ax.y * cosb - ax.z * sina) + this->z * (ax.x * ax.z * cosb + ax.y * sina);
        float ny = this->x * (ax.y * ax.x * cosb + ax.z * sina) + this->y * (ax.y * ax.y * cosb + cosa) + this->z * (ax.y * ax.z * cosb - ax.x * sina);
        float nz = this->x * (ax.z * ax.x * cosb - ax.y * sina) + this->y * (ax.z * ax.y * cosb + ax.x * sina) + this->z * (ax.z * ax.z * cosb + cosa);
        p.x = nx;
        p.y = ny;
        p.z = nz;
        return p;
    }

    Mach1Point3D normalize() {
        Mach1Point3D p;
        float length = (float)sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
        if (length > 0) {
            p.x /= length;
            p.y /= length;
            p.z /= length;
        } else {
            // error
        }
        return p;
    }

    Mach1Point3D getNormalized() const {
        float length = (float)sqrtf(x * x + y * y + z * z);
        if (length > 0) {
            Mach1Point3D p;
            p.x = this->x / length;
            p.y = this->y / length;
            p.z = this->z / length;
            return p;
        } else {
            Mach1Point3D p;
            p.x = 0;
            p.y = 0;
            p.z = 0;
            return p;
        }
    }

    Mach1Point3D getRotated(float angle, const Mach1Point3D &axis) const {
        Mach1Point3D ax = axis.getNormalized();
        float a = (float)(angle * DEG_TO_RAD);
        float sina = sinf(a);
        float cosa = cosf(a);
        float cosb = 1.0f - cosa;

        Mach1Point3D p;
        p.x = this->x * (ax.x * ax.x * cosb + cosa) + this->y * (ax.x * ax.y * cosb - ax.z * sina) + this->z * (ax.x * ax.z * cosb + ax.y * sina);
        p.y = this->x * (ax.y * ax.x * cosb + ax.z * sina) + this->y * (ax.y * ax.y * cosb + cosa) + this->z * (ax.y * ax.z * cosb - ax.x * sina);
        p.z = this->x * (ax.z * ax.x * cosb - ax.y * sina) + this->y * (ax.z * ax.y * cosb + ax.x * sina) + this->z * (ax.z * ax.z * cosb + cosa);
        return p;
    }

    Mach1Point3D getCrossed(const Mach1Point3D &b) const {
        Mach1Point3D p;
        p.x = this->y * b.z - this->z * b.y;
        p.y = this->z * b.x - this->x * b.z;
        p.z = this->x * b.y - this->y * b.x;
        return p;
    }

    float dot(const Mach1Point3D &vec1, const Mach1Point3D &vec2) {
        return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
    }

    float distance(const Mach1Point3D &vec1, const Mach1Point3D &vec2) {
        return sqrt(powf(vec1.x - vec2.x, 2) + powf(vec1.y - vec2.y, 2) + powf(vec1.z - vec2.z, 2));
    }
    
    Mach1Point3D Mach1Point3D_create() {
        Mach1Point3D p;
        p.x = 0;
        p.y = 0;
        p.z = 0;
        return p;
    }
    Mach1Point3D Mach1Point3D_create(float X, float Y) {
        Mach1Point3D p;
        p.x = X;
        p.y = Y;
        p.z = 0;
        return p;
    }
    Mach1Point3D Mach1Point3D_create(float X, float Y, float Z) {
        Mach1Point3D p;
        p.x = X;
        p.y = Y;
        p.z = Z;
        return p;
    }
} Mach1Point3D;
