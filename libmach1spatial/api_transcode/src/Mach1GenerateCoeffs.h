#pragma once

#include <vector>
#include "Mach1Point3D.h"

std::vector<float> generateCoeffSetForPoint(float x, float y, float z, std::vector<Mach1Point3D> targetStandart);
std::vector<std::vector<float>> generateCoeffSetForPoints(std::vector<Mach1Point3D> points, std::vector<Mach1Point3D> targetStandart);
