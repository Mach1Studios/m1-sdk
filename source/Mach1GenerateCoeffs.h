#include "Mach1Point3DCore.h"

/*
 // OG version of this

 void calculateAutoCoeffs() {
     for (auto &i: points) {
         if (i.autoCoeffs.size() < 8) {
             i.autoCoeffs.resize(8);
         }

         float rawAutoCoeffs[8];
         float autoCoeffsSum = 0;
         for (int j = 0; j < 8; j++) {
             Mach1Point3DCore dataPointInProperSpace = Mach1Point3DCore(-(i.y - 0.5),
                                                       i.x - 0.5,
                                                       0) * 2;
             float rawCoeff = (dataPointInProperSpace
                             - m1SpherePoints[j]).length();
             float falloff = 1; // the bigger this value, the closer you need to be
             // to a point for it to affect coeffs
             rawAutoCoeffs[j] = ofClamp(1 + (1.22 - rawCoeff) * falloff, 0, 1);
             autoCoeffsSum += rawAutoCoeffs[j];
         }

         for (int j = 0; j < 8; j++) {
             i.autoCoeffs[j] = rawAutoCoeffs[j] / autoCoeffsSum;
         }
     }
 }
 
 */

std::vector<float> generateCoeffSetForPoint(float x, float y, float z, std::vector<Mach1Point3DCore> targetStandart);
std::vector< std::vector<float> > generateCoeffSetForPoints(std::vector<Mach1Point3DCore> points, std::vector<Mach1Point3DCore> targetStandart);
