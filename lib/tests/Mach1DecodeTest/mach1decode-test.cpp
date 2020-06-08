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

	struct INPUT_DATA {
		Mach1PlatformType platformMode;
		Mach1DecodeAlgoType outputMode;
		float yaw;
		float pitch;
		float roll;
		float filterSpeed;
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
TODO: add all other output modes
 */
		{
			"Case: MACH1SPATIAL | Y0P0R0",
			{ Mach1PlatformDefault, Mach1DecodeAlgoSpatial, 0.0, 0.0, 0.0, 1.0 },
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
			"Case: MACH1SPATIAL | Y90P0R0",
			{ Mach1PlatformDefault, Mach1DecodeAlgoSpatial, 90.0, 0.0, 0.0, 1.0 },
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
			"Case: MACH1SPATIAL | Y45P0R0",
			{ Mach1PlatformDefault, Mach1DecodeAlgoSpatial, 45.0, 0.0, 0.0, 1.0 },
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
			"Case: MACH1SPATIAL | Y0P90R0",
			{ Mach1PlatformDefault, Mach1DecodeAlgoSpatial, 0.0, 90.0, 0.0, 1.0 },
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
			"Case: MACH1SPATIAL | Y0P0R90",
			{ Mach1PlatformDefault, Mach1DecodeAlgoSpatial, 0.0, 0.0, 90.0, 1.0 },
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

		m1Decode.setRotationDegrees(Mach1Point3D{ test.input.yaw, test.input.pitch, test.input.roll });

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

	getchar();
}

TEST_LIST = {
	{ "results", test_results },
	{ NULL, NULL }
};
