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
		const float r2o12 = (float)sqrt(2.0) / 12.0f;
		const float r2o20 = (float)sqrt(2.0) / 20.0f;

		return
		  {

		{
			"M1Horizon", "AFormat",
			{   
				Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1 }),
			}
		},
		{
			"AFormat", "M1Horizon",
			{   
				Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1 }),
			}
		},
	};
			
		}
	};
};
