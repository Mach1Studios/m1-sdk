#include "acutest.h"
#include "Mach1Encode.h"

#include <iostream>
#include <map>

void test_gains(void)
{
	std::map<Mach1EncodeInputModeType, std::string> inputModeNames = {
		{ Mach1EncodeInputModeMono , "Mono" },
		{ Mach1EncodeInputModeStereo , "Stereo" },
		{ Mach1EncodeInputModeQuad , "Quad" },
		{ Mach1EncodeInputModeLCRS , "LCRS" },
		{ Mach1EncodeInputModeAFormat , "AFormat" },
		{ Mach1EncodeInputModeBFormat , "BFormat" },
		{ Mach1EncodeInputModeBFOAACN , "BFOAACN" },
		{ Mach1EncodeInputModeBFOAFUMA , "BFOAFUMA" },
		{ Mach1EncodeInputModeB2OAACN , "B2OAACN" },
		{ Mach1EncodeInputModeB2OAFUMA , "B2OAFUMA" },
		{ Mach1EncodeInputModeB3OAACN , "B3OAACN" },
		{ Mach1EncodeInputModeB3OAFUMA , "B3OAFUMA" },
		{ Mach1EncodeInputModeLCR , "LCR" },
	};

	std::map<Mach1EncodeOutputModeType, std::string> outputModeNames = {
		{ Mach1EncodeOutputMode4Ch , "4Ch" },
		{ Mach1EncodeOutputMode8Ch , "8Ch" },
		{ Mach1EncodeOutputMode12Ch , "12Ch" },
		{ Mach1EncodeOutputMode14Ch , "14Ch" },
		{ Mach1EncodeOutputMode16Ch , "16Ch" },
		{ Mach1EncodeOutputMode18Ch , "18Ch" },
	};

	struct INPUT_DATA {
		Mach1EncodeInputModeType inputMode;
		Mach1EncodeOutputModeType outputMode;
		bool isotropicEncode;
		float rotation;
		float diverge;
		float pitch;
		bool autoOrbit;
		float stereoSpread;
		float stereoRotate;
	};

	struct OUTPUT_DATA {
		std::vector<std::vector<float>> gains;
	};
	
	struct CASE {
		std::string name;
		INPUT_DATA input;
		OUTPUT_DATA output;
	};

	std::vector<CASE> cases = {
		{
			"case 01",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode4Ch, false, 0.0, 0.0, 0.0, false, 0.0, 0.0 },
			{
				{
					{ 0.25, 0.25, 0.25, 0.25 },
				},
			}
		},
		{
			"case 02",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode8Ch, false, 0.0, 0.0, 0.0, false, 0.0, 0.0 },
			{
				{
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
				},
			}
		},
		{
			"case 03",
			{ Mach1EncodeInputModeStereo, Mach1EncodeOutputMode8Ch, false, 0.0, 0.0, 0.0, false, 0.0, 0.0 },
			{
				{
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
				},
			}
		},
	};
	
	std::cout << std::endl;

	for (auto test : cases) {
		Mach1Encode m1Encode;

		m1Encode.setInputMode(test.input.inputMode);
		m1Encode.setOutputMode(test.input.outputMode);

		m1Encode.setIsotropicEncode(test.input.isotropicEncode);

		m1Encode.setRotation(test.input.rotation);
		m1Encode.setDiverge(test.input.diverge);
		m1Encode.setPitch(test.input.pitch);

		m1Encode.setAutoOrbit(test.input.autoOrbit);
		m1Encode.setStereoSpread(test.input.stereoSpread);
		m1Encode.setStereoRotate(test.input.stereoRotate);

		m1Encode.generatePointResults();
		//auto points = m1Encode.getPoints();
		auto gains = m1Encode.getGains();

		std::cout
			<< "testing " << test.name << ", "
			<< inputModeNames[test.input.inputMode] << " > " << outputModeNames[test.input.outputMode]
			<< std::endl;

		bool passed = true;
		for (size_t i = 0; i < gains.size(); i++) {
			for (size_t j = 0; j < gains[i].size(); j++) {
				bool check = fabs(test.output.gains[i][j] - gains[i][j]) < 0.0001;
				if (check == false) {
					TEST_CHECK_(check, "%s pass. gain with index [%d][%d]", test.name.c_str(), i, j);
					passed = check;
				}
			}
		}
	}
}

TEST_LIST = {
	{ "gains", test_gains },
	{ NULL, NULL }
};
