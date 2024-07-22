#include "Mach1GenerateCoeffs.h"

namespace Mach1Utils {
float clamp(float a, float min, float max) {
    return (a < min) ? min : ((a > max) ? max : a);
}
}; // namespace Mach1Utils

std::vector<float> generateCoeffSetForPoint(float x, float y, float z, std::vector<Mach1Point3D> targetStandart) {
    std::vector<float> result;
    result.resize(targetStandart.size());

    //    vector<vector<float>> arg = {{0.5, 0.5, 0.3}};

    float coeffSum = 0;

    for (int i = 0; i < targetStandart.size(); i++) {
        float rawCoeff = sqrt(pow(x - targetStandart[i].x, 2) +
                              pow(y - targetStandart[i].y, 2) +
                              pow(z - targetStandart[i].z, 2));

        float falloff = 1; // the bigger this value, the closer you need to be
                           // to a point for it to affect coeffs
        result[i] = Mach1Utils::clamp(1 + (1.22 - rawCoeff) * falloff, 0, 1);
        coeffSum += result[i];
    }

    for (int i = 0; i < targetStandart.size(); i++) {
        result[i] /= coeffSum;
    }

    return result;
}

std::vector<std::vector<float>> generateCoeffSetForPoints(std::vector<Mach1Point3D> points, std::vector<Mach1Point3D> targetStandart) {
    std::vector<std::vector<float>> vec(targetStandart.size());

    for (auto &p : points) {
        std::vector<float> v = generateCoeffSetForPoint(p.x, p.y, p.z, targetStandart);
        for (size_t i = 0; i < vec.size(); i++)
            vec[i].push_back(v[i]);
    }

    return vec;
}