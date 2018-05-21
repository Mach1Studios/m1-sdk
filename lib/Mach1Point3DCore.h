//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.
//
//  Header file

#pragma once

#include <vector>
#include <cmath>
#include <chrono>
#include <string>
#include <algorithm>

#ifndef DEG_TO_RAD
#define DEG_TO_RAD (PI/180.0)
#endif

#ifndef PI
#define PI       3.14159265358979323846
#endif 

struct Mach1Point3DCore {
	float x, y, z;

	Mach1Point3DCore();

	Mach1Point3DCore(float X, float Y, float Z);

	Mach1Point3DCore(float X, float Y);

	Mach1Point3DCore operator+(const Mach1Point3DCore& pnt) const;
	Mach1Point3DCore operator*(const float f) const;
	Mach1Point3DCore operator*(const Mach1Point3DCore& vec) const;
	Mach1Point3DCore operator-(const Mach1Point3DCore& vec) const;
	float length() const;
	float operator[] (int index);

	Mach1Point3DCore& rotate(float angle, const Mach1Point3DCore& axis);
	Mach1Point3DCore& normalize();
	Mach1Point3DCore getNormalized() const;
	Mach1Point3DCore getRotated(float angle, const Mach1Point3DCore& axis) const;
};
