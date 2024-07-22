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

#ifdef __cplusplus
class Mach1Point4D {
  public:
    float x, y, z, w;

    Mach1Point4D();
    Mach1Point4D(float X, float Y, float Z, float W);

    Mach1Point4D operator+(const Mach1Point4D &pnt) const;
    Mach1Point4D operator*(const float f) const;
    Mach1Point4D operator*(const Mach1Point4D &vec) const;
    Mach1Point4D operator-(const Mach1Point4D &vec) const;
    float length() const;
    float operator[](int index);
};
#else
struct Mach1Point4D {
    float x, y, z, w;
};
#endif
