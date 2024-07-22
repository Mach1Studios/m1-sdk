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

    Mach1Point3D();
    Mach1Point3D(float X, float Y, float Z);
    Mach1Point3D(float X, float Y);

    Mach1Point3D operator+(const Mach1Point3D &pnt) const;
    Mach1Point3D operator*(const float f) const;
    Mach1Point3D operator*(const Mach1Point3D &vec) const;
    Mach1Point3D operator-(const Mach1Point3D &vec) const;
    Mach1Point3D operator/(float f);
    Mach1Point3D operator-();
    float length() const;
    float operator[](int index);

    Mach1Point3D &rotate(float angle, const Mach1Point3D &axis);
    Mach1Point3D &normalize();
    Mach1Point3D getNormalized() const;
    Mach1Point3D getRotated(float angle, const Mach1Point3D &axis) const;
    Mach1Point3D getCrossed(const Mach1Point3D &b) const;

    static float dot(const Mach1Point3D &vec1, const Mach1Point3D &vec2);
    static float distance(const Mach1Point3D &vec1, const Mach1Point3D &vec2);
};

bool operator==(const Mach1Point3D &p1, const Mach1Point3D &p2);
bool operator!=(const Mach1Point3D &p1, const Mach1Point3D &p2);
#else
struct Mach1Point3D {
    float x, y, z;
};
#endif
