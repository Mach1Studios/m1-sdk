//  Mach1 Spatial SDK
//  Copyright © 2017 Mach1. All rights reserved.

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
#include <cstring>
#include <map>
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

bool segments_intersection(Mach1Point3D a_first, Mach1Point3D a_second, Mach1Point3D b_first, Mach1Point3D b_second, Mach1Point3D &ip) {
    Mach1Point3D da = a_second - a_first;
    Mach1Point3D db = b_second - b_first;
    Mach1Point3D dc = b_first - a_first;

    if (da.dot(dc, da.getCrossed(db)) != 0.0) // lines are not coplanar
        return false;

    float s = da.dot(dc.getCrossed(db), da.getCrossed(db)) / powf(da.getCrossed(db).length(), 2);
    if (s >= 0.0 && s <= 1.0) {
        ip = a_first + da * Mach1Point3D{s, s, s};
        return true;
    }

    return false;
}

bool segment_intersect_face(Mach1Point3D p1line, Mach1Point3D p2line, Mach1Point3D p1, Mach1Point3D p2, Mach1Point3D p3, Mach1Point3D &point) {
    Mach1Point3D e0 = p2 - p1;
    Mach1Point3D e1 = p3 - p1;

    Mach1Point3D dir = p2line - p1line;
    Mach1Point3D dir_norm = dir.getNormalized();

    Mach1Point3D h = dir_norm.getCrossed(e1);
    const float a = h.dot(e0, h);

    float eps = 0.00001;

    if (a > -eps && a < eps) {
        return false;
    }

    Mach1Point3D s = p1line - p1;
    const float f = 1.0f / a;
    const float u = f * s.dot(s, h);

    if (u < 0.0f || u > 1.0f) {
        return false;
    }

    Mach1Point3D q = s.getCrossed(e0);
    const float v = f * q.dot(dir_norm, q);

    if (v < 0.0f || u + v > 1.0f) {
        return false;
    }

    const float t = f * q.dot(e1, q);

    if (t > eps && t < sqrtf(q.dot(dir, dir))) { // segment intersection
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

std::vector<Mach1Point3D> M1EncodeCorePointResults::getChannelPoints() {
    return std::vector<Mach1Point3D>(ppoints, std::end(ppoints));
}

std::vector<std::vector<float> > M1EncodeCorePointResults::getGains() {
    return gains;
}

std::vector<std::string> M1EncodeCorePointResults::getChannelPointsNames() {
    return std::vector<std::string>(pointsNames, std::end(pointsNames));
}

std::vector<float> M1EncodeCorePointResults::getGainsForInputChannelByName(std::string pointName) {
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

// Helper method to assign names & points to resultingPoints, applying the 4CH Y=0 flatten if needed
void M1EncodeCore::assignResultingPointsNamesAndCoordinates(
    const std::vector<std::string> &names,
    const std::vector<Mach1Point3D> &pnts)
{
    for (int i = 0; i < resultingPoints.pointsCount; i++) {
        resultingPoints.pointsNames[i] = names[i];
        resultingPoints.ppoints[i] = pnts[i];
        if (outputMode == OUTPUT_SPATIAL_4CH) {
            resultingPoints.ppoints[i].y = 0.0f;
        }
    }
}

// TODO: Refactor all functions to use this and finalize this design
float M1EncodeCore::getCoeffForChannelPoint(float x, float y, float z, Mach1Point3D point, bool ignoreZ) {
    // map from [-1,1] to [0,1]
    point.x = (point.x / (1 / 0.707) + 1) / 2;
    point.y = (point.y / (1 / 0.707) + 1) / 2;
    point.z = (point.z / (1 / 0.707) + 1) / 2;

    // Calculate distance from center point (0.5, 0.5, 0.5) in normalized space
    float centerX = 0.5f, centerY = 0.5f, centerZ = 0.5f;
    float distFromCenter = pow(
        pow(x - centerX, 2.0) + 
        pow(y - centerY, 2.0) + 
        pow(z - centerZ, 2.0), 
        0.5
    );

    // Calculate max possible distance from center to corners
    // In a normalized unit cube [0,1], max distance from center to corner is sqrt(3)/2
    float maxDistFromCenter = 0.866f; // sqrt(3)/2 ≈ 0.866
    
    // Normalize distance from center to [0,1] range
    float normalizedDistFromCenter = distFromCenter / maxDistFromCenter;
    
    // Calculate point-to-point distance for regular coefficient
    float dist = pow(pow(point.x - x, 2.0) + pow(point.y - y, 2.0) + pow(point.z - z, 2.0), 0.5);
    dist = 1 - dist;
    dist = (dist - 0.6) / 0.4;
    dist = clamp(dist, 0, 1);

    // Apply isotropic equal power curve if enabled
    if (pannerMode == MODE_ISOTROPICEQUALPOWER) {
        // Apply equal power curve
        dist = sqrt(1 - pow(dist - 1, 2));
        
        // Apply center-based gain compensation
        // As we approach center (normalizedDistFromCenter = 0), add up to +6dB gain
        float centerGainDB = 6.0f * (1.0f - normalizedDistFromCenter);
        float centerGainMultiplier = pow(10.0f, centerGainDB / 20.0f);
        dist *= centerGainMultiplier;
    }

    return dist;
}

void M1EncodeCore::processGains(float x, float y, float z, std::vector<float> &result) {

    /*
     * X = Left(-) to Right(+) (from a top down perspective)
     * Y = Front(+) to Back(-) (from a top down perspective)
     * Z = Top(+) to Bottom(-) (from a top down perspective)
     */

    static std::vector<Mach1Point3D> m1Spatial_4_Def = {
        {-1, 1, 0},
        {1, 1, 0},
        {-1, -1, 0},
        {1, -1, 0}};

    static std::vector<Mach1Point3D> m1Spatial_8_Def = {
        {-1, 1, 1},
        {1, 1, 1},
        {-1, -1, 1},
        {1, -1, 1},

        {-1, 1, -1},
        {1, 1, -1},
        {-1, -1, -1},
        {1, -1, -1}};

    static std::vector<Mach1Point3D> m1Spatial_14_Def = {
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

    static std::map<OutputMode, std::vector<Mach1Point3D> > standards = {
        {OUTPUT_SPATIAL_4CH, m1Spatial_4_Def},
        {OUTPUT_SPATIAL_8CH, m1Spatial_8_Def},
        {OUTPUT_SPATIAL_14CH, m1Spatial_14_Def},
    };

    std::vector<Mach1Point3D> pointsSet;
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
    std::vector<std::vector<int> > m1Spatial_4_Lines_Def = {
        /// QUAD LINES
        {0, 1},
        {1, 3},
        {3, 2},
        {2, 0},
    };
    std::vector<std::vector<int> > m1Spatial_4_Plane_Def = {
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
    std::vector<std::vector<int> > m1Spatial_8_Lines_Def = {
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
    std::vector<std::vector<int> > m1Spatial_8_Plane_Def = {
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

    /// MACH1SPATIAL-14
    /* TOP VIEW
     [0]___[8]___[1]
      |           |
   [11]    [12]   [9]
      |           |
     [2]___[10]__[3]
     */
    std::vector<std::vector<int> > m1Spatial_14_Lines_Def = {
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
    std::vector<std::vector<int> > m1Spatial_14_Plane_Def = {
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

    static std::map<OutputMode, std::vector<std::vector<int> > > lines = {
        {OUTPUT_SPATIAL_4CH, m1Spatial_4_Lines_Def},
        {OUTPUT_SPATIAL_8CH, m1Spatial_8_Lines_Def},
        {OUTPUT_SPATIAL_14CH, m1Spatial_14_Lines_Def},
    };

    static std::map<OutputMode, std::vector<std::vector<int> > > planes = {
        {OUTPUT_SPATIAL_4CH, m1Spatial_4_Plane_Def},
        {OUTPUT_SPATIAL_8CH, m1Spatial_8_Plane_Def},
        {OUTPUT_SPATIAL_14CH, m1Spatial_14_Plane_Def},
    };

    std::vector<std::vector<int> > linesSet = lines[outputMode];
    std::vector<std::vector<int> > planesSet = planes[outputMode];

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
        Mach1Point3D p1 = pointsSet[linesSet[i][0]];
        Mach1Point3D p2 = pointsSet[linesSet[i][1]];
        float d = distance_to_segment(x, y, z, p1.x, p1.y, p1.z, p2.x, p2.y, p2.z);
        distToLines.push_back(d);
    }

    std::vector<float> distToPlanes;
    for (int i = 0; i < planesSet.size(); i++) {
        Mach1Point3D p1 = pointsSet[planesSet[i][0]];
        Mach1Point3D p2 = pointsSet[planesSet[i][1]];
        Mach1Point3D p3 = pointsSet[planesSet[i][2]];
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

    // normalize initial gains
    float gSum = 0;
    for (int i = 0; i < gains.size(); i++) {
        gSum += gains[i];
    }
    for (int i = 0; i < gains.size(); i++) {
        gains[i] /= gSum;
    }

    // Use gains normalization for center point
    float dist_to_center = clamp(sqrt(dist_sq(x, y, z, 0, 0, 0)) / 0.5f, 0.0f, 1.0f);
    for (int i = 0; i < gains.size(); i++) {
        gains[i] = map_(dist_to_center, 0.0f, 1.0f, 1.0f / gains.size(), gains[i]);
    }

    // **Add normalization here to ensure gains sum to 1.0 after mapping**
    float totalGain = 0.0f;
    for (int i = 0; i < gains.size(); i++) {
        totalGain += gains[i];
    }
    if (totalGain > 0.0f) {
        for (int i = 0; i < gains.size(); i++) {
            gains[i] /= totalGain;
        }
    }

    result = gains;
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

    // NOTE: This +6dB boost is to ensure that after encoding->decoding we gain match
    // to the source sound if we are encoded and decoded at the same azimuth and elevation values
    // It is advised to use `setOutputGain(float gain)` when exploring signal based gain adjustment solutions
    outputGainLinearMultipler = 2.0;
    gainCompensationLinearMultiplier = 1.0;

    // init additional arrays
    arr_Points = new Mach1Point3D[MAX_POINTS_COUNT];

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
    if (arr_Points) {
        delete[] arr_Points;
        arr_Points = nullptr;
    }

    if (arr_Gains) {
        for (int i = 0; i < MAX_POINTS_COUNT; i++) {
            if (arr_Gains[i]) {
                delete[] arr_Gains[i];
                arr_Gains[i] = nullptr;
            }
        }
        delete[] arr_Gains;
        arr_Gains = nullptr;
    }

    if (arr_PointsNames) {
        for (int i = 0; i < MAX_POINTS_COUNT; i++) {
            if (arr_PointsNames[i]) {
                delete[] arr_PointsNames[i];
                arr_PointsNames[i] = nullptr;
            }
        }
        delete[] arr_PointsNames;
        arr_PointsNames = nullptr;
    }

    if (arr_GainsForInputChannelNamed) {
        delete[] arr_GainsForInputChannelNamed;
        arr_GainsForInputChannelNamed = nullptr;
    }

    if (arr_ResultingCoeffsDecoded) {
        delete[] arr_ResultingCoeffsDecoded;
        arr_ResultingCoeffsDecoded = nullptr;
    }
}

// Copy constructor
M1EncodeCore::M1EncodeCore(const M1EncodeCore &other) : M1EncodeCore() {
    *this = other;
}

// Copy assignment operator
M1EncodeCore& M1EncodeCore::operator=(const M1EncodeCore& other) {
    if (this != &other) {
        // Copy basic data members
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

        // Copy arrays safely
        if (other.arr_Points && arr_Points) {
            std::copy(other.arr_Points, other.arr_Points + MAX_POINTS_COUNT, arr_Points);
        }

        if (other.arr_Gains && arr_Gains) {
            for (int i = 0; i < MAX_POINTS_COUNT; i++) {
                if (other.arr_Gains[i] && arr_Gains[i]) {
                    std::copy(other.arr_Gains[i], other.arr_Gains[i] + MAX_CHANNELS_COUNT, arr_Gains[i]);
                }
            }
        }

        if (other.arr_PointsNames && arr_PointsNames) {
            for (int i = 0; i < MAX_POINTS_COUNT; i++) {
                if (other.arr_PointsNames[i] && arr_PointsNames[i]) {
                    std::strcpy(arr_PointsNames[i], other.arr_PointsNames[i]);
                }
            }
        }

        if (other.arr_GainsForInputChannelNamed && arr_GainsForInputChannelNamed) {
            std::copy(other.arr_GainsForInputChannelNamed, other.arr_GainsForInputChannelNamed + MAX_CHANNELS_COUNT, arr_GainsForInputChannelNamed);
        }

        if (other.arr_ResultingCoeffsDecoded && arr_ResultingCoeffsDecoded) {
            std::copy(other.arr_ResultingCoeffsDecoded, other.arr_ResultingCoeffsDecoded + MAX_CHANNELS_COUNT, arr_ResultingCoeffsDecoded);
        }
    }
    return *this;
}

void M1EncodeCore::generatePointResults() {
    long tStart = getCurrentTime();

    if (pannerMode != MODE_ISOTROPICLINEAR && pannerMode != MODE_ISOTROPICEQUALPOWER) {
        pannerMode = MODE_PERIPHONICLINEAR;
    }

    Mach1Point3D centerpoint;
    float normalisedOutputDiverge = diverge * (1 / cos(PI * 0.25f));
    centerpoint = {(float)cos((azimuth)*PI * 2) * normalisedOutputDiverge, 0, (float)sin((azimuth)*PI * 2) * normalisedOutputDiverge};

    if (inputMode == INPUT_MONO) {
        resultingPoints.pointsCount = 1;
        resultingPoints.pointsNames[0] = "M";

        if (outputMode == OUTPUT_SPATIAL_4CH) {
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
        std::vector<Mach1Point3D> pnts = {
            {(float)cos((sRotationInRadians)) * sSpread, 0, (float)sin((sRotationInRadians)) * sSpread},
            {(float)-cos((sRotationInRadians)) * sSpread, 0, (float)-sin((sRotationInRadians)) * sSpread}};

        for (int i = 0; i < resultingPoints.pointsCount; i++) {
            resultingPoints.pointsNames[i] = names[i];
            if (outputMode == OUTPUT_SPATIAL_4CH) {
                resultingPoints.ppoints[i] = pnts[i] + centerpoint;
            } else if (pannerMode == MODE_ISOTROPICLINEAR || pannerMode == MODE_ISOTROPICEQUALPOWER) {
                resultingPoints.ppoints[i] = pnts[i] + Mach1Point3D{centerpoint.x * (float)sin((elevation + 1) * PI / 2), elevation, centerpoint.z * (float)sin((elevation + 1) * PI / 2)};
            } else if (pannerMode == MODE_PERIPHONICLINEAR) {
                resultingPoints.ppoints[i] = pnts[i] + Mach1Point3D{centerpoint.x, elevation, centerpoint.z};
            }
        }
    } else if (inputMode == INPUT_QUAD) {

        resultingPoints.pointsCount = 4;

        std::vector<std::string> names = {"L", "R", "Ls", "Rs"};
        std::vector<Mach1Point3D> pnts = {
            {(float)cos((azimuth + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge},
        };

        assignResultingPointsNamesAndCoordinates(names, pnts);
    } else if (inputMode == INPUT_LCRS) {

        resultingPoints.pointsCount = 4;

        std::vector<std::string> names = {"L", "C", "R", "S"};
        if (frontSurroundPerspective) {
            centerpoint = {(float)cos((azimuth + 0.0f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.0f) * PI * 2) * diverge};
        } else {
            centerpoint = {0, 0, 0};
        }
        std::vector<Mach1Point3D> pnts = {
            {(float)cos((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge},
            centerpoint,
            {(float)cos((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.5f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.5f) * PI * 2) * diverge},
        };
        pnts[1] = (pnts[0] + pnts[2]) / 2;
        pnts[3] = -pnts[1];

        assignResultingPointsNamesAndCoordinates(names, pnts);

    } else if (inputMode == INPUT_AFORMAT) {

        resultingPoints.pointsCount = 4;

        std::vector<std::string> names = {"FLU", "FRD", "BLD", "BRU"};
        std::vector<Mach1Point3D> pnts = {
            {(float)cos((azimuth + 0.125f - 0.25f) * PI * 2) * diverge, (1 * diverge), (float)sin((azimuth + 0.125f - 0.25f) * PI * 2) * diverge},
            {(float)cos((azimuth + 0.125f) * PI * 2) * diverge, (-1 * diverge), (float)sin((azimuth + 0.125f) * PI * 2) * diverge},
            {(float)cos((azimuth + 0.125f + 0.5f) * PI * 2) * diverge, (-1 * diverge), (float)sin((azimuth + 0.125f + 0.5f) * PI * 2) * diverge},
            {(float)cos((azimuth + 0.125f + 0.25f) * PI * 2) * diverge, (1 * diverge), (float)sin((azimuth + 0.125f + 0.25f) * PI * 2) * diverge},
        };

        assignResultingPointsNamesAndCoordinates(names, pnts);

    } else if (inputMode == INPUT_1OAACN) { // duplicate?

        resultingPoints.pointsCount = 7;

        std::vector<std::string> names = {"W", "1", "2", "3", "-1", "-2", "-3"};
        std::vector<Mach1Point3D> pnts = {
            {0, 0, 0},                                                                                                 // W
            {(float)cos((azimuth + 0.250f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.250f) * PI * 2) * diverge}, // Left/Right / L
            {0, 1.0f * diverge, 0},                                                                                    // Top/Bottom / T
            {(float)cos((azimuth + 0.0f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.0f) * PI * 2) * diverge},     // Front/Back / F
            {(float)cos((azimuth + 0.75f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.75f) * PI * 2) * diverge},   // -Left/Right / R
            {0, -1.0f * diverge, 0},                                                                                   // -Top/Bottom / B
            {(float)cos((azimuth + 0.5f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.5f) * PI * 2) * diverge},     // -Front/Back / B
        };

        assignResultingPointsNamesAndCoordinates(names, pnts);

    } else if (inputMode == INPUT_1OAFUMA) {

        resultingPoints.pointsCount = 7;

        std::vector<std::string> names = {"W", "X", "Y", "Z", "-X", "-Y", "-Z"};
        std::vector<Mach1Point3D> pnts = {
            {0, 0, 0},                                                                                               // W
            {(float)cos((azimuth + 0.0f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.0f) * PI * 2) * diverge},   // Front/Back / F
            {(float)cos((azimuth + 0.25f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.25f) * PI * 2) * diverge}, // Left/Right / L
            {0, 1.0f * diverge, 0},                                                                                  // Top/Bottom / T
            {(float)cos((azimuth + 0.5f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.5f) * PI * 2) * diverge},   // -Front/Back / B
            {(float)cos((azimuth + 0.75f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.75f) * PI * 2) * diverge}, // -Left/Right / R
            {0, -1.0f * diverge, 0},                                                                                 // -Top/Bottom / B
        };

        assignResultingPointsNamesAndCoordinates(names, pnts);
    } else if (inputMode == INPUT_2OAACN || inputMode == INPUT_2OAFUMA || inputMode == INPUT_3OAACN || inputMode == INPUT_3OAFUMA) { // duplicate?

        /*
        TODO: Rework this into something smarter
        Currently expects 2OA and 3OA conversion to Mach1Spatial8 externally from this API
        This API will just supply the azimuths for Mach1Spatial8 Cuboid
        */

        resultingPoints.pointsCount = 8;

        std::vector<std::string> names = {"1", "2", "3", "4", "5", "6", "7", "8"};
        std::vector<Mach1Point3D> pnts = {
            {(float)cos((azimuth + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge, 1.0f * diverge, (float)sin((azimuth + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge, 1.0f * diverge, (float)sin((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge, 1.0f * diverge, (float)sin((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge, 1.0f * diverge, (float)sin((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge, -1.0f * diverge, (float)sin((azimuth + 0.125f - 0.25f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge, -1.0f * diverge, (float)sin((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge, -1.0f * diverge, (float)sin((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge, -1.0f * diverge, (float)(float)sin((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge},
        };

        assignResultingPointsNamesAndCoordinates(names, pnts);
    } else if (inputMode == INPUT_LCR) {

        resultingPoints.pointsCount = 3;

        std::vector<std::string> names = {"L", "C", "R"};
        if (frontSurroundPerspective) {
            centerpoint = {(float)cos((azimuth + 0.0f) * PI * 2) * diverge, 0, (float)sin((azimuth + 0.0f) * PI * 2) * diverge};
        } else {
            centerpoint = {0, 0, 0};
        }
        std::vector<Mach1Point3D> pnts = {
            {(float)cos((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge},
            centerpoint,
            {(float)cos((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge},
        };
        pnts[1] = (pnts[0] + pnts[2]) / 2;

        assignResultingPointsNamesAndCoordinates(names, pnts);
    } else if (inputMode == INPUT_FIVE_ZERO) {

        resultingPoints.pointsCount = 5;

        std::vector<std::string> names = {"L", "C", "R", "Ls", "Rs"};
        if (frontSurroundPerspective) {
            centerpoint = {(float)cos((azimuth + 0.0f) * PI * 2) * diverge, elevation, (float)sin((azimuth + 0.0f) * PI * 2) * diverge};
        } else {
            centerpoint = {0, elevation, 0};
        }
        std::vector<Mach1Point3D> pnts = {
            {(float)cos((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge},
            centerpoint,
            {(float)cos((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge},
        };

        assignResultingPointsNamesAndCoordinates(names, pnts);
    } else if (inputMode == INPUT_FIVE_ONE_FILM) {

        resultingPoints.pointsCount = 6;

        std::vector<std::string> names = {"L", "C", "R", "Ls", "Rs", "LFE"};
        if (frontSurroundPerspective) {
            centerpoint = {(float)cos((azimuth + 0.0f) * PI * 2) * diverge, elevation, (float)sin((azimuth + 0.0f) * PI * 2) * diverge};
        } else {
            centerpoint = {0, elevation, 0};
        }
        std::vector<Mach1Point3D> pnts = {
            {(float)cos((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge},
            centerpoint,
            {(float)cos((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge},
            {0, 0, 0},
        };

        assignResultingPointsNamesAndCoordinates(names, pnts);
    } else if (inputMode == INPUT_FIVE_ONE_DTS) {

        resultingPoints.pointsCount = 6;

        std::vector<std::string> names = {"L", "R", "Ls", "Rs", "C", "LFE"};
        if (frontSurroundPerspective) {
            centerpoint = {(float)cos((azimuth + 0.0f) * PI * 2) * diverge, elevation, (float)sin((azimuth + 0.0f) * PI * 2) * diverge};
        } else {
            centerpoint = {0, elevation, 0};
        }
        std::vector<Mach1Point3D> pnts = {
            {(float)cos((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge},
            centerpoint,
            {0, 0, 0},
        };

        assignResultingPointsNamesAndCoordinates(names, pnts);
    } else if (inputMode == INPUT_FIVE_ONE_SMPTE) {

        resultingPoints.pointsCount = 6;

        std::vector<std::string> names = {"L", "R", "C", "LFE", "Ls", "Rs"};
        if (frontSurroundPerspective) {
            centerpoint = {(float)cos((azimuth + 0.0f) * PI * 2) * diverge, elevation, (float)sin((azimuth + 0.0f) * PI * 2) * diverge};
        } else {
            centerpoint = {0, elevation, 0};
        }
        std::vector<Mach1Point3D> pnts = {
            {(float)cos((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth - 0.125f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f) * PI * 2) * normalisedOutputDiverge},
            centerpoint,
            {0, 0, 0},
            {(float)cos((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f + 0.5f) * PI * 2) * normalisedOutputDiverge},
            {(float)cos((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge, elevation, (float)sin((azimuth + 0.125f + 0.25f) * PI * 2) * normalisedOutputDiverge},
        };

        assignResultingPointsNamesAndCoordinates(names, pnts);
    }
    
    // Calculate adjusted diverge for gain compensation only for isotropic equal power mode
    float adjustedOutputGain = outputGainLinearMultipler;  // Default to regular output gain

    // Calculate a channel based gain compensation since we will lose power from dividing the signal into the soundfield
    if (gainCompensationActive)
    {
        switch (getOutputChannelsCount())
        {
            case 4:
                gainCompensationLinearMultiplier *= 1.0f;
                break;
            case 8:
                gainCompensationLinearMultiplier *= 2.0f;
                break;
            case 14:
                gainCompensationLinearMultiplier *= 4.57088137f;
                break;
        }
        adjustedOutputGain *= gainCompensationLinearMultiplier;
    }
    
    if (pannerMode == MODE_ISOTROPICEQUALPOWER) {
        float divergeThreshold = 0.707106f;
        float maxGainCompensation_dB = 6.0f; // Maximum gain compensation at diverge = 0.0

        float d_comp = (diverge <= divergeThreshold) ? 1.0f - (diverge / divergeThreshold) : 0.0f;

        // Calculate the gain multiplier based on d_comp
        float gainMultiplier = powf(2.0f, d_comp); // Exponential increase up to 6dB
        adjustedOutputGain *= gainMultiplier;
    }
    
    resultingPoints.gains.resize(resultingPoints.pointsCount);
    for (int i = 0; i < resultingPoints.pointsCount; i++) {

        resultingPoints.gains[i].resize(getOutputChannelsCount());

        // Generating gains
        std::vector<float> gains;
        if (outputMode == OUTPUT_SPATIAL_4CH) {
            // force ppoints to center of soundfield's elevation
            processGains(resultingPoints.ppoints[i].z, resultingPoints.ppoints[i].x, 0, gains);
        } else {
            // process 3d ppoints
            processGains(resultingPoints.ppoints[i].z, resultingPoints.ppoints[i].x, resultingPoints.ppoints[i].y, gains);
        }
        // applying output gain to gains and assigning to the current index channel
        for (int j = 0; j < getOutputChannelsCount(); j++) {
            resultingPoints.gains[i][j] = gains[j] * adjustedOutputGain;
        }
    }
    timeLastCalculation = getCurrentTime() - tStart;
}

#ifdef M1ENCODE_INLINE_DECODE
// if `M1ENCODE_INLINE_DECODE` is defined we support the following function but require linking to M1Decode
void M1EncodeCore::getResultingCoeffsDecoded(Mach1DecodeMode decodeMode, float *decodeResult, float *result) {
    // clear
    for (int i = 0; i < resultingPoints.pointsCount * 2; i++)
        result[i] = 0;

    // TODO: check on these numbers

    int decodeResultSize = 0;
    switch (decodeMode) {
    case Mach1DecodeAlgoSpatial_4:
        decodeResultSize = 8;
        break;
    case Mach1DecodeAlgoSpatial_8:
        decodeResultSize = 16;
        break;
    case Mach1DecodeAlgoSpatial_14:
        decodeResultSize = 28;
        break;
    default:
        break;
    }

    // decode
    if (getOutputChannelsCount() * 2 != decodeResultSize) {
        std::cout << "[MACH1] Warning: The Mach1EncodeOutputMode in use is not suitable for the Mach1DecodeMode selected" << std::endl;
    }

    for (int j = 0; j < resultingPoints.pointsCount; j++) {
        for (int i = 0; i < getOutputChannelsCount(); i++) {
            result[j * 2 + 0] += decodeResult[i * 2 + 0] * resultingPoints.gains[j][i]; // left
            result[j * 2 + 1] += decodeResult[i * 2 + 1] * resultingPoints.gains[j][i]; // right
        }
    }
}
#endif

M1EncodeCore::InputMode M1EncodeCore::getInputMode() {
    return inputMode;
}

M1EncodeCore::OutputMode M1EncodeCore::getOutputMode() {
    return outputMode;
}

M1EncodeCore::PannerMode M1EncodeCore::getPannerMode() {
    return pannerMode;
}

bool M1EncodeCore::getAutoOrbit() {
    return autoOrbit;
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
    case OUTPUT_SPATIAL_4CH:
        return 4;
    case OUTPUT_SPATIAL_8CH:
        return 8;
    case OUTPUT_SPATIAL_14CH:
        return 14;
    }
    return 0;
}

void M1EncodeCore::setInputMode(InputMode inputMode) {
    this->inputMode = inputMode;
}

void M1EncodeCore::setOutputMode(OutputMode outputMode) {
    this->outputMode = outputMode;
    
    // Set default gain compensation based on output mode
    switch (outputMode) {
        case OUTPUT_SPATIAL_4CH:
            gainCompensationLinearMultiplier = 1.0f;
            break;
        case OUTPUT_SPATIAL_8CH:
            gainCompensationLinearMultiplier = 2.0f;
            break;
        case OUTPUT_SPATIAL_14CH:
            gainCompensationLinearMultiplier = 4.57088137f;
            break;
    }
    // Enable gain compensation by default when changing output mode
    gainCompensationActive = true;
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

float M1EncodeCore::getOutputGain(bool isDecibel = false) {
    if (isDecibel) {
        return 20.0f * std::log10(outputGainLinearMultipler);
    } else {
        return outputGainLinearMultipler;
    }
}

void M1EncodeCore::setGainCompensation(float gainMultipler, bool isDecibel) {
    float linearValue;
    if (isDecibel) {
        linearValue = std::pow(10.0f, gainMultipler / 20.0f);
    } else {
        linearValue = gainMultipler;
    }
    
    this->gainCompensationLinearMultiplier = linearValue;
    
    // Check if the value is close to unity gain (within ±0.01 linear or ±0.086dB)
    const float epsilon = 0.01f;
    this->gainCompensationActive = std::abs(linearValue - 1.0f) > epsilon;
}

void M1EncodeCore::setGainCompensationActive(bool active) {
    gainCompensationActive = active;
}

float M1EncodeCore::getGainCompensation(bool isDecibel) {
    if (isDecibel) {
        return 20.0f * log10f(gainCompensationLinearMultiplier);
    }
    return gainCompensationLinearMultiplier;
}

bool M1EncodeCore::getGainCompensationActive() {
    return gainCompensationActive;
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
