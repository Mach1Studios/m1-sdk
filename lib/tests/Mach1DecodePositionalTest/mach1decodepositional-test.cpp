#include "acutest.h"
#include "Mach1DecodePositional.h"

#include <iostream>
#include <math.h>
#include <map>

float mapFloat(float input, float inMin, float inMax, float outMin, float outMax){
    return (input - inMin) / (inMax - inMin) * (outMax - outMin) + outMin;
}

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
		float listenerPosX;
		float listenerPosY;
		float listenerPosZ;
		float listenerYaw;
		float listenerPitch;
		float listenerRoll;
		float m1PosX;
		float m1PosY;
		float m1PosZ;
		float m1Yaw;
		float m1Pitch;
		float m1Roll;
		float scaleX;
		float scaleY;
		float scaleZ;
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
			"Case: 0 ",
			{ Mach1PlatformDefault, Mach1DecodeAlgoSpatial, 
				0.0, 0.0, 0.0, //listener pos
				0.0, 0.0, 0.0, //listener rot
				0.0, 0.0, 5.0, //m1 pos
				0.0, 0.0, 0.0, //m1 rot
				1.0, 1.0, 1.0, //m1 scale
				false, false, false, false, false, 1.0 },
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
				5.0,
			}
		},
		{
			"Case: 90 Yaw ",
			{ Mach1PlatformDefault, Mach1DecodeAlgoSpatial, 
				0.0, 0.0, 0.0, //listener pos
				90.0, 0.0, 0.0, //listener rot
				0.0, 0.0, 5.0, //m1 pos
				0.0, 0.0, 0.0, //m1 rot
				1.0, 1.0, 1.0, //m1 scale
				false, false, false, false, false, 1.0 },
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
					1.0, 1.0,
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
		m1Decode.setUseBlendMode(test.input.blendMode);
		m1Decode.setIgnoreTopBottom(false);
		m1Decode.setMuteWhenInsideObject(test.input.muteWhenInside);
		m1Decode.setMuteWhenOutsideObject(test.input.muteWhenOutside);

		m1Decode.setUseAttenuation(test.input.atttenuate);
		m1Decode.setUsePlaneCalculation(test.input.planeMode);

		m1Decode.setDecoderAlgoPosition(Mach1Point3D {test.input.m1PosX, test.input.m1PosY, test.input.m1PosZ});
		m1Decode.setDecoderAlgoRotation(Mach1Point3D {test.input.m1Yaw, test.input.m1Pitch, test.input.m1Roll});
		m1Decode.setDecoderAlgoScale(Mach1Point3D {test.input.scaleX, test.input.scaleY, test.input.scaleZ});
		m1Decode.setListenerRotation(Mach1Point3D {test.input.listenerYaw, test.input.listenerPitch, test.input.listenerRoll});
		m1Decode.setListenerPosition(Mach1Point3D {test.input.listenerPosX, test.input.listenerPosY, test.input.listenerPosZ});

		m1Decode.setUseYawForRotation(true);
		m1Decode.setUsePitchForRotation(true);
		m1Decode.setUseRollForRotation(true);
		//Distance Application:
		float distance = m1Decode.getDist();

		/*
		 Mapping distance to arbitrary linear curve
		 Design your own distance coefficient curve here
		 This example: Linear curve of 100% -> 0% from 0 to 10 distance away
		*/
		float attenuation = mapFloat(distance, 0, 10, 1, 0);

		m1Decode.evaluatePositionResults();
		std::vector<float> results(20);
		m1Decode.getCoefficients(results);

		std::cout
			<< "testing " << test.name << ": "
			<< platformModeNames[test.input.platformMode] << " > " << outputModeNames[test.input.outputMode] << ", "
			<< "m1PosRot: " << test.input.m1PosX << "," << test.input.m1PosY << "," << test.input.m1PosZ << "|" << test.input.m1Yaw << "," << test.input.m1Pitch << "," << test.input.m1Roll << " || "
			<< "ListenerPosRot: " << test.input.listenerYaw << "," << test.input.listenerPitch << "," << test.input.listenerRoll << "|" << test.input.listenerPosX << "," << test.input.listenerPosY << "," << test.input.listenerPosZ;

		int counter = 0;

		for (size_t i = 0; i < results.size(); i++) {
			bool check = fabs(test.output.results[i] - results[i]) < 0.0001;
			counter += check;
			if (check == false) {
				TEST_CHECK_(check, "%s | Error with index [%d]", test.name.c_str(), i);
			}
			if (counter == results.size()){
				std::cout << "... " << "\033[1;32mpassed\033[0m\n";
			}
		}
	}
}

TEST_LIST = {
	{ "results", test_results },
	{ NULL, NULL }
};
