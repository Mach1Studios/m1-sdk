#include "Mach1Encode.h"
#include <acutest.h>

#include <iostream>
#include <map>
#include <math.h>

void test_results(void) {
    std::map<Mach1EncodeInputMode, std::string> inputModeNames = {
        {Mono, "MONO"},
        {Stereo, "STEREO"},
        {Quad, "QUAD"},
        {LCRS, "LCRS"},
        {AFormat, "AFORMAT"},
        {BFOAACN, "1OA-ACN"},
        {BFOAFUMA, "1OA-FUMA"},
        {B2OAACN, "2OA-ACN"},
        {B2OAFUMA, "2OA-FUMA"},
        {B3OAACN, "3OA-ACN"},
        {B3OAFUMA, "3OA-FUMA"},
        {LCR, "LCR"},
        {FiveDotZero, "5.0"},
        {FiveDotOneFilm, "5.1-FILM"},
        {FiveDotOneDTS, "5.1-DTS"},
        {FiveDotOneSMTPE, "5.1-SMPTE"}};

    std::map<Mach1EncodeOutputMode, std::string> outputModeNames = {
        {M1Spatial_4, "MACH1SPATIAL-4"},
        {M1Spatial_8, "MACH1SPATIAL-8"},
        {M1Spatial_12, "MACH1SPATIAL-12"},
        {M1Spatial_14, "MACH1SPATIAL-14"},
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
        Mach1EncodeInputMode inputMode;
        Mach1EncodeOutputMode outputMode;
        Mach1EncodePannerMode pannerMode;
        float azimuth;
        float diverge;
        float elevation;
        bool autoOrbit;
        float stereoSpread;
        float stereoRotate;
        InputType inputType;
    };

    struct OUTPUT_DATA {
        std::vector<std::vector<float>> results;
    };

    struct CASE {
        std::string name;
        INPUT_DATA input;
        OUTPUT_DATA output;
    };

    std::vector<CASE> cases = {
        /*
        MONO - MACH1HORIZON-4
        TODO: add more input tests with less rounded inputs
        TODO: add all other output modes
         */
        {"case: ENCODE | MONO->MACH1HORIZON-4",
         {Mono, M1Spatial_4, IsotropicLinear, 0.0, 0.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
         {
             {
                 {0.5, 0.5, 0.5, 0.5},
             },
         }},
        {"case: ENCODE | MONO->MACH1HORIZON-4 | Diverge",
         {Mono, M1Spatial_4, IsotropicLinear, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
         {
             {
                 {1.0, 1.0, 0.0, 0.0},
             },
         }},
        {"case: ENCODE | MONO->MACH1HORIZON-4 | Azimuth 90",
         {Mono, M1Spatial_4, IsotropicLinear, 90.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
         {
             {
                 {0.0, 1.0, 0.0, 1.0},
             },
         }},
        {"case: ENCODE | MONO->MACH1HORIZON-4 | Azimuth 180",
         {Mono, M1Spatial_4, IsotropicLinear, 180.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
         {
             {
                 {0.0, 0.0, 1.0, 1.0}, 
             },
         }},
        {"case: ENCODE | MONO->MACH1HORIZON-4 | Azimuth 270",
         {Mono, M1Spatial_4, IsotropicLinear, 270.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
         {
             {
                 {1.0, 0.0, 1.0, 0.0},
             },
         }},
        /*
        MONO - MACH1SPATIAL-8
        TODO: add more input tests with less rounded inputs
         */
        {
            "case: ENCODE | MONO->MACH1SPATIAL-8",
            {Mono, M1Spatial_8, IsotropicLinear, 0.0, 0.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
            {
                {
                    {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25},
                },
            }},
        {"case: ENCODE | MONO->MACH1SPATIAL-8 | Diverge",
         {Mono, M1Spatial_8, IsotropicLinear, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
         {
             {
                {0.5, 0.5, 0.0, 0.0, 0.5, 0.5, 0.0, 0.0}, 
             },
         }},
        {"case: ENCODE | MONO->MACH1SPATIAL-8 | Azimuth 90",
         {Mono, M1Spatial_8, IsotropicLinear, 90.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
         {
             {
                 {0.0, 0.5, 0.0, 0.5, 0.0, 0.5, 0.0, 0.5},
             },
         }},
        {"case: ENCODE | MONO->MACH1SPATIAL-8 | Azimuth 180",
         {Mono, M1Spatial_8, IsotropicLinear, 180.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
         {
             {
                 {0.0, 0.0, 0.5, 0.5, 0.0, 0.0, 0.5, 0.5},
             },
         }},
        {"case: ENCODE | MONO->MACH1SPATIAL-8 | Azimuth 270",
         {Mono, M1Spatial_8, IsotropicLinear, 270.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
         {
             {
                 {0.5, 0.0, 0.5, 0.0, 0.5, 0.0, 0.5, 0.0},
             },
         }},
        {"case: ENCODE | MONO->MACH1SPATIAL-8 | Height",
         {Mono, M1Spatial_8, IsotropicLinear, 90.0, 1.0, 90.0, true, 0.0, 0.0, unsignedDegrees},
         {
             {
                 {0.5, 0.5, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0},
             },
         }},
        {"case: ENCODE | MONO->MACH1SPATIAL-8 | Periphonic Height",
         {Mono, M1Spatial_8, PeriphonicLinear, 0.0, 1.0, 90.0, true, 0.0, 0.0, unsignedDegrees},
         {
             {
                 {1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
             },
         }},
        {"case: ENCODE | MONO->MACH1SPATIAL-8 | Periphonic Height Corner",
         {Mono, M1Spatial_8, PeriphonicLinear, 45.0, 1.0, 90.0, true, 0.0, 0.0, unsignedDegrees},
         {
             {
                 {0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
             },
         }},
        /*
        MONO - MACH1SPATIAL-12
        */
        {
            "case: ENCODE | MONO->MACH1SPATIAL-12",
            {Mono, M1Spatial_12, IsotropicLinear, 0.0, 0.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
            {
                {
                    {0.166666, 0.166666, 0.166666, 0.166666, 0.166666, 0.166666, 0.166666, 0.166666, 0.166666, 0.166666, 0.166666, 0.166666},
                },
            }},
        {"case: ENCODE | MONO->MACH1SPATIAL-12 | Diverge",
         {Mono, M1Spatial_12, IsotropicLinear, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
         {
             {
                 {0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0},
             },
         }},
        {"case: ENCODE | MONO->MACH1SPATIAL-12 | Azimuth 90",
         {Mono, M1Spatial_12, IsotropicLinear, 90.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
         {
             {
                 {0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0},
             },
         }},
        {"case: ENCODE | MONO->MACH1SPATIAL-12 | Azimuth 180",
         {Mono, M1Spatial_12, IsotropicLinear, 180.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
         {
             {
                 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0},
             },
         }},
        {"case: ENCODE | MONO->MACH1SPATIAL-12 | Azimuth 270",
         {Mono, M1Spatial_12, IsotropicLinear, 270.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
         {
             {
                 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
             },
         }},
        {"case: ENCODE | MONO->MACH1SPATIAL-12 | Height",
         {Mono, M1Spatial_12, IsotropicLinear, 90.0, 1.0, 90.0, true, 0.0, 0.0, unsignedDegrees},
         {
             {
                 {0.5, 0.5, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0},
             },
         }},
        /*
        MONO - MACH1SPATIAL-14
        */
        {
            "case: ENCODE | MONO->MACH1SPATIAL-14",
            {Mono, M1Spatial_14, IsotropicLinear, 0.0, 0.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
            {
                {
                    {0.142856, 0.142856, 0.142856, 0.142856, 0.142856, 0.142856, 0.142856, 0.142856, 0.142856, 0.142856, 0.142856, 0.142856, 0.142856, 0.142856},
                },
            }},
        {"case: ENCODE | MONO->MACH1SPATIAL-14 | Diverge",
         {Mono, M1Spatial_14, IsotropicLinear, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
         {
             {
                 {0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0},
             },
         }},
        {"case: ENCODE | MONO->MACH1SPATIAL-14 | Azimuth 90",
         {Mono, M1Spatial_14, IsotropicLinear, 90.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
         {
             {
                 {0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0},
             },
         }},
        {"case: ENCODE | MONO->MACH1SPATIAL-14 | Azimuth 180",
         {Mono, M1Spatial_14, IsotropicLinear, 180.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
         {
             {
                 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0},
             },
         }},
        {"case: ENCODE | MONO->MACH1SPATIAL-14 | Azimuth 270",
         {Mono, M1Spatial_14, IsotropicLinear, 270.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
         {
             {
                 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0},
             },
         }},
        {"case: ENCODE | MONO->MACH1SPATIAL-14 | Height",
         {Mono, M1Spatial_14, IsotropicLinear, 90.0, 1.0, 90.0, true, 0.0, 0.0, unsignedDegrees},
         {
             {
                 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0},
             },
         }},
        {
            "case: ENCODE | MONO->MACH1SPATIAL-8 | IsotropicLinear - Center",
            {Mono, M1Spatial_8, IsotropicLinear, 0.0, 0.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
            {
                {
                    {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25},
                },
            }
        },
        {
            "case: ENCODE | MONO->MACH1SPATIAL-8 | IsotropicEqualPower - Center",
            {Mono, M1Spatial_8, IsotropicEqualPower, 0.0, 0.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
            {
                {
                    {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5},
                },
            }
        },
        // Equal Power Mode - Cardinal Direction Tests
        {
            "case: ENCODE | MONO->MACH1SPATIAL-8 | IsotropicEqualPower - Front",
            {Mono, M1Spatial_8, IsotropicEqualPower, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
            {
                {
                    {0.5, 0.5, 0.0, 0.0, 0.5, 0.5, 0.0, 0.0},
                },
            }
        },
        {
            "case: ENCODE | MONO->MACH1SPATIAL-8 | IsotropicEqualPower - Right",
            {Mono, M1Spatial_8, IsotropicEqualPower, 90.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
            {
                {
                    {0.0, 0.5, 0.0, 0.5, 0.0, 0.5, 0.0, 0.5},
                },
            }
        },
        {
            "case: ENCODE | MONO->MACH1SPATIAL-8 | IsotropicEqualPower - Back",
            {Mono, M1Spatial_8, IsotropicEqualPower, 180.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
            {
                {
                    {0.0, 0.0, 0.5, 0.5, 0.0, 0.0, 0.5, 0.5},
                },
            }
        },
        {
            "case: ENCODE | MONO->MACH1SPATIAL-8 | IsotropicEqualPower - Left",
            {Mono, M1Spatial_8, IsotropicEqualPower, 270.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
            {
                {
                    {0.5, 0.0, 0.5, 0.0, 0.5, 0.0, 0.5, 0.0},
                },
            }
        },
        // Equal Power Mode - Diagonal Tests
        {
            "case: ENCODE | MONO->MACH1SPATIAL-8 | IsotropicEqualPower - Front-Right",
            {Mono, M1Spatial_8, IsotropicEqualPower, 45.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
            {
                {
                    {0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0},
                },
            }
        },
        {
            "case: ENCODE | MONO->MACH1SPATIAL-8 | IsotropicEqualPower - Edge Gain",
            {Mono, M1Spatial_8, IsotropicEqualPower, 90.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
            {
                {
                    {0.0, 0.5, 0.0, 0.5, 0.0, 0.5, 0.0, 0.5}, // No boost at edge
                },
            }
        },
        // Height Tests with Equal Power
        {
            "case: ENCODE | MONO->MACH1SPATIAL-8 | IsotropicEqualPower - Top Center",
            {Mono, M1Spatial_8, IsotropicEqualPower, 0.0, 1.0, 90.0, true, 0.0, 0.0, unsignedDegrees},
            {
                {
                    {0.5, 0.5, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0},
                },
            }
        },
        {
            "case: ENCODE | MONO->MACH1SPATIAL-8 | IsotropicEqualPower - Bottom Center",
            {Mono, M1Spatial_8, IsotropicEqualPower, 0.0, 1.0, -90.0, true, 0.0, 0.0, unsignedDegrees},
            {
                {
                    {0.0, 0.0, 0.0, 0.0, 0.5, 0.5, 0.5, 0.5},
                },
            }
        },
        /*
        STEREO - MACH1SPATIAL-8
        TODO: add more input tests with less rounded inputs
        TODO: add all other output modes
         */
        {
            "case: ENCODE | STEREO->MACH1SPATIAL-8",
            {Stereo, M1Spatial_8, IsotropicLinear, 0.0, 0.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
            {
                {
                    {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25},
                    {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25},
                },
            }},
        {"case: ENCODE | STEREO->MACH1SPATIAL-8 | Diverge",
         {Stereo, M1Spatial_8, IsotropicLinear, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
         {
             {
                 {0.5, 0.5, 0.0, 0.0, 0.5, 0.5, 0.0, 0.0},
                 {0.5, 0.5, 0.0, 0.0, 0.5, 0.5, 0.0, 0.0},
             },
         }},
        {"case: ENCODE | STEREO->MACH1SPATIAL-8 | Height",
         {Stereo, M1Spatial_8, IsotropicLinear, 0.0, 1.0, 90.0, true, 0.0, 0.0, unsignedDegrees},
         {
             {
                 {0.5, 0.5, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0},
                 {0.5, 0.5, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0},
             },
         }},
        {"case: ENCODE | STEREO->MACH1SPATIAL-8 | Stereo Spread",
         {Stereo, M1Spatial_8, IsotropicLinear, 0.0, 1.0, 0.0, true, 1.0, 0.0, unsignedDegrees},
         {
             {
                 {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0},
                 {0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0},
             },
         }},
        {"case: ENCODE | STEREO->MACH1SPATIAL-8 | Stereo Spread Height",
         {Stereo, M1Spatial_8, IsotropicLinear, 0.0, 1.0, 90.0, true, 1.0, 0.0, unsignedDegrees},
         {
             {
                 {1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0},
                 {0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0},
             },
         }},
        {"case: ENCODE | STEREO->MACH1SPATIAL-8 | Stereo Spread Height - Periphonic",
         {Stereo, M1Spatial_8, PeriphonicLinear, 0.0, 1.0, 90.0, true, 1.0, 0.0, unsignedDegrees},
         {
             {
                 {2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
                 {0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
             },
         }},
        {"case: ENCODE | STEREO->MACH1SPATIAL-8 | Stereo Rotation",
         {Stereo, M1Spatial_8, IsotropicLinear, 0.0, 0.0, 0.0, false, 1.0, 90.0, unsignedDegrees},
         {
             {
                 {0.5, 0.5, 0.0, 0.0, 0.5, 0.5, 0.0, 0.0},
                 {0.0, 0.0, 0.5, 0.5, 0.0, 0.0, 0.5, 0.5},
             },
         }},
        /*
        LCR
         */
        {
            "case: ENCODE | LCR->MACH1SPATIAL-8",
            {LCR, M1Spatial_8, IsotropicLinear, 0.0, 0.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
            {
                {
                    {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25},
                    {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25},
                    {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25},
                },
            }},
        {"case: ENCODE | LCR->MACH1SPATIAL-8 | Diverge",
         {LCR, M1Spatial_8, IsotropicLinear, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
         {
             {
                 {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0},
                 {0.5, 0.5, 0.0, 0.0, 0.5, 0.5, 0.0, 0.0},
                 {0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0},
             },
         }},
        /*
        QUAD - MACH1SPATIAL-8
        TODO: add more input tests with less rounded inputs
        TODO: add all other output modes
         */
        {
            "case: ENCODE | QUAD->MACH1SPATIAL-8",
            {Quad, M1Spatial_8, IsotropicLinear, 0.0, 0.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
            {
                {
                    {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25},
                    {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25},
                    {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25},
                    {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25},
                },
            }},
        {"case: ENCODE | QUAD->MACH1SPATIAL-8 | Diverge",
         {Quad, M1Spatial_8, IsotropicLinear, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
         {
             {
                 {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0},
                 {0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0},
                 {0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0},
                 {0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0},
             },
         }},
        {"case: ENCODE | QUAD->MACH1SPATIAL-8 | Height",
         {Quad, M1Spatial_8, IsotropicLinear, 0.0, 1.0, 90.0, true, 0.0, 0.0, unsignedDegrees},
         {
             {
                 {2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
                 {0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
                 {0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0},
                 {0.0, 0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 0.0},
             },
         }},
        /*
        LCRS
         */
        {
            "case: ENCODE | LCRS->MACH1SPATIAL-8",
            {LCRS, M1Spatial_8, IsotropicLinear, 0.0, 0.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
            {
                {
                    {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25},
                    {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25},
                    {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25},
                    {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25},
                },
            }},
        {"case: ENCODE | LCRS->MACH1SPATIAL-8 | Diverge",
         {LCRS, M1Spatial_8, IsotropicLinear, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
         {
             {
                 {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0},
                 {0.5, 0.5, 0.0, 0.0, 0.5, 0.5, 0.0, 0.0},
                 {0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0},
                 {0.0, 0.0, 0.5, 0.5, 0.0, 0.0, 0.5, 0.5},
             },
         }},
        /*
        AFormat
         */
        {
            "case: ENCODE | AFORMAT->MACH1SPATIAL-8",
            {AFormat, M1Spatial_8, IsotropicLinear, 0.0, 0.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
            {
                {
                    {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25},
                    {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25},
                    {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25},
                    {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25},
                },
            }},
        {"case: ENCODE | AFORMAT->MACH1SPATIAL-8 | Diverge",
         {AFormat, M1Spatial_8, IsotropicLinear, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
         {
             {
                 {0.904223*2, 0.0475445*2, 0.0475445*2, 0.000687696*2, 0.0, 0.0, 0.0, 0.0},
                 {0.0, 0.0, 0.0, 0.0, 0.0475445*2, 0.904223*2, 0.000687696*2, 0.0475445*2},
                 {0.0, 0.0, 0.0, 0.0, 0.0475445*2, 0.000687696*2, 0.904223*2, 0.0475445*2},
                 {0.000687696*2, 0.0475445*2, 0.0475445*2, 0.904223*2, 0.0, 0.0, 0.0, 0.0},
             },
         }},
        /*
        1OA ACNSN3D
         */
        {
            "case: ENCODE | 1OA-ACN->MACH1SPATIAL-8",
            {BFOAACN, M1Spatial_8, IsotropicLinear, 0.0, 0.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
            {
                {
                    {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25},
                    {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25},
                    {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25},
                    {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25},
                    {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25},
                    {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25},
                    {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25},
                },
            }},
        /*
        1OA FuMa
         */
        {
            "case: ENCODE | 1OA-FUMA->MACH1SPATIAL-8",
            {BFOAFUMA, M1Spatial_8, IsotropicLinear, 0.0, 0.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
            {
                {
                    {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25},
                    {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25},
                    {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25},
                    {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25},
                    {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25},
                    {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25},
                    {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25},
                },
            }},
        /*
        SURROUND 5.0, 5.1
        */
        {
            "case: ENCODE | 5.0->MACH1SPATIAL-8 | Diverge", // "L","C","R","Ls","Rs"
            {FiveDotZero, M1Spatial_8, IsotropicLinear, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
            {
                {
                    {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0},
                    {0.5, 0.5, 0.0, 0.0, 0.5, 0.5, 0.0, 0.0},
                    {0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0},
                    {0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0},
                    {0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0},
                },
            }},
        {"case: ENCODE | 5.1->MACH1SPATIAL-8 | Diverge", // "L","C","R","Ls","Rs","LFE"
         {FiveDotOneFilm, M1Spatial_8, IsotropicLinear, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
         {
             {
                 {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0},
                 {0.5, 0.5, 0.0, 0.0, 0.5, 0.5, 0.0, 0.0},
                 {0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0},
                 {0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0},
                 {0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0},
                 {0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25},
             },
         }},
        /*
        Input Data Checks
         */
        {
            "case: ENCODE | Azimuth: signed degrees",
            {Mono, M1Spatial_8, IsotropicLinear, -90.0, 1.0, 0.0, true, 0.0, 0.0, signedDegrees},
            {
                {
                    {0.5, 0.0, 0.5, 0.0, 0.5, 0.0, 0.5, 0.0},
                },
            }},
        {"case: ENCODE | Azimuth: unsigned degrees",
         {Mono, M1Spatial_8, IsotropicLinear, 270.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees},
         {
             {
                 {0.5, 0.0, 0.5, 0.0, 0.5, 0.0, 0.5, 0.0},
             },
         }},
        {"case: ENCODE | Azimuth: signed radians",
         {Mono, M1Spatial_8, IsotropicLinear, -1.5707963268, 1.0, 0.0, true, 0.0, 0.0, signedRadians},
         {
             {
                 {0.5, 0.0, 0.5, 0.0, 0.5, 0.0, 0.5, 0.0},
             },
         }},
        {"case: ENCODE | Azimuth: unsigned radians",
         {Mono, M1Spatial_8, IsotropicLinear, 4.7123889804, 1.0, 0.0, true, 0.0, 0.0, unsignedRadians},
         {
             {
                 {0.5, 0.0, 0.5, 0.0, 0.5, 0.0, 0.5, 0.0},
             },
         }},
        {"case: ENCODE | Azimuth: signed norm",
         {Mono, M1Spatial_8, IsotropicLinear, -0.25, 1.0, 0.0, true, 0.0, 0.0, signedNormalized},
         {
             {
                 {0.5, 0.0, 0.5, 0.0, 0.5, 0.0, 0.5, 0.0},
             },
         }},
        {"case: ENCODE | Azimuth: unsigned norm",
         {Mono, M1Spatial_8, IsotropicLinear, 0.75, 1.0, 0.0, true, 0.0, 0.0, unsignedNormalized},
         {
             {
                 {0.5, 0.0, 0.5, 0.0, 0.5, 0.0, 0.5, 0.0},
             },
         }},
        {"case: ENCODE | Elevation: signed degrees",
         {Mono, M1Spatial_8, IsotropicLinear, 0.0, 1.0, -90.0, true, 0.0, 0.0, signedDegrees},
         {
             {
                 {0.0, 0.0, 0.0, 0.0, 0.5, 0.5, 0.5, 0.5},
             },
         }},
        {"case: ENCODE | Elevation: signed radians",
         {Mono, M1Spatial_8, IsotropicLinear, 0.0, 1.0, -1.5707963268, true, 0.0, 0.0, signedRadians},
         {
             {
                 {0.0, 0.0, 0.0, 0.0, 0.5, 0.5, 0.5, 0.5},
             },
         }},
        {"case: ENCODE | Elevation: signed norm",
         {Mono, M1Spatial_8, IsotropicLinear, 0.0, 1.0, -1.0, true, 0.0, 0.0, signedNormalized},
         {
             {
                 {0.0, 0.0, 0.0, 0.0, 0.5, 0.5, 0.5, 0.5},
             },
         }},
        {"case: ENCODE | Elevation: unsigned norm",
         {Mono, M1Spatial_8, IsotropicLinear, 0.0, 1.0, -1.0, true, 0.0, 0.0, unsignedNormalized},
         {
             {
                 {0.0, 0.0, 0.0, 0.0, 0.5, 0.5, 0.5, 0.5}, // TODO: uh oh!?
             },
         }},
        // TODO: write out the results for OrbitRotation input checks
        //  {
        //      "OrbitRotation: signed degrees",
        //      { Mono, M1Spatial_8, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, signedDegrees },
        //      {
        //          {
        //              { 0.0, 0.0, 0.0, 0.0, 0.25, 0.25, 0.25, 0.25 },
        //          },
        //      }
        //  },
        //  {
        //      "OrbitRotation: unsigned degrees",
        //      { Stereo, M1Spatial_8, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
        //      {
        //          {
        //              { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
        //          },
        //      }
        //  },
        //  {
        //      "OrbitRotation: signed radians",
        //      { Stereo, M1Spatial_8, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, signedRadians },
        //      {
        //          {
        //              { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
        //          },
        //      }
        //  },
        //  {
        //      "OrbitRotation: unsigned radians",
        //      { Stereo, M1Spatial_8, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedRadians },
        //      {
        //          {
        //              { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
        //          },
        //      }
        //  },
        //  {
        //      "OrbitRotation: signed norm",
        //      { Stereo, M1Spatial_8, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, signedRadians },
        //      {
        //          {
        //              { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
        //          },
        //      }
        //  },
        //  {
        //      "OrbitRotation: unsigned norm",
        //      { Stereo, M1Spatial_8, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedRadians },
        //      {
        //          {
        //              { 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
        //          },
        //      }
        //  },
    };

    std::cout << std::endl;

    int passedCases = 0;

    for (auto test : cases) {
        Mach1Encode<float> m1Encode;

        m1Encode.setInputMode(test.input.inputMode);
        m1Encode.setOutputMode(test.input.outputMode);

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
        m1Encode.setStereoSpread(test.input.stereoSpread);
        if (test.input.inputType == signedDegrees || test.input.inputType == unsignedDegrees) {
            m1Encode.setOrbitRotationDegrees(test.input.stereoRotate);
        } else if (test.input.inputType == signedRadians || test.input.inputType == unsignedRadians) {
            m1Encode.setOrbitRotationRadians(test.input.stereoRotate);
        } else if (test.input.inputType == signedNormalized || test.input.inputType == unsignedNormalized) {
            m1Encode.setOrbitRotation(test.input.stereoRotate);
        } else if (test.input.inputType == quaternion) {
            // case for quat
        }

        m1Encode.setPannerMode(test.input.pannerMode);

        m1Encode.generatePointResults();
        // auto points = m1Encode.getPoints();
        auto results = m1Encode.getGains();

        std::cout
            << "testing " << test.name << ": "
            << inputModeNames[test.input.inputMode] << " > " << outputModeNames[test.input.outputMode] << " with " << inputTypeNames[test.input.inputType];

        bool passed = true;
        for (size_t i = 0; i < results.size(); i++) {
            int counter = 0;
            for (size_t j = 0; j < results[i].size(); j++) {
                bool check = fabs(test.output.results[i][j] - results[i][j]) < 0.0001;
                counter += check;
                if (check == false) {
                    TEST_CHECK_(check, "%s | Error with index [%zu][%zu]", test.name.c_str(), i, j);
                    std::cout << "index: [" << i << "][" << j << "]: " << test.output.results[i][j] << ", should be: " << results[i][j];
                    std::cout << std::endl;
                }
            }
            if (counter != results[i].size()) {
                passed = false;
            }
        }
        if (passed) {
            std::cout << "... "
                      << "\033[1;32mpassed\033[0m\n"; // TODO: make this also work in windows
            passedCases++;
        }
    }
    std::cout << "passed cases: " << passedCases << "\n"; // TODO: make this also work in windows
}

TEST_LIST = {
    {"results", test_results},
    {NULL, NULL}};
