//  Mach1 SDK
//  Copyright © 2017 Mach1. All rights reserved.
//
//  Implementation

/*
DISCLAIMER:
This header file is not an example of use but an decoder that will require periodic
updates and should not be integrated in sections but remain as an update-able factored file.
*/

 
#include "Mach1Point3DCore.h"

Mach1Point3DCore::Mach1Point3DCore() {
	x = 0;
	y = 0;
	z = 0;
}

Mach1Point3DCore::Mach1Point3DCore(float X, float Y, float Z) {
	x = X;
	y = Y;
	z = Z;
}

Mach1Point3DCore::Mach1Point3DCore(float X, float Y) {
	x = X;
	y = Y;
	z = 0;
}

Mach1Point3DCore Mach1Point3DCore::operator+(const Mach1Point3DCore& pnt) const {
	return Mach1Point3DCore(x + pnt.x, y + pnt.y, z + pnt.z);
}


Mach1Point3DCore Mach1Point3DCore::operator*(const float f) const {
	return Mach1Point3DCore(x*f, y*f, z*f);
}


Mach1Point3DCore Mach1Point3DCore::operator*(const Mach1Point3DCore& vec) const {
	return Mach1Point3DCore(x*vec.x, y*vec.y, z*vec.z);
}


Mach1Point3DCore Mach1Point3DCore::operator-(const Mach1Point3DCore& vec) const {
	return Mach1Point3DCore(x - vec.x, y - vec.y, z - vec.z);
}

float Mach1Point3DCore::length() const {
	return (float)sqrt(x*x + y*y + z*z);
}
 

float Mach1Point3DCore::operator[] (int index) {
	float arr[3] = { x, y, z };
	return arr[index];
}

Mach1Point3DCore& Mach1Point3DCore::rotate(float angle, const Mach1Point3DCore& axis) {
	Mach1Point3DCore ax = axis.getNormalized();
	float a = (float)(angle*DEG_TO_RAD);
	float sina = sin(a);
	float cosa = cos(a);
	float cosb = 1.0f - cosa;

	float nx = x*(ax.x*ax.x*cosb + cosa)
		+ y*(ax.x*ax.y*cosb - ax.z*sina)
		+ z*(ax.x*ax.z*cosb + ax.y*sina);
	float ny = x*(ax.y*ax.x*cosb + ax.z*sina)
		+ y*(ax.y*ax.y*cosb + cosa)
		+ z*(ax.y*ax.z*cosb - ax.x*sina);
	float nz = x*(ax.z*ax.x*cosb - ax.y*sina)
		+ y*(ax.z*ax.y*cosb + ax.x*sina)
		+ z*(ax.z*ax.z*cosb + cosa);
	x = nx; y = ny; z = nz;
	return *this;
}

Mach1Point3DCore& Mach1Point3DCore::normalize() {
	float length = (float)sqrt(x*x + y*y + z*z);
	if (length > 0) {
		x /= length;
		y /= length;
		z /= length;
	}
	return *this;
}


Mach1Point3DCore Mach1Point3DCore::getNormalized() const {
	float length = (float)sqrt(x*x + y*y + z*z);
	if (length > 0) {
		return Mach1Point3DCore(x / length, y / length, z / length);
	}
	else {
		return Mach1Point3DCore();
	}
}


Mach1Point3DCore Mach1Point3DCore::getRotated(float angle, const Mach1Point3DCore& axis) const {
	Mach1Point3DCore ax = axis.getNormalized();
	float a = (float)(angle*DEG_TO_RAD);
	float sina = sin(a);
	float cosa = cos(a);
	float cosb = 1.0f - cosa;

	return Mach1Point3DCore(x*(ax.x*ax.x*cosb + cosa)
		+ y*(ax.x*ax.y*cosb - ax.z*sina)
		+ z*(ax.x*ax.z*cosb + ax.y*sina),
		x*(ax.y*ax.x*cosb + ax.z*sina)
		+ y*(ax.y*ax.y*cosb + cosa)
		+ z*(ax.y*ax.z*cosb - ax.x*sina),
		x*(ax.z*ax.x*cosb - ax.y*sina)
		+ y*(ax.z*ax.y*cosb + ax.x*sina)
		+ z*(ax.z*ax.z*cosb + cosa));
}
 