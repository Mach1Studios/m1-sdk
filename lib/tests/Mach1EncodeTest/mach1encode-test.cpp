#include "acutest.h"
#include "Mach1Encode.h"

#include <iostream>
#include <math.h>
#include <map>

void test_results(void)
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
			"Case: Encode MONO -> MACH1HORIZON",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode4Ch, true, 0.0, 0.0, 0.0, true, 0.0, 0.0 },
			{
				{
					{ 0.25, 0.25, 0.25, 0.25 },
				},
			}
		},
		{
			"Case: Encode MONO -> MACH1HORIZON | Diverge",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode4Ch, true, 0.0, 1.0, 0.0, true, 0.0, 0.0 },
			{
				{
					{ 0.5, 0.5, 0.0, 0.0 },
				},
			}
		},
		{
			"Case: Encode MONO -> MACH1HORIZON | Rotation",
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
			"Case: Encode MONO -> MACH1SPATIAL",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode8Ch, true, 0.0, 0.0, 0.0, true, 0.0, 0.0 },
			{
				{
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
				},
			}
		},
		{
			"Case: Encode MONO -> MACH1SPATIAL | Diverge",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode8Ch, true, 0.0, 1.0, 0.0, true, 0.0, 0.0 },
			{
				{
					{ 0.25, 0.25, 0.0, 0.0, 0.25, 0.25, 0.0, 0.0 },
				},
			}
		},
		{
			"Case: Encode MONO -> MACH1SPATIAL | Rotation",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode8Ch, true, 90.0, 1.0, 0.0, true, 0.0, 0.0 },
			{
				{
					{ 0.0, 0.25, 0.0, 0.25, 0.0, 0.25, 0.0, 0.25 },
				},
			}
		},
		{
			"Case: Encode MONO -> MACH1SPATIAL | Height",
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
			"Case: Encode STEREO -> MACH1SPATIAL",
			{ Mach1EncodeInputModeStereo, Mach1EncodeOutputMode8Ch, true, 0.0, 0.0, 0.0, true, 0.0, 0.0 },
			{
				{
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
				},
			}
		},
		{
			"Case: Encode STEREO -> MACH1SPATIAL | Diverge",
			{ Mach1EncodeInputModeStereo, Mach1EncodeOutputMode8Ch, true, 0.0, 1.0, 0.0, true, 0.0, 0.0 },
			{
				{
					{ 0.25, 0.25, 0.0, 0.0, 0.25, 0.25, 0.0, 0.0 },
					{ 0.25, 0.25, 0.0, 0.0, 0.25, 0.25, 0.0, 0.0 },
				},
			}
		},
		{
			"Case: Encode STEREO -> MACH1SPATIAL | Height",
			{ Mach1EncodeInputModeStereo, Mach1EncodeOutputMode8Ch, true, 0.0, 1.0, 90.0, true, 0.0, 0.0 },
			{
				{
					{ 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
					{ 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
				},
			}
		},
		{
			"Case: Encode STEREO -> MACH1SPATIAL | Stereo Spread",
			{ Mach1EncodeInputModeStereo, Mach1EncodeOutputMode8Ch, true, 0.0, 1.0, 0.0, true, 1.0, 0.0 },
			{
				{
					{ 0.5, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0 },
					{ 0.0, 0.5, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0 },
				},
			}
		},
		{
			"Case: Encode STEREO -> MACH1SPATIAL | Stereo Rotation",
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
			"Case: Encode QUAD -> MACH1SPATIAL",
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
			"Case: Encode QUAD -> MACH1SPATIAL | Diverge",
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
			"Case: Encode QUAD -> MACH1SPATIAL | Height",
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
			<< "testing " << test.name << ", "
			<< inputModeNames[test.input.inputMode] << " > " << outputModeNames[test.input.outputMode]
			<< std::endl;

		bool passed = true;
		for (size_t i = 0; i < results.size(); i++) {
			for (size_t j = 0; j < results[i].size(); j++) {
				bool check = fabs(test.output.results[i][j] - results[i][j]) < 0.0001;
				if (check == false) {
					TEST_CHECK_(check, "%s Pass. Results with index [%d][%d]", test.name.c_str(), i, j);
					passed = check;
				}
			}
		}
	}
}

TEST_LIST = {
	{ "results", test_results },
	{ NULL, NULL }
};
