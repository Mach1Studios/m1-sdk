#include "Mach1Decode.h"
#include "Mach1Encode.h"
#include <acutest.h>
#include <nlohmann/json.hpp>

#include <iostream>
#include <map>
#include <math.h>

std::map<Mach1EncodeInputModeType, std::string> encodeInputModeNames = {
    {Mach1EncodeInputModeMono, "MONO"},
    {Mach1EncodeInputModeStereo, "STEREO"},
    {Mach1EncodeInputModeQuad, "QUAD"},
    {Mach1EncodeInputModeLCRS, "LCRS"},
    {Mach1EncodeInputModeAFormat, "AFORMAT"},
    {Mach1EncodeInputModeBFOAACN, "1OA-ACN"},
    {Mach1EncodeInputModeBFOAFUMA, "1OA-FUMA"},
    {Mach1EncodeInputModeB2OAACN, "2OA-ACN"},
    {Mach1EncodeInputModeB2OAFUMA, "2OA-FUMA"},
    {Mach1EncodeInputModeB3OAACN, "3OA-ACN"},
    {Mach1EncodeInputModeB3OAFUMA, "3OA-FUMA"},
    {Mach1EncodeInputModeLCR, "LCR"},
};

std::map<Mach1EncodeOutputModeType, std::string> encodeOutputModeNames = {
    {Mach1EncodeOutputModeM1Horizon_4, "MACH1HORIZON-4"},
    {Mach1EncodeOutputModeM1Spatial_8, "MACH1SPATIAL-8"},
    {Mach1EncodeOutputModeM1Spatial_12, "MACH1SPATIAL-12"},
    {Mach1EncodeOutputModeM1Spatial_14, "MACH1SPATIAL-14"},
    {Mach1EncodeOutputModeM1Spatial_32, "MACH1SPATIAL-32"},
    {Mach1EncodeOutputModeM1Spatial_36, "MACH1SPATIAL-36"},
    {Mach1EncodeOutputModeM1Spatial_48, "MACH1SPATIAL-48"},
    {Mach1EncodeOutputModeM1Spatial_60, "MACH1SPATIAL-60"},
};

enum InputType {
    signedDegrees,
    unsignedDegrees,
    signedRadians,
    unsignedRadians,
    signedNormalized,
    unsignedNormalized,
    quaternion,
};

std::map<InputType, std::string> inputTypeNames = {
    {signedDegrees, "-180->180"},
    {unsignedDegrees, "0->360"},
    {signedRadians, "-PI->PI"},
    {unsignedRadians, "0->2PI"},
    {signedNormalized, "-1.0->1.0"},
    {unsignedNormalized, "0->1"},
    {quaternion, "quat"},
};

std::map<Mach1PlatformType, std::string> platformModeNames = {
    {Mach1PlatformDefault, "Default"},
    {Mach1PlatformUnity, "Unity"},
    {Mach1PlatformUE, "UE"},
    {Mach1PlatformOfEasyCam, "OFEasyCam"},
    {Mach1PlatformAndroid, "Android"},
    {Mach1PlatformiOSTableTop_ZVertical, "iOS_ZVertical"},
    {Mach1PlatformiOSPortraitHandheld_YVertical, "iOS_YVertical"},
    {Mach1PlatformiOSPortrait_YawOnly, "iOS_YawOnly"},
};

std::map<Mach1DecodeAlgoType, std::string> decodeModeNames = {
    {Mach1DecodeAlgoSpatial_8, "Spatial - 8Ch"},
    {Mach1DecodeAlgoHorizon_4, "Horizon - 4Ch"},
    {Mach1DecodeAlgoHorizonPairs, "Horizon Pairs - 8Ch"},
    {Mach1DecodeAlgoSpatial_12, "Spatial - 12Ch"},
    {Mach1DecodeAlgoSpatial_14, "Spatial - 14Ch"},
    {Mach1DecodeAlgoSpatial_32, "Spatial - 32Ch"},
    {Mach1DecodeAlgoSpatial_36, "Spatial - 36Ch"},
    {Mach1DecodeAlgoSpatial_48, "Spatial - 48Ch"},
    {Mach1DecodeAlgoSpatial_60, "Spatial - 60Ch"},
};

