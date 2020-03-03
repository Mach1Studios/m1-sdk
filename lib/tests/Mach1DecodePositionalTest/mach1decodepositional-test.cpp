#include "acutest.h"
#include "Mach1DecodePositional.h"

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
		float posX;
		float posY;
		float posZ;
		bool atttenuate;
		bool planeMode;
		bool blendMode;
		bool muteWhenInside;
		bool muteWhenOutside;
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
		POSITIONAL TESTING
		TODO: add more input tests with less rounded inputs
		TODO: add all other output modes
		 */
		{
			"Case: MACH1SPATIAL | Y0P0R0",
			{ Mach1PlatformDefault, Mach1DecodeAlgoSpatial, 0.0, 0.0, 0.0, 0.0, 0.0, 5.0, true, false, false, false, false, 1.0 },
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
			"Case: MACH1SPATIAL | Y90P0R0",
			{ Mach1PlatformDefault, Mach1DecodeAlgoSpatial, 90.0, 0.0, 0.0, 0.0, 0.0, 5.0, true, false, false, false, false, 1.0 },
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
		/*
		PLATFORM TESTING
		TODO: add more input tests with less rounded inputs
		 */
	};

	std::cout << std::endl;

	for (auto test : cases) {
		Mach1DecodePositional m1Decode;

		m1Decode.setPlatformType(test.input.platformMode);
		m1Decode.setDecodeAlgoType(test.input.outputMode);
		m1Decode.setFilterSpeed(test.input.filterSpeed);

		m1Decode.evaluatePositionResults();

		std::vector<float> results(20);
		m1Decode.getCoefficients(results);

		std::cout
			<< "testing " << test.name << ", "
			<< platformModeNames[test.input.platformMode] << " > " << outputModeNames[test.input.outputMode]
			<< std::endl;

		bool passed = true;
		for (size_t i = 0; i < results.size(); i++) {
			bool check = fabs(test.output.results[i] - results[i]) < 0.0001;
			if (check == false) {
				TEST_CHECK_(check, "%s Pass. Results with index [%d]", test.name.c_str(), i);
				passed = check;
			}
		}
	}
}

TEST_LIST = {
	{ "results", test_results },
	{ NULL, NULL }
};
