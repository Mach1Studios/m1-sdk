#include "Mach1DecodePositional.h"
#include <acutest.h>

#include <iostream>
#include <map>
#include <math.h>

float mapFloat(float input, float inMin, float inMax, float outMin, float outMax) {
    return (input - inMin) / (inMax - inMin) * (outMax - outMin) + outMin;
}

void test_results(void) {
    std::map<Mach1PlatformType, std::string> platformModeNames = {
        {Mach1PlatformDefault, "Default"}, // y, p, r
        {Mach1PlatformUnity, "Unity"}, // p, y, r
        {Mach1PlatformUE, "UE"}, // p, r, y
        {Mach1PlatformOfEasyCam, "OFEasyCam"}, 
        {Mach1PlatformAndroid, "Android"},
        {Mach1PlatformiOS, "iOS"},
        {Mach1PlatformiOSTableTop_ZVertical, "iOS_ZVertical"},
        {Mach1PlatformiOSPortraitHandheld_YVertical, "iOS_YVertical"},
        {Mach1PlatformiOSPortrait_YawOnly, "iOS_YawOnly"},
    };

    std::map<Mach1DecodeAlgoType, std::string> outputModeNames = {
        {Mach1DecodeAlgoHorizon_4, "Horizon - 4Ch"},
        {Mach1DecodeAlgoSpatial_8, "Spatial - 8Ch"},
        {Mach1DecodeAlgoSpatial_12, "Spatial - 12Ch"},
        {Mach1DecodeAlgoSpatial_14, "Spatial - 14Ch"},
    };

    struct INPUT_DATA {
        Mach1PlatformType platformMode;
        Mach1DecodeAlgoType outputMode;
        float listenerPosX, listenerPosY, listenerPosZ;
        float listenerYaw, listenerPitch, listenerRoll;
        float m1PosX, m1PosY, m1PosZ;
        float m1Yaw, m1Pitch, m1Roll;
        float scaleX, scaleY, scaleZ;
        bool atttenuate, planeMode, muteWhenInside, muteWhenOutside;
        float filterSpeed;
    };

    struct OUTPUT_DATA {
        std::vector<float> results;
        float distance;
    };

    struct CASE {
        std::string name;
        INPUT_DATA input;
        OUTPUT_DATA output;
    };

    std::vector<CASE> cases = {
        /*
        MUTE TESTING
        Testing for mute functions when inside/outside the positional reference shape/point
        */
        {
            "case: POSITIONAL | INSIDE OBJECT MUTE",
            {
                Mach1PlatformDefault, Mach1DecodeAlgoSpatial_8,
                0.0, 0.0, 0.0, // listener pos
                0.0, 0.0, 0.0, // listener rot
                0.0, 0.0, 0.0, // m1 pos
                0.0, 0.0, 0.0, // m1 rot
                1.0, 1.0, 1.0, // m1 scale
                false, false, true, false, 1.0
            },
            {
                {
                    0.0, 0.0,
                    0.0, 0.0,
                    0.0, 0.0,
                    0.0, 0.0,
                    0.0, 0.0,
                    0.0, 0.0,
                    0.0, 0.0,
                    0.0, 0.0,
                },
                0.0,
            }
        },
        {
            "case: POSITIONAL | OUTSIDE OBJECT MUTE",
            {
                Mach1PlatformDefault, Mach1DecodeAlgoSpatial_8,
                2.0, 2.0, 2.0, // listener pos
                0.0, 0.0, 0.0, // listener rot
                0.0, 0.0, 0.0, // m1 pos
                0.0, 0.0, 0.0, // m1 rot
                1.0, 1.0, 1.0, // m1 scale
                false, false, false, true, 1.0
            },
            {
                {
                    0.0, 0.0,
                    0.0, 0.0,
                    0.0, 0.0,
                    0.0, 0.0,
                    0.0, 0.0,
                    0.0, 0.0,
                    0.0, 0.0,
                    0.0, 0.0,
                },
                0.0,
            }
        },
        /*
        POSITIONAL TESTING
        TODO: add more input tests with less rounded inputs
        TODO: add all other output modes
         */
        {
            "case: POSITIONAL | 0Y",
            {
                Mach1PlatformDefault, Mach1DecodeAlgoSpatial_8,
                0.0, 0.0, 0.0, // listener pos
                0.0, 0.0, 0.0, // listener rot
                0.0, 0.0, 5.0, // m1 pos
                0.0, 0.0, 0.0, // m1 rot
                1.0, 1.0, 1.0, // m1 scale
                false, false, false, false, 1.0
            },
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
                5.0,
            }
        },
        {
            "case: POSITIONAL | 0Y | Test Pos 1",
            // should require a 45 yaw deg rotation to face
            {
                Mach1PlatformDefault, Mach1DecodeAlgoSpatial_8,
                10.0, 0.0, 0.0, // listener pos
                0.0, 0.0, 0.0, // listener rot
                0.0, 0.0, 10.0, // m1 pos
                0.0, 0.0, 0.0, // m1 rot
                1.0, 1.0, 1.0, // m1 scale
                false, false, false, false, 1.0
            },
            {
                {
                    0.250000, 0.000000,
                    0.250000, 0.250000,
                    0.000000, 0.000000,
                    0.000000, 0.250000,
                    0.250000, 0.000000,
                    0.250000, 0.250000,
                    0.000000, 0.000000,
                    0.000000, 0.250000,
                },
                14.1421356237,
            }
        },
        {
            "case: POSITIONAL | 0Y | Test Pos 2",
            {
                Mach1PlatformDefault, Mach1DecodeAlgoSpatial_8,
                40.0, 0.0, 0.0, // listener pos
                -90.0, 0.0, 0.0, // listener rot
                0.0, 0.0, 0.0, // m1 pos
                0.0, 0.0, 0.0, // m1 rot
                1.0, 1.0, 1.0, // m1 scale
                false, false, false, false, 1.0
            },
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
                40.0,
            }
        },
        {
            "case: POSITIONAL | 0Y | Test Pos 3",
            {
                Mach1PlatformDefault, Mach1DecodeAlgoSpatial_8,
                0.0, 0.0, 0.0, // listener pos
                0.0, 90.0, 0.0, // listener rot
                0.0, 80.0, 0.0, // m1 pos
                0.0, 0.0, 0.0, // m1 rot
                1.0, 1.0, 1.0, // m1 scale
                false, false, false, false, 1.0
            },
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
                80.0,
            }
        },
        {
            "case: POSITIONAL | 90Y",
            {
                Mach1PlatformDefault, Mach1DecodeAlgoSpatial_8,
                0.0, 0.0, 0.0, //listener pos
                90.0, 0.0, 0.0, //listener rot
                0.0, 0.0, 5.0, //m1 pos
                0.0, 0.0, 0.0, //m1 rot
                1.0, 1.0, 1.0, //m1 scale
                false, false, false, false, 1.0
            },
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
                5.0,
            }
        },
        {
            "case: POSITIONAL | -90Y",
            {
                Mach1PlatformDefault, Mach1DecodeAlgoSpatial_8,
                0.0, 0.0, 0.0, //listener pos
                -90.0, 0.0, 0.0, //listener rot
                0.0, 0.0, 5.0, //m1 pos
                0.0, 0.0, 0.0, //m1 rot
                1.0, 1.0, 1.0, //m1 scale
                false, false, false, false, 1.0
            },
            {
                {
                    0.0, 0.5,
                    0.0, 0.0,
                    0.5, 0.0,
                    0.0, 0.0,
                    0.0, 0.5,
                    0.0, 0.0,
                    0.5, 0.0,
                    0.0, 0.0,
                },
                5.0,
            }
        },
        {
            "case: POSITIONAL | -90Y 45P",
            {
                Mach1PlatformDefault, Mach1DecodeAlgoSpatial_8,
                0.0, 0.0, 0.0, //listener pos
                -90.0, 45.0, 0.0, //listener rot
                0.0, 0.0, 5.0, //m1 pos
                0.0, 0.0, 0.0, //m1 rot
                1.0, 1.0, 1.0, //m1 scale
                false, false, false, false, 1.0
            },
            {
                {
                    0.064039, 0.602562,
                    0.000000, 0.166699,
                    0.602562, 0.064039,
                    0.166699, 0.000000,
                    0.000000, 0.166699,
                    0.000000, 0.000000,
                    0.166699, 0.000000,
                    0.000000, 0.000000,
                },
                5.0,
            }
        },
        {
            "case: POSITIONAL | 10Distance 45R",
            {
                Mach1PlatformDefault, Mach1DecodeAlgoSpatial_8,
                0.0, 0.0, 0.0, //listener pos
                0.0, 0.0, 45.0, //listener rot
                0.0, 0.0, 10.0, //m1 pos
                0.0, 0.0, 0.0, //m1 rot
                1.0, 1.0, 1.0, //m1 scale
                false, false, false, false, 1.0
            },
            {
                {
                    0.602562, 0.000000,
                    0.166699, 0.166699,
                    0.064040, 0.000000,
                    0.000000, 0.000000,
                    0.166699, 0.166699,
                    0.000000, 0.602562,
                    0.000000, 0.000000,
                    0.000000, 0.064040,
                },
                10.0,
            }
        },
        /*
        PLATFORM TESTING
         */
        {
            "case: POSITIONAL | Unity | 0Y",
            {
                Mach1PlatformUnity, Mach1DecodeAlgoSpatial_8,
                0.0, 0.0, 0.0, //listener pos in UNITY
                0.0, 0.0, 0.0, //listener rot in UNITY
                0.0, 0.0, 5.0, //m1 pos in UNITY
                0.0, 0.0, 0.0, //m1 rot in UNITY
                1.0, 1.0, 1.0, //m1 scale
                false, false, false, false, 1.0
            },
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
                5.0,
            }
        },
        {
            "case: POSITIONAL | Unity | 90Y",
            {
                Mach1PlatformUnity, Mach1DecodeAlgoSpatial_8,
                0.0, 0.0, 0.0, //listener pos in UNITY
                0.0, 90.0, 0.0, //listener rot in UNITY
                0.0, 0.0, 5.0, //m1 pos in UNITY
                0.0, 0.0, 0.0, //m1 rot in UNITY
                1.0, 1.0, 1.0, //m1 scale
                false, false, false, false, 1.0
            },
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
                5.0,
            }
        },
        {
            "case: POSITIONAL | Unity | POS LEFT 0Y0P0R",
            {
                Mach1PlatformUnity, Mach1DecodeAlgoSpatial_8,
                0.0, 0.0, 0.0, //listener pos in UNITY
                0.0, 0.0, 0.0, //listener rot in UNITY
                -3.0, 0.0, 0.0, //m1 pos in UNITY
                0.0, 0.0, 0.0, //m1 rot in UNITY
                1.0, 1.0, 1.0, //m1 scale
                false, false, false, false, 1.0
            },
            {
                {
                     0.000000, 0.000000,
                     0.500000, 0.000000,
                     0.000000, 0.000000,
                     0.000000, 0.500000,
                     0.000000, 0.000000,
                     0.500000, 0.000000,
                     0.000000, 0.000000,
                     0.000000, 0.500000,
                },
                3.0,
            }
        },
        {
            "case: POSITIONAL | Unity | LEFT POS 90Y",
            // This is equivelent to the obj being forward and to the right, and the listener facing to +90 Yaw to the right
            // meaning by facing -45deg Yaw to the left we face the obj
            {
                Mach1PlatformUnity, Mach1DecodeAlgoSpatial_8,
                0.0, 0.0, 0.0, //listener pos in UNITY
                0.0, 90.0, 0.0, //listener rot in UNITY
                5.0, 0.0, 5.0, //m1 pos in UNITY
                0.0, 0.0, 0.0, //m1 rot in UNITY
                1.0, 1.0, 1.0, //m1 scale
                false, false, false, false, 1.0
            },
            {
                {
                    0.250000, 0.000000,
                    0.250000, 0.250000,
                    0.000000, 0.000000,
                    0.000000, 0.250000,
                    0.250000, 0.000000,
                    0.250000, 0.250000,
                    0.000000, 0.000000,
                    0.000000, 0.250000,
                },
                7.07106781187,
            }
        },
        {
            "case: POSITIONAL | Unity | 90P",
            {
                Mach1PlatformUnity, Mach1DecodeAlgoSpatial_8,
                0.0, 0.0, 0.0, //listener pos in UNITY
                -90.0, 0.0, 0.0, //listener rot in UNITY
                0.0, 0.0, 5.0, //m1 pos in UNITY
                0.0, 0.0, 0.0, //m1 rot in UNITY
                1.0, 1.0, 1.0, //m1 scale
                false, false, false, false, 1.0
            },
            {
                {
                    0.500000, 0.000000,
                    0.000000, 0.500000,
                    0.500000, 0.000000,
                    0.000000, 0.500000,
                    0.000000, 0.000000,
                    0.000000, 0.000000,
                    0.000000, 0.000000,
                    0.000000, 0.000000,
                },
                5.0,
            }
        },
        {
            "case: POSITIONAL | Unity | 90R",
            {
                Mach1PlatformUnity, Mach1DecodeAlgoSpatial_8,
                0.0, 0.0, 0.0, //listener pos in UNITY
                0.0, 0.0, -90.0, //listener rot in UNITY
                0.0, 0.0, 5.0, //m1 pos in UNITY
                0.0, 0.0, 0.0, //m1 rot in UNITY
                1.0, 1.0, 1.0, //m1 scale
                false, false, false, false, 1.0
            },
            {
                {
                    0.500000, 0.000000,
                    0.500000, 0.000000,
                    0.000000, 0.000000,
                    0.000000, 0.000000,
                    0.000000, 0.500000,
                    0.000000, 0.500000,
                    0.000000, 0.000000,
                    0.000000, 0.000000,
                },
                5.0,
            }
        },
        {
            "case: POSITIONAL | Unity | 45R",
            {
                Mach1PlatformUnity, Mach1DecodeAlgoSpatial_8,
                0.0, 0.0, 0.0, //listener pos in UNITY
                0.0, 0.0, -45.0, //listener rot in UNITY
                0.0, 0.0, 5.0, //m1 pos in UNITY
                0.0, 0.0, 0.0, //m1 rot in UNITY
                1.0, 1.0, 1.0, //m1 scale
                false, false, false, false, 1.0
            },
            {
                {
                    0.602562, 0.000000,
                    0.166699, 0.166699,
                    0.064040, 0.000000,
                    0.000000, 0.000000,
                    0.166699, 0.166699,
                    0.000000, 0.602562,
                    0.000000, 0.000000,
                    0.000000, 0.064040,
                },
                5.0,
            }
        },
        {
            "case: POSITIONAL | Unity | 45Y 45P 45R",
            {
                Mach1PlatformUnity, Mach1DecodeAlgoSpatial_8,
                0.0, 0.0, 0.0, //listener pos in UNITY
                -45.0, 45.0, -45.0, //listener rot in UNITY
                0.0, 0.0, 5.0, //m1 pos in UNITY
                0.0, 0.0, 0.0, //m1 rot in UNITY
                1.0, 1.0, 1.0, //m1 scale
                false, false, false, false, 1.0
            },
            {
                {
                    0.449698, 0.000000,
                    0.278955, 0.345074,
                    0.176489, 0.000000,
                    0.073994, 0.222423,
                    0.020865, 0.000000,
                    0.000000, 0.268909,
                    0.000000, 0.000000,
                    0.000000, 0.163594,
                },
                5.0,
            }
        },
        {
            "case: POSITIONAL | UE | 0Y",
            {
                Mach1PlatformUE, Mach1DecodeAlgoSpatial_8,
                0.0, 0.0, 0.0, //listener pos
                0.0, 0.0, 0.0, //listener rot
                5.0, 0.0, 0.0, //m1 pos
                0.0, 0.0, 0.0, //m1 rot
                1.0, 1.0, 1.0, //m1 scale
                false, false, false, false, 1.0
            },
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
                5.0,
            }
        },
        {
            "case: POSITIONAL | UE | 90Y",
            {
                Mach1PlatformUE, Mach1DecodeAlgoSpatial_8,
                0.0, 0.0, 0.0, //listener pos
                0.0, 0.0, 90.0, //listener rot
                5.0, 0.0, 0.0, //m1 pos
                0.0, 0.0, 0.0, //m1 rot
                1.0, 1.0, 1.0, //m1 scale
                false, false, false, false, 1.0
            },
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
                5.0,
            }
        },
        {
            "case: POSITIONAL | UE | LEFT POS 90Y",
            {
                Mach1PlatformUE, Mach1DecodeAlgoSpatial_8,
                0.0, 0.0, 0.0, //listener pos
                0.0, 0.0, 90.0, //listener rot
                5.0, 5.0, 0.0, //m1 pos
                0.0, 0.0, 0.0, //m1 rot
                1.0, 1.0, 1.0, //m1 scale
                false, false, false, false, 1.0
            },
            {
                {
                    0.250000, 0.000000,
                    0.250000, 0.250000,
                    0.000000, 0.000000,
                    0.000000, 0.250000,
                    0.250000, 0.000000,
                    0.250000, 0.250000,
                    0.000000, 0.000000,
                    0.000000, 0.250000,
                },
                7.07107,
            }
        },
        {
            "case: POSITIONAL | UE | 45Y 45P 45R",
            {
                Mach1PlatformUE, Mach1DecodeAlgoSpatial_8,
                0.0, 0.0, 0.0, //listener pos
                45.0, 45.0, 45.0, //listener rot
                5.0, 0.0, 0.0, //m1 pos
                0.0, 0.0, 0.0, //m1 rot
                1.0, 1.0, 1.0, //m1 scale
                false, false, false, false, 1.0
            },
            {
                {
                    0.449697, 0.000000,
                    0.278955, 0.345074,
                    0.176489, 0.000000,
                    0.073994, 0.222423,
                    0.020865, 0.000000,
                    0.000000, 0.268909,
                    0.000000, 0.000000,
                    0.000000, 0.163594,
                },
                5.0,
            }
        },
    };

    std::cout << std::endl;

    for (auto test : cases) {
        Mach1DecodePositional m1Decode;

        m1Decode.setPlatformType(test.input.platformMode);
        m1Decode.setDecodeAlgoType(test.input.outputMode);
        m1Decode.setFilterSpeed(test.input.filterSpeed);
        m1Decode.setMuteWhenInsideObject(test.input.muteWhenInside);
        m1Decode.setMuteWhenOutsideObject(test.input.muteWhenOutside);

        m1Decode.setUseAttenuation(test.input.atttenuate);
        m1Decode.setUsePlaneCalculation(test.input.planeMode);

        m1Decode.setDecoderAlgoPosition(Mach1Point3D{test.input.m1PosX, test.input.m1PosY, test.input.m1PosZ});
        m1Decode.setDecoderAlgoRotation(Mach1Point3D{test.input.m1Yaw, test.input.m1Pitch, test.input.m1Roll});
        m1Decode.setDecoderAlgoScale(Mach1Point3D{test.input.scaleX, test.input.scaleY, test.input.scaleZ});
        m1Decode.setListenerRotation(Mach1Point3D{test.input.listenerYaw, test.input.listenerPitch, test.input.listenerRoll});
        m1Decode.setListenerPosition(Mach1Point3D{test.input.listenerPosX, test.input.listenerPosY, test.input.listenerPosZ});

        m1Decode.setUseYawForRotation(true);
        m1Decode.setUsePitchForRotation(true);
        m1Decode.setUseRollForRotation(true);
        
        std::vector<float> results;
        results.resize(m1Decode.getFormatCoeffCount());
        m1Decode.evaluatePositionResults();
        m1Decode.getCoefficients(results);

        // Distance Application:
        float distance = m1Decode.getDist();
        float attenuation = mapFloat(distance, 0, 10, 1, 0);
        /*
         Mapping distance to arbitrary linear curve
         Design your own distance coefficient curve here
         This example: Linear curve of 100% -> 0% from 0 to 10 distance away
        */

        std::cout
            << "testing " << test.name << ": "
            << platformModeNames[test.input.platformMode] << " > " << outputModeNames[test.input.outputMode] << ", "
            << "m1PosRot: " << test.input.m1PosX << "," << test.input.m1PosY << "," << test.input.m1PosZ << "|" << test.input.m1Yaw << "," << test.input.m1Pitch << "," << test.input.m1Roll << " || "
            << "ListenerPosRot: " << test.input.listenerPosX << "," << test.input.listenerPosY << "," << test.input.listenerPosZ << "|" << test.input.listenerYaw << "," << test.input.listenerPitch << "," << test.input.listenerRoll;

        int counter = 0;
        std::cout << "Array Size: " << results.size();

        bool distCheck = fabs(distance - test.output.distance) < 0.0001;

        for (size_t i = 0; i < results.size(); i++) {
            bool check = fabs(test.output.results[i] - results[i]) < 0.0001;
            counter += check;

            if (check == false) {
                TEST_CHECK_(check, "%s | Error with index [%d]", test.name.c_str(), i);
                std::cout << "index: [" << i << "]: " << results[i] << ", should be: " << test.output.results[i];
                std::cout << std::endl;
            }
            if (counter == results.size()) {
                std::cout << " "
                          << "\033[1;32mpassed\033[0m\n";
            }
        }

        if (distCheck == false) {
                std::cout << "\033[31m" << "FAILED " << "\033[0m" << "Distance: " << distance << " should be: " << test.output.distance;  
                std::cout << std::endl;
        }

    }

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    getchar();
#endif
}

TEST_LIST = {
    {"results", test_results},
    {NULL, NULL}};
