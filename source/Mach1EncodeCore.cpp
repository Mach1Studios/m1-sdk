//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

/*
Internal Orientation Implementation:
    -  Azimuth[+] = rotate right 0-1 [Range: 0->360 | -180->180]
    -  Azimuth[-] = rotate left 0-1 [Range: 0->360 | -180->180]
    -  Elevation[+] = rotate up 0-1 [Range: -90->90]
    -  Elevation[-] = rotate down 0-1 [Range: -90->90]
    -  OrbitRotation[+] = rotate right 0-1 [Range: 0->360 | -180->180]
    -  OrbitRotation[-] = rotate left 0-1 [Range: 0->360 | -180->180]

Mach1EncodeCore normalizes all input ranges to an unsigned "0 to 1" range for Azimuth, Elevation and OrbitRotation.
 */

#include "Mach1EncodeCore.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <math.h>
#include <vector>

float M1EncodeCore::clamp(float n, float lower, float upper) {
    return (std::max)(lower, (std::min)(n, upper));
}

float clamp(float n, float lower, float upper) {
    return (std::max)(lower, (std::min)(n, upper));
}

float map_(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float dist_sq(float lx1, float ly1, float lz1, float lx2, float ly2, float lz2) {
    return (powf(lx1 - lx2, 2) + powf(ly1 - ly2, 2) + powf(lz1 - lz2, 2));
}

float distance_to_segment(float px, float py, float pz, float lx1, float ly1, float lz1, float lx2, float ly2, float lz2) {
    float line_dist = dist_sq(lx1, ly1, lz1, lx2, ly2, lz2);
    if (line_dist == 0)
        return dist_sq(px, py, pz, lx1, ly1, lz1);
    float t = ((px - lx1) * (lx2 - lx1) + (py - ly1) * (ly2 - ly1) + (pz - lz1) * (lz2 - lz1)) / line_dist;
    t = clamp(t, 0.0, 1.0);
    return dist_sq(px, py, pz, lx1 + t * (lx2 - lx1), ly1 + t * (ly2 - ly1), lz1 + t * (lz2 - lz1));
}

bool segments_intersection(Mach1Point3DCore a_first, Mach1Point3DCore a_second, Mach1Point3DCore b_first, Mach1Point3DCore b_second, Mach1Point3DCore &ip) {
    Mach1Point3DCore da = a_second - a_first;
    Mach1Point3DCore db = b_second - b_first;
    Mach1Point3DCore dc = b_first - a_first;

    if (Mach1Point3DCore::dot(dc, da.getCrossed(db)) != 0.0) // lines are not coplanar
        return false;

    float s = Mach1Point3DCore::dot(dc.getCrossed(db), da.getCrossed(db)) / powf(da.getCrossed(db).length(), 2);
    if (s >= 0.0 && s <= 1.0) {
        ip = a_first + da * Mach1Point3DCore(s, s, s);
        return true;
    }

    return false;
}

bool segment_intersect_face(Mach1Point3DCore p1line, Mach1Point3DCore p2line, Mach1Point3DCore p1, Mach1Point3DCore p2, Mach1Point3DCore p3, Mach1Point3DCore &point) {
    Mach1Point3DCore e0 = p2 - p1;
    Mach1Point3DCore e1 = p3 - p1;

    Mach1Point3DCore dir = p2line - p1line;
    Mach1Point3DCore dir_norm = dir.getNormalized();

    Mach1Point3DCore h = dir_norm.getCrossed(e1);
    const float a = Mach1Point3DCore::dot(e0, h);

    float eps = 0.00001;

    if (a > -eps && a < eps) {
        return false;
    }

    Mach1Point3DCore s = p1line - p1;
    const float f = 1.0f / a;
    const float u = f * Mach1Point3DCore::dot(s, h);

    if (u < 0.0f || u > 1.0f) {
        return false;
    }

    Mach1Point3DCore q = s.getCrossed(e0);
    const float v = f * Mach1Point3DCore::dot(dir_norm, q);

    if (v < 0.0f || u + v > 1.0f) {
        return false;
    }

    const float t = f * Mach1Point3DCore::dot(e1, q);

    if (t > eps && t < sqrtf(Mach1Point3DCore::dot(dir, dir))) { // segment intersection
        point = p1line + dir_norm * t;
        return true;
    }

    return false;
}

float distance_to_plane(float p_x, float p_y, float p_z, float p0_x, float p0_y, float p0_z, float p1_x, float p1_y, float p1_z, float p2_x, float p2_y, float p2_z) {
    float U_x = p1_x - p0_x;
    float V_x = p2_x - p0_x;
    float U_y = p1_y - p0_y;
    float V_y = p2_y - p0_y;
    float U_z = p1_z - p0_z;
    float V_z = p2_z - p0_z;

    float n_x = (U_y * V_z) - (U_z * V_y);
    float n_y = (U_z * V_x) - (U_x * V_z);
    float n_z = (U_x * V_y) - (U_y * V_x);

    float dist = sqrt((n_x * n_x) + (n_y * n_y) + (n_z * n_z));
    n_x /= dist;
    n_y /= dist;
    n_z /= dist;

    dist = fabs((p_x - p0_x) * n_x + (p_y - p0_y) * n_y + (p_z - p0_z) * n_z);
    return dist;
}

M1EncodeCorePointResults::M1EncodeCorePointResults() {
    for (int i = 0; i < MAX_POINTS_COUNT; i++) {
        pointsNames[i] = "";
        ppoints[i].x = 0;
        ppoints[i].y = 0;
        ppoints[i].z = 0;
    }
    pointsCount = 0;
}

M1EncodeCorePointResults::~M1EncodeCorePointResults() {
}

std::vector<Mach1Point3DCore> M1EncodeCorePointResults::getPoints() {
    return std::vector<Mach1Point3DCore>(ppoints, std::end(ppoints));
}

std::vector<std::vector<float>> M1EncodeCorePointResults::getGains() {
    return gains;
}

std::vector<std::string> M1EncodeCorePointResults::getPointsNames() {
    return std::vector<std::string>(pointsNames, std::end(pointsNames));
}

std::vector<float> M1EncodeCorePointResults::getGainsForInputChannelNamed(std::string pointName) {
    for (int i = 0; i < pointsCount; i++) {
        if (pointsNames[i] == pointName)
            return gains[i];
    }

    std::vector<float> result;
    if (gains.size() > 0) {
        result.resize(gains[0].size());
    }
    return result;
}

int M1EncodeCorePointResults::getPointsCount() {
    return pointsCount;
}

float M1EncodeCore::getCoeffForStandardPoint(float x, float y, float z, Mach1Point3DCore point, bool ignoreZ) {
    // map from [-1,1] to [0,1]
    point.x = (point.x / (1 / 0.707) + 1) / 2;
    point.y = (point.y / (1 / 0.707) + 1) / 2;
    point.z = (point.z / (1 / 0.707) + 1) / 2;

    float dist = pow(pow(point.x - x, 2.0) + pow(point.y - y, 2.0) + pow(point.z - z, 2.0), 0.5);
    dist = 1 - dist;
    dist = (dist - 0.6) / 0.4;
    dist = clamp(dist, 0, 1);
    // dist = dist * dist * dist * dist; // apply easeInQuart

    // "pan law" experiment
    if (pannerMode == MODE_ISOTROPICEQUALPOWER) {
        dist = sqrt(1 - pow(dist - 1, 2));
    }

    return dist;
}

std::vector<float> M1EncodeCore::getCoeffSetForStandardPointSet(float x, float y, float z, std::vector<Mach1Point3DCore> &pointSet, bool ignoreZ) {
    std::vector<float> result;

    std::vector<Mach1Point3DCore> points = pointSet;
    float len = 0;

    // normalize cube
    for (auto &i : points) {
        len = (std::max)(len, i.length());
    }
    for (auto &i : points) {
        i = i / len;
    }

    for (auto &i : points) {
        result.push_back(getCoeffForStandardPoint(x, y, z, i, ignoreZ));
    }

    // normalize coeffs
    float c = 0;
    for (auto &i : result) {
        c += i;
    }

    for (auto &i : result) {
        i /= c;
    }

    return result;
}

void M1EncodeCore::processGainsChannels(float x, float y, float z, std::vector<float> &result) {

    /*
     * X = Left(-) to Right(+) (from a top down perspective)
     * Y = Front(+) to Back(-) (from a top down perspective)
     * Z = Top(+) to Bottom(-) (from a top down perspective)
     */

    static std::vector<Mach1Point3DCore> m1Spatial_4_Def = {
        {-1, 1, 0},
        {1, 1, 0},
        {-1, -1, 0},
        {1, -1, 0}};

    static std::vector<Mach1Point3DCore> m1Spatial_8_Def = {
        {-1, 1, 1},
        {1, 1, 1},
        {-1, -1, 1},
        {1, -1, 1},

        {-1, 1, -1},
        {1, 1, -1},
        {-1, -1, -1},
        {1, -1, -1}};

    static std::vector<Mach1Point3DCore> m1Spatial_12_Def = {
        {-1, 1, 1},
        {1, 1, 1},
        {-1, -1, 1},
        {1, -1, 1},

        {-1, 1, -1},
        {1, 1, -1},
        {-1, -1, -1},
        {1, -1, -1},

        {0, 1 / 0.707, 0},
        {1 / 0.707, 0, 0},
        {0, -1 / 0.707, 0},
        {-1 / 0.707, 0, 0}};

    static std::vector<Mach1Point3DCore> m1Spatial_14_Def = {
        {-1, 1, 1},
        {1, 1, 1},
        {-1, -1, 1},
        {1, -1, 1},

        {-1, 1, -1},
        {1, 1, -1},
        {-1, -1, -1},
        {1, -1, -1},

        {0, 1 / 0.707, 0},
        {1 / 0.707, 0, 0},
        {0, -1 / 0.707, 0},
        {-1 / 0.707, 0, 0},

        {0, 0, 1 / 0.707},
        {0, 0, -1 / 0.707}};

    static std::vector<Mach1Point3DCore> m1Spatial_32_Def = {
        {0.00000, 1.32048, 0.50689},
        {-0.74953, 1.19950, 0.00000},
        {0.00000, 1.32048, -0.50689},
        {0.74953, 1.19950, 0.00000},
        {0.00000, 0.74953, 1.19950},
        {-0.81928, 0.81928, 0.81128},
        {-1.32048, 0.50689, 0.00000},
        {-0.81928, 0.81928, -0.81128},
        {0.00000, 0.74953, -1.19950},
        {0.81928, 0.81928, -0.81128},
        {1.32048, 0.50689, 0.00000},
        {0.81928, 0.81928, 0.81128},
        {-0.50681, -0.00885, 1.32048},
        {-1.19950, -0.00000, 0.74953},
        {-1.21240, -0.00000, -0.72848},
        {-0.50681, 0.00885, -1.32048},
        {0.00000, -1.32048, 0.50689},
        {0.74953, -1.19950, 0.00000},
        {0.00000, -1.32048, -0.50689},
        {-0.74953, -1.19950, 0.00000},
        {0.00000, -0.74953, 1.19950},
        {0.81928, -0.81928, 0.81128},
        {1.32048, -0.50689, 0.00000},
        {0.81928, -0.81928, -0.81128},
        {0.00000, -0.74953, -1.19950},
        {-0.81928, -0.81928, -0.81128},
        {-1.32048, -0.50689, 0.00000},
        {-0.81928, -0.81928, 0.81128},
        {0.50681, -0.00885, 1.32048},
        {1.19950, 0.00000, 0.74953},
        {1.19950, 0.00000, -0.74953},
        {0.50681, 0.00885, -1.32048}};

    static std::vector<Mach1Point3DCore> m1Spatial_36_Def = {
        {-0.43310, 0.71779, 1.13922},
        {1.13922, -0.43310, 0.71779},
        {0.43310, -0.71779, 1.13922},
        {0.71779, 1.13922, -0.43310},
        {1.13922, 0.43310, -0.71779},
        {-0.71779, 1.13922, 0.43310},
        {-1.13922, 0.43310, 0.71779},
        {-0.71779, -1.13922, -0.43310},
        {-1.13922, -0.43310, -0.71779},
        {0.71779, -1.13922, 0.43310},
        {0.43310, 0.71779, -1.13922},
        {-0.43310, -0.71779, -1.13922},
        {-0.34445, 0.88595, -1.04740},
        {-1.04740, -0.34445, 0.88595},
        {0.34445, -0.88595, -1.04740},
        {0.88595, -1.04740, -0.34445},
        {-1.04740, 0.34445, -0.88595},
        {-0.88595, -1.04740, 0.34445},
        {1.04740, 0.34445, 0.88595},
        {-0.88595, 1.04740, -0.34445},
        {1.04740, -0.34445, -0.88595},
        {0.88595, 1.04740, 0.34445},
        {0.34445, 0.88595, 1.04740},
        {-0.34445, -0.88595, 1.04740},
        {1.35378, -0.40488, -0.06298},
        {-0.06298, 1.35378, -0.40488},
        {-1.35378, 0.40488, -0.06298},
        {-0.40488, -0.06298, 1.35378},
        {-0.06298, -1.35378, 0.40488},
        {0.40488, -0.06298, -1.35378},
        {0.06298, -1.35378, -0.40488},
        {0.40488, 0.06298, 1.35378},
        {0.06298, 1.35378, 0.40488},
        {-0.40488, 0.06298, -1.35378},
        {-1.35378, -0.40488, 0.06298},
        {1.35378, 0.40488, 0.06298}};

    static std::vector<Mach1Point3DCore> m1Spatial_48_Def = {
        {0.50006, 1.32018, -0.08763},
        {-0.50006, 1.32018, 0.08763},
        {-0.08763, 1.32018, -0.50006},
        {0.08763, 1.32018, 0.50006},
        {0.50006, -1.32018, 0.08763},
        {-0.50006, -1.32018, -0.08763},
        {-0.08763, -1.32018, 0.50006},
        {0.08763, -1.32018, -0.50006},
        {1.32018, -0.08763, 0.50006},
        {1.32018, 0.08763, -0.50006},
        {1.32018, -0.50006, -0.08763},
        {1.32018, 0.50006, 0.08763},
        {-1.32018, 0.08763, 0.50006},
        {-1.32018, -0.08763, -0.50006},
        {-1.32018, 0.50006, -0.08763},
        {-1.32018, -0.50006, 0.08763},
        {-0.08763, 0.50006, 1.32018},
        {0.08763, -0.50006, 1.32018},
        {-0.50006, -0.08763, 1.32018},
        {0.50006, 0.08763, 1.32018},
        {0.08763, 0.50006, -1.32018},
        {-0.08763, -0.50006, -1.32018},
        {0.50006, -0.08763, -1.32018},
        {-0.50006, 0.08763, -1.32018},
        {0.90487, 0.99978, 0.42693},
        {-0.90487, 0.99978, -0.42693},
        {0.42693, 0.99978, -0.90487},
        {-0.42693, 0.99978, 0.90487},
        {0.90487, -0.99978, -0.42693},
        {-0.90487, -0.99978, 0.42693},
        {0.42693, -0.99978, 0.90487},
        {-0.42693, -0.99978, -0.90487},
        {0.99978, 0.42693, 0.90487},
        {0.99978, -0.42693, -0.90487},
        {0.99978, -0.90487, 0.42693},
        {0.99978, 0.90487, -0.42693},
        {-0.99978, -0.42693, 0.90487},
        {-0.99978, 0.42693, -0.90487},
        {-0.99978, 0.90487, 0.42693},
        {-0.99978, -0.90487, -0.42693},
        {0.42693, 0.90487, 0.99978},
        {-0.42693, -0.90487, 0.99978},
        {-0.90487, 0.42693, 0.99978},
        {0.90487, -0.42693, 0.99978},
        {-0.42693, 0.90487, -0.99978},
        {0.42693, -0.90487, -0.99978},
        {0.90487, 0.42693, -0.99978},
        {-0.90487, -0.42693, -0.99978}};

    static std::vector<Mach1Point3DCore> m1Spatial_60_Def = {
        {0.77221, -1.06624, -0.51714},
        {-0.51714, 0.77221, -1.06624},
        {-0.77221, 1.06624, -0.51714},
        {-1.06624, -0.51714, 0.77221},
        {-0.51714, -0.77221, 1.06624},
        {1.06624, -0.51714, -0.77221},
        {0.51714, -0.77221, -1.06624},
        {1.06624, 0.51714, 0.77221},
        {0.51714, 0.77221, 1.06624},
        {-1.06624, 0.51714, -0.77221},
        {-0.77221, -1.06624, 0.51714},
        {0.77221, 1.06624, 0.51714},
        {-1.00870, 0.99036, 0.04822},
        {0.04822, -1.00870, 0.99035},
        {1.00870, -0.99036, 0.04822},
        {0.99036, 0.04822, -1.00870},
        {0.04822, 1.00870, -0.99036},
        {-0.99036, 0.04822, 1.00870},
        {-0.04822, 1.00870, 0.99036},
        {-0.99036, -0.04822, -1.00870},
        {-0.04822, -1.00870, -0.99035},
        {0.99036, -0.04822, 1.00870},
        {1.00870, 0.99036, -0.04822},
        {-1.00870, -0.99036, -0.04822},
        {0.10898, 0.39071, -1.35502},
        {-1.35502, 0.10898, 0.39071},
        {-0.10898, -0.39071, -1.35502},
        {0.39071, -1.35502, 0.10898},
        {-1.35502, -0.10898, -0.39071},
        {-0.39071, -1.35502, -0.10898},
        {1.35502, -0.10898, 0.39071},
        {-0.39071, 1.35502, 0.10898},
        {1.35502, 0.10898, -0.39071},
        {0.39071, 1.35502, -0.10898},
        {-0.10898, 0.39071, 1.35502},
        {0.10898, -0.39071, 1.35502},
        {-1.10800, 0.63907, 0.60377},
        {0.60377, -1.10800, 0.63907},
        {1.10800, -0.63907, 0.60377},
        {0.63907, 0.60377, -1.10800},
        {0.60377, 1.10800, -0.63907},
        {-0.63907, 0.60377, 1.10800},
        {-0.60377, 1.10800, 0.63907},
        {-0.63907, -0.60377, -1.10800},
        {-0.60377, -1.10800, -0.63907},
        {0.63907, -0.60377, 1.10800},
        {1.10800, 0.63907, -0.60377},
        {-1.10800, -0.63907, -0.60377},
        {-1.31996, -0.47890, 0.17020},
        {0.17020, -1.31996, -0.47890},
        {1.31996, 0.47890, 0.17020},
        {-0.47890, 0.17020, -1.31996},
        {0.17020, 1.31996, 0.47890},
        {0.47890, 0.17020, 1.31996},
        {-0.17020, 1.31996, -0.47890},
        {0.47890, -0.17020, -1.31996},
        {-0.17020, -1.31996, 0.47890},
        {-0.47890, -0.17020, 1.31996},
        {1.31996, -0.47890, -0.17020},
        {-1.31996, 0.47890, -0.17020}};

    static std::map<OutputMode, std::vector<Mach1Point3DCore>> standards = {
        {OUTPUT_SPATIAL_8CH, m1Spatial_8_Def},
        {OUTPUT_HORIZON_4CH, m1Spatial_4_Def},
        {OUTPUT_SPATIAL_12CH, m1Spatial_12_Def},
        {OUTPUT_SPATIAL_14CH, m1Spatial_14_Def},
        {OUTPUT_SPATIAL_32CH, m1Spatial_32_Def},
        {OUTPUT_SPATIAL_36CH, m1Spatial_36_Def},
        {OUTPUT_SPATIAL_48CH, m1Spatial_48_Def},
        {OUTPUT_SPATIAL_60CH, m1Spatial_60_Def},
    };

    std::vector<Mach1Point3DCore> pointsSet;
    if (standards.find(outputMode) != standards.end()) {
        pointsSet = standards[outputMode];
    }

    /// NEW ENCODE ALGO SDK 3.0

    /// MACH1SPATIAL-4
    /* TOP VIEW
     [0]_________[1]
       |          |
       |          |
       |          |
     [2]_________[3]
     */
    std::vector<std::vector<int>> m1Spatial_4_Lines_Def = {
        /// QUAD LINES
        {0, 1},
        {1, 3},
        {3, 2},
        {2, 0},
    };
    std::vector<std::vector<int>> m1Spatial_4_Plane_Def = {
        {0, 1, 2},
        {1, 2, 3},
    };

    /// MACH1SPATIAL-8
    /* TOP VIEW
     [0]_________[1]
       |          |
       |          |
       |          |
     [2]_________[3]
     */
    std::vector<std::vector<int>> m1Spatial_8_Lines_Def = {
        /// TOP QUAD LINES
        {0, 1},
        {1, 3},
        {3, 2},
        {2, 0},
        /// TOP TO BOTTOM LINES
        {0, 4},
        {1, 5},
        {2, 6},
        {3, 7},
        /// BOTTOM QUAD LINES
        {4, 5},
        {5, 7},
        {7, 6},
        {6, 4},
    };
    std::vector<std::vector<int>> m1Spatial_8_Plane_Def = {
        /// FRONT FACE
        {0, 1, 5},
        {5, 4, 0},
        /// RIGHT FACE
        {1, 3, 7},
        {7, 5, 1},
        /// BACK FACE
        {3, 2, 6},
        {6, 7, 3},
        /// LEFT FACE
        {2, 0, 4},
        {4, 6, 2},
        /// TOP FACE
        {0, 1, 3},
        {3, 2, 0},
        /// BOTTOM FACE
        {4, 5, 7},
        {7, 6, 4},
    };

    /// MACH1SPATIAL-12
    /* TOP VIEW
     [0]___[8]___[1]
      |           |
   [11]           [9]
      |           |
     [2]___[10]__[3]
     */
    std::vector<std::vector<int>> m1Spatial_12_Lines_Def = {
        /// TOP QUAD LINES
        {0, 1},
        {1, 3},
        {3, 2},
        {2, 0},
        /// TOP TO BOTTOM LINES
        {0, 4},
        {1, 5},
        {2, 6},
        {3, 7},
        /// BOTTOM QUAD LINES
        {4, 5},
        {5, 7},
        {7, 6},
        {6, 4},
        /// FRONT FACE LINES
        {0, 8},
        {1, 8},
        {4, 8},
        {5, 8},
        /// RIGHT FACE LINES
        {1, 9},
        {3, 9},
        {5, 9},
        {7, 9},
        /// BACK FACE LINES
        {3, 10},
        {2, 10},
        {7, 10},
        {6, 10},
        /// LEFT FACE LINES
        {0, 11},
        {2, 11},
        {4, 11},
        {6, 11},
    };
    std::vector<std::vector<int>> m1Spatial_12_Plane_Def = {
        /// FRONT FACE TO POINT [8]
        {0, 1, 8},
        {1, 5, 8},
        {5, 4, 8},
        {4, 0, 8},
        /// RIGHT FACE TO POINT [9]
        {1, 3, 9},
        {3, 7, 9},
        {7, 5, 9},
        {5, 1, 9},
        /// BACK FACE TO POINT [10]
        {3, 2, 10},
        {2, 6, 10},
        {6, 7, 10},
        {7, 3, 10},
        /// LEFT FACE TO POINT [11]
        {2, 0, 11},
        {0, 4, 11},
        {4, 6, 11},
        {6, 2, 11},
        /// TOP FACE
        {0, 1, 2},
        {1, 2, 3},
        /// BOTTOM FACE
        {4, 5, 6},
        {5, 6, 7},
    };

    /// MACH1SPATIAL-14
    /* TOP VIEW
     [0]___[8]___[1]
      |           |
   [11]    [12]   [9]
      |           |
     [2]___[10]__[3]
     */
    std::vector<std::vector<int>> m1Spatial_14_Lines_Def = {
        /// TOP QUAD LINES
        {0, 1},
        {1, 3},
        {3, 2},
        {2, 0},
        /// TOP TO BOTTOM LINES
        {0, 4},
        {1, 5},
        {2, 6},
        {3, 7},
        /// BOTTOM QUAD LINES
        {4, 5},
        {5, 7},
        {7, 6},
        {6, 4},
        /// FRONT FACE LINES
        {0, 8},
        {1, 8},
        {4, 8},
        {5, 8},
        /// RIGHT FACE LINES
        {1, 9},
        {3, 9},
        {5, 9},
        {7, 9},
        /// BACK FACE LINES
        {3, 10},
        {2, 10},
        {7, 10},
        {6, 10},
        /// LEFT FACE LINES
        {0, 11},
        {2, 11},
        {4, 11},
        {6, 11},
        /// TOP FACE LINES
        {0, 12},
        {1, 12},
        {2, 12},
        {3, 12},
        /// BOTTOM FACE LINES
        {4, 13},
        {5, 13},
        {6, 13},
        {7, 13},
    };
    std::vector<std::vector<int>> m1Spatial_14_Plane_Def = {
        /// FRONT FACE TO POINT [8]
        {0, 1, 8},
        {1, 5, 8},
        {5, 4, 8},
        {4, 0, 8},
        /// RIGHT FACE TO POINT [9]
        {1, 3, 9},
        {3, 7, 9},
        {7, 5, 9},
        {5, 1, 9},
        /// BACK FACE TO POINT [10]
        {3, 2, 10},
        {2, 6, 10},
        {6, 7, 10},
        {7, 3, 10},
        /// LEFT FACE TO POINT [11]
        {2, 0, 11},
        {0, 4, 11},
        {4, 6, 11},
        {6, 2, 11},
        /// TOP FACE TO POINT [12]
        {0, 1, 12},
        {1, 3, 12},
        {3, 2, 12},
        {2, 0, 12},
        /// BOTTOM FACE TO POINT [13]
        {4, 5, 13},
        {5, 7, 13},
        {7, 6, 13},
        {6, 4, 13},
    };

    /// MACH1SPATIAL-32
    std::vector<std::vector<int>> m1Spatial_32_Lines_Def = {
        {14, 26},
        {26, 6},
        {30, 10},
        {10, 22},
        {29, 22},
        {5, 1},
        {1, 6},
        {5, 12},
        {12, 4},
        {7, 8},
        {8, 15},
        {7, 15},
        {15, 14},
        {7, 14},
        {14, 6},
        {7, 6},
        {25, 14},
        {25, 15},
        {15, 24},
        {25, 19},
        {19, 26},
        {25, 26},
        {27, 19},
        {19, 16},
        {27, 26},
        {31, 24},
        {31, 15},
        {18, 19},
        {18, 25},
        {25, 24},
        {18, 17},
        {17, 16},
        {18, 16},
        {23, 30},
        {30, 22},
        {23, 22},
        {22, 17},
        {23, 17},
        {23, 18},
        {18, 24},
        {23, 24},
        {23, 31},
        {31, 30},
        {28, 4},
        {13, 12},
        {13, 5},
        {5, 6},
        {13, 6},
        {13, 26},
        {13, 27},
        {27, 12},
        {11, 29},
        {29, 10},
        {11, 10},
        {10, 3},
        {11, 4},
        {11, 28},
        {28, 29},
        {2, 8},
        {2, 7},
        {7, 1},
        {0, 4},
        {0, 11},
        {11, 3},
        {0, 3},
        {3, 2},
        {0, 2},
        {2, 1},
        {0, 1},
        {0, 5},
        {5, 4},
        {9, 30},
        {9, 31},
        {31, 8},
        {9, 8},
        {9, 2},
        {9, 3},
        {9, 10},
        {20, 12},
        {20, 27},
        {27, 16},
        {20, 28},
        {28, 12},
        {21, 17},
        {21, 22},
        {21, 29},
        {21, 28},
        {21, 20},
        {20, 16},
        {21, 16},
    };
    std::vector<std::vector<int>> m1Spatial_32_Plane_Def = {
        {14, 26, 6},
        {30, 10, 22},
        {29, 22, 10},
        {5, 1, 6},
        {5, 12, 4},
        {7, 8, 15},
        {7, 15, 14},
        {7, 14, 6},
        {7, 6, 1},
        {25, 14, 15},
        {25, 15, 24},
        {25, 19, 26},
        {25, 26, 14},
        {27, 19, 16},
        {27, 26, 19},
        {31, 24, 15},
        {31, 15, 8},
        {18, 19, 25},
        {18, 25, 24},
        {18, 17, 16},
        {18, 16, 19},
        {23, 30, 22},
        {23, 22, 17},
        {23, 17, 18},
        {23, 18, 24},
        {23, 24, 31},
        {23, 31, 30},
        {28, 4, 12},
        {13, 12, 5},
        {13, 5, 6},
        {13, 6, 26},
        {13, 26, 27},
        {13, 27, 12},
        {11, 29, 10},
        {11, 10, 3},
        {11, 4, 28},
        {11, 28, 29},
        {2, 8, 7},
        {2, 7, 1},
        {0, 4, 11},
        {0, 11, 3},
        {0, 3, 2},
        {0, 2, 1},
        {0, 1, 5},
        {0, 5, 4},
        {9, 30, 31},
        {9, 31, 8},
        {9, 8, 2},
        {9, 2, 3},
        {9, 3, 10},
        {9, 10, 30},
        {20, 12, 27},
        {20, 27, 16},
        {20, 28, 12},
        {21, 17, 22},
        {21, 22, 29},
        {21, 29, 28},
        {21, 28, 20},
        {21, 20, 16},
        {21, 16, 17},
    };

    /// MACH1SPATIAL-36
    std::vector<std::vector<int>> m1Spatial_36_Lines_Def = {
        {13, 23},
        {23, 27},
        {13, 27},
        {27, 6},
        {35, 1},
        {1, 24},
        {0, 6},
        {2, 27},
        {4, 35},
        {35, 24},
        {5, 6},
        {5, 26},
        {26, 6},
        {19, 26},
        {19, 5},
        {5, 25},
        {17, 23},
        {31, 22},
        {22, 0},
        {31, 0},
        {0, 27},
        {31, 27},
        {31, 2},
        {2, 1},
        {20, 4},
        {4, 24},
        {32, 22},
        {22, 21},
        {32, 25},
        {32, 5},
        {5, 0},
        {32, 0},
        {3, 4},
        {4, 10},
        {3, 10},
        {10, 25},
        {3, 25},
        {3, 32},
        {32, 21},
        {3, 21},
        {21, 35},
        {3, 35},
        {12, 19},
        {19, 25},
        {12, 25},
        {34, 17},
        {17, 13},
        {34, 13},
        {13, 6},
        {34, 6},
        {18, 21},
        {18, 22},
        {18, 31},
        {31, 1},
        {18, 1},
        {18, 35},
        {29, 10},
        {29, 4},
        {16, 26},
        {16, 19},
        {7, 17},
        {7, 11},
        {11, 30},
        {9, 1},
        {9, 24},
        {14, 11},
        {11, 29},
        {14, 29},
        {29, 20},
        {14, 30},
        {33, 29},
        {33, 16},
        {16, 12},
        {33, 12},
        {12, 10},
        {33, 10},
        {8, 34},
        {34, 26},
        {8, 26},
        {8, 16},
        {8, 33},
        {33, 11},
        {8, 11},
        {8, 7},
        {7, 34},
        {28, 23},
        {28, 17},
        {28, 7},
        {7, 30},
        {28, 30},
        {30, 9},
        {28, 9},
        {9, 2},
        {28, 2},
        {2, 23},
        {15, 24},
        {15, 9},
        {15, 30},
        {15, 14},
        {14, 20},
        {15, 20},
        {20, 24},
    };
    std::vector<std::vector<int>> m1Spatial_36_Plane_Def = {
        {13, 23, 27},
        {13, 27, 6},
        {35, 1, 24},
        {0, 6, 27},
        {2, 27, 23},
        {4, 35, 24},
        {5, 6, 0},
        {5, 26, 6},
        {19, 26, 5},
        {19, 5, 25},
        {17, 23, 13},
        {31, 22, 0},
        {31, 0, 27},
        {31, 27, 2},
        {31, 2, 1},
        {20, 4, 24},
        {32, 22, 21},
        {32, 25, 5},
        {32, 5, 0},
        {32, 0, 22},
        {3, 4, 10},
        {3, 10, 25},
        {3, 25, 32},
        {3, 32, 21},
        {3, 21, 35},
        {3, 35, 4},
        {12, 19, 25},
        {12, 25, 10},
        {34, 17, 13},
        {34, 13, 6},
        {34, 6, 26},
        {18, 21, 22},
        {18, 22, 31},
        {18, 31, 1},
        {18, 1, 35},
        {18, 35, 21},
        {29, 10, 4},
        {29, 4, 20},
        {16, 26, 19},
        {16, 19, 12},
        {7, 17, 34},
        {7, 11, 30},
        {9, 1, 2},
        {9, 24, 1},
        {14, 11, 29},
        {14, 29, 20},
        {14, 30, 11},
        {33, 29, 11},
        {33, 16, 12},
        {33, 12, 10},
        {33, 10, 29},
        {8, 34, 26},
        {8, 26, 16},
        {8, 16, 33},
        {8, 33, 11},
        {8, 11, 7},
        {8, 7, 34},
        {28, 23, 17},
        {28, 17, 7},
        {28, 7, 30},
        {28, 30, 9},
        {28, 9, 2},
        {28, 2, 23},
        {15, 24, 9},
        {15, 9, 30},
        {15, 30, 14},
        {15, 14, 20},
        {15, 20, 24},
    };

    /// MACH1SPATIAL-48
    std::vector<std::vector<int>> m1Spatial_48_Lines_Def = {
        {30, 4},
        {4, 34},
        {17, 41},
        {41, 30},
        {13, 47},
        {47, 39},
        {13, 37},
        {37, 47},
        {22, 33},
        {33, 45},
        {22, 45},
        {45, 21},
        {23, 20},
        {20, 22},
        {23, 22},
        {22, 21},
        {23, 21},
        {21, 47},
        {23, 47},
        {31, 39},
        {31, 47},
        {31, 21},
        {31, 45},
        {45, 7},
        {1, 25},
        {25, 38},
        {1, 2},
        {2, 25},
        {40, 24},
        {24, 3},
        {44, 37},
        {37, 25},
        {44, 25},
        {44, 2},
        {2, 26},
        {44, 26},
        {26, 20},
        {44, 20},
        {44, 23},
        {23, 37},
        {11, 35},
        {35, 24},
        {11, 9},
        {9, 35},
        {0, 2},
        {0, 1},
        {1, 3},
        {0, 3},
        {0, 24},
        {0, 35},
        {35, 26},
        {0, 26},
        {10, 33},
        {33, 9},
        {10, 9},
        {10, 11},
        {11, 8},
        {10, 8},
        {8, 34},
        {43, 19},
        {19, 17},
        {43, 17},
        {17, 30},
        {43, 30},
        {30, 34},
        {43, 34},
        {42, 38},
        {38, 12},
        {15, 39},
        {39, 29},
        {15, 13},
        {13, 39},
        {36, 12},
        {12, 15},
        {36, 15},
        {15, 29},
        {36, 29},
        {29, 41},
        {36, 42},
        {42, 12},
        {5, 29},
        {5, 39},
        {5, 31},
        {31, 7},
        {14, 38},
        {14, 25},
        {14, 37},
        {14, 13},
        {14, 15},
        {14, 12},
        {32, 24},
        {32, 40},
        {40, 19},
        {32, 19},
        {32, 43},
        {43, 8},
        {32, 8},
        {32, 11},
        {11, 24},
        {46, 9},
        {46, 33},
        {46, 22},
        {46, 20},
        {46, 26},
        {46, 35},
        {28, 4},
        {4, 7},
        {28, 7},
        {28, 45},
        {28, 33},
        {28, 10},
        {10, 34},
        {28, 34},
        {16, 19},
        {18, 17},
        {18, 19},
        {18, 16},
        {16, 42},
        {18, 42},
        {18, 36},
        {36, 41},
        {18, 41},
        {6, 30},
        {6, 41},
        {6, 29},
        {6, 5},
        {5, 7},
        {6, 7},
        {6, 4},
        {27, 3},
        {27, 1},
        {1, 38},
        {27, 38},
        {27, 42},
        {27, 16},
        {16, 40},
        {27, 40},
        {40, 3},
    };
    std::vector<std::vector<int>> m1Spatial_48_Plane_Def = {
        {30, 4, 34},
        {17, 41, 30},
        {13, 47, 39},
        {13, 37, 47},
        {22, 33, 45},
        {22, 45, 21},
        {23, 20, 22},
        {23, 22, 21},
        {23, 21, 47},
        {23, 47, 37},
        {31, 39, 47},
        {31, 47, 21},
        {31, 21, 45},
        {31, 45, 7},
        {1, 25, 38},
        {1, 2, 25},
        {40, 24, 3},
        {44, 37, 25},
        {44, 25, 2},
        {44, 2, 26},
        {44, 26, 20},
        {44, 20, 23},
        {44, 23, 37},
        {11, 35, 24},
        {11, 9, 35},
        {0, 2, 1},
        {0, 1, 3},
        {0, 3, 24},
        {0, 24, 35},
        {0, 35, 26},
        {0, 26, 2},
        {10, 33, 9},
        {10, 9, 11},
        {10, 11, 8},
        {10, 8, 34},
        {43, 19, 17},
        {43, 17, 30},
        {43, 30, 34},
        {43, 34, 8},
        {42, 38, 12},
        {15, 39, 29},
        {15, 13, 39},
        {36, 12, 15},
        {36, 15, 29},
        {36, 29, 41},
        {36, 42, 12},
        {5, 29, 39},
        {5, 39, 31},
        {5, 31, 7},
        {14, 38, 25},
        {14, 25, 37},
        {14, 37, 13},
        {14, 13, 15},
        {14, 15, 12},
        {14, 12, 38},
        {32, 24, 40},
        {32, 40, 19},
        {32, 19, 43},
        {32, 43, 8},
        {32, 8, 11},
        {32, 11, 24},
        {46, 9, 33},
        {46, 33, 22},
        {46, 22, 20},
        {46, 20, 26},
        {46, 26, 35},
        {46, 35, 9},
        {28, 4, 7},
        {28, 7, 45},
        {28, 45, 33},
        {28, 33, 10},
        {28, 10, 34},
        {28, 34, 4},
        {16, 19, 40},
        {18, 17, 19},
        {18, 19, 16},
        {18, 16, 42},
        {18, 42, 36},
        {18, 36, 41},
        {18, 41, 17},
        {6, 30, 41},
        {6, 41, 29},
        {6, 29, 5},
        {6, 5, 7},
        {6, 7, 4},
        {6, 4, 30},
        {27, 3, 1},
        {27, 1, 38},
        {27, 38, 42},
        {27, 42, 16},
        {27, 16, 40},
        {27, 40, 3},
    };

    /// MACH1SPATIAL-60
    std::vector<std::vector<int>> m1Spatial_60_Lines_Def = {
        {6, 5},
        {5, 0},
        {6, 0},
        {0, 49},
        {58, 0},
        {58, 5},
        {5, 32},
        {58, 32},
        {32, 30},
        {55, 5},
        {20, 6},
        {6, 49},
        {21, 30},
        {30, 7},
        {21, 7},
        {7, 53},
        {29, 56},
        {56, 10},
        {37, 13},
        {13, 56},
        {39, 55},
        {55, 24},
        {16, 40},
        {40, 39},
        {16, 39},
        {39, 24},
        {31, 2},
        {2, 12},
        {31, 12},
        {12, 42},
        {31, 42},
        {42, 52},
        {8, 53},
        {33, 31},
        {31, 52},
        {46, 39},
        {50, 32},
        {32, 46},
        {50, 7},
        {50, 30},
        {44, 47},
        {47, 43},
        {44, 43},
        {43, 20},
        {44, 20},
        {20, 49},
        {44, 49},
        {49, 29},
        {23, 10},
        {10, 48},
        {23, 48},
        {48, 47},
        {23, 47},
        {23, 44},
        {44, 29},
        {23, 29},
        {29, 10},
        {28, 47},
        {28, 48},
        {48, 25},
        {27, 49},
        {27, 37},
        {37, 56},
        {27, 56},
        {27, 29},
        {45, 21},
        {21, 53},
        {45, 53},
        {53, 35},
        {45, 35},
        {35, 13},
        {45, 13},
        {38, 58},
        {58, 30},
        {38, 30},
        {38, 21},
        {38, 45},
        {45, 37},
        {26, 55},
        {55, 6},
        {26, 6},
        {26, 20},
        {26, 24},
        {19, 43},
        {19, 47},
        {19, 28},
        {28, 9},
        {1, 9},
        {9, 2},
        {1, 16},
        {16, 24},
        {36, 42},
        {36, 41},
        {41, 42},
        {59, 9},
        {59, 28},
        {28, 25},
        {59, 25},
        {25, 36},
        {59, 36},
        {36, 12},
        {59, 12},
        {59, 2},
        {11, 52},
        {52, 8},
        {11, 8},
        {8, 7},
        {11, 7},
        {11, 33},
        {33, 52},
        {54, 16},
        {54, 1},
        {1, 2},
        {54, 2},
        {54, 31},
        {54, 33},
        {33, 40},
        {54, 40},
        {15, 5},
        {15, 55},
        {15, 39},
        {15, 46},
        {15, 32},
        {17, 41},
        {17, 36},
        {34, 53},
        {34, 35},
        {14, 37},
        {14, 27},
        {27, 0},
        {14, 0},
        {14, 58},
        {14, 38},
        {38, 37},
        {51, 19},
        {19, 9},
        {51, 9},
        {51, 1},
        {1, 24},
        {51, 24},
        {51, 26},
        {26, 43},
        {51, 43},
        {22, 46},
        {46, 40},
        {22, 40},
        {22, 33},
        {22, 11},
        {11, 50},
        {22, 50},
        {50, 46},
        {4, 13},
        {4, 10},
        {4, 56},
        {18, 41},
        {41, 34},
        {18, 34},
        {34, 8},
        {18, 8},
        {18, 52},
        {18, 42},
        {57, 4},
        {4, 35},
        {57, 35},
        {57, 34},
        {57, 41},
        {3, 10},
        {3, 4},
        {3, 57},
        {57, 17},
        {3, 17},
        {17, 25},
        {3, 25},
        {3, 48},
    };
    std::vector<std::vector<int>> m1Spatial_60_Plane_Def = {
        {6, 5, 0},
        {6, 0, 49},
        {58, 0, 5},
        {58, 5, 32},
        {58, 32, 30},
        {55, 5, 6},
        {20, 6, 49},
        {21, 30, 7},
        {21, 7, 53},
        {29, 56, 10},
        {37, 13, 56},
        {39, 55, 24},
        {16, 40, 39},
        {16, 39, 24},
        {31, 2, 12},
        {31, 12, 42},
        {31, 42, 52},
        {8, 53, 7},
        {33, 31, 52},
        {46, 39, 40},
        {50, 32, 46},
        {50, 7, 30},
        {50, 30, 32},
        {44, 47, 43},
        {44, 43, 20},
        {44, 20, 49},
        {44, 49, 29},
        {23, 10, 48},
        {23, 48, 47},
        {23, 47, 44},
        {23, 44, 29},
        {23, 29, 10},
        {28, 47, 48},
        {28, 48, 25},
        {27, 49, 0},
        {27, 37, 56},
        {27, 56, 29},
        {27, 29, 49},
        {45, 21, 53},
        {45, 53, 35},
        {45, 35, 13},
        {45, 13, 37},
        {38, 58, 30},
        {38, 30, 21},
        {38, 21, 45},
        {38, 45, 37},
        {26, 55, 6},
        {26, 6, 20},
        {26, 20, 43},
        {26, 24, 55},
        {19, 43, 47},
        {19, 47, 28},
        {19, 28, 9},
        {1, 9, 2},
        {1, 16, 24},
        {36, 42, 12},
        {36, 41, 42},
        {59, 9, 28},
        {59, 28, 25},
        {59, 25, 36},
        {59, 36, 12},
        {59, 12, 2},
        {59, 2, 9},
        {11, 52, 8},
        {11, 8, 7},
        {11, 7, 50},
        {11, 33, 52},
        {54, 16, 1},
        {54, 1, 2},
        {54, 2, 31},
        {54, 31, 33},
        {54, 33, 40},
        {54, 40, 16},
        {15, 5, 55},
        {15, 55, 39},
        {15, 39, 46},
        {15, 46, 32},
        {15, 32, 5},
        {17, 41, 36},
        {17, 36, 25},
        {34, 53, 8},
        {34, 35, 53},
        {14, 37, 27},
        {14, 27, 0},
        {14, 0, 58},
        {14, 58, 38},
        {14, 38, 37},
        {51, 19, 9},
        {51, 9, 1},
        {51, 1, 24},
        {51, 24, 26},
        {51, 26, 43},
        {51, 43, 19},
        {22, 46, 40},
        {22, 40, 33},
        {22, 33, 11},
        {22, 11, 50},
        {22, 50, 46},
        {4, 13, 35},
        {4, 10, 56},
        {4, 56, 13},
        {18, 41, 34},
        {18, 34, 8},
        {18, 8, 52},
        {18, 52, 42},
        {18, 42, 41},
        {57, 4, 35},
        {57, 35, 34},
        {57, 34, 41},
        {57, 41, 17},
        {3, 10, 4},
        {3, 4, 57},
        {3, 57, 17},
        {3, 17, 25},
        {3, 25, 48},
        {3, 48, 10},
    };

    static std::map<OutputMode, std::vector<std::vector<int>>> lines = {
        {OUTPUT_HORIZON_4CH, m1Spatial_4_Lines_Def},
        {OUTPUT_SPATIAL_8CH, m1Spatial_8_Lines_Def},
        {OUTPUT_SPATIAL_12CH, m1Spatial_12_Lines_Def},
        {OUTPUT_SPATIAL_14CH, m1Spatial_14_Lines_Def},
        {OUTPUT_SPATIAL_32CH, m1Spatial_32_Lines_Def},
        {OUTPUT_SPATIAL_36CH, m1Spatial_36_Lines_Def},
        {OUTPUT_SPATIAL_48CH, m1Spatial_48_Lines_Def},
        {OUTPUT_SPATIAL_60CH, m1Spatial_60_Lines_Def},

    };

    static std::map<OutputMode, std::vector<std::vector<int>>> planes = {
        {OUTPUT_HORIZON_4CH, m1Spatial_4_Plane_Def},
        {OUTPUT_SPATIAL_8CH, m1Spatial_8_Plane_Def},
        {OUTPUT_SPATIAL_12CH, m1Spatial_12_Plane_Def},
        {OUTPUT_SPATIAL_14CH, m1Spatial_14_Plane_Def},
        {OUTPUT_SPATIAL_32CH, m1Spatial_32_Plane_Def},
        {OUTPUT_SPATIAL_36CH, m1Spatial_36_Plane_Def},
        {OUTPUT_SPATIAL_48CH, m1Spatial_48_Plane_Def},
        {OUTPUT_SPATIAL_60CH, m1Spatial_60_Plane_Def},
    };

    std::vector<std::vector<int>> linesSet = lines[outputMode];
    std::vector<std::vector<int>> planesSet = planes[outputMode];

    // simple clamp
    float minX = std::numeric_limits<float>::max(), maxX = std::numeric_limits<float>::min();
    float minY = std::numeric_limits<float>::max(), maxY = std::numeric_limits<float>::min();
    float minZ = std::numeric_limits<float>::max(), maxZ = std::numeric_limits<float>::min();

    for (int i = 0; i < pointsSet.size(); i++) {
        minX = (std::min)(minX, pointsSet[i].x);
        minY = (std::min)(minY, pointsSet[i].y);
        minZ = (std::min)(minZ, pointsSet[i].z);

        maxX = (std::max)(maxX, pointsSet[i].x);
        maxY = (std::max)(maxY, pointsSet[i].y);
        maxZ = (std::max)(maxZ, pointsSet[i].z);
    }

    x = clamp(x, minX, maxX);
    y = clamp(y, minY, maxY);
    z = clamp(z, minZ, maxZ);

    float dMin = std::numeric_limits<float>::max();
    float dMax = 0;
    float dMinIdx = 0;
    float dSum = 0;
    std::vector<float> distToPoins;
    for (int i = 0; i < pointsSet.size(); i++) {
        float d = sqrt(dist_sq(x, y, z, pointsSet[i].x, pointsSet[i].y, pointsSet[i].z));
        distToPoins.push_back(d);
        if (d < dMin) {
            dMin = d;
            dMinIdx = i;
        }
        if (d > dMax) {
            dMax = d;
        }
        dSum += d;
    }

    std::vector<float> distToLines;
    for (int i = 0; i < linesSet.size(); i++) {
        Mach1Point3DCore p1 = pointsSet[linesSet[i][0]];
        Mach1Point3DCore p2 = pointsSet[linesSet[i][1]];
        float d = distance_to_segment(x, y, z, p1.x, p1.y, p1.z, p2.x, p2.y, p2.z);
        distToLines.push_back(d);
    }

    std::vector<float> distToPlanes;
    for (int i = 0; i < planesSet.size(); i++) {
        Mach1Point3DCore p1 = pointsSet[planesSet[i][0]];
        Mach1Point3DCore p2 = pointsSet[planesSet[i][1]];
        Mach1Point3DCore p3 = pointsSet[planesSet[i][2]];
        float d = distance_to_plane(x, y, z, p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, p3.x, p3.y, p3.z);
        distToPlanes.push_back(d);
    }

    std::vector<float> gains;
    for (int i = 0; i < pointsSet.size(); i++) {
        float eps = 0.000001;
        float g = 0;
        if (dMin > 0) {
            float d = distToPoins[i];

            // v1
            g = 1 - d / dSum;
            g = g / (d * d);

            // v2
            // g = 1 / d;

            // v3
            /*
            {
                float eX = 1 - (pointsSet[i].x + 1) / 2;
                float eY = 1 - (pointsSet[i].y + 1) / 2;
                float eZ = 1 - (pointsSet[i].z + 1) / 2;

                float pX = (x + 1) / 2;
                float pY = (y + 1) / 2;
                float pZ = (z + 1) / 2;

                g = fabs(eX - pX) * fabs(eY - pY) * fabs(eZ - pZ);
                continue;
            }
            */

            if (d < eps) {
                g = std::numeric_limits<float>::max() * eps;
            } else {
                // signal shared between the two points, line mode
                {
                    float k = std::numeric_limits<float>::max();
                    for (int j = 0; j < linesSet.size(); j++) {
                        if (linesSet[j][0] == i || linesSet[j][1] == i) {
                            if (distToLines[j] < k)
                                k = distToLines[j];
                        }
                    }

                    if (k == 0)
                        k = eps; // fix if point on the line
                    g = g / k;
                }

                // signal shared between the planes, plane mode
                {
                    float k = std::numeric_limits<float>::max();
                    for (int j = 0; j < planesSet.size(); j++) {
                        if (planesSet[j][0] == i || planesSet[j][1] == i || planesSet[j][2] == i) {
                            if (distToPlanes[j] < k)
                                k = distToPlanes[j];
                        }
                    }

                    if (k == 0)
                        k = eps; // fix if point on the line
                    g = g / k;
                }
            }
        } else {
            g = (i == dMinIdx ? 1 : 0);
        }
        gains.push_back(g);
    }

    // normalize gains
    float gSum = 0;
    for (int i = 0; i < gains.size(); i++) {
        gSum += gains[i];
    }

    for (int i = 0; i < gains.size(); i++) {
        gains[i] = gains[i] / gSum;
    }

    // use gains normalization for center point
    float dist_to_center = clamp(sqrt(dist_sq(x, y, z, 0, 0, 0)) / 0.5, 0, 1);
    // dist_to_center = 1 - powf(1 - dist_to_center, 5); // easeOutQuint
    for (int i = 0; i < gains.size(); i++) {
        gains[i] = map_(dist_to_center, 0, 1, 1.0 / pointsSet.size(), gains[i]);
    }

    result = gains;
    // result = getCoeffSetForStandardPointSet(x, y, z, pointsSet, outputMode == OUTPUT_HORIZON_4CH ? true : false);
}

int M1EncodeCore::getInputModeFromString(std::string name) {
    for (int i = 0; i < inputModeNames.size(); i++) {
        if (inputModeNames[i].name == name) {
            return inputModeNames[i].mode;
        }
    }
    return -1;
}

int M1EncodeCore::getOutputModeFromString(std::string name) {
    for (int i = 0; i < outputModeNames.size(); i++) {
        if (outputModeNames[i].name == name) {
            return outputModeNames[i].mode;
        }
    }
    return -1;
}

M1EncodeCore::M1EncodeCore() {
    inputMode = InputMode::INPUT_MONO;
    outputMode = OutputMode::OUTPUT_SPATIAL_8CH;
    pannerMode = PannerMode::MODE_ISOTROPICLINEAR;

    azimuth = 0;
    diverge = 0;
    elevation = 0;
    frontSurroundPerspective = true; // default set surround formats to be front first person perspective

    orbitRotation = 0;
    sSpread = 0;
    autoOrbit = false;

    ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    timeLastCalculation = 0;

    outputGainLinearMultipler = 1.0;

    // init additional arrays
    arr_Points = new Mach1Point3DCore[MAX_POINTS_COUNT];

    arr_Gains = new float *[MAX_POINTS_COUNT];
    for (int i = 0; i < MAX_POINTS_COUNT; i++) {
        arr_Gains[i] = new float[MAX_CHANNELS_COUNT];
    }

    arr_PointsNames = new char *[MAX_POINTS_COUNT];
    for (int i = 0; i < MAX_POINTS_COUNT; i++) {
        arr_PointsNames[i] = new char[255];
        arr_PointsNames[i][0] = '\0';
    }

    arr_GainsForInputChannelNamed = new float[MAX_CHANNELS_COUNT];
    arr_ResultingCoeffsDecoded = new float[MAX_CHANNELS_COUNT];
}

M1EncodeCore::~M1EncodeCore() {
    delete[] arr_Points;

    for (int i = 0; i < MAX_POINTS_COUNT; i++) {
        delete[] arr_Gains[i];
    }
    delete[] arr_Gains;

    for (int i = 0; i < MAX_POINTS_COUNT; i++) {
        delete[] arr_PointsNames[i];
    }
    delete[] arr_PointsNames;

    delete[] arr_GainsForInputChannelNamed;

    delete[] arr_ResultingCoeffsDecoded;
}

// Copy constructor
M1EncodeCore::M1EncodeCore(const M1EncodeCore& other) {
    inputMode = other.inputMode;
    outputMode = other.outputMode;
    pannerMode = other.pannerMode;
    azimuth = other.azimuth;
    diverge = other.diverge;
    elevation = other.elevation;
    frontSurroundPerspective = other.frontSurroundPerspective;
    orbitRotation = other.orbitRotation;
    sSpread = other.sSpread;
    autoOrbit = other.autoOrbit;
    ms = other.ms;
    timeLastCalculation = other.timeLastCalculation;
    outputGainLinearMultipler = other.outputGainLinearMultipler;

    // Copy additional arrays
    arr_Points = new Mach1Point3DCore[MAX_POINTS_COUNT];
    std::copy(other.arr_Points, other.arr_Points + MAX_POINTS_COUNT, arr_Points);

    arr_Gains = new float*[MAX_POINTS_COUNT];
    for (int i = 0; i < MAX_POINTS_COUNT; i++) {
        arr_Gains[i] = new float[MAX_CHANNELS_COUNT];
        std::copy(other.arr_Gains[i], other.arr_Gains[i] + MAX_CHANNELS_COUNT, arr_Gains[i]);
    }

    arr_PointsNames = new char*[MAX_POINTS_COUNT];
    for (int i = 0; i < MAX_POINTS_COUNT; i++) {
        arr_PointsNames[i] = new char[255];
        strcpy(arr_PointsNames[i], other.arr_PointsNames[i]);
    }

    std::copy(other.arr_GainsForInputChannelNamed, other.arr_GainsForInputChannelNamed + MAX_CHANNELS_COUNT, arr_GainsForInputChannelNamed);
    std::copy(other.arr_ResultingCoeffsDecoded, other.arr_ResultingCoeffsDecoded + MAX_CHANNELS_COUNT, arr_ResultingCoeffsDecoded);
}

// Copy assignment operator
M1EncodeCore& M1EncodeCore::operator=(const M1EncodeCore& other) {
    if (this != &other) {
        inputMode = other.inputMode;
        outputMode = other.outputMode;
        pannerMode = other.pannerMode;
        azimuth = other.azimuth;
        diverge = other.diverge;
        elevation = other.elevation;
        frontSurroundPerspective = other.frontSurroundPerspective;
        orbitRotation = other.orbitRotation;
        sSpread = other.sSpread;
        autoOrbit = other.autoOrbit;
        ms = other.ms;
        timeLastCalculation = other.timeLastCalculation;
        outputGainLinearMultipler = other.outputGainLinearMultipler;

        // Copy additional arrays
        std::copy(other.arr_Points, other.arr_Points + MAX_POINTS_COUNT, arr_Points);

        for (int i = 0; i < MAX_POINTS_COUNT; i++) {
            std::copy(other.arr_Gains[i], other.arr_Gains[i] + MAX_CHANNELS_COUNT, arr_Gains[i]);
        }

        for (int i = 0; i < MAX_POINTS_COUNT; i++) {
            strcpy(arr_PointsNames[i], other.arr_PointsNames[i]);
        }

        std::copy(other.arr_GainsForInputChannelNamed, other.arr_GainsForInputChannelNamed + MAX_CHANNELS_COUNT, arr_GainsForInputChannelNamed);
        std::copy(other.arr_ResultingCoeffsDecoded, other.arr_ResultingCoeffsDecoded + MAX_CHANNELS_COUNT, arr_ResultingCoeffsDecoded);
    }
    return *this;
}

void M1EncodeCore::generatePointResults() {
    long tStart = getCurrentTime();

    if (pannerMode != MODE_ISOTROPICLINEAR && pannerMode != MODE_ISOTROPICEQUALPOWER) {
        pannerMode = MODE_PERIPHONICLINEAR;
    }

    Mach1Point3DCore centerpoint;

    float normalisedOutputDiverge = diverge * (1 / cos(PI * 0.25f));
    centerpoint = {(float)cos((azimuth)*PI * 2) * normalisedOutputDiverge, 0, (float)sin((azimuth)*PI * 2) * normalisedOutputDiverge};

    if (inputMode == INPUT_MONO) {

        resultingPoints.pointsCount = 1;
        resultingPoints.pointsNames[0] = "M";

        if (outputMode == OUTPUT_HORIZON_4CH) {
            resultingPoints.ppoints[0] = centerpoint;
        } else {
            if (pannerMode == MODE_ISOTROPICLINEAR || pannerMode == MODE_ISOTROPICEQUALPOWER) {
                resultingPoints.ppoints[0] = {centerpoint.x * (float)sin((-elevation + 1) * PI / 2), (float)cos((-elevation + 1) * PI / 2) * normalisedOutputDiverge, centerpoint.z * (float)sin((-elevation + 1) * PI / 2)};
            } else if (pannerMode == MODE_PERIPHONICLINEAR) {
                resultingPoints.ppoints[0] = {centerpoint.x, elevation, centerpoint.z};
            }
        }
    } else if (inputMode == INPUT_STEREO) {

        float sRotationInRadians;
        if (autoOrbit) {
            sRotationInRadians = azimuth * PI * 2 - PI / 2;
        } else {
            sRotationInRadians = orbitRotation * PI * 2 - PI / 2;
        }

        resultingPoints.pointsCount = 2;

        std::vector<std::string> names = {"L", "R"};
        std::vector<Mach1Point3DCore> pnts = {
            {(float)cos((sRotationInRadians)) * sSpread, 0, (float)sin((sRotationInRadians)) * sSpread},
            {(float)-cos((sRotationInRadians)) * sSpread, 0, (float)-sin((sRotationInRadians)) * sSpread}};

        for (int i = 0; i < resultingPoints.pointsCount; i++) {
            resultingPoints.pointsNames[i] = names[i];
            if (outputMode == OUTPUT_HORIZON_4CH) {
                resultingPoints.ppoints[i] = pnts[i] + centerpoint;
            } else if (pannerMode == MODE_ISOTROPICLINEAR || pannerMode == MODE_ISOTROPICEQUALPOWER) {
                resultingPoints.ppoints[i] = pnts[i] + Mach1Point3DCore{centerpoint.x * (float)sin((elevation + 1) * PI / 2), elevation, centerpoint.z * (float)sin((elevation + 1) * PI / 2)};
            } else if (pannerMode == MODE_PERIPHONICLINEAR) {
                resultingPoints.ppoints[i] = pnts[i] + Mach1Point3DCore{centerpoint.x, elevation, centerpoint.z};
            }
        }
    } else if (inputMode == INPUT_QUAD) {

        resultingPoints.pointsCount = 4;

        std::vector<std::string> names = {"L", "R", "Ls", "Rs"};
        std::vector<Mach1Point3DCore> pnts = {
            {(float)cos((azimuth + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge},
        };

        for (int i = 0; i < resultingPoints.pointsCount; i++) {
            resultingPoints.pointsNames[i] = names[i];
            resultingPoints.ppoints[i] = pnts[i];
            if (outputMode == OUTPUT_HORIZON_4CH) {
                resultingPoints.ppoints[i].y = 0;
            }
        }
    } else if (inputMode == INPUT_LCRS) {

        resultingPoints.pointsCount = 4;

        std::vector<std::string> names = {"L", "C", "R", "S"};
        if (frontSurroundPerspective) {
            centerpoint = {(float)cos((azimuth + 0.0f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.0f) * PI * 2) * diverge};
        } else {
            centerpoint = {0, 0, 0};
        }
        std::vector<Mach1Point3DCore> pnts = {
            {(float)cos((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge},
            centerpoint,
            {(float)cos((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.5f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.5f) * PI * 2) * diverge},
        };
        pnts[1] = (pnts[0] + pnts[2]) / 2;
        pnts[3] = -pnts[1];

        for (int i = 0; i < resultingPoints.pointsCount; i++) {
            resultingPoints.pointsNames[i] = names[i];
            resultingPoints.ppoints[i] = pnts[i];
            if (outputMode == OUTPUT_HORIZON_4CH) {
                resultingPoints.ppoints[i].y = 0;
            }
        }

    } else if (inputMode == INPUT_AFORMAT) {

        resultingPoints.pointsCount = 4;

        std::vector<std::string> names = {"FLU", "FRD", "BLD", "BRU"};
        std::vector<Mach1Point3DCore> pnts = {
            {(float)cos((azimuth + 0.125f - 0.25f) * PI * 2) * diverge, (1 * diverge), (float)sin((azimuth + 0.125f - 0.25f) * PI * 2) * diverge},
            {(float)cos((azimuth + 0.125f) * PI * 2) * diverge, (-1 * diverge), (float)sin((azimuth + 0.125f) * PI * 2) * diverge},
            {(float)cos((azimuth + 0.125f + 0.5f) * PI * 2) * diverge, (-1 * diverge), (float)sin((azimuth + 0.125f + 0.5f) * PI * 2) * diverge},
            {(float)cos((azimuth + 0.125f + 0.25f) * PI * 2) * diverge, (1 * diverge), (float)sin((azimuth + 0.125f + 0.25f) * PI * 2) * diverge},
        };

        for (int i = 0; i < resultingPoints.pointsCount; i++) {
            resultingPoints.pointsNames[i] = names[i];
            resultingPoints.ppoints[i] = pnts[i];
            if (outputMode == OUTPUT_HORIZON_4CH) {
                resultingPoints.ppoints[i].y = 0;
            }
        }

    } else if (inputMode == INPUT_1OAACN) { // duplicate?

        resultingPoints.pointsCount = 7;

        std::vector<std::string> names = {"W", "1", "2", "3", "-1", "-2", "-3"};
        std::vector<Mach1Point3DCore> pnts = {
            {0, 0, 0},                                                                                                 // W
            {(float)cos((azimuth + 0.250f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.250f) * PI * 2) * diverge}, // Left/Right / L
            {0, 1.0f * diverge, 0},                                                                                    // Top/Bottom / T
            {(float)cos((azimuth + 0.0f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.0f) * PI * 2) * diverge},     // Front/Back / F
            {(float)cos((azimuth + 0.75f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.75f) * PI * 2) * diverge},   // -Left/Right / R
            {0, -1.0f * diverge, 0},                                                                                   // -Top/Bottom / B
            {(float)cos((azimuth + 0.5f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.5f) * PI * 2) * diverge},     // -Front/Back / B
        };

        /*
        X = left/right angle
        Y = up/down angle
        Z = front/back angle
         */

        for (int i = 0; i < resultingPoints.pointsCount; i++) {
            resultingPoints.pointsNames[i] = names[i];
            resultingPoints.ppoints[i] = pnts[i];
            if (outputMode == OUTPUT_HORIZON_4CH) {
                resultingPoints.ppoints[i].y = 0;
            }
        }

    } else if (inputMode == INPUT_1OAFUMA) {

        resultingPoints.pointsCount = 7;

        std::vector<std::string> names = {"W", "X", "Y", "Z", "-X", "-Y", "-Z"};
        std::vector<Mach1Point3DCore> pnts = {
            {0, 0, 0},                                                                                               // W
            {(float)cos((azimuth + 0.0f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.0f) * PI * 2) * diverge},   // Front/Back / F
            {(float)cos((azimuth + 0.25f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.25f) * PI * 2) * diverge}, // Left/Right / L
            {0, 1.0f * diverge, 0},                                                                                  // Top/Bottom / T
            {(float)cos((azimuth + 0.5f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.5f) * PI * 2) * diverge},   // -Front/Back / B
            {(float)cos((azimuth + 0.75f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.75f) * PI * 2) * diverge}, // -Left/Right / R
            {0, -1.0f * diverge, 0},                                                                                 // -Top/Bottom / B
        };

        for (int i = 0; i < resultingPoints.pointsCount; i++) {
            resultingPoints.pointsNames[i] = names[i];
            resultingPoints.ppoints[i] = pnts[i];
            if (outputMode == OUTPUT_HORIZON_4CH) {
                resultingPoints.ppoints[i].y = 0;
            }
        }
    } else if (inputMode == INPUT_2OAACN || inputMode == INPUT_2OAFUMA || inputMode == INPUT_3OAACN || inputMode == INPUT_3OAFUMA) { // duplicate?

        /*
        TODO: Rework this into something smarter
        Currently expects 2OA and 3OA conversion to Mach1Spatial8 externally from this API
        This API will just supply the azimuths for Mach1Spatial8 Cuboid
        */

        resultingPoints.pointsCount = 8;

        std::vector<std::string> names = {"1", "2", "3", "4", "5", "6", "7", "8"};
        std::vector<Mach1Point3DCore> pnts = {
            {(float)cos((azimuth + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge, 1.0f * diverge, (float)sin((azimuth + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge, 1.0f * diverge, (float)sin((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge, 1.0f * diverge, (float)sin((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge, 1.0f * diverge, (float)sin((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge, -1.0f * diverge, (float)sin((azimuth + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge, -1.0f * diverge, (float)sin((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge, -1.0f * diverge, (float)sin((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge, -1.0f * diverge, (float)(float)sin((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge},
        };

        for (int i = 0; i < resultingPoints.pointsCount; i++) {
            resultingPoints.pointsNames[i] = names[i];
            resultingPoints.ppoints[i] = pnts[i];
            if (outputMode == OUTPUT_HORIZON_4CH) {
                resultingPoints.ppoints[i].y = 0;
            }
        }
    } else if (inputMode == INPUT_LCR) {

        resultingPoints.pointsCount = 3;

        std::vector<std::string> names = {"L", "C", "R"};
        if (frontSurroundPerspective) {
            centerpoint = {(float)cos((azimuth + 0.0f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.0f) * PI * 2) * diverge};
        } else {
            centerpoint = {0, 0, 0};
        }
        std::vector<Mach1Point3DCore> pnts = {
            {(float)cos((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge},
            centerpoint,
            {(float)cos((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge},
        };
        pnts[1] = (pnts[0] + pnts[2]) / 2;

        for (int i = 0; i < resultingPoints.pointsCount; i++) {
            resultingPoints.pointsNames[i] = names[i];
            resultingPoints.ppoints[i] = pnts[i];
            if (outputMode == OUTPUT_HORIZON_4CH) {
                resultingPoints.ppoints[i].y = 0;
            }
        }
    } else if (inputMode == INPUT_FIVE_ZERO) {

        resultingPoints.pointsCount = 5;

        std::vector<std::string> names = {"L", "C", "R", "Ls", "Rs"};
        if (frontSurroundPerspective) {
            centerpoint = {(float)cos((azimuth + 0.0f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.0f) * PI * 2) * diverge};
        } else {
            centerpoint = {0, 0, 0};
        }
        std::vector<Mach1Point3DCore> pnts = {
            {(float)cos((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge},
            centerpoint,
            {(float)cos((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge},
        };
        pnts[1] = (pnts[0] + pnts[2]) / 2;

        for (int i = 0; i < resultingPoints.pointsCount; i++) {
            resultingPoints.pointsNames[i] = names[i];
            resultingPoints.ppoints[i] = pnts[i];
            if (outputMode == OUTPUT_HORIZON_4CH) {
                resultingPoints.ppoints[i].y = 0;
            }
        }
    } else if (inputMode == INPUT_FIVE_ONE_FILM) {

        resultingPoints.pointsCount = 6;

        std::vector<std::string> names = {"L", "C", "R", "Ls", "Rs", "LFE"};
        if (frontSurroundPerspective) {
            centerpoint = {(float)cos((azimuth + 0.0f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.0f) * PI * 2) * diverge};
        } else {
            centerpoint = {0, 0, 0};
        }
        std::vector<Mach1Point3DCore> pnts = {
            {(float)cos((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge},
            centerpoint,
            {(float)cos((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge},
            {0, 0, 0},
        };
        pnts[1] = (pnts[0] + pnts[2]) / 2;

        for (int i = 0; i < resultingPoints.pointsCount; i++) {
            resultingPoints.pointsNames[i] = names[i];
            resultingPoints.ppoints[i] = pnts[i];
            if (outputMode == OUTPUT_HORIZON_4CH) {
                resultingPoints.ppoints[i].y = 0;
            }
        }
    } else if (inputMode == INPUT_FIVE_ONE_DTS) {

        resultingPoints.pointsCount = 6;

        std::vector<std::string> names = {"L", "R", "Ls", "Rs", "C", "LFE"};
        if (frontSurroundPerspective) {
            centerpoint = {(float)cos((azimuth + 0.0f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.0f) * PI * 2) * diverge};
        } else {
            centerpoint = {0, 0, 0};
        }
        std::vector<Mach1Point3DCore> pnts = {
            {(float)cos((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge},
            centerpoint,
            {0, 0, 0},
        };
        pnts[4] = (pnts[0] + pnts[1]) / 2;

        for (int i = 0; i < resultingPoints.pointsCount; i++) {
            resultingPoints.pointsNames[i] = names[i];
            resultingPoints.ppoints[i] = pnts[i];
            if (outputMode == OUTPUT_HORIZON_4CH) {
                resultingPoints.ppoints[i].y = 0;
            }
        }
    } else if (inputMode == INPUT_FIVE_ONE_SMPTE) {

        resultingPoints.pointsCount = 6;

        std::vector<std::string> names = {"L", "R", "C", "LFE", "Ls", "Rs"};
        if (frontSurroundPerspective) {
            centerpoint = {(float)cos((azimuth + 0.0f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.0f) * PI * 2) * diverge};
        } else {
            centerpoint = {0, 0, 0};
        }
        std::vector<Mach1Point3DCore> pnts = {
            {(float)cos((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge},
            centerpoint,
            {0, 0, 0},
            {(float)cos((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge},
        };
        pnts[2] = (pnts[0] + pnts[1]) / 2;

        for (int i = 0; i < resultingPoints.pointsCount; i++) {
            resultingPoints.pointsNames[i] = names[i];
            resultingPoints.ppoints[i] = pnts[i];
            if (outputMode == OUTPUT_HORIZON_4CH) {
                resultingPoints.ppoints[i].y = 0;
            }
        }
    }

    // Generating channel gains

    resultingPoints.gains.resize(resultingPoints.pointsCount);
    for (int i = 0; i < resultingPoints.pointsCount; i++) {

        resultingPoints.gains[i].resize(getOutputChannelsCount());

        // Generating gains
        std::vector<float> gains;
        if (outputMode == OUTPUT_HORIZON_4CH) {
            // force ppoints to center of soundfield's elevation
            processGainsChannels(resultingPoints.ppoints[i].z, resultingPoints.ppoints[i].x, 0, gains);
        } else {
            // process 3d ppoints
            processGainsChannels(resultingPoints.ppoints[i].z, resultingPoints.ppoints[i].x, resultingPoints.ppoints[i].y, gains);
        }
        // applying output gain to gains and assigning to the current index channel
        for (int j = 0; j < getOutputChannelsCount(); j++) {
            resultingPoints.gains[i][j] = gains[j] * outputGainLinearMultipler;
        }
    }
    timeLastCalculation = getCurrentTime() - tStart;
}

void M1EncodeCore::getResultingCoeffsDecoded(Mach1DecodeAlgoType decodeType, float *decodeResult, float *result) {
    // clear
    for (int i = 0; i < resultingPoints.pointsCount * 2; i++)
        result[i] = 0;

    // TODO: check on these numbers

    int decodeResultSize = 0;
    switch (decodeType) {
    case Mach1DecodeAlgoSpatial_8:
        decodeResultSize = 16;
        break;
    case Mach1DecodeAlgoHorizon_4:
        decodeResultSize = 8;
        break;
    case Mach1DecodeAlgoHorizonPairs:
        decodeResultSize = 8;
        break;
    case Mach1DecodeAlgoSpatial_12:
        decodeResultSize = 24;
        break;
    case Mach1DecodeAlgoSpatial_14:
        decodeResultSize = 28;
        break;
    case Mach1DecodeAlgoSpatial_32:
        decodeResultSize = 64;
        break;
    case Mach1DecodeAlgoSpatial_36:
        decodeResultSize = 72;
        break;
    case Mach1DecodeAlgoSpatial_48:
        decodeResultSize = 96;
        break;
    case Mach1DecodeAlgoSpatial_60:
        decodeResultSize = 120;
        break;
    default:
        break;
    }

    // decode - 8, 16
    if (getOutputChannelsCount() * 2 != decodeResultSize) {
        std::cout << "[MACH1] Warning: The Mach1EncodeOutputModeType in use is not suitable for the Mach1DecodeAlgoType selected" << std::endl;
    }

    for (int j = 0; j < resultingPoints.pointsCount; j++) {
        for (int i = 0; i < getOutputChannelsCount(); i++) {
            result[j * 2 + 0] += decodeResult[i * 2 + 0] * resultingPoints.gains[j][i]; // left
            result[j * 2 + 1] += decodeResult[i * 2 + 1] * resultingPoints.gains[j][i]; // right
        }
    }
}

M1EncodeCore::InputMode M1EncodeCore::getInputMode() {
    return inputMode;
}

M1EncodeCore::OutputMode M1EncodeCore::getOutputMode() {
    return outputMode;
}

int M1EncodeCore::getInputChannelsCount() {
    switch (inputMode) {
    case INPUT_MONO:
        return 1;
    case INPUT_STEREO:
        return 2;
    case INPUT_LCR:
        return 3;
    case INPUT_QUAD:
        return 4;
    case INPUT_LCRS:
        return 4;
    case INPUT_AFORMAT:
        return 4;
    case INPUT_FIVE_ZERO:
        return 5;
    case INPUT_FIVE_ONE_FILM:
        return 6;
    case INPUT_FIVE_ONE_DTS:
        return 6;
    case INPUT_FIVE_ONE_SMPTE:
        return 6;
    case INPUT_1OAACN:
        return 4;
    case INPUT_1OAFUMA:
        return 4;
    case INPUT_2OAACN:
        return 9;
    case INPUT_2OAFUMA:
        return 9;
    case INPUT_3OAACN:
        return 16;
    case INPUT_3OAFUMA:
        return 16;
    }
    return 0;
}

int M1EncodeCore::getOutputChannelsCount() {
    switch (outputMode) {
    case OUTPUT_SPATIAL_8CH:
        return 8;
    case OUTPUT_HORIZON_4CH:
        return 4;
    case OUTPUT_SPATIAL_12CH:
        return 12;
    case OUTPUT_SPATIAL_14CH:
        return 14;
    case OUTPUT_SPATIAL_32CH:
        return 32;
    case OUTPUT_SPATIAL_36CH:
        return 36;
    case OUTPUT_SPATIAL_48CH:
        return 48;
    case OUTPUT_SPATIAL_60CH:
        return 60;
    }
    return 0;
}

void M1EncodeCore::setInputMode(InputMode inputMode) {
    this->inputMode = inputMode;
}

void M1EncodeCore::setOutputMode(OutputMode outputMode) {
    this->outputMode = outputMode;
}

void M1EncodeCore::setAzimuth(float azimuthFromMinus1To1) {
    this->azimuth = azimuthFromMinus1To1;
}

void M1EncodeCore::setAzimuthDegrees(float azimuthDegrees) {
    azimuthDegrees = fmod(azimuthDegrees, 360.0); // protect a 360 cycle
    if (azimuthDegrees < 0) {                     // check if -180 to 180, convert to 0-360
        azimuthDegrees += 360.0;
    }
    this->azimuth = azimuthDegrees / 360.0;
}

void M1EncodeCore::setAzimuthRadians(float azimuthRadians) {
    azimuthRadians = fmod(azimuthRadians, PI * 2.0); // protect a 360 cycle
    if (azimuthRadians < 0.0) {                      // check if -180 to 180, convert to 0-360
        azimuthRadians += PI * 2.0;
    }
    this->azimuth = azimuthRadians / (PI * 2.0);
}

void M1EncodeCore::setDiverge(float divergeFromMinus1To1) {
    divergeFromMinus1To1 = clamp(divergeFromMinus1To1, -1, 1);
    this->diverge = divergeFromMinus1To1;
}

void M1EncodeCore::setElevation(float elevationFromMinus1to1) {
    elevationFromMinus1to1 = clamp(elevationFromMinus1to1, -1, 1);
    this->elevation = elevationFromMinus1to1;
}

void M1EncodeCore::setElevationDegrees(float elevationFromMinus90to90) {
    elevationFromMinus90to90 = clamp(elevationFromMinus90to90, -90, 90);
    this->elevation = elevationFromMinus90to90 / 90;
}

void M1EncodeCore::setElevationRadians(float elevationFromMinusHalfPItoHalfPI) {
    elevationFromMinusHalfPItoHalfPI = clamp(elevationFromMinusHalfPItoHalfPI, -PI / 2, PI / 2);
    this->elevation = elevationFromMinusHalfPItoHalfPI / (PI / 2);
}

void M1EncodeCore::setPannerMode(PannerMode pannerMode) {
    this->pannerMode = pannerMode;
}

void M1EncodeCore::setFrontSurroundPerspective(bool frontSurroundPerspective) {
    this->frontSurroundPerspective = frontSurroundPerspective;
}

void M1EncodeCore::setOutputGain(float outputGainMultipler, bool isDecibel) {
    if (isDecibel) {
        this->outputGainLinearMultipler = std::pow(10.0f, outputGainMultipler / 20.0f);
    } else {
        this->outputGainLinearMultipler = outputGainMultipler;
    }
}

void M1EncodeCore::setAutoOrbit(bool autoOrbit) {
    this->autoOrbit = autoOrbit;
}

void M1EncodeCore::setOrbitRotation(float orbitRotationFromMinusOnetoOne) {
    this->orbitRotation = orbitRotationFromMinusOnetoOne;
}

void M1EncodeCore::setOrbitRotationDegrees(float orbitRotationDegrees) {
    orbitRotationDegrees = fmod(orbitRotationDegrees, 360.0); // protect a 360 cycle
    if (orbitRotationDegrees < 0) {                           // check if -180 to 180, convert to 0-360
        orbitRotationDegrees += 360.0;
    }
    this->orbitRotation = orbitRotationDegrees / 360.0;
}

void M1EncodeCore::setOrbitRotationRadians(float orbitRotationRadians) {
    orbitRotationRadians = fmod(orbitRotationRadians, PI * 2.0); // protect a 360 cycle
    if (orbitRotationRadians < 0.0) {                            // check if -180 to 180, convert to 0-360
        orbitRotationRadians += PI * 2.0;
    }
    this->orbitRotation = orbitRotationRadians / (PI * 2.0);
}

void M1EncodeCore::setStereoSpread(float sSpreadFrom0to1) {
    sSpreadFrom0to1 = clamp(sSpreadFrom0to1, -1, 1);
    this->sSpread = sSpreadFrom0to1;
}

long M1EncodeCore::getCurrentTime() {
    return (long)(duration_cast<milliseconds>(system_clock::now().time_since_epoch()) - ms).count();
}

long M1EncodeCore::getLastCalculationTime() {
    return timeLastCalculation;
}
