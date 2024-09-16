#ifndef MACH1KEYPOINT_H
#define MACH1KEYPOINT_H

//  Mach1 Spatial SDK
//  Copyright © 2017 Mach1. All rights reserved.

#include "Mach1Point3D.h"

struct Mach1KeyPoint {
    long long sample;
    Mach1Point3D point;
};

#endif // MACH1KEYPOINT_H
