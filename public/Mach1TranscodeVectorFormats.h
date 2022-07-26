//  Mach1 Spatial SDK
//  Copyright © 2017-2022 Mach1. All rights reserved.
//
// References: 
//

#pragma once

#include "Mach1TranscodeConstants.h"

/*
 * Description of matrices
 * - Inputs are rows
 * - Outputs are columns
 *
 *  I1 -> | I1 | I2 | I3 | I4 |
 *  I2 -> | I1 | I2 | I3 | I4 |
 *  I3 -> | I1 | I2 | I3 | I4 |
 *			↓    ↓    ↓    ↓ 
 *			O1   O2   O3   O4
 */

namespace Mach1TranscodeConstants {
	class MatricesVector {
	public:
		static std::vector<Mach1TranscodeMatrix> getData() {
			return {
				{
					"M1Horizon", "M1Horizon",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1 }),
					}
				},
				{
					"M1Horizon", "M1Horizon",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1 }),
					}
				},
				{
					"M1Horizon", "M1HorizonPairs",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0, 0, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 1, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 1, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1, 1, 0, 0, 0 }),
					}
				},
				{
					"M1Horizon", "M1Spatial",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0, 0, 0.707106f }),
					}
				},
				{
					"M1Horizon", "M1SpatialPairs",
					{
						Mach1TranscodeChannel::Coeffs({ 0.5f, 0, 0, 0.5f, 0, 0, 0, 0, 0.5f, 0, 0, 0.5f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.5f, 0, 0, 0, 0, 0.5f, 0, 0, 0.5f, 0, 0, 0, 0, 0.5f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.5f, 0, 0, 0.5f, 0, 0, 0, 0, 0.5f, 0, 0, 0.5f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0.5f, 0, 0, 0.5f, 0, 0, 0, 0, 0.5f, 0, 0, 0.5f }),
					}
				},
				{
					"M1HorizonS", "M1Horizon",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.25f, 0.25f, 0.25f, 0.25f }),
						Mach1TranscodeChannel::Coeffs({ 0.25f, 0.25f, 0.25f, 0.25f }),
					}
				},
				{
					"M1HorizonS", "M1HorizonPairs",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0, 0, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 1, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 1, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1, 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 1, 0, 1, 0, 1, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 1, 0, 1, 0, 1 }),
					}
				},
				{
					"M1HorizonS", "M1Spatial",
					{
						Mach1TranscodeChannel::Coeffs({ 0.5f, 0, 0, 0, 0.5f, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.5f, 0, 0, 0, 0.5f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.5f, 0, 0, 0, 0.5f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.5f, 0, 0, 0, 0.5f }),
						Mach1TranscodeChannel::Coeffs({ 0.125f, 0.125f, 0.125f, 0.125f, 0.125f, 0.125f, 0.125f, 0.125f }),
						Mach1TranscodeChannel::Coeffs({ 0.125f, 0.125f, 0.125f, 0.125f, 0.125f, 0.125f, 0.125f, 0.125f }),
					}
				},
				{
					"M1HorizonS", "M1SpatialPairs",
					{
						Mach1TranscodeChannel::Coeffs({ 0.5f, 0, 0, 0.5f, 0, 0, 0, 0, 0.5f, 0, 0, 0.5f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.5f, 0, 0, 0, 0, 0.5f, 0, 0, 0.5f, 0, 0, 0, 0, 0.5f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.5f, 0, 0, 0.5f, 0, 0, 0, 0, 0.5f, 0, 0, 0.5f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0.5f, 0, 0, 0.5f, 0, 0, 0, 0, 0.5f, 0, 0, 0.5f }),
						Mach1TranscodeChannel::Coeffs({ 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1 }),
					}
				},
				{
					"M1Spatial", "M1Spatial",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 1 }),
					}
				},
				{
					"M1Spatial", "M1Horizon",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f }),
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f }),
					}
				},
				{
					"M1Spatial", "M1HorizonPairs",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0, 0, 0, 0.707106f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0.707106f, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.707106f, 0.707106f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0.707106f, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0, 0, 0, 0.707106f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0.707106f, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.707106f, 0.707106f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0.707106f, 0, 0, 0 }),
					}
				},
				{
					"M1Spatial", "M1SpatialFaces",
					{
						Mach1TranscodeChannel::Coeffs({ 0.471404f, 0.471404f, 0, 0, 0.471404f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.471404f, 0, 0, 0.471404f, 0.471404f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.471404f, 0.471404f, 0, 0.471404f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.471404f, 0.471404f, 0.471404f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.471404f, 0.471404f, 0, 0, 0, 0.471404f }),
						Mach1TranscodeChannel::Coeffs({ 0.471404f, 0, 0, 0.471404f, 0, 0.471404f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.471404f, 0.471404f, 0, 0, 0.471404f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.471404f, 0.471404f, 0, 0.471404f }),
					}
				},
				{
					"M1Spatial", "M1SpatialPairs",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 }),
					}
				},
				{
					"M1SpatialS", "M1Horizon",
					{
						Mach1TranscodeChannel::Coeffs({ 0.5f, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.5f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.5f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.5f }),
						Mach1TranscodeChannel::Coeffs({ 0.5f, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.5f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.5f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.5f }),
						Mach1TranscodeChannel::Coeffs({ 0.25f, 0.25f, 0.25f, 0.25f }),
						Mach1TranscodeChannel::Coeffs({ 0.25f, 0.25f, 0.25f, 0.25f }),
					}
				},
				{
					"M1SpatialS", "M1HorizonPairs",
					{
						Mach1TranscodeChannel::Coeffs({ 0.5f, 0, 0, 0, 0, 0, 0, 0.5f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.5f, 0.5f, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.5f, 0.5f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.5f, 0.5f, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.5f, 0, 0, 0, 0, 0, 0, 0.5f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.5f, 0.5f, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.5f, 0.5f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.5f, 0.5f, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 1, 0, 1, 0, 1, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 1, 0, 1, 0, 1 }),
					}
				},
				{
					"M1SpatialS", "M1Spatial",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ 0.125f, 0.125f, 0.125f, 0.125f, 0.125f, 0.125f, 0.125f, 0.125f }),
						Mach1TranscodeChannel::Coeffs({ 0.125f, 0.125f, 0.125f, 0.125f, 0.125f, 0.125f, 0.125f, 0.125f }),
					}
				},
				{
					"M1SpatialS", "M1SpatialPairs",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1 }),
					}
				},
				{
					"M1SpatialFaces", "M1Spatial",
					{
						Mach1TranscodeChannel::Coeffs({ 0.353553f, 0.353553f, 0, 0, 0.353553f, 0.353553f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.353553f, 0, 0.353553f, 0, 0.353553f, 0, 0.353553f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.353553f, 0.353553f, 0, 0, 0.353553f, 0.353553f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.353553f, 0, 0.353553f, 0, 0.353553f, 0, 0.353553f }),
						Mach1TranscodeChannel::Coeffs({ 0.353553f, 0.353553f, 0.353553f, 0.353553f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0.353553f, 0.353553f, 0.353553f, 0.353553f }),
					}
				},			
			};
		}
	};
};
