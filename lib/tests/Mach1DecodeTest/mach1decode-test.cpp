#include "acutest.h"
#include "Mach1Decode.h"

#include <iostream>
#include <math.h>
#include <map>

void test_results(void)
{
	std::map<Mach1PlatformType, std::string> platformModeNames = {
		{ Mach1PlatformDefault , "Default" },
		{ Mach1PlatformUnity , "Unity" },
		{ Mach1PlatformUE , "UE" },
		{ Mach1PlatformOfEasyCam , "OFEasyCam" },
		{ Mach1PlatformAndroid , "Android" },
		{ Mach1PlatformiOS , "iOS" },
	};

	std::map<Mach1DecodeAlgoType, std::string> outputModeNames = {
		{ Mach1DecodeAlgoSpatial , "Spatial - 8Ch" },
		{ Mach1DecodeAlgoAltSpatial , "Spatial Alt - 8Ch" },
		{ Mach1DecodeAlgoHorizon , "Horizon - 4Ch" },
		{ Mach1DecodeAlgoHorizonPairs , "Horizon Pairs - 8Ch" },
		{ Mach1DecodeAlgoSpatialPairs , "Spatial Pairs - 16Ch" },
		{ Mach1DecodeAlgoSpatialPlus , "Spatial Plus - 12Ch" },
		{ Mach1DecodeAlgoSpatialPlusPlus , "Spatial Plus Plus - 14Ch" },
		// { Mach1DecodeAlgoSpatialExt , "Spatial Ext - 16Ch" },
		// { Mach1DecodeAlgoSpatialExtPlus , "Spatial Ext Plus - 18Ch" },
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
        { signedDegrees, "-180->180"},
        { unsignedDegrees, "0->360"},
        { signedRadians, "-PI->PI"},
        { unsignedRadians, "0->2PI"},
        { signedNormalized, "-1.0->1.0"},
        { unsignedNormalized, "0->1"},
        { quaternion, "quat"},
    };

	struct INPUT_DATA {
		Mach1PlatformType platformMode;
		Mach1DecodeAlgoType outputMode;
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
            "Case: MACH1HORIZON | Y0P0R0",
            { Mach1PlatformDefault, Mach1DecodeAlgoHorizon, 0.0, 0.0, 0.0, 1.0, unsignedDegrees },
            {
                {
                    { 
                        1.0, 0.0,
                        0.0, 1.0,
                        0.0, 0.0,
                        0.0, 0.0,
                        1.0, 1.0,
                    },
                },
            }
        },
        {
            "Case: MACH1HORIZON | Y45P0R0",
            { Mach1PlatformDefault, Mach1DecodeAlgoHorizon, 45.0, 0.0, 0.0, 1.0, unsignedDegrees },
            {
                {
                    { 
                        0.5, 0.0,
                        0.5, 0.5,
                        0.0, 0.0,
                        0.0, 0.5,
                        1.0, 1.0,
                    },
                },
            }
        },
        {
            "Case: MACH1HORIZON | Y90P0R0",
            { Mach1PlatformDefault, Mach1DecodeAlgoHorizon, 90.0, 0.0, 0.0, 1.0, unsignedDegrees },
            {
                {
                    { 
                        0.0, 0.0,
                        1.0, 0.0,
                        0.0, 0.0,
                        0.0, 1.0,
                        1.0, 1.0,
                    },
                },
            }
        },
        {
            "Case: MACH1HORIZON | Y180P0R0",
            { Mach1PlatformDefault, Mach1DecodeAlgoHorizon, 180.0, 0.0, 0.0, 1.0, unsignedDegrees },
            {
                {
                    { 
                        0.0, 0.0,
                        0.0, 0.0,
                        0.0, 1.0,
                        1.0, 0.0,
                        1.0, 1.0,
                    },
                },
            }
        },
        {
            "Case: MACH1HORIZON | Y270P0R0",
            { Mach1PlatformDefault, Mach1DecodeAlgoHorizon, 270.0, 0.0, 0.0, 1.0, unsignedDegrees },
            {
                {
                    { 
                        0.0, 1.0,
                        0.0, 0.0,
                        1.0, 0.0,
                        0.0, 0.0,
                        1.0, 1.0,
                    },
                },
            }
        },
		{
			"Case: MACH1SPATIAL | Y0P0R0",
			{ Mach1PlatformDefault, Mach1DecodeAlgoSpatial, 0.0, 0.0, 0.0, 1.0, unsignedDegrees },
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
						1.0, 1.0,
					},
				},
			}
		},
		{
			"Case: MACH1SPATIAL | Y45P0R0",
			{ Mach1PlatformDefault, Mach1DecodeAlgoSpatial, 45.0, 0.0, 0.0, 1.0, unsignedDegrees },
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
						1.0, 1,0,
					},
				},
			}
		},
        {
            "Case: MACH1SPATIAL | Y90P0R0",
            { Mach1PlatformDefault, Mach1DecodeAlgoSpatial, 90.0, 0.0, 0.0, 1.0, unsignedDegrees },
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
                        1.0, 1,0,
                    },
                },
            }
        },
        {
            "Case: MACH1SPATIAL | Y90P0R0 (Norm)",
            { Mach1PlatformDefault, Mach1DecodeAlgoSpatial, 0.25, 0.0, 0.0, 1.0, unsignedNormalized },
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
                        1.0, 1,0,
                    },
                },
            }
        },
        {
            "Case: MACH1SPATIAL | Y90P0R0 (Radians)",
            { Mach1PlatformDefault, Mach1DecodeAlgoSpatial, 1.5708, 0.0, 0.0, 1.0, signedRadians },
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
                        1.0, 1,0,
                    },
                },
            }
        },
        {
            "Case: MACH1SPATIAL | Y135P0R0",
            { Mach1PlatformDefault, Mach1DecodeAlgoSpatial, 135.0, 0.0, 0.0, 1.0, unsignedDegrees },
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
                        1.0, 1,0,
                    },
                },
            }
        },
        {
            "Case: MACH1SPATIAL | Y180P0R0",
            { Mach1PlatformDefault, Mach1DecodeAlgoSpatial, 180.0, 0.0, 0.0, 1.0, unsignedDegrees },
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
                        1.0, 1,0,
                    },
                },
            }
        },
        {
            "Case: MACH1SPATIAL | Y225P0R0",
            { Mach1PlatformDefault, Mach1DecodeAlgoSpatial, 225.0, 0.0, 0.0, 1.0, unsignedDegrees },
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
                        1.0, 1,0,
                    },
                },
            }
        },
        {
            "Case: MACH1SPATIAL | Y-135P0R0",
            { Mach1PlatformDefault, Mach1DecodeAlgoSpatial, -135.0, 0.0, 0.0, 1.0, signedDegrees },
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
                        1.0, 1,0,
                    },
                },
            }
        },
        {
            "Case: MACH1SPATIAL | Y315P0R0",
            { Mach1PlatformDefault, Mach1DecodeAlgoSpatial, 315.0, 0.0, 0.0, 1.0, unsignedDegrees },
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
                        1.0, 1,0,
                    },
                },
            }
        },
        {
            "Case: MACH1SPATIAL | Y-45P0R0",
            { Mach1PlatformDefault, Mach1DecodeAlgoSpatial, -45.0, 0.0, 0.0, 1.0, signedDegrees },
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
                        1.0, 1,0,
                    },
                },
            }
        },
		{
			"Case: MACH1SPATIAL | Y0P90R0",
			{ Mach1PlatformDefault, Mach1DecodeAlgoSpatial, 0.0, 90.0, 0.0, 1.0, unsignedDegrees },
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
						1.0, 1,0,
					},
				},
			}
		},
        {
            "Case: MACH1SPATIAL | Y0P90R0 (Radians)",
            { Mach1PlatformDefault, Mach1DecodeAlgoSpatial, 0.0, 1.5708, 0.0, 1.0, signedRadians },
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
                        1.0, 1,0,
                    },
                },
            }
        },
        {
            "Case: MACH1SPATIAL | Y0P90R0 (Signed-Norm)",
            { Mach1PlatformDefault, Mach1DecodeAlgoSpatial, 0.0, 0.25, 0.0, 1.0, signedNormalized },
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
                        1.0, 1,0,
                    },
                },
            }
        },
        {
            "Case: MACH1SPATIAL | Y0P-90R0",
            { Mach1PlatformDefault, Mach1DecodeAlgoSpatial, 0.0, -90.0, 0.0, 1.0, signedDegrees },
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
                        1.0, 1,0,
                    },
                },
            }
        },
		{
			"Case: MACH1SPATIAL | Y0P0R90",
			{ Mach1PlatformDefault, Mach1DecodeAlgoSpatial, 0.0, 0.0, 90.0, 1.0, unsignedDegrees },
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
						1.0, 1,0,
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
						1.0, 1,0,
						*/ 
					},
				},
			}
		},
        {
            "Case: MACH1SPATIAL | Y0P0R90 (Radians)",
            { Mach1PlatformDefault, Mach1DecodeAlgoSpatial, 0.0, 0.0, 1.5708, 1.0, signedRadians },
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
                        1.0, 1,0,
                    },
                },
            }
        },
        {
            "Case: MACH1SPATIAL | Y0P0R90 (Norm)",
            { Mach1PlatformDefault, Mach1DecodeAlgoSpatial, 0.0, 0.0, 0.25, 1.0, signedNormalized },
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
                        1.0, 1,0,
                    },
                },
            }
        },
        {
            "Case: MACH1SPATIAL | Y0P0R-90 (Norm)",
            { Mach1PlatformDefault, Mach1DecodeAlgoSpatial, 0.0, 0.0, -0.25, 1.0, signedNormalized },
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
                        1.0, 1,0,
                    },
                },
            }
        },
		/*
PLATFORM TESTING
TODO: add more input tests with less rounded inputs
 */
	};

	std::cout << std::endl;

	for (auto test : cases) {
		Mach1Decode m1Decode;

		m1Decode.setPlatformType(test.input.platformMode);
		m1Decode.setDecodeAlgoType(test.input.outputMode);
		m1Decode.setFilterSpeed(test.input.filterSpeed);

        if (test.input.inputType == signedDegrees || test.input.inputType == unsignedDegrees){
            m1Decode.setRotationDegrees(Mach1Point3D{ test.input.yaw, test.input.pitch, test.input.roll });
        } else if (test.input.inputType == signedRadians || test.input.inputType == unsignedRadians){
            m1Decode.setRotationRadians(Mach1Point3D{ test.input.yaw, test.input.pitch, test.input.roll });
        } else if (test.input.inputType == signedNormalized || test.input.inputType == unsignedNormalized){
            m1Decode.setRotation(Mach1Point3D{ test.input.yaw, test.input.pitch, test.input.roll });
        } else if (test.input.inputType == quaternion) {
            //TODO: add quat check
            //m1Decode.setRotationQuat(Mach1Point4D{ test.input.w, test.input.x, test.input.y, test.input.z});
        } else {
            m1Decode.setRotationDegrees(Mach1Point3D{ test.input.yaw, test.input.pitch, test.input.roll });
        }

		m1Decode.beginBuffer();
        auto results = m1Decode.decodeCoeffs(0, 0);
        m1Decode.endBuffer();

		std::cout
			<< "testing " << test.name << ": "
			<< platformModeNames[test.input.platformMode] << " > " << outputModeNames[test.input.outputMode];

		int counter = 0;

		for (size_t i = 0; i < results.size(); i++) {
			bool check = fabs(test.output.results[i] - results[i]) < 0.0001;
			counter += check;
			if (check == false) {
				TEST_CHECK_(check, "%s | Error with index [%d]", test.name.c_str(), i);
				std::cout << "index: [" << i << "]: " << results[i] << ", should be: " << test.output.results[i];
				std::cout << std::endl;
			}
			if (counter == results.size()){
				std::cout << "... " << "\033[1;32mpassed\033[0m\n";
			}
		}
	}
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    getchar();
#endif
}

TEST_LIST = {
	{ "results", test_results },
	{ NULL, NULL }
};
