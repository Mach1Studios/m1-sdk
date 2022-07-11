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
		{ Mach1EncodeInputModeBFOAACN , "1OA-ACN" },
		{ Mach1EncodeInputModeBFOAFUMA , "1OA-FUMA" },
		{ Mach1EncodeInputModeB2OAACN , "2OA-ACN" },
		{ Mach1EncodeInputModeB2OAFUMA , "2OA-FUMA" },
		{ Mach1EncodeInputModeB3OAACN , "3OA-ACN" },
		{ Mach1EncodeInputModeB3OAFUMA , "3OA-FUMA" },
		{ Mach1EncodeInputModeLCR , "LCR" },
		{ Mach1EncodeInputMode5dot0 , "5.0" },
		{ Mach1EncodeInputMode5dot1Film , "5.1-FILM" },
		{ Mach1EncodeInputMode5dot1DTS , "5.1-DTS" },
		{ Mach1EncodeInputMode5dot1SMTPE , "5.1-SMPTE" }
	};

	std::map<Mach1EncodeOutputModeType, std::string> outputModeNames = {
		{ Mach1EncodeOutputModeM1Horizon_4 , "MACH1HORIZON-4" },
		{ Mach1EncodeOutputModeM1Spatial_8 , "MACH1SPATIAL-8" },
		{ Mach1EncodeOutputModeM1Spatial_12 , "MACH1SPATIAL-12" },
		{ Mach1EncodeOutputModeM1Spatial_14 , "MACH1SPATIAL-14" },
		{ Mach1EncodeOutputModeM1Spatial_16 , "MACH1SPATIAL-16" },
		{ Mach1EncodeOutputModeM1Spatial_18 , "MACH1SPATIAL-18" },
		{ Mach1EncodeOutputModeM1Spatial_20 , "MACH1SPATIAL-20" },
		{ Mach1EncodeOutputModeM1Spatial_32 , "MACH1SPATIAL-32" },
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
			"case: ENCODE | MONO->MACH1HORIZON",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputModeM1Horizon_4, true, 0.0, 0.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.25, 0.25, 0.25, 0.25 },
				},
			}
		},
		{
			"case: ENCODE | MONO->MACH1HORIZON | Diverge",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputModeM1Horizon_4, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.5, 0.5, 0.0, 0.0 },
				},
			}
		},
		{
			"case: ENCODE | MONO->MACH1HORIZON | Azimuth 90",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputModeM1Horizon_4, true, 90.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.0, 0.5, 0.0, 0.5 },
				},
			}
		},
		{
			"case: ENCODE | MONO->MACH1HORIZON | Azimuth 180",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputModeM1Horizon_4, true, 180.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.0, 0.0, 0.5, 0.5 },
				},
			}
		},
		{
			"case: ENCODE | MONO->MACH1HORIZON | Azimuth 270",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputModeM1Horizon_4, true, 270.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
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
			"case: ENCODE | MONO->MACH1SPATIAL",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputModeM1Spatial_8, true, 0.0, 0.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
				},
			}
		},
		{
			"case: ENCODE | MONO->MACH1SPATIAL | Diverge",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputModeM1Spatial_8, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.25, 0.25, 0.0, 0.0, 0.25, 0.25, 0.0, 0.0 },
				},
			}
		},
		{
			"case: ENCODE | MONO->MACH1SPATIAL | Azimuth 90",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputModeM1Spatial_8, true, 90.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.0, 0.25, 0.0, 0.25, 0.0, 0.25, 0.0, 0.25 },
				},
			}
		},
		{
			"case: ENCODE | MONO->MACH1SPATIAL | Azimuth 180",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputModeM1Spatial_8, true, 180.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.0, 0.0, 0.25, 0.25, 0.0, 0.0, 0.25, 0.25 },
				},
			}
		},
		{
			"case: ENCODE | MONO->MACH1SPATIAL | Azimuth 270",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputModeM1Spatial_8, true, 270.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.25, 0.0, 0.25, 0.0, 0.25, 0.0, 0.25, 0.0 },
				},
			}
		},
		{
			"case: ENCODE | MONO->MACH1SPATIAL | Height",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputModeM1Spatial_8, true, 90.0, 1.0, 90.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.25, 0.25, 0.25, 0.25, 0.0, 0.0, 0.0, 0.0 },
				},
			}
		},
		{
			"case: ENCODE | MONO->MACH1SPATIAL | Periphonic Height",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputModeM1Spatial_8, false, 0.0, 1.0, 90.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
				},
			}
		},
		{
			"case: ENCODE | MONO->MACH1SPATIAL | Periphonic Height Corner",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputModeM1Spatial_8, false, 45.0, 1.0, 90.0, true, 0.0, 0.0, unsignedDegrees },
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
			"case: ENCODE | STEREO->MACH1SPATIAL",
			{ Mach1EncodeInputModeStereo, Mach1EncodeOutputModeM1Spatial_8, true, 0.0, 0.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
				},
			}
		},
		{
			"case: ENCODE | STEREO->MACH1SPATIAL | Diverge",
			{ Mach1EncodeInputModeStereo, Mach1EncodeOutputModeM1Spatial_8, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.25, 0.25, 0.0, 0.0, 0.25, 0.25, 0.0, 0.0 },
					{ 0.25, 0.25, 0.0, 0.0, 0.25, 0.25, 0.0, 0.0 },
				},
			}
		},
		{
			"case: ENCODE | STEREO->MACH1SPATIAL | Height",
			{ Mach1EncodeInputModeStereo, Mach1EncodeOutputModeM1Spatial_8, true, 0.0, 1.0, 90.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.25, 0.25, 0.25, 0.25, 0.0, 0.0, 0.0, 0.0 },
					{ 0.25, 0.25, 0.25, 0.25, 0.0, 0.0, 0.0, 0.0 },
				},
			}
		},
		{
			"case: ENCODE | STEREO->MACH1SPATIAL | Stereo Spread",
			{ Mach1EncodeInputModeStereo, Mach1EncodeOutputModeM1Spatial_8, true, 0.0, 1.0, 0.0, true, 1.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.5, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0 },
					{ 0.0, 0.5, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0 },
				},
			}
		},
		{
			"case: ENCODE | STEREO->MACH1SPATIAL | Stereo Spread Height",
			{ Mach1EncodeInputModeStereo, Mach1EncodeOutputModeM1Spatial_8, true, 0.0, 1.0, 90.0, true, 1.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.5, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0 },
					{ 0.0, 0.5, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0 },
				},
			}
		},
		{
			"case: ENCODE | STEREO->MACH1SPATIAL | Stereo Spread Height - Periphonic",
			{ Mach1EncodeInputModeStereo, Mach1EncodeOutputModeM1Spatial_8, false, 0.0, 1.0, 90.0, true, 1.0, 0.0, unsignedDegrees },
			{
				{
					{ 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
					{ 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
				},
			}
		},
		{
			"case: ENCODE | STEREO->MACH1SPATIAL | Stereo Rotation",
			{ Mach1EncodeInputModeStereo, Mach1EncodeOutputModeM1Spatial_8, true, 0.0, 0.0, 0.0, false, 1.0, 90.0, unsignedDegrees },
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
			"case: ENCODE | LCR->MACH1SPATIAL",
			{ Mach1EncodeInputModeLCR, Mach1EncodeOutputModeM1Spatial_8, true, 0.0, 0.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
				},
			}
		},
		{
			"case: ENCODE | LCR->MACH1SPATIAL | Diverge",
			{ Mach1EncodeInputModeLCR, Mach1EncodeOutputModeM1Spatial_8, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.5, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0 },
					{ 0.25, 0.25, 0.0, 0.0, 0.25, 0.25, 0.0, 0.0 },
					{ 0.0, 0.5, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0 },
				},
			}
		},
