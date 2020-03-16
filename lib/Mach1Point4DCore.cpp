//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#include "Mach1Point4DCore.h"

Mach1Point4DCore::Mach1Point4DCore() {
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}

Mach1Point4DCore::Mach1Point4DCore(float X, float Y, float Z, float W) {
	x = X;
	y = Y;
	z = Z;
	w = W;
}
 
Mach1Point4DCore Mach1Point4DCore::operator+(const Mach1Point4DCore& pnt) const {
	return Mach1Point4DCore(x + pnt.x, y + pnt.y, z + pnt.z, w + pnt.w);
}

Mach1Point4DCore Mach1Point4DCore::operator*(const float f) const {
	return Mach1Point4DCore(x*f, y*f, z*f, w*f);
}


Mach1Point4DCore Mach1Point4DCore::operator*(const Mach1Point4DCore& vec) const {
	return Mach1Point4DCore(x*vec.x, y*vec.y, z*vec.z, w*vec.w);
}


Mach1Point4DCore Mach1Point4DCore::operator-(const Mach1Point4DCore& vec) const {
	return Mach1Point4DCore(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
}

float Mach1Point4DCore::length() const {
	return (float)sqrt(x*x + y*y + z*z + w*w);
}

float Mach1Point4DCore::operator[] (int index) {
	float arr[4] = { x, y, z, w };
	return arr[index];
}