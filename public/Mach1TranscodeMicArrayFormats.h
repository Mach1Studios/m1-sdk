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
class MatricesMicArray {
public:
	static std::vector<Mach1TranscodeMatrix> getData() {
		const float r2o2 = (float)sqrt(2.0) / 2.0f;
		const float r2o4 = (float)sqrt(2.0) / 4.0f;
		const float r2o8 = (float)sqrt(2.0) / 8.0f;

		return {
				{
					"1.0", "M1Spatial",
					{
                        Mach1TranscodeChannel::Panner(0, 0, 1, {-6, true}),
					}
				},
				{
					"2.0", "M1Spatial",
					{
                        Mach1TranscodeChannel::Panner(-45, 0, 1),
						Mach1TranscodeChannel::Coeffs({ 0, 0.5f, 0, 0, 0, 0.5f, 0, 0 })
					}
				},
				{
					"Ambeo", "M1Spatial",
					{
						Mach1TranscodeChannel::Coeffs({ 1.70710599f, -0.001728f, 0.000685f, -0.31546f, 0.005525f, -0.317307f, -0.311014f, -0.037532f }),
						Mach1TranscodeChannel::Coeffs({ -0.31979799f, 0.024362f, -0.083181f, -0.31374699f, -0.004715f, 1.77277803f, -0.32243899f, -0.001391f }),
						Mach1TranscodeChannel::Coeffs({ -0.17735f, -0.32358f, -0.19385999f, -0.18116499f, -0.219145f, -0.18212f, 1.787938f, -0.223065f }),
						Mach1TranscodeChannel::Coeffs({ -0.32679999f, -0.002112f, 0.003135f, 1.71765006f, -0.03372f, -0.32499999f, -0.31977001f, 0.0062f }),
					}
				},
				{
					"SPS-200", "M1Spatial",
					{
						Mach1TranscodeChannel::Coeffs({ 1.70000899f, 0.000097f, -0.005204f, -0.36458799f, 0.022792f, -0.360383f, -0.36231801f, -0.037654f }),
						Mach1TranscodeChannel::Coeffs({ -0.36043099f, 0.022792f, -0.037529f, -0.36238801f, 0.000104f, 1.70005906f, -0.36461499f, -0.005212f }),
						Mach1TranscodeChannel::Coeffs({ -0.199072f, -0.33234701f, -0.218069f, -0.19542401f, -0.24213f, -0.199532f, 1.70079505f, -0.23848f }),
						Mach1TranscodeChannel::Coeffs({ -0.198948f, -0.238601f, -0.243719f, 1.70087302f, -0.33212301f, -0.196025f, -0.198909f, -0.21687099f }),
					}
				},
				{
					"TetraMic", "M1Spatial",
					{
						Mach1TranscodeChannel::Coeffs({ 2.36442399f, -0.000757f, -0.005645f, -0.52798003f, 0.033468f, -0.51843101f, -0.52326399f, -0.087658f }),
						Mach1TranscodeChannel::Coeffs({ -0.52781701f, 0.031688f, -0.087271f, -0.51984698f, -0.005087f, 2.36920309f, -0.52968502f, -0.002606f }),
						Mach1TranscodeChannel::Coeffs({ -0.27798f, -0.53587401f, -0.308456f, -0.27288699f, -0.339082f, -0.28054601f, 2.35262704f, -0.33451f }),
						Mach1TranscodeChannel::Coeffs({ -0.706792f, -0.008215f, -0.000956f, 2.53077793f, 0.246269f, -0.69186598f, -0.68480098f, 0.035837f }),
					}
				},
				{
					"NT-SF1", "M1Spatial",
					{
						Mach1TranscodeChannel::Coeffs({ 1.61973f, -0.000089f, -0.003503f, -0.32746801f, 0.022924f, -0.32106999f, -0.32451501f, -0.03195f }),
						Mach1TranscodeChannel::Coeffs({ -0.32611799f, 0.021812f, -0.033303f, -0.321188f, -0.003148f, 1.62159598f, -0.32668999f, -0.001212f }),
						Mach1TranscodeChannel::Coeffs({ -0.178378f, -0.29723999f, -0.19407f, -0.174988f, -0.217002f, -0.178848f, 1.62242603f, -0.213596f }),
						Mach1TranscodeChannel::Coeffs({ -0.178344f, -0.21367501f, -0.21852399f, 1.62255704f, -0.296904f, -0.175521f, -0.178372f, -0.193075f }),
					}
				},
			};
		}
	};
};
