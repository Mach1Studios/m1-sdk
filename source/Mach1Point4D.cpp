//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#include "Mach1Point4D.h"

#ifdef __cplusplus
Mach1Point4D::Mach1Point4D() {
    x = 0;
    y = 0;
    z = 0;
    w = 0;
}

Mach1Point4D::Mach1Point4D(float X, float Y, float Z, float W) {
    x = X;
    y = Y;
    z = Z;
    w = W;
}

Mach1Point4D Mach1Point4D::operator+(const Mach1Point4D &pnt) const {
    return Mach1Point4D(x + pnt.x, y + pnt.y, z + pnt.z, w + pnt.w);
}

Mach1Point4D Mach1Point4D::operator*(const float f) const {
    return Mach1Point4D(x * f, y * f, z * f, w * f);
}

Mach1Point4D Mach1Point4D::operator*(const Mach1Point4D &vec) const {
    return Mach1Point4D(x * vec.x, y * vec.y, z * vec.z, w * vec.w);
}

Mach1Point4D Mach1Point4D::operator-(const Mach1Point4D &vec) const {
    return Mach1Point4D(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
}

float Mach1Point4D::length() const {
    return (float)sqrtf(x * x + y * y + z * z + w * w);
}

float Mach1Point4D::operator[](int index) {
    float arr[4] = {x, y, z, w};
    return arr[index];
}
#endif