/*
QUAD - MACH1SPATIAL
TODO: add more input tests with less rounded inputs
TODO: add all other output modes
 */
		{
			"case: ENCODE | QUAD->MACH1SPATIAL",
			{ Mach1EncodeInputModeQuad, Mach1EncodeOutputModeM1Spatial_8, true, 0.0, 0.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
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
			"case: ENCODE | QUAD->MACH1SPATIAL | Diverge",
			{ Mach1EncodeInputModeQuad, Mach1EncodeOutputModeM1Spatial_8, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
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
			"case: ENCODE | QUAD->MACH1SPATIAL | Height",
			{ Mach1EncodeInputModeQuad, Mach1EncodeOutputModeM1Spatial_8, true, 0.0, 1.0, 90.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
					{ 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
					{ 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
					{ 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0 },
				},
			}
		},
/*
LCRS
 */
		{
			"case: ENCODE | LCRS->MACH1SPATIAL",
			{ Mach1EncodeInputModeLCRS, Mach1EncodeOutputModeM1Spatial_8, true, 0.0, 0.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
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
			"case: ENCODE | LCRS->MACH1SPATIAL | Diverge",
			{ Mach1EncodeInputModeLCRS, Mach1EncodeOutputModeM1Spatial_8, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
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
			"case: ENCODE | AFORMAT->MACH1SPATIAL",
			{ Mach1EncodeInputModeAFormat, Mach1EncodeOutputModeM1Spatial_8, true, 0.0, 0.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
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
			"case: ENCODE | AFORMAT->MACH1SPATIAL | Diverge",
			{ Mach1EncodeInputModeAFormat, Mach1EncodeOutputModeM1Spatial_8, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.728553, 0.125, 0.125, 0.021447, 0.0, 0.0, 0.0, 0.0 },
					{ 0.0, 0.0, 0.0, 0.0, 0.125, 0.728553, 0.021447, 0.125 },
					{ 0.0, 0.0, 0.0, 0.0, 0.125, 0.021447, 0.728553, 0.125 },
					{ 0.021447, 0.125, 0.125, 0.728553, 0.0, 0.0, 0.0, 0.0 },
				},
			}
		},
/*
1OA ACNSN3D
 */
		{
			"case: ENCODE | 1OA-ACN->MACH1SPATIAL",
			{ Mach1EncodeInputModeBFOAACN, Mach1EncodeOutputModeM1Spatial_8, true, 0.0, 0.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
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
			"case: ENCODE | 1OA-FUMA->MACH1SPATIAL",
			{ Mach1EncodeInputModeBFOAFUMA, Mach1EncodeOutputModeM1Spatial_8, true, 0.0, 0.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
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
SURROUND 5.0, 5.1
*/
		{
			"case: ENCODE | 5.0->MACH1SPATIAL | Diverge", // "L","C","R","Ls","Rs"
			{ Mach1EncodeInputMode5dot0, Mach1EncodeOutputModeM1Spatial_8, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.5, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0 },
					{ 0.25, 0.25, 0.0, 0.0, 0.25, 0.25, 0.0, 0.0 },
					{ 0.0, 0.5, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0 },
					{ 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.5, 0.0 },
					{ 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.5 },
				},
			}
		},
		{
			"case: ENCODE | 5.1->MACH1SPATIAL | Diverge", // "L","C","R","Ls","Rs","LFE"
			{ Mach1EncodeInputMode5dot1Film, Mach1EncodeOutputModeM1Spatial_8, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.5, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0 },
					{ 0.25, 0.25, 0.0, 0.0, 0.25, 0.25, 0.0, 0.0 },
					{ 0.0, 0.5, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0 },
					{ 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.5, 0.0 },
					{ 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.5 },
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
				},
			}
		},
/*
Input Data Checks
 */
		{
			"case: ENCODE | Azimuth: signed degrees",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputModeM1Spatial_8, true, -90.0, 1.0, 0.0, true, 0.0, 0.0, signedDegrees },
			{
				{
					{ 0.25, 0.0, 0.25, 0.0, 0.25, 0.0, 0.25, 0.0 },
				},
			}
		},
		{
			"case: ENCODE | Azimuth: unsigned degrees",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputModeM1Spatial_8, true, 270.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
			{
				{
					{ 0.25, 0.0, 0.25, 0.0, 0.25, 0.0, 0.25, 0.0 },
				},
			}
		},
		{
			"case: ENCODE | Azimuth: signed radians",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputModeM1Spatial_8, true, -1.5707963268, 1.0, 0.0, true, 0.0, 0.0, signedRadians },
			{
				{
					{ 0.25, 0.0, 0.25, 0.0, 0.25, 0.0, 0.25, 0.0 },
				},
			}
		},
		{
			"case: ENCODE | Azimuth: unsigned radians",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputModeM1Spatial_8, true, 4.7123889804, 1.0, 0.0, true, 0.0, 0.0, unsignedRadians },
			{
				{
					{ 0.25, 0.0, 0.25, 0.0, 0.25, 0.0, 0.25, 0.0 },
				},
			}
		},
		{
			"case: ENCODE | Azimuth: signed norm",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputModeM1Spatial_8, true, -0.25, 1.0, 0.0, true, 0.0, 0.0, signedNormalized },
			{
				{
					{ 0.25, 0.0, 0.25, 0.0, 0.25, 0.0, 0.25, 0.0 },
				},
			}
		},
		{
			"case: ENCODE | Azimuth: unsigned norm",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputModeM1Spatial_8, true, 0.75, 1.0, 0.0, true, 0.0, 0.0, unsignedNormalized },
			{
				{
					{ 0.25, 0.0, 0.25, 0.0, 0.25, 0.0, 0.25, 0.0 },
				},
			}
		},
		{
			"case: ENCODE | Elevation: signed degrees",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputModeM1Spatial_8, true, 0.0, 1.0, -90.0, true, 0.0, 0.0, signedDegrees },
			{
				{
					{ 0.0, 0.0, 0.0, 0.0, 0.25, 0.25, 0.25, 0.25 },
				},
			}
		},
		{
			"case: ENCODE | Elevation: signed radians",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputModeM1Spatial_8, true, 0.0, 1.0, -1.5707963268, true, 0.0, 0.0, signedRadians },
			{
				{
					{ 0.0, 0.0, 0.0, 0.0, 0.25, 0.25, 0.25, 0.25 },
				},
			}
		},
		{
			"case: ENCODE | Elevation: signed norm",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputModeM1Spatial_8, true, 0.0, 1.0, -1.0, true, 0.0, 0.0, signedNormalized },
			{
				{
					{ 0.0, 0.0, 0.0, 0.0, 0.25, 0.25, 0.25, 0.25 },
				},
			}
		},
		{
			"case: ENCODE | Elevation: unsigned norm",
			{ Mach1EncodeInputModeMono, Mach1EncodeOutputModeM1Spatial_8, true, 0.0, 1.0, -1.0, true, 0.0, 0.0, unsignedNormalized },
			{
				{
					{ 0.0, 0.0, 0.0, 0.0, 0.25, 0.25, 0.25, 0.25 }, //TODO: uh oh!?
				},
			}
		},
		//TODO: write out the results for OrbitRotation input checks
		// {
		// 	"OrbitRotation: signed degrees",
		// 	{ Mach1EncodeInputModeMono, Mach1EncodeOutputModeM1Spatial_8, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, signedDegrees },
		// 	{
		// 		{
		// 			{ 0.0, 0.0, 0.0, 0.0, 0.25, 0.25, 0.25, 0.25 },
		// 		},
		// 	}
		// },
		// {
		// 	"OrbitRotation: unsigned degrees",
		// 	{ Mach1EncodeInputModeStereo, Mach1EncodeOutputModeM1Spatial_8, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedDegrees },
		// 	{
		// 		{
		// 			{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
		// 		},
		// 	}
		// },
		// {
		// 	"OrbitRotation: signed radians",
		// 	{ Mach1EncodeInputModeStereo, Mach1EncodeOutputModeM1Spatial_8, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, signedRadians },
		// 	{
		// 		{
		// 			{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
		// 		},
		// 	}
		// },
		// {
		// 	"OrbitRotation: unsigned radians",
		// 	{ Mach1EncodeInputModeStereo, Mach1EncodeOutputModeM1Spatial_8, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedRadians },
		// 	{
		// 		{
		// 			{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
		// 		},
		// 	}
		// },
		// {
		// 	"OrbitRotation: signed norm",
		// 	{ Mach1EncodeInputModeStereo, Mach1EncodeOutputModeM1Spatial_8, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, signedRadians },
		// 	{
		// 		{
		// 			{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
		// 		},
		// 	}
		// },
		// {
		// 	"OrbitRotation: unsigned norm",
		// 	{ Mach1EncodeInputModeStereo, Mach1EncodeOutputModeM1Spatial_8, true, 0.0, 1.0, 0.0, true, 0.0, 0.0, unsignedRadians },
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

		if (test.input.inputType == signedDegrees || test.input.inputType == unsignedDegrees){
			m1Encode.setAzimuthDegrees(test.input.azimuth);
		} else if (test.input.inputType == signedRadians || test.input.inputType == unsignedRadians){
			m1Encode.setAzimuthRadians(test.input.azimuth);
		} else if (test.input.inputType == signedNormalized || test.input.inputType == unsignedNormalized){
			m1Encode.setAzimuth(test.input.azimuth);
		} else if (test.input.inputType == quaternion) {
			// case for quat
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
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	getchar();
#endif
}

TEST_LIST = {
	{ "results", test_results },
	{ NULL, NULL }
};
