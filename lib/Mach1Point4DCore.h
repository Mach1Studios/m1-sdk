//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#pragma once

#include <vector>
#include <cmath>
#include <chrono>
#include <string>
#include <algorithm>

#include "Mach1Point4D.h"

#ifndef DEG_TO_RAD
#define DEG_TO_RAD (PI/180.0)
#endif

#ifndef PI
#define PI       3.14159265358979323846f
#endif 

class Mach1Point4DCore {
public:
	float x, y, z, w;

	Mach1Point4DCore();
	Mach1Point4DCore(float X, float Y, float Z, float W);
	Mach1Point4DCore(const Mach1Point4D& pnt);

	Mach1Point4DCore operator+(const Mach1Point4DCore& pnt) const;
	Mach1Point4DCore operator*(const float f) const;
	Mach1Point4DCore operator*(const Mach1Point4DCore& vec) const;
	Mach1Point4DCore operator-(const Mach1Point4DCore& vec) const;
	float length() const;
	float operator[] (int index);
};
