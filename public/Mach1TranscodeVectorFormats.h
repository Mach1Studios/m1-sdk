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

	class MatricesVector
	{
	public:
		static std::vector<Mach1TranscodeMatrix> getData() {
			const float oor2 = (float)(1.0 / sqrt(2.0));
			const float r2o3 = (float)sqrt(2.0) / 3.0f;
			const float r2o4 = (float)sqrt(2.0) / 4.0f;
			return

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
				Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0 }),
				Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0 }),
			}
		},
		{
			"M1Horizon", "M1Spatial",
			{   
				Mach1TranscodeChannel::Coeffs({ oor2, 0, 0, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, oor2, 0, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, oor2, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, oor2 }),
				Mach1TranscodeChannel::Coeffs({ oor2, 0, 0, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, oor2, 0, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, oor2, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, oor2 }),
			}
		},
		{
			"M1Horizon", "M1SpatialPairs",
			{   
				Mach1TranscodeChannel::Coeffs({ 0.5f, 0, 0, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0.5f, 0, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0.5f, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0.5f, 0, 0, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.5f }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0.5f, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0.5f, 0, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.5f }),
				Mach1TranscodeChannel::Coeffs({ 0.5f, 0, 0, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0.5f, 0, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0.5f, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0.5f, 0, 0, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.5f }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0.5f, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0.5f, 0, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.5f }),
			}
		},
		// M1Horizon + Stereo
		{
			"M1HorizonS", "M1Horizon",
			{   
				Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0, 0.25f, 0.25f }),
				Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0.25f, 0.25f }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0, 0.25f, 0.25f }),
				Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0, 0.25f, 0.25f }),
			}
		},
		{
			"M1HorizonS", "M1HorizonPairs",
			{   
				Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0, 1, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 1 }),
				Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 1, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1, 0, 1 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1, 1, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0, 0, 1 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0, 1, 0 }),
				Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0, 0, 1 }),
			}
		},
		{
			"M1HorizonS", "M1Spatial",
			{   
				Mach1TranscodeChannel::Coeffs({ 0.5f, 0, 0, 0, 0.125f, 0.125f }),
				Mach1TranscodeChannel::Coeffs({ 0, 0.5f, 0, 0, 0.125f, 0.125f }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0.5f, 0, 0.125f, 0.125f }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.5f, 0.125f, 0.125f }),
				Mach1TranscodeChannel::Coeffs({ 0.5f, 0, 0, 0, 0.125f, 0.125f }),
				Mach1TranscodeChannel::Coeffs({ 0, 0.5f, 0, 0, 0.125f, 0.125f }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0.5f, 0, 0.125f, 0.125f }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.5f, 0.125f, 0.125f }),
			}
		},
		{
			"M1HorizonS", "M1SpatialPairs",
			{   
				Mach1TranscodeChannel::Coeffs({ 0.5f, 0, 0, 0, 1, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0.5f, 0, 0, 0, 1 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0.5f, 0, 1, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0.5f, 0, 0, 0, 0, 1 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.5f, 1, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0.5f, 0, 0, 1 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0.5f, 0, 0, 1, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.5f, 0, 1 }),
				Mach1TranscodeChannel::Coeffs({ 0.5f, 0, 0, 0, 1, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0.5f, 0, 0, 0, 1 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0.5f, 0, 1, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0.5f, 0, 0, 0, 0, 1 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.5f, 1, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0.5f, 0, 0, 1 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0.5f, 0, 0, 1, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.5f, 0, 1 }),
			}
		},
		// --- M1Spatial ---
		{
			"M1Spatial", "M1Spatial", // requested for conversions of number of files
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
				Mach1TranscodeChannel::Coeffs({ oor2, 0, 0, 0, oor2, 0, 0, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, oor2, 0, 0, 0, oor2, 0, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, oor2, 0, 0, 0, oor2, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, oor2, 0, 0, 0, oor2 }),
			}
		},
		{
			"M1Spatial", "M1HorizonPairs",
			{   
				Mach1TranscodeChannel::Coeffs({ oor2, 0, 0, 0, oor2, 0, 0, 0 }), //000L
				Mach1TranscodeChannel::Coeffs({ 0, oor2, 0, 0, 0, oor2, 0, 0 }), //000R
				Mach1TranscodeChannel::Coeffs({ 0, oor2, 0, 0, 0, oor2, 0, 0 }), //090L
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, oor2, 0, 0, 0, oor2 }), //090R
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, oor2, 0, 0, 0, oor2 }), //180L
				Mach1TranscodeChannel::Coeffs({ 0, 0, oor2, 0, 0, 0, oor2, 0 }), //180R
				Mach1TranscodeChannel::Coeffs({ 0, 0, oor2, 0, 0, 0, oor2, 0 }), //270L
				Mach1TranscodeChannel::Coeffs({ oor2, 0, 0, 0, oor2, 0, 0, 0 }), //270R
			}
		},
		{
			"M1Spatial", "M1SpatialFaces",
			//Fc, Lc, Bc, Rc, Tc, Bc
			{   
				Mach1TranscodeChannel::Coeffs({ r2o3, r2o3, 0, 0, r2o3, r2o3, 0, 0 }),
				Mach1TranscodeChannel::Coeffs({ r2o3, 0, r2o3, 0, r2o3, 0, r2o3, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, r2o3, r2o3, 0, 0, r2o3, r2o3 }),
				Mach1TranscodeChannel::Coeffs({ 0, r2o3, 0, r2o3, 0, r2o3, 0, r2o3 }),
				Mach1TranscodeChannel::Coeffs({ r2o3, r2o3, r2o3, r2o3, 0, 0, 0, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, r2o3, r2o3, r2o3, r2o3 }),
			}
		},
		{
			"M1Spatial", "M1SpatialPairs", //TODO: fix this
			{   
				Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0, 0, 0, 0, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0, 0, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0, 0, 0, 0, 0 }),
				Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0, 0, 0, 0, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1, 0, 0, 0, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0, 0, 0, 0, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0, 0, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1, 0, 0, 0, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 1, 0, 0, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 1, 0, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 1, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 1, 0, 0, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 1 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 1, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 1, 0, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 1 }),
			}
		},
		// --- M1Spatial + Stereo ( std::vector<std::vector<float>>S) ---
		{
			"M1SpatialS", "M1Horizon",
			{   
				Mach1TranscodeChannel::Coeffs({ 0.5f, 0, 0, 0, 0.5f, 0, 0, 0, 0.25f, 0.25f }),
				Mach1TranscodeChannel::Coeffs({ 0, 0.5f, 0, 0, 0, 0.5f, 0, 0, 0.25f, 0.25f }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0.5f, 0, 0, 0, 0.5f, 0, 0.25f, 0.25f }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.5f, 0, 0, 0, 0.5f, 0.25f, 0.25f }),
			}
		},
		{
			"M1SpatialS", "M1HorizonPairs",
			{   
				Mach1TranscodeChannel::Coeffs({ 0.5f, 0, 0, 0, 0.5f, 0, 0, 0, 1, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0.5f, 0, 0, 0, 0.5f, 0, 0, 0, 1 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0.5f, 0, 0, 0, 0.5f, 0, 0, 1, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.5f, 0, 0, 0, 0.5f, 0, 1 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.5f, 0, 0, 0, 0.5f, 1, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0.5f, 0, 0, 0, 0.5f, 0, 0, 1 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0.5f, 0, 0, 0, 0.5f, 0, 1, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0.5f, 0, 0, 0, 0.5f, 0, 0, 0, 0, 1 }),
			}
		},
		{
			"M1SpatialS", "M1Spatial",
			{   
				Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0, 0, 0, 0, 0, 0.125f, 0.125f }),
				Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0, 0, 0, 0.125f, 0.125f }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0, 0, 0, 0, 0, 0.125f, 0.125f }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1, 0, 0, 0, 0, 0.125f, 0.125f }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 1, 0, 0, 0, 0.125f, 0.125f }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 1, 0, 0, 0.125f, 0.125f }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 1, 0, 0.125f, 0.125f }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 1, 0.125f, 0.125f }),
			}
		},
		{
			"M1SpatialS", "M1SpatialPairs",
			{   
				Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0, 0, 0, 0, 0, 1, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0, 0, 0, 0, 1 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0, 0, 0, 0, 0, 1, 0 }),
				Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1, 0, 0, 0, 0, 1, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0, 0, 0, 0, 0, 0, 1 }),
				Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0, 0, 0, 1, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 1, 0, 0, 0, 1, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 1, 0, 0, 0, 1 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 1, 0, 1, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 1, 1, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 1, 0, 0, 1, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 1, 0, 1 }),
			}
		},
		// --- M1Spatial Faces ---
		{
			"M1SpatialFaces", "M1Spatial",
				// FC, LC, BC, RC, TC, BC
			{   
				Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, 0, 0, r2o4, 0 }),
				Mach1TranscodeChannel::Coeffs({ r2o4, 0, 0, r2o4, r2o4, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, r2o4, r2o4, 0, r2o4, 0 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, r2o4, r2o4, r2o4, 0 }),
				Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, 0, 0, 0, r2o4 }),
				Mach1TranscodeChannel::Coeffs({ r2o4, 0, 0, r2o4, 0, r2o4 }),
				Mach1TranscodeChannel::Coeffs({ 0, r2o4, r2o4, 0, 0, r2o4 }),
				Mach1TranscodeChannel::Coeffs({ 0, 0, r2o4, r2o4, 0, r2o4 }),
			}
		},
	};
			
		}
	};
};