void test_results(void) {
    struct INPUT_DATA {
        Mach1EncodeInputModeType encodeInputMode;
        Mach1EncodeOutputModeType encodeOutputMode;
        bool isotropicEncode;
        float azimuth;
        float diverge;
        float elevation;
        bool autoOrbit;
        float orbitSpread;
        float orbitRotation;
        InputType inputType;

        Mach1PlatformType platformMode;
        Mach1DecodeAlgoType decodeMode;
        float yaw;
        float pitch;
        float roll;
        float filterSpeed;

        std::vector<float> volumes;
    };

    struct OUTPUT_DATA {
        std::vector<float> results;
    };

    struct CASE {
        std::string name;
        INPUT_DATA input;
        OUTPUT_DATA output;
    };

    std::vector<CASE> cases = {
        {"Test 1 - Right channel prior",
         {Mach1EncodeInputModeMono, Mach1EncodeOutputModeM1Spatial_8, true, 90.0, 0.5, 0.0, true, 0.0, 0.0, unsignedDegrees, Mach1PlatformDefault, Mach1DecodeAlgoSpatial_8, 0.0, 0.0, 0.0, 1.0, {1.0}},
         {{0.0, 0.2}}},
    };

    std::cout << std::endl;

    for (auto test : cases) {
        // encode part
        Mach1Encode m1Encode;

        m1Encode.setInputMode(test.input.encodeInputMode);
        m1Encode.setOutputMode(test.input.encodeOutputMode);

        if (test.input.inputType == signedDegrees || test.input.inputType == unsignedDegrees) {
            m1Encode.setAzimuthDegrees(test.input.azimuth);
        } else if (test.input.inputType == signedRadians || test.input.inputType == unsignedRadians) {
            m1Encode.setAzimuthRadians(test.input.azimuth);
        } else if (test.input.inputType == signedNormalized || test.input.inputType == unsignedNormalized) {
            m1Encode.setAzimuth(test.input.azimuth);
        } else if (test.input.inputType == quaternion) {
            // case for quat
        }

        m1Encode.setDiverge(test.input.diverge);

        if (test.input.inputType == signedDegrees || test.input.inputType == unsignedDegrees) {
            m1Encode.setElevationDegrees(test.input.elevation);
        } else if (test.input.inputType == signedRadians || test.input.inputType == unsignedRadians) {
            m1Encode.setElevationRadians(test.input.elevation);
        } else if (test.input.inputType == signedNormalized || test.input.inputType == unsignedNormalized) {
            m1Encode.setElevation(test.input.elevation);
        } else if (test.input.inputType == quaternion) {
            // case for quat
        }

        m1Encode.setAutoOrbit(test.input.autoOrbit);
        m1Encode.setStereoSpread(test.input.orbitSpread);
        if (test.input.inputType == signedDegrees || test.input.inputType == unsignedDegrees) {
            m1Encode.setOrbitRotationDegrees(test.input.orbitRotation);
        } else if (test.input.inputType == signedRadians || test.input.inputType == unsignedRadians) {
            m1Encode.setOrbitRotationRadians(test.input.orbitRotation);
        } else if (test.input.inputType == signedNormalized || test.input.inputType == unsignedNormalized) {
            m1Encode.setOrbitRotation(test.input.orbitRotation);
        } else if (test.input.inputType == quaternion) {
            // case for quat
        }

        m1Encode.generatePointResults();
        // auto points = m1Encode.getPoints();
        auto encodeResults = m1Encode.getGains();

        // decode part
        Mach1Decode m1Decode;

        m1Decode.setPlatformType(test.input.platformMode);
        m1Decode.setDecodeAlgoType(test.input.decodeMode);
        m1Decode.setFilterSpeed(test.input.filterSpeed);

        m1Decode.setRotationDegrees(Mach1Point3D{test.input.yaw, test.input.pitch, test.input.roll});

        m1Decode.beginBuffer();
        auto decodeResults = m1Decode.decodeCoeffs(0, 0);
        m1Decode.endBuffer();

        std::cout
            << "testing " << test.name << ": "
            << encodeInputModeNames[test.input.encodeInputMode] << " > " << encodeOutputModeNames[test.input.encodeOutputMode] << " with " << inputTypeNames[test.input.inputType] << " , "
            << platformModeNames[test.input.platformMode] << " > " << decodeModeNames[test.input.decodeMode];

        int counter = 0;

        // encode buffer
        std::vector<float> bufferEncoded(m1Encode.getInputChannelsCount() * m1Encode.getOutputChannelsCount());
        for (size_t c = 0; c < encodeResults.size(); c++) {
            for (size_t k = 0; k < encodeResults[c].size(); k++) {
                bufferEncoded[k * m1Encode.getInputChannelsCount() + c] = test.input.volumes[c] * encodeResults[c][k];
            }
        }

        // decode buffer
        std::vector<float> bufferDecoded(2);
        float sample = 0;
        int cOffset = 0;
        int inputChannelsCount = bufferEncoded.size() / m1Encode.getPointsCount();
        int outChannelsCount = bufferDecoded.size();

        for (size_t c = 0; c < outChannelsCount; c++) {
            sample = 0;
            cOffset = c < m1Encode.getPointsCount() ? c : 0;
            for (size_t k = 0; k < inputChannelsCount; k++) {
                sample += bufferEncoded[k * m1Encode.getPointsCount() + cOffset] * decodeResults[k * 2 + c];
            }
            bufferDecoded[c] = sample;
        }

        for (size_t i = 0; i < bufferDecoded.size(); i++) {
            bool check = fabs(test.output.results[i] - bufferDecoded[i]) < 0.1;
            if (check == false) {
                TEST_CHECK_(check, "%s | Error with index [%d]", test.name.c_str(), i);
                std::cout << "index: [" << i << "]: " << bufferDecoded[i] << ", should be: " << test.output.results[i];
                std::cout << std::endl;
            }
            counter += check;
        }

        if (counter == bufferDecoded.size()) {
            std::cout << " "
                      << "\033[1;32mpassed\033[0m\n";
        }
    }

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    getchar();
#endif
}

TEST_LIST = {
    {"results", test_results},
    {NULL, NULL}};
