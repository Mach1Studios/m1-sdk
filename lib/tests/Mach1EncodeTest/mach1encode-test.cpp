#include "acutest.h"
#include "Mach1Encode.h"

#include <iostream>
#include <math.h>
#include <map>

void test_results(void)
{
	std::map<Mach1EncodeInputModeType, std::string> inputModeNames = {
		{ Mach1EncodeInputModeMono , "MONO" },
		{ Mach1EncodeInputModeStereo , "STEREO" },
		{ Mach1EncodeInputModeQuad , "QUAD" },
		{ Mach1EncodeInputModeLCRS , "LCRS" },
		{ Mach1EncodeInputModeAFormat , "AFORMAT" },
		{ Mach1EncodeInputModeBFormat , "1OA-ACN-DEPRE" },
		{ Mach1EncodeInputModeBFOAACN , "1OA-ACN" },
		{ Mach1EncodeInputModeBFOAFUMA , "1OA-FUMA" },
		{ Mach1EncodeInputModeB2OAACN , "2OA-ACN" },
		{ Mach1EncodeInputModeB2OAFUMA , "2OA-FUMA" },
		{ Mach1EncodeInputModeB3OAACN , "3OA-ACN" },
		{ Mach1EncodeInputModeB3OAFUMA , "3OA-FUMA" },
		{ Mach1EncodeInputModeLCR , "LCR" },
	};

	std::map<Mach1EncodeOutputModeType, std::string> outputModeNames = {
		{ Mach1EncodeOutputMode4Ch , "MACH1HORIZON" },
		{ Mach1EncodeOutputMode8Ch , "MACH1SPATIAL" },
		{ Mach1EncodeOutputMode12Ch , "MACH1SPATIAL+" },
		{ Mach1EncodeOutputMode14Ch , "MACH1SPATIAL++" },
		{ Mach1EncodeOutputMode16Ch , "MACH1SPATIALEXT" },
		{ Mach1EncodeOutputMode18Ch , "MACH1SPATIALEXT+" },
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
		std::vector<std::vector<float>> results;
	};
	
	struct CASE {
		std::string name;
		INPUT_DATA input;
		OUTPUT_DATA output;
	};

	std::vector<CASE> cases = {
/*
MONO - MACH1HORIZON
TODO: add more input tests with less rounded inputs
TODO: add all other output modes
 */
		{
			"Encode",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode4Ch, true, 0.0, 0.0, 0.0, true, 0.0, 0.0 },
			{
				{
					{ 0.25, 0.25, 0.25, 0.25 },
				},
			}
		},
		{
			"Encode Diverge",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode4Ch, true, 0.0, 1.0, 0.0, true, 0.0, 0.0 },
			{
				{
					{ 0.5, 0.5, 0.0, 0.0 },
				},
			}
		},
		{
			"Encode Rotation",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode4Ch, true, 90.0, 1.0, 0.0, true, 0.0, 0.0 },
			{
				{
					{ 0.0, 0.5, 0.0, 0.5 },
				},
			}
		},
/*
MONO - MACH1SPATIAL
TODO: add more input tests with less rounded inputs
 */
		{
			"Encode",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode8Ch, true, 0.0, 0.0, 0.0, true, 0.0, 0.0 },
			{
				{
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
				},
			}
		},
		{
			"Encode Diverge",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode8Ch, true, 0.0, 1.0, 0.0, true, 0.0, 0.0 },
			{
				{
					{ 0.25, 0.25, 0.0, 0.0, 0.25, 0.25, 0.0, 0.0 },
				},
			}
		},
		{
			"Encode Rotation",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode8Ch, true, 90.0, 1.0, 0.0, true, 0.0, 0.0 },
			{
				{
					{ 0.0, 0.25, 0.0, 0.25, 0.0, 0.25, 0.0, 0.25 },
				},
			}
		},
		{
			"Encode Height",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode8Ch, true, 90.0, 1.0, 90.0, true, 0.0, 0.0 },
			{
				{
					{ 0.25, 0.25, 0.25, 0.25, 0.0, 0.0, 0.0, 0.0 },
				},
			}
		},
