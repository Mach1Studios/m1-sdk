//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#pragma once

#include <algorithm>
#include <chrono>
#include <cmath>
#include <string>
#include <vector>

#ifndef DEG_TO_RAD
#    define DEG_TO_RAD (PI / 180.0)
#endif

#ifndef PI
#    define PI 3.14159265358979323846f
#endif

struct Mach1Point4D {
    float x, y, z, w;

    Mach1Point4D operator+(const Mach1Point4D &pnt) const {
        struct Mach1Point4D p;
        p.x = this->x + pnt.x;
        p.y = this->y + pnt.y;
        p.z = this->z + pnt.z;
        p.w = this->w + pnt.w;
        return p;
    }

    Mach1Point4D operator*(const float f) const {
        struct Mach1Point4D p;
        p.x = this->x * f;
        p.y = this->y * f;
        p.z = this->z * f;
        p.w = this->w * f;
        return p;
    }

    Mach1Point4D operator*(const Mach1Point4D &pnt) const {
        struct Mach1Point4D p;
        p.x = this->x * pnt.x;
        p.y = this->y * pnt.y;
        p.z = this->z * pnt.z;
        p.w = this->w * pnt.w;
        return p;
    }

    Mach1Point4D operator-(const Mach1Point4D &pnt) const {
        struct Mach1Point4D p;
        p.x = this->x - pnt.x;
        p.y = this->y - pnt.y;
        p.z = this->z - pnt.z;
        p.w = this->w - pnt.w;
        return p;
    }

    float length() const {
        return (float)sqrtf(x * x + y * y + z * z + w * w);
    }

    float operator[](int index) {
        float arr[4] = {x, y, z, w};
        return arr[index];
    }

    Mach1Point4D Mach1Point4D_create() {
        Mach1Point4D p;
        p.x = 0;
        p.y = 0;
        p.z = 0;
        p.w = 0;
        return p;
    }
    Mach1Point4D Mach1Point4D_create(float X, float Y, float Z, float W) {
        Mach1Point4D p;
        p.x = X;
        p.y = Y;
        p.z = Z;
        p.w = W;
        return p;
    }
};
