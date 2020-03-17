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
		Mach1EncodeInputModeType inputMode;
		Mach1EncodeOutputModeType outputMode;
		bool isotropicEncode;
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
MONO - MACH1HORIZON
TODO: add more input tests with less rounded inputs
TODO: add all other output modes
 */
		{
			"Encode",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode4Ch, true, 0.0, 0.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.25, 0.25, 0.25, 0.25 },
				},
			}
		},
		{
			"Encode Diverge",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode4Ch, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.5, 0.5, 0.0, 0.0 },
				},
			}
		},
		{
			"Encode Rotation 90",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode4Ch, true, 90.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.0, 0.5, 0.0, 0.5 },
				},
			}
		},
		{
			"Encode Rotation 180",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode4Ch, true, 180.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.0, 0.0, 0.5, 0.5 },
				},
			}
		},
		{
			"Encode Rotation 270",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode4Ch, true, 270.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.5, 0.0, 0.5, 0.0 },
				},
			}
		},
/*
MONO - MACH1SPATIAL
TODO: add more input tests with less rounded inputs
 */
		{
			"Encode",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode8Ch, true, 0.0, 0.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
				},
			}
		},
		{
			"Encode Diverge",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode8Ch, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.25, 0.25, 0.0, 0.0, 0.25, 0.25, 0.0, 0.0 },
				},
			}
		},
		{
			"Encode Rotation 90",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode8Ch, true, 90.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.0, 0.25, 0.0, 0.25, 0.0, 0.25, 0.0, 0.25 },
				},
			}
		},
		{
			"Encode Rotation 180",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode8Ch, true, 180.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.0, 0.0, 0.25, 0.25, 0.0, 0.0, 0.25, 0.25 },
				},
			}
		},
		{
			"Encode Rotation 270",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode8Ch, true, 270.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.25, 0.0, 0.25, 0.0, 0.25, 0.0, 0.25, 0.0 },
				},
			}
		},
		{
			"Encode Height",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode8Ch, true, 90.0, 1.0, 90.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.25, 0.25, 0.25, 0.25, 0.0, 0.0, 0.0, 0.0 },
				},
			}
		},
		{
			"Encode Periphonic Height",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode8Ch, false, 0.0, 1.0, 90.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
				},
			}
		},
		{
			"Encode Periphonic Height Corner",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode8Ch, false, 45.0, 1.0, 90.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
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
			{ Mach1EncodeInputModeStereo, Mach1EncodeOutputMode8Ch, true, 0.0, 0.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
				},
			}
		},
		{
			"Encode Diverge",
			{ Mach1EncodeInputModeStereo, Mach1EncodeOutputMode8Ch, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.25, 0.25, 0.0, 0.0, 0.25, 0.25, 0.0, 0.0 },
					{ 0.25, 0.25, 0.0, 0.0, 0.25, 0.25, 0.0, 0.0 },
				},
			}
		},
		{
			"Encode Height",
			{ Mach1EncodeInputModeStereo, Mach1EncodeOutputMode8Ch, false, 0.0, 1.0, 90.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
					{ 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
				},
			}
		},
		{
			"Encode Stereo Spread",
			{ Mach1EncodeInputModeStereo, Mach1EncodeOutputMode8Ch, true, 0.0, 1.0, 0.0, true, 1.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.5, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0 },
					{ 0.0, 0.5, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0 },
				},
			}
		},
		{
			"Encode Stereo Spread Height",
			{ Mach1EncodeInputModeStereo, Mach1EncodeOutputMode8Ch, true, 0.0, 1.0, 0.0, true, 1.0, 0.0, unsignedDegrees },
			{
				{
					{ 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
					{ 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
				},
			}
		},
		{
			"Encode Stereo Rotation",
			{ Mach1EncodeInputModeStereo, Mach1EncodeOutputMode8Ch, true, 0.0, 1.0, 0.0, false, 1.0, 90.0, unsignedDegrees },
			{
				{
					{ 0.25, 0.25, 0.0, 0.0, 0.25, 0.25, 0.0, 0.0 },
					{ 0.0, 0.0, 0.25, 0.25, 0.0, 0.0, 0.25, 0.25 },
				},
			}
		},
/*
LCR
 */
		{
			"Encode",
			{ Mach1EncodeInputModeLCR, Mach1EncodeOutputMode8Ch, true, 0.0, 0.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
				},
			}
		},
		{
			"Encode Diverge",
			{ Mach1EncodeInputModeLCR, Mach1EncodeOutputMode8Ch, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.5, 0.5, 0.0, 0.0, 0.5, 0.5, 0.0, 0.0 },
					{ 0.25, 0.25, 0.0, 0.0, 0.25, 0.25, 0.0, 0.0 },
					{ 0.5, 0.5, 0.0, 0.0, 0.5, 0.5, 0.0, 0.0 },
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
			{ Mach1EncodeInputModeQuad, Mach1EncodeOutputMode8Ch, true, 0.0, 0.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
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
			{ Mach1EncodeInputModeQuad, Mach1EncodeOutputMode8Ch, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
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
			{ Mach1EncodeInputModeQuad, Mach1EncodeOutputMode8Ch, true, 0.0, 1.0, 90.0, true, 0.0, 0.0, unsignedDegrees },
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
		{
			"Encode",
			{ Mach1EncodeInputModeLCRS, Mach1EncodeOutputMode8Ch, true, 0.0, 0.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
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
			{ Mach1EncodeInputModeLCRS, Mach1EncodeOutputMode8Ch, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.5, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0 },
					{ 0.25, 0.25, 0.0, 0.0, 0.25, 0.25, 0.0, 0.0 },
					{ 0.0, 0.5, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0 },
					{ 0.0, 0.0, 0.25, 0.25, 0.0, 0.0, 0.25, 0.25 },
				},
			}
		},
/*
AFormat
 */
		{
			"Encode",
			{ Mach1EncodeInputModeAFormat, Mach1EncodeOutputMode8Ch, true, 0.0, 0.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
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
			{ Mach1EncodeInputModeAFormat, Mach1EncodeOutputMode8Ch, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
					{ 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0 },
					{ 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 },
					{ 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0 },
				},
			}
		},
/*
1OA ACNSN3D
 */
		{
			"Encode",
			{ Mach1EncodeInputModeBFOAACN, Mach1EncodeOutputMode8Ch, true, 0.0, 0.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
				},
			}
		},
/*
1OA FuMa
 */
		{
			"Encode",
			{ Mach1EncodeInputModeBFOAFUMA, Mach1EncodeOutputMode8Ch, true, 0.0, 0.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
				},
			}
		},
/*
Input Data Checks
 */
		{
			"Azimuth: signed degrees",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode8Ch, true, -90.0, 1.0, 0.0, true, 0.0, 0.0, signedDegrees },
			{
				{
					{ 0.25, 0.0, 0.25, 0.0, 0.25, 0.0, 0.25, 0.0 },
				},
			}
		},
		{
			"Azimuth: unsigned degrees",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode8Ch, true, 270.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.25, 0.0, 0.25, 0.0, 0.25, 0.0, 0.25, 0.0 },
				},
			}
		},
		{
			"Azimuth: signed radians",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode8Ch, true, -1.5707963268, 1.0, 0.0, true, 0.0, 0.0, signedRadians },
			{
				{
					{ 0.25, 0.0, 0.25, 0.0, 0.25, 0.0, 0.25, 0.0 },
				},
			}
		},
		{
			"Azimuth: unsigned radians",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode8Ch, true, 4.7123889804, 1.0, 0.0, true, 0.0, 0.0, unsignedRadians },
			{
				{
					{ 0.25, 0.0, 0.25, 0.0, 0.25, 0.0, 0.25, 0.0 },
				},
			}
		},
		{
			"Azimuth: signed norm",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode8Ch, true, -0.5, 1.0, 0.0, true, 0.0, 0.0, signedNormalized },
			{
				{
					{ 0.25, 0.0, 0.25, 0.0, 0.25, 0.0, 0.25, 0.0 },
				},
			}
		},
		{
			"Azimuth: unsigned norm",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode8Ch, true, 0.75, 1.0, 0.0, true, 0.0, 0.0, unsignedNormalized },
			{
				{
					{ 0.25, 0.0, 0.25, 0.0, 0.25, 0.0, 0.25, 0.0 },
				},
			}
		},
		{
			"Elevation: signed degrees",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode8Ch, true, 0.0, 1.0, -90.0, true, 0.0, 0.0, signedDegrees },
			{
				{
					{ 0.0, 0.0, 0.0, 0.0, 0.25, 0.25, 0.25, 0.25 },
				},
			}
		},
		{
			"Elevation: unsigned degrees",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode8Ch, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.0, 0.0, 0.0, 0.0, 0.25, 0.25, 0.25, 0.25 }, //TODO: uh oh!?
				},
			}
		},
		{
			"Elevation: signed radians",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode8Ch, true, 0.0, 1.0, -1.5707963268, true, 0.0, 0.0, signedRadians },
			{
				{
					{ 0.0, 0.0, 0.0, 0.0, 0.25, 0.25, 0.25, 0.25 },
				},
			}
		},
		{
			"Elevation: unsigned radians",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode8Ch, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedRadians },
			{
				{
					{ 0.0, 0.0, 0.0, 0.0, 0.25, 0.25, 0.25, 0.25 }, //TODO: uh oh!?
				},
			}
		},
		{
			"Elevation: signed norm",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode8Ch, true, 0.0, 1.0, -1.0, true, 0.0, 0.0, signedNormalized },
			{
				{
					{ 0.0, 0.0, 0.0, 0.0, 0.25, 0.25, 0.25, 0.25 },
				},
			}
		},
		{
			"Elevation: unsigned norm",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode8Ch, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedNormalized },
			{
				{
					{ 0.0, 0.0, 0.0, 0.0, 0.25, 0.25, 0.25, 0.25 }, //TODO: uh oh!?
				},
			}
		},
		//TODO: write out the results for OrbitRotation input checks
		// {
		// 	"OrbitRotation: signed degrees",
		// 	{ Mach1EncodeInputModeMono, Mach1EncodeOutputMode8Ch, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, signedDegrees },
		// 	{
		// 		{
		// 			{ 0.0, 0.0, 0.0, 0.0, 0.25, 0.25, 0.25, 0.25 },
		// 		},
		// 	}
		// },
		// {
		// 	"OrbitRotation: unsigned degrees",
		// 	{ Mach1EncodeInputModeStereo, Mach1EncodeOutputMode8Ch, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
		// 	{
		// 		{
		// 			{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
		// 		},
		// 	}
		// },
		// {
		// 	"OrbitRotation: signed radians",
		// 	{ Mach1EncodeInputModeStereo, Mach1EncodeOutputMode8Ch, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, signedRadians },
		// 	{
		// 		{
		// 			{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
		// 		},
		// 	}
		// },
		// {
		// 	"OrbitRotation: unsigned radians",
		// 	{ Mach1EncodeInputModeStereo, Mach1EncodeOutputMode8Ch, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedRadians },
		// 	{
		// 		{
		// 			{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
		// 		},
		// 	}
		// },
		// {
		// 	"OrbitRotation: signed norm",
		// 	{ Mach1EncodeInputModeStereo, Mach1EncodeOutputMode8Ch, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, signedRadians },
		// 	{
		// 		{
		// 			{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
		// 		},
		// 	}
		// },
		// {
		// 	"OrbitRotation: unsigned norm",
		// 	{ Mach1EncodeInputModeStereo, Mach1EncodeOutputMode8Ch, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedRadians },
		// 	{
		// 		{
		// 			{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
		// 		},
		// 	}
		// },
	};

	std::cout << std::endl;

	for (auto test : cases) {
		Mach1Encode m1Encode;

		m1Encode.setInputMode(test.input.inputMode);
		m1Encode.setOutputMode(test.input.outputMode);

		m1Encode.setIsotropicEncode(test.input.isotropicEncode);

		if (test.input.inputType == signedDegrees || test.input.inputType == unsignedDegrees){
			m1Encode.setAzimuthDegrees(test.input.azimuth);
		} else if (test.input.inputType == signedRadians || test.input.inputType == unsignedRadians){
			m1Encode.setAzimuthRadians(test.input.azimuth);
		} else if (test.input.inputType == signedNormalized || test.input.inputType == unsignedNormalized){
			m1Encode.setAzimuth(test.input.azimuth);
		} else if (test.input.inputType == quaternion) {
			// case for quat
		} else {
			m1Encode.setRotation(test.input.azimuth);
		}

		m1Encode.setDiverge(test.input.diverge);

		if (test.input.inputType == signedDegrees || test.input.inputType == unsignedDegrees){
			m1Encode.setElevationDegrees(test.input.elevation);
		} else if (test.input.inputType == signedRadians || test.input.inputType == unsignedRadians){
			m1Encode.setElevationRadians(test.input.elevation);
		} else if (test.input.inputType == signedNormalized || test.input.inputType == unsignedNormalized){
			m1Encode.setElevation(test.input.elevation);
		} else if (test.input.inputType == quaternion) {
			// case for quat
		} else {
			m1Encode.setPitch(test.input.elevation);
		}

		m1Encode.setAutoOrbit(test.input.autoOrbit);
		m1Encode.setStereoSpread(test.input.stereoSpread);
		if (test.input.inputType == signedDegrees || test.input.inputType == unsignedDegrees){
			m1Encode.setOrbitRotationDegrees(test.input.stereoRotate);
		} else if (test.input.inputType == signedRadians || test.input.inputType == unsignedRadians){
			m1Encode.setOrbitRotationRadians(test.input.stereoRotate);
		} else if (test.input.inputType == signedNormalized || test.input.inputType == unsignedNormalized){
			m1Encode.setOrbitRotation(test.input.stereoRotate);
		} else if (test.input.inputType == quaternion) {
			// case for quat
		} else {
			m1Encode.setStereoRotate(test.input.stereoRotate);
		}

		m1Encode.generatePointResults();
		//auto points = m1Encode.getPoints();
		auto results = m1Encode.getGains();

		std::cout
			<< "testing " << test.name << ": "
			<< inputModeNames[test.input.inputMode] << " > " << outputModeNames[test.input.outputMode] << " with " << inputTypeNames[test.input.inputType];

		int counter = 0;

		for (size_t i = 0; i < results.size(); i++) {
			for (size_t j = 0; j < results[i].size(); j++) {
				bool check = fabs(test.output.results[i][j] - results[i][j]) < 0.0001;
				counter += check;
				if (check == false) {
					TEST_CHECK_(check, "%s | Error with index [%d][%d]", test.name.c_str(), i, j);
					std::cout << "index: [" << i << "][" << j << "]: " << results[i][j] << ", should be: " << test.output.results[i][j];
					std::cout << std::endl;
				}
			}
			if (counter == results[i].size()){
				std::cout << "... " << "\033[1;32mpassed\033[0m\n"; //TODO: make this also work in windows
			}
		}
	}
}

TEST_LIST = {
	{ "results", test_results },
	{ NULL, NULL }
};