/*
STEREO - MACH1SPATIAL
TODO: add more input tests with less rounded inputs
TODO: add all other output modes
 */
		{
			"Encode",
			{ Mach1EncodeInputModeStereo, Mach1EncodeOutputMode8Ch, true, 0.0, 0.0, 0.0, true, 0.0, 0.0 },
			{
				{
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
				},
			}
		},
		{
			"Encode Diverge",
			{ Mach1EncodeInputModeStereo, Mach1EncodeOutputMode8Ch, true, 0.0, 1.0, 0.0, true, 0.0, 0.0 },
			{
				{
					{ 0.25, 0.25, 0.0, 0.0, 0.25, 0.25, 0.0, 0.0 },
					{ 0.25, 0.25, 0.0, 0.0, 0.25, 0.25, 0.0, 0.0 },
				},
			}
		},
		{
			"Encode Height",
			{ Mach1EncodeInputModeStereo, Mach1EncodeOutputMode8Ch, true, 0.0, 1.0, 90.0, true, 0.0, 0.0 },
			{
				{
					{ 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
					{ 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
				},
			}
		},
		{
			"Encode Stereo Spread",
			{ Mach1EncodeInputModeStereo, Mach1EncodeOutputMode8Ch, true, 0.0, 1.0, 0.0, true, 1.0, 0.0 },
			{
				{
					{ 0.5, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0 },
					{ 0.0, 0.5, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0 },
				},
			}
		},
		{
			"Encode Stereo Rotation",
			{ Mach1EncodeInputModeStereo, Mach1EncodeOutputMode8Ch, true, 0.0, 1.0, 0.0, false, 1.0, 90.0 },
			{
				{
					{ 0.0, 0.5, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0 },
					{ 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.5 },
				},
			}
		},
/*
QUAD - MACH1SPATIAL
TODO: add more input tests with less rounded inputs
TODO: add all other output modes
 */
		{
			"Encode",
			{ Mach1EncodeInputModeStereo, Mach1EncodeOutputMode8Ch, true, 0.0, 0.0, 0.0, true, 0.0, 0.0 },
			{
				{
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
				},
			}
		},
		{
			"Encode Diverge",
			{ Mach1EncodeInputModeStereo, Mach1EncodeOutputMode8Ch, true, 0.0, 1.0, 0.0, true, 0.0, 0.0 },
			{
				{
					{ 0.5, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0 },
					{ 0.0, 0.5, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0 },
					{ 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.5, 0.0 },
					{ 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.5 },
				},
			}
		},
		{
			"Encode Height",
			{ Mach1EncodeInputModeStereo, Mach1EncodeOutputMode8Ch, true, 0.0, 1.0, 90.0, true, 0.0, 0.0 },
			{
				{
					{ 0.0, 0.5, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0 },
					{ 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.5 },
					{ 0.5, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0 },
					{ 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.5, 0.0 },
				},
			}
		},
/*
LCRS
 */

/*
1OA ACNSN3D
 */

/*
1OA FuMa
 */

/*
LCR
 */
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
		auto results = m1Encode.getGains();

		std::cout
			<< "testing " << test.name << ": "
			<< inputModeNames[test.input.inputMode] << " > " << outputModeNames[test.input.outputMode];

		int counter = 0;

		for (size_t i = 0; i < results.size(); i++) {
			for (size_t j = 0; j < results[i].size(); j++) {
				bool check = fabs(test.output.results[i][j] - results[i][j]) < 0.0001;
				counter += check;
				if (check == false) {
					TEST_CHECK_(check, "%s | Results with index [%d][%d]", test.name.c_str(), i, j);
				}
			}
			if (counter == results[i].size()){
				std::cout << "... " << "\033[1;32mpassed\033[0m\n";
			}
		}
	}
}

TEST_LIST = {
	{ "results", test_results },
	{ NULL, NULL }
};
