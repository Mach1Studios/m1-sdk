//  Mach1 Spatial SDK
//  Copyright © 2017-2022 Mach1. All rights reserved.
//
// References: 
//

#pragma once

#include "Mach1TranscodeConstants.h"

/*
 * Description of matrices
 * - Inputs are columns
 * - Outputs are rows
 *
 *    I1   I2   I3   I4
 *    ↓    ↓    ↓    ↓
 *  | I1 | I2 | I3 | I4 | -> O1
 *  | I1 | I2 | I3 | I4 | -> O2
 *  | I1 | I2 | I3 | I4 | -> O3
 *
 */

namespace Mach1TranscodeConstants {

class MatricesMicArray
{
public:
	static std::vector<Mach1TranscodeMatrix> getData() {
		const float r2o2 = (float)sqrt(2.0) / 2.0f;
		const float r2o4 = (float)sqrt(2.0) / 4.0f;
		const float r2o8 = (float)sqrt(2.0) / 8.0f;

		return
		  {

		{
			"Ambeo", "M1Spatial", // HARPEX AFORMAT AMBEO
			{
				Mach1TranscodeChannel::Coeffs({  1.707106f, -0.319798f, -0.177350f, -0.326800f }),  // Top Front Left
				Mach1TranscodeChannel::Coeffs({ -0.001728f,  0.024362f, -0.323580f, -0.002112f }),  // Top Front Right
				Mach1TranscodeChannel::Coeffs({  0.000685f, -0.083181f, -0.193860f,  0.003135f }),  // Top Back Left
				Mach1TranscodeChannel::Coeffs({ -0.315460f, -0.313747f, -0.181165f,  1.717650f }),  // Top Back Right
				Mach1TranscodeChannel::Coeffs({  0.005525f, -0.004715f, -0.219145f, -0.033720f }),  // Bottom Front Left
				Mach1TranscodeChannel::Coeffs({ -0.317307f,  1.772778f, -0.182120f, -0.325000f }),  // Bottom Front Right
				Mach1TranscodeChannel::Coeffs({ -0.311014f, -0.322439f,  1.787938f, -0.319770f }),  // Bottom Back Left
				Mach1TranscodeChannel::Coeffs({ -0.037532f, -0.001391f, -0.223065f,  0.006200f }),	// Bottom Back Right
			}
		},
		{
			"SPS-200", "M1Spatial", // HARPEX AFORMAT AMBEO
			{
				Mach1TranscodeChannel::Coeffs({  1.700009f, -0.360431f, -0.199072f, -0.198948f }),  // Top Front Left
				Mach1TranscodeChannel::Coeffs({  0.000097f,  0.022792f, -0.332347f, -0.238601f }),  // Top Front Right
				Mach1TranscodeChannel::Coeffs({ -0.005204f, -0.037529f, -0.218069f, -0.243719f }),  // Top Back Left
				Mach1TranscodeChannel::Coeffs({ -0.364588f, -0.362388f, -0.195424f,  1.700873f }),  // Top Back Right
				Mach1TranscodeChannel::Coeffs({  0.022792f,  0.000104f, -0.242130f, -0.332123f }),  // Bottom Front Left
				Mach1TranscodeChannel::Coeffs({ -0.360383f,  1.700059f, -0.199532f, -0.196025f }),  // Bottom Front Right
				Mach1TranscodeChannel::Coeffs({ -0.362318f, -0.364615f,  1.700795f, -0.198909f }),  // Bottom Back Left
				Mach1TranscodeChannel::Coeffs({ -0.037654f, -0.005212f, -0.238480f, -0.216871f }),	// Bottom Back Right
			}
		},
		{
			"TetraMic", "M1Spatial", // HARPEX AFORMAT AMBEO
			// TetraMic: Left Front Up, Right Front Down, Left Back Down and Right Back Up
			{
				Mach1TranscodeChannel::Coeffs({  2.364424f, -0.527817f, -0.277980f, -0.706792f }),  // Top Front Left
				Mach1TranscodeChannel::Coeffs({ -0.000757f,  0.031688f, -0.535874f, -0.008215f }),  // Top Front Right
				Mach1TranscodeChannel::Coeffs({ -0.005645f, -0.087271f, -0.308456f, -0.000956f }),  // Top Back Left
				Mach1TranscodeChannel::Coeffs({ -0.527980f, -0.519847f, -0.272887f,  2.530778f }),  // Top Back Right
				Mach1TranscodeChannel::Coeffs({  0.033468f, -0.005087f, -0.339082f,  0.246269f }),  // Bottom Front Left
				Mach1TranscodeChannel::Coeffs({ -0.518431f,  2.369203f, -0.280546f, -0.691866f }),  // Bottom Front Right
				Mach1TranscodeChannel::Coeffs({ -0.523264f, -0.529685f,  2.352627f, -0.684801f }),  // Bottom Back Left
				Mach1TranscodeChannel::Coeffs({ -0.087658f, -0.002606f, -0.334510f,  0.035837f }),	// Bottom Back Right
			}
		},
		{
			"NT-SF1", "M1Spatial", // HARPEX AFORMAT AMBEO
			{
				Mach1TranscodeChannel::Coeffs({  1.619730f, -0.326118f, -0.178378f, -0.178344f }),  // Top Front Left
				Mach1TranscodeChannel::Coeffs({ -0.000089f,  0.021812f, -0.297240f, -0.213675f }),  // Top Front Right
				Mach1TranscodeChannel::Coeffs({ -0.003503f, -0.033303f, -0.194070f, -0.218524f }),  // Top Back Left
				Mach1TranscodeChannel::Coeffs({ -0.327468f, -0.321188f, -0.174988f,  1.622557f }),  // Top Back Right
				Mach1TranscodeChannel::Coeffs({  0.022924f, -0.003148f, -0.217002f, -0.296904f }),  // Bottom Front Left
				Mach1TranscodeChannel::Coeffs({ -0.321070f,  1.621596f, -0.178848f, -0.175521f }),  // Bottom Front Right
				Mach1TranscodeChannel::Coeffs({ -0.324515f, -0.326690f,  1.622426f, -0.178372f }),  // Bottom Back Left
				Mach1TranscodeChannel::Coeffs({ -0.031950f, -0.001212f, -0.213596f, -0.193075f }),	// Bottom Back Right
			}
		},
	};
			
		}
	};
};
