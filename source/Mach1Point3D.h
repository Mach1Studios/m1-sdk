//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#pragma once

#include <algorithm>
#include <chrono>
#include <cmath>
#include <string>
#include <vector>

#ifndef DEG_TO_RAD
#    define DEG_TO_RAD (PI / 180.0f)
#endif

#ifndef PI
#    define PI 3.14159265358979323846f
#endif

#ifdef __cplusplus

class Mach1Point3D {
  public:
    float x, y, z;

    Mach1Point3D() {
        x = 0;
        y = 0;
        z = 0;
    }

    Mach1Point3D(float X, float Y, float Z) {
        x = X;
        y = Y;
        z = Z;
    }

    Mach1Point3D(float X, float Y) {
        x = X;
        y = Y;
        z = 0;
    }

    Mach1Point3D operator+(const Mach1Point3D &pnt) const {
        return Mach1Point3D(x + pnt.x, y + pnt.y, z + pnt.z);
    }

    Mach1Point3D operator*(const float f) const {
        return Mach1Point3D(x * f, y * f, z * f);
    }

    Mach1Point3D operator*(const Mach1Point3D &vec) const {
        return Mach1Point3D(x * vec.x, y * vec.y, z * vec.z);
    }

    Mach1Point3D operator-(const Mach1Point3D &vec) const {
        return Mach1Point3D(x - vec.x, y - vec.y, z - vec.z);
    }

    Mach1Point3D operator/(float f) {
        return {this->x / f, this->y / f, this->z / f};
    }

    bool operator==(const Mach1Point3D &p2) {
        return (this->x == p2.x && this->y == p2.y && this->z == p2.z);
    }

    bool operator!=(const Mach1Point3D &p2) {
        return !(*this == p2);
    }

    Mach1Point3D operator-() {
        return {-this->x, -this->y, -this->z};
    }

    float length() const {
        return (float)sqrtf(x * x + y * y + z * z);
    }

    float operator[](int index) {
        float arr[3] = {x, y, z};
        return arr[index];
    }

    Mach1Point3D &rotate(float angle, const Mach1Point3D &axis) {
        Mach1Point3D ax = axis.getNormalized();
        float a = (float)(angle * DEG_TO_RAD);
        float sina = sinf(a);
        float cosa = cosf(a);
        float cosb = 1.0f - cosa;

        float nx = x * (ax.x * ax.x * cosb + cosa) + y * (ax.x * ax.y * cosb - ax.z * sina) + z * (ax.x * ax.z * cosb + ax.y * sina);
        float ny = x * (ax.y * ax.x * cosb + ax.z * sina) + y * (ax.y * ax.y * cosb + cosa) + z * (ax.y * ax.z * cosb - ax.x * sina);
        float nz = x * (ax.z * ax.x * cosb - ax.y * sina) + y * (ax.z * ax.y * cosb + ax.x * sina) + z * (ax.z * ax.z * cosb + cosa);
        x = nx;
        y = ny;
        z = nz;
        return *this;
    }

    Mach1Point3D &normalize() {
        float length = (float)sqrtf(x * x + y * y + z * z);
        if (length > 0) {
            x /= length;
            y /= length;
            z /= length;
        }
        return *this;
    }

    Mach1Point3D getNormalized() const {
        float length = (float)sqrtf(x * x + y * y + z * z);
        if (length > 0) {
            return Mach1Point3D(x / length, y / length, z / length);
        } else {
            return Mach1Point3D();
        }
    }

    Mach1Point3D getRotated(float angle, const Mach1Point3D &axis) const {
        Mach1Point3D ax = axis.getNormalized();
        float a = (float)(angle * DEG_TO_RAD);
        float sina = sinf(a);
        float cosa = cosf(a);
        float cosb = 1.0f - cosa;

        return Mach1Point3D(x * (ax.x * ax.x * cosb + cosa) + y * (ax.x * ax.y * cosb - ax.z * sina) + z * (ax.x * ax.z * cosb + ax.y * sina),
                                x * (ax.y * ax.x * cosb + ax.z * sina) + y * (ax.y * ax.y * cosb + cosa) + z * (ax.y * ax.z * cosb - ax.x * sina),
                                x * (ax.z * ax.x * cosb - ax.y * sina) + y * (ax.z * ax.y * cosb + ax.x * sina) + z * (ax.z * ax.z * cosb + cosa));
    }

    Mach1Point3D getCrossed(const Mach1Point3D &b) const {
        return Mach1Point3D(
            y * b.z - z * b.y,
            z * b.x - x * b.z,
            x * b.y - y * b.x);
    }

    float dot(const Mach1Point3D &vec1, const Mach1Point3D &vec2) {
        return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
    }

    float distance(const Mach1Point3D &vec1, const Mach1Point3D &vec2) {
        return sqrt(powf(vec1.x - vec2.x, 2) + powf(vec1.y - vec2.y, 2) + powf(vec1.z - vec2.z, 2));
    }
};

#else

struct Mach1Point3D {
    float x, y, z;
};

#endif
