#include "Mach1Decode.h"
#include <acutest.h>

#include <iostream>
#include <map>
#include <math.h>

void test_results(void) {
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

    std::map<Mach1DecodeMode, std::string> outputModeNames = {
        {M1DecodeSpatial_4, "Horizon - 4Ch"},
        {M1DecodeSpatial_8, "Spatial - 8Ch"},
        {M1DecodeSpatial_12, "Spatial - 12Ch"},
        {M1DecodeSpatial_14, "Spatial - 14Ch"},
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

    struct INPUT_DATA {
        Mach1PlatformType platformMode;
        Mach1DecodeMode outputMode;
        float yaw;
        float pitch;
        float roll;
        float filterSpeed;
        InputType inputType;
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
        /*
        DECODE TESTING
        TODO: add more input tests with less rounded inputs
        TODO: add all other modes
         */
        {
            "case: DECODE | MACH1HORIZON | Y0P0R0",
            {Mach1PlatformDefault, M1DecodeSpatial_4, 0.0, 0.0, 0.0, 1.0, unsignedDegrees},
            {
                {
                    {
                        1.0, 0.0,
                        0.0, 1.0,
                        0.0, 0.0,
                        0.0, 0.0,
                    },
                },
            }},
        {
            "case: DECODE | MACH1HORIZON | Y45P0R0",
            {Mach1PlatformDefault, M1DecodeSpatial_4, 45.0, 0.0, 0.0, 1.0, unsignedDegrees},
            {
                {
                    {
                        0.5, 0.0,
                        0.5, 0.5,
                        0.0, 0.0,
                        0.0, 0.5,
                    },
                },
            }
        },
        {
            "case: DECODE | MACH1HORIZON | Y90P0R0",
            {Mach1PlatformDefault, M1DecodeSpatial_4, 90.0, 0.0, 0.0, 1.0, unsignedDegrees},
            {
                {
                    {
                        0.0, 0.0,
                        1.0, 0.0,
                        0.0, 0.0,
                        0.0, 1.0,
                    },
                },
            }
        },
        {"case: DECODE | MACH1HORIZON | Y180P0R0",
         {Mach1PlatformDefault, M1DecodeSpatial_4, 180.0, 0.0, 0.0, 1.0, unsignedDegrees},
         {
             {
                 {
                     0.0, 0.0,
                     0.0, 0.0,
                     0.0, 1.0,
                     1.0, 0.0,
                 },
             },
         }},
        {"case: DECODE | MACH1HORIZON | Y270P0R0",
         {Mach1PlatformDefault, M1DecodeSpatial_4, 270.0, 0.0, 0.0, 1.0, unsignedDegrees},
         {
             {
                 {
                     0.0, 1.0,
                     0.0, 0.0,
                     1.0, 0.0,
                     0.0, 0.0,
                 },
             },
         }},
        {"case: DECODE | MACH1SPATIAL-8 | Y0P0R0",
         {Mach1PlatformDefault, M1DecodeSpatial_8, 0.0, 0.0, 0.0, 1.0, unsignedDegrees},
         {
             {
                 {
                     0.5, 0.0,
                     0.0, 0.5,
                     0.0, 0.0,
                     0.0, 0.0,
                     0.5, 0.0,
                     0.0, 0.5,
                     0.0, 0.0,
                     0.0, 0.0,
                 },
             },
         }},
        {"case: DECODE | MACH1SPATIAL-8 | Y45P0R0",
         {Mach1PlatformDefault, M1DecodeSpatial_8, 45.0, 0.0, 0.0, 1.0, unsignedDegrees},
         {
             {
                 {
                     0.25, 0.0,
                     0.25, 0.25,
                     0.0, 0.0,
                     0.0, 0.25,
                     0.25, 0.0,
                     0.25, 0.25,
                     0.0, 0.0,
                     0.0, 0.25,
                 },
             },
         }},
        {"case: DECODE | MACH1SPATIAL-8 | Y90P0R0",
         {Mach1PlatformDefault, M1DecodeSpatial_8, 90.0, 0.0, 0.0, 1.0, unsignedDegrees},
         {
             {
                 {
                     0.0, 0.0,
                     0.5, 0.0,
                     0.0, 0.0,
                     0.0, 0.5,
                     0.0, 0.0,
                     0.5, 0.0,
                     0.0, 0.0,
                     0.0, 0.5,
                 },
             },
         }},
        {"case: DECODE | MACH1SPATIAL-8 | Y90P0R0 (Norm)",
         {Mach1PlatformDefault, M1DecodeSpatial_8, 0.25, 0.0, 0.0, 1.0, unsignedNormalized},
         {
             {
                 {
                     0.0, 0.0,
                     0.5, 0.0,
                     0.0, 0.0,
                     0.0, 0.5,
                     0.0, 0.0,
                     0.5, 0.0,
                     0.0, 0.0,
                     0.0, 0.5,
                 },
             },
         }},
        {"case: DECODE | MACH1SPATIAL-8 | Y90P0R0 (Radians)",
         {Mach1PlatformDefault, M1DecodeSpatial_8, 1.5708, 0.0, 0.0, 1.0, signedRadians},
         {
             {
                 {
                     0.0, 0.0,
                     0.5, 0.0,
                     0.0, 0.0,
                     0.0, 0.5,
                     0.0, 0.0,
                     0.5, 0.0,
                     0.0, 0.0,
                     0.0, 0.5,
                 },
             },
         }},
        {"case: DECODE | MACH1SPATIAL-8 | Y135P0R0",
         {Mach1PlatformDefault, M1DecodeSpatial_8, 135.0, 0.0, 0.0, 1.0, unsignedDegrees},
         {
             {
                 {
                     0.0, 0.0,
                     0.25, 0.0,
                     0.0, 0.25,
                     0.25, 0.25,
                     0.0, 0.0,
                     0.25, 0.0,
                     0.0, 0.25,
                     0.25, 0.25,
                 },
             },
         }},
        {"case: DECODE | MACH1SPATIAL-8 | Y180P0R0",
         {Mach1PlatformDefault, M1DecodeSpatial_8, 180.0, 0.0, 0.0, 1.0, unsignedDegrees},
         {
             {
                 {
                     0.0, 0.0,
                     0.0, 0.0,
                     0.0, 0.5,
                     0.5, 0.0,
                     0.0, 0.0,
                     0.0, 0.0,
                     0.0, 0.5,
                     0.5, 0.0,
                 },
             },
         }},
        {"case: DECODE | MACH1SPATIAL-8 | Y225P0R0",
         {Mach1PlatformDefault, M1DecodeSpatial_8, 225.0, 0.0, 0.0, 1.0, unsignedDegrees},
         {
             {
                 {
                     0.0, 0.25,
                     0.0, 0.0,
                     0.25, 0.25,
                     0.25, 0.0,
                     0.0, 0.25,
                     0.0, 0.0,
                     0.25, 0.25,
                     0.25, 0.0,
                 },
             },
         }},
        {"case: DECODE | MACH1SPATIAL-8 | Y-135P0R0",
         {Mach1PlatformDefault, M1DecodeSpatial_8, -135.0, 0.0, 0.0, 1.0, signedDegrees},
         {
             {
                 {
                     0.0, 0.25,
                     0.0, 0.0,
                     0.25, 0.25,
                     0.25, 0.0,
                     0.0, 0.25,
                     0.0, 0.0,
                     0.25, 0.25,
                     0.25, 0.0,
                 },
             },
         }},
        {"case: DECODE | MACH1SPATIAL-8 | Y315P0R0",
         {Mach1PlatformDefault, M1DecodeSpatial_8, 315.0, 0.0, 0.0, 1.0, unsignedDegrees},
         {
             {
                 {
                     0.25, 0.25,
                     0.0, 0.25,
                     0.25, 0.0,
                     0.0, 0.0,
                     0.25, 0.25,
                     0.0, 0.25,
                     0.25, 0.0,
                     0.0, 0.0,
                 },
             },
         }},
        {"case: DECODE | MACH1SPATIAL-8 | Y-45P0R0",
         {Mach1PlatformDefault, M1DecodeSpatial_8, -45.0, 0.0, 0.0, 1.0, signedDegrees},
         {
             {
                 {
                     0.25, 0.25,
                     0.0, 0.25,
                     0.25, 0.0,
                     0.0, 0.0,
                     0.25, 0.25,
                     0.0, 0.25,
                     0.25, 0.0,
                     0.0, 0.0,
                 },
             },
         }},
        {"case: DECODE | MACH1SPATIAL-8 | Y0P90R0",
         {Mach1PlatformDefault, M1DecodeSpatial_8, 0.0, 90.0, 0.0, 1.0, unsignedDegrees},
         {
             {
                 {
                     0.5, 0.0,
                     0.0, 0.5,
                     0.5, 0.0,
                     0.0, 0.5,
                     0.0, 0.0,
                     0.0, 0.0,
                     0.0, 0.0,
                     0.0, 0.0,
                 },
             },
         }},
        {"case: DECODE | MACH1SPATIAL-8 | Y0P90R0 (Radians)",
         {Mach1PlatformDefault, M1DecodeSpatial_8, 0.0, 1.5708, 0.0, 1.0, signedRadians},
         {
             {
                 {
                     0.5, 0.0,
                     0.0, 0.5,
                     0.5, 0.0,
                     0.0, 0.5,
                     0.0, 0.0,
                     0.0, 0.0,
                     0.0, 0.0,
                     0.0, 0.0,
                 },
             },
         }},
        {"case: DECODE | MACH1SPATIAL-8 | Y0P90R0 (Signed-Norm)",
         {Mach1PlatformDefault, M1DecodeSpatial_8, 0.0, 0.25, 0.0, 1.0, signedNormalized},
         {
             {
                 {
                     0.5, 0.0,
                     0.0, 0.5,
                     0.5, 0.0,
                     0.0, 0.5,
                     0.0, 0.0,
                     0.0, 0.0,
                     0.0, 0.0,
                     0.0, 0.0,
                 },
             },
         }},
        {"case: DECODE | MACH1SPATIAL-8 | Y0P-90R0",
         {Mach1PlatformDefault, M1DecodeSpatial_8, 0.0, -90.0, 0.0, 1.0, signedDegrees},
         {
             {
                 {
                     0.0, 0.0,
                     0.0, 0.0,
                     0.0, 0.0,
                     0.0, 0.0,
                     0.5, 0.0,
                     0.0, 0.5,
                     0.5, 0.0,
                     0.0, 0.5,
                 },
             },
         }},
        {"case: DECODE | MACH1SPATIAL-8 | Y0P0R90",
         {Mach1PlatformDefault, M1DecodeSpatial_8, 0.0, 0.0, 90.0, 1.0, unsignedDegrees},
         {
             {
                 {
                     0.5, 0.0,
                     0.5, 0.0,
                     0.0, 0.0,
                     0.0, 0.0,
                     0.0, 0.5,
                     0.0, 0.5,
                     0.0, 0.0,
                     0.0, 0.0,
                     /*
                     Should this be?:
                     0.25, 0.0,
                     0.25, 0.0,
                     0.25, 0.0,
                     0.25, 0.0,
                     0.0, 0.25,
                     0.0, 0.25,
                     0.0, 0.25,
                     0.0, 0.25,
                     */
                 },
             },
         }},
        {"case: DECODE | MACH1SPATIAL-8 | Y0P0R90 (Radians)",
         {Mach1PlatformDefault, M1DecodeSpatial_8, 0.0, 0.0, 1.5708, 1.0, signedRadians},
         {
             {
                 {
                     0.5, 0.0,
                     0.5, 0.0,
                     0.0, 0.0,
                     0.0, 0.0,
                     0.0, 0.5,
                     0.0, 0.5,
                     0.0, 0.0,
                     0.0, 0.0,
                 },
             },
         }},
        {"case: DECODE | MACH1SPATIAL-8 | Y0P0R90 (Norm)",
         {Mach1PlatformDefault, M1DecodeSpatial_8, 0.0, 0.0, 0.25, 1.0, signedNormalized},
         {
             {
                 {
                     0.5, 0.0,
                     0.5, 0.0,
                     0.0, 0.0,
                     0.0, 0.0,
                     0.0, 0.5,
                     0.0, 0.5,
                     0.0, 0.0,
                     0.0, 0.0,
                 },
             },
         }},
        {"case: DECODE | MACH1SPATIAL-8 | Y0P0R-90 (Norm)",
         {Mach1PlatformDefault, M1DecodeSpatial_8, 0.0, 0.0, -0.25, 1.0, signedNormalized},
         {
             {
                 {
                     0.0, 0.5,
                     0.0, 0.5,
                     0.0, 0.0,
                     0.0, 0.0,
                     0.5, 0.0,
                     0.5, 0.0,
                     0.0, 0.0,
                     0.0, 0.0,
                 },
             },
         }},
        // MACH1SPATIAL-12
        // {
        //  "case: DECODE | MACH1SPATIAL-12 | Y0P0R0",
        //  { Mach1PlatformDefault, M1DecodeSpatial_12, 0.0, 0.0, 0.0, 1.0, unsignedDegrees },
        //  {
        //      {
        //          {
        //              0.5, 0.0,
        //              0.0, 0.5,
        //              0.0, 0.0,
        //              0.0, 0.0,
        //              0.5, 0.0,
        //              0.0, 0.5,
        //              0.0, 0.0,
        //              0.0, 0.0,

        //              0.5, 0.5,
        //              0.0, 0.0,
        //              0.0, 0.0,
        //              0.0, 0.0,
        //          },
        //      },
        //  }
        // },
        // {
        //  "case: DECODE | MACH1SPATIAL-12 | Y45P0R0",
        //  { Mach1PlatformDefault, M1DecodeSpatial_12, 45.0, 0.0, 0.0, 1.0, unsignedDegrees },
        //  {
        //      {
        //          {
        //              0.25, 0.0,
        //              0.25, 0.25,
        //              0.0, 0.0,
        //              0.0, 0.25,
        //              0.25, 0.0,
        //              0.25, 0.25,
        //              0.0, 0.0,
        //              0.0, 0.25,

        //              0.5, 0.5,
        //              0.0, 0.0,
        //              0.0, 0.0,
        //              0.0, 0.0,
        //          },
        //      },
        //  }
        // },
        // {
        //  "case: DECODE | MACH1SPATIAL-12 | Y90P0R0",
        //  { Mach1PlatformDefault, M1DecodeSpatial_12, 90.0, 0.0, 0.0, 1.0, unsignedDegrees },
        //  {
        //      {
        //          {
        //              0.0, 0.0,
        //              0.5, 0.0,
        //              0.0, 0.0,
        //              0.0, 0.5,
        //              0.0, 0.0,
        //              0.5, 0.0,
        //              0.0, 0.0,
        //              0.0, 0.5,

        //              0.5, 0.5,
        //              0.0, 0.0,
        //              0.0, 0.0,
        //              0.0, 0.0,
        //          },
        //      },
        //  }
        // },
        /*
PLATFORM TESTING
TODO: add more input tests with less rounded inputs
 */
    };

    std::cout << std::endl;

    for (auto test : cases) {
        Mach1Decode<float> m1Decode;

        m1Decode.setPlatformType(test.input.platformMode);
        m1Decode.setDecodeMode(test.input.outputMode);
        m1Decode.setFilterSpeed(test.input.filterSpeed);

        if (test.input.inputType == signedDegrees || test.input.inputType == unsignedDegrees) {
            m1Decode.setRotationDegrees(Mach1Point3D{test.input.yaw, test.input.pitch, test.input.roll});
        } else if (test.input.inputType == signedRadians || test.input.inputType == unsignedRadians) {
            m1Decode.setRotationRadians(Mach1Point3D{test.input.yaw, test.input.pitch, test.input.roll});
        } else if (test.input.inputType == signedNormalized || test.input.inputType == unsignedNormalized) {
            m1Decode.setRotation(Mach1Point3D{test.input.yaw, test.input.pitch, test.input.roll});
        } else if (test.input.inputType == quaternion) {
            // TODO: add quat check
            // m1Decode.setRotationQuat(Mach1Point4D{ test.input.w, test.input.x, test.input.y, test.input.z});
        } else {
            m1Decode.setRotationDegrees(Mach1Point3D{test.input.yaw, test.input.pitch, test.input.roll});
        }

        auto results = m1Decode.decodeCoeffs(0, 0);

        std::cout
            << "testing " << test.name << ": "
            << platformModeNames[test.input.platformMode] << " > " << outputModeNames[test.input.outputMode];

        int counter = 0;

        for (size_t i = 0; i < results.size(); i++) {
            bool check = fabs(test.output.results[i] - results[i]) < 0.0001;
            counter += check;
            if (check == false) {
                TEST_CHECK_(check, "%s | Error with index [%zu]", test.name.c_str(), i);
                std::cout << "index: [" << i << "]: " << results[i] << ", should be: " << test.output.results[i];
                std::cout << std::endl;
            }
            if (counter == results.size()) {
                std::cout << " "
                          << "\033[1;32mpassed\033[0m\n";
            }
        }
    }
}

TEST_LIST = {
    {"results", test_results},
    {NULL, NULL}};
