//  Mach1 Spatial SDK
//  Copyright © 2017-2022 Mach1. All rights reserved.
//
// TODO: 
// - 5.0.6/Auro-11: FL, FC, FR, Ls, Rs, TFL, TFC, TFR, TBL, TBR, Above
// - 7.0.6/Auro-13: FL, FC, FR, Lss, Rss, Lsr, Rsr, TFL, TFC, TFR, TBL, TBR, Above
// - 8.0.2/THX-10.0: 30degL, C, 30degR, 60degL, 60degR, Ls, BC, Rs, TLs, TRs
// - 10.0.2/THX-12.0: 30degL, C, 30degR, 60degL, 60degR, Ls, BC, Rs, TLs, TRs, 90degL, 90degR
// - 22.2: https://en.wikipedia.org/wiki/22.2_surround_sound | FL, FR, FC, LFE1, BL, BR, FLc, FRc, BC, LFE2, SiL (side left), SiR, TpFL, TpFR, TpFC, TpC, TpBL, TpBR, TpSiL, TpSiR, TpBC, BtFC, BtFL, BtFR
// - Add 7.0 and 7.1 -> M1Horizon conversions 

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

 /*
  * Multichannel Format/Configuration Naming Conventions:
  * - M or Music			= `Music Mix` (Channels are spaced out evenly throughout the horizontal soundfield)
  * - C or Cinema			= `Cinema Mix` (Channels are more focused on the front)
  * - S or SideSurround 	= `Side Surround Mix` (Surround channels are oriented more to the sides instead of rear (+-110 azimuth instead of +-135))
  * - R or RearSurround 	= `Rear Surround Mix` (Surround channels are oriented more to the rears instead of sides (+-154 azimuth instead of +-135))
  * - SIM or Simulated		= `Simulated Room Mix` (Lessens the divergence of virtual speakers to quickly simulate hearing front/back soundfield within a real world listening environment)
  *
  * Channel Order Variants:
  * - Default / Film / C|24 = L, C, R, Ls, Rs, LFE ...
  * - SMPTE					= L, R, C, LFE, Ls, Rs ...
  * - Dts					= L, R, Ls, Rs, C, LFE ...
  */

namespace Mach1TranscodeConstants {
	class MatricesSurround {
	public:
		static std::vector<Mach1TranscodeMatrix> getData() {
			const float oor4 = (float)(1.0 / sqrt(4.0));
			const float oor8 = (float)(1.0 / sqrt(8.0));
			const float oor16 = (float)(1.0 / sqrt(16.0));
			const float r2o2 = (float)sqrt(2.0) / 2.0f;
			const float r2o4 = (float)sqrt(2.0) / 4.0f;
			const float r2o8 = (float)sqrt(2.0) / 8.0f;
			const float r2o20 = (float)sqrt(2.0) / 20.0f;
			return {
				// --- 5.x.x Configurations --- //
				// -> M1Horizon & Quad_M
				{
					"5.0_M", "M1Horizon",
						//L, C, R, Ls, Rs
					{
						Mach1TranscodeChannel::Coeffs({ 1, r2o4, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, 0, 0, 1 }),
					}
				},
				{
					"5.0_C", "M1Horizon",
						//L, C, R, Ls, Rs
					{
						Mach1TranscodeChannel::Coeffs({ 1, r2o2, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 1 }),
					}
				},
				{
					"5.0_S", "M1Horizon",
					{
						//L, C, R, Ls, Rs
						// Rear channels ±110° azimuth
						Mach1TranscodeChannel::Coeffs({ 1, r2o2, 0, 0, 0.258155 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 1, 0.258155, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0.741845 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.741845, 0 }),
					}
				},
				{
					"5.0_R", "M1Horizon",
					{
						// L, C, R, Ls, Rs, LFE
						// Rear channels ±154° azimuth
						Mach1TranscodeChannel::Coeffs({ 1, r2o2, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.190025, 0.809975 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.809975, 0.190025 }),
					}
				},
				{
					"5.0_C_SIM", "M1Horizon",
					//L, C, R, Ls, Rs, LFE with front facing
					// Rear channels ±135° azimuth
					//Channel Divergence = 70%
					//Center Channel Divergence = 50%
					{
						Mach1TranscodeChannel::Coeffs({ 0.722500, 0.426777, 0.127500, 0.127500, 0.022500 }),
						Mach1TranscodeChannel::Coeffs({ 0.127500, 0.426777, 0.722500, 0.022500, 0.127500 }),
						Mach1TranscodeChannel::Coeffs({ 0.127500, 0.073223, 0.022500, 0.722500, 0.127500 }),
						Mach1TranscodeChannel::Coeffs({ 0.022500, 0.073223, 0.127500, 0.127500, 0.722500 }),
					}
				},
				{
					"5.1_M", "M1Horizon",
						//L, C, R, Ls, Rs, LFE
					{
						Mach1TranscodeChannel::Coeffs({ 1, r2o4, 0, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, 1, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, 0, 1, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, 0, 0, 1, r2o8 }),
					}
				},
				{
					"5.1_C", "M1Horizon",
					{
						// Rear channels ±135° azimuth
						Mach1TranscodeChannel::Coeffs({ 1, r2o2, 0, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 1, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 1, r2o8 }),
					}
				},
				{
					"5.1_S", "M1Horizon",
					{
						// Rear channels ±110° azimuth
						Mach1TranscodeChannel::Coeffs({ 1, r2o2, 0, 0, 0.258155, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 1, 0.258155, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0.741845, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.741845, 0, r2o8 }),
					}
				},
				{
					"5.1_R", "M1Horizon",
					{
						// L, C, R, Ls, Rs, LFE
						// Rear channels ±154° azimuth
						Mach1TranscodeChannel::Coeffs({ 1, r2o2, 0, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 1, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.190025, 0.809975 , r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.809975, 0.190025, r2o8 }),
					}
				},
				{
					"5.1_M_SMPTE", "M1Horizon",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, r2o4, r2o8, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, r2o4, r2o8, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o4, r2o8, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o4, r2o8, 0, 1 }),
					}
				},
				{
					"5.1_M_Dts", "M1Horizon",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0, r2o4, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, r2o4, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0, r2o4, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1, r2o4, r2o8 }),
					}
				},
				{
					"5.1_C_SIM", "M1Horizon",
					//L, C, R, Ls, Rs, LFE with front facing
					// Rear channels ±135° azimuth
					//Channel Divergence = 70%
					//Center Channel Divergence = 50%
					{
						Mach1TranscodeChannel::Coeffs({ 0.722500, 0.426777, 0.127500, 0.127500, 0.022500, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0.127500, 0.426777, 0.722500, 0.022500, 0.127500, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0.127500, 0.073223, 0.022500, 0.722500, 0.127500, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0.022500, 0.073223, 0.127500, 0.127500, 0.722500, r2o8 }),
					}
				},
				// -> M1Spatial
				{
					"5.0_M", "M1Spatial",
					//L, C , R, Ls, Rs
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o8, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, 0, r2o2, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, 0, 0, r2o2 }),
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o8, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, 0, r2o2, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, 0, 0, r2o2 }),
					}
				},
				{
					"5.0_C", "M1Spatial",
					//L, C , R, Ls, Rs
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, r2o2, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, 0, r2o2 }),
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, r2o2, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, 0, r2o2 }),
					}
				},
				{
					"5.0_S", "M1Spatial",
					//L, C , R, Ls, Rs
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, 0, 0.129078, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, r2o2, 0, 0.129078 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, 0.370922, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, 0, 0.370922 }),
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, 0, 0.129078, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, r2o2, 0, 0.129078 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, 0.370922, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, 0, 0.370922 }),
					}
				},
				{
					"5.0_R", "M1Spatial",
					//L, C , R, Lsr, Rsr, LFE
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, 0.095012, 0.404988 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, 0.404988, 0.095012 }),
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, 0, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, r2o2, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, 0.095012, 0.404988 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, 0.404988, 0.095012 }),
					}
				},
				{
					"5.0_C_SIM", "M1Spatial",
					//L, C, R, Ls, Rs, LFE with front facing
					// Rear channels ±135° azimuth
					//Channel Divergence = 70%
					//Center Channel Divergence = 50%
					{
						Mach1TranscodeChannel::Coeffs({ 0.361250, 0.213388, 0.063750, 0.063750, 0.011250 }),
						Mach1TranscodeChannel::Coeffs({ 0.063750, 0.213388, 0.361250, 0.011250, 0.063750 }),
						Mach1TranscodeChannel::Coeffs({ 0.063750, 0.036612, 0.011250, 0.361250, 0.063750 }),
						Mach1TranscodeChannel::Coeffs({ 0.011250, 0.036612, 0.063750, 0.063750, 0.361250 }),
						Mach1TranscodeChannel::Coeffs({ 0.361250, 0.213388, 0.063750, 0.063750, 0.011250 }),
						Mach1TranscodeChannel::Coeffs({ 0.063750, 0.213388, 0.361250, 0.011250, 0.063750 }),
						Mach1TranscodeChannel::Coeffs({ 0.063750, 0.036612, 0.011250, 0.361250, 0.063750 }),
						Mach1TranscodeChannel::Coeffs({ 0.011250, 0.036612, 0.063750, 0.063750, 0.361250 }),
					}
				},
				{
					"5.1_M", "M1Spatial",
					//L, C, R, Ls, Rs, LFE
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o8, 0, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, r2o2, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, 0, r2o2, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, 0, 0, r2o2, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o8, 0, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, r2o2, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, 0, r2o2, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, 0, 0, r2o2, r2o8 }),
					}
				},
				{
					"5.1_C", "M1Spatial",
					//L, C, R, Ls, Rs, LFE
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, 0, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, r2o2, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, r2o2, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, 0, r2o2, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, 0, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, r2o2, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, r2o2, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, 0, r2o2, r2o8 }),
					}
				},
				{
					"5.1_S", "M1Spatial",
					//L, C , R, Ls, Rs, LFE
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, 0, 0.129078, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, r2o2, 0, 0.129078, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, 0.370922, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, 0, 0.370922, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, 0, 0.129078, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, r2o2, 0, 0.129078, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, 0.370922, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, 0, 0.370922, r2o8 }),
					}
				},
				{
					"5.1_R", "M1Spatial",
					//L, C , R, Lsr, Rsr, LFE
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, 0, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, r2o2, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, 0.095012, 0.404988, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, 0.404988, 0.095012, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, 0, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, r2o2, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, 0.095012, 0.404988, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, 0.404988, 0.095012, r2o8 }),
					}
				},
				{
					"5.1_M_SMPTE", "M1Spatial",
					//L, R, C, LFE, Ls, Rs
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, r2o8, r2o8, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, r2o8, r2o8, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o8, r2o8, r2o2, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o8, r2o8, 0, r2o2 }),
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, r2o8, r2o8, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, r2o8, r2o8, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o8, r2o8, r2o2, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o8, r2o8, 0, r2o2 }),
					}
				},
				{
					"5.1_M_Dts", "M1Spatial",
					//L, R, Ls, Rs, C, LFE
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o8, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, r2o8, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, r2o8, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, r2o8, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o8, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, r2o8, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, r2o8, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, r2o8, r2o8 }),
					}
				},
				{
					"5.1_C_SIM", "M1Spatial",
					// L, C, R, Ls, Rs, LFE with front facing
					// Rear channels ±135° azimuth
					// Channel Divergence = 70%
					// Center Channel Divergence = 50%
					{
						Mach1TranscodeChannel::Coeffs({ 0.361250, 0.213388, 0.063750, 0.063750, 0.011250, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0.063750, 0.213388, 0.361250, 0.011250, 0.063750, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0.063750, 0.036612, 0.011250, 0.361250, 0.063750, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0.011250, 0.036612, 0.063750, 0.063750, 0.361250, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0.361250, 0.213388, 0.063750, 0.063750, 0.011250, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0.063750, 0.213388, 0.361250, 0.011250, 0.063750, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0.063750, 0.036612, 0.011250, 0.361250, 0.063750, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0.011250, 0.036612, 0.063750, 0.063750, 0.361250, r2o8 }),
					}
				},
				{
					"5.1.2_C", "M1Spatial",
					//L  C  R  Ls Rs LFE Lts Rts
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, 0, 0, 0, r2o8, 0.3750f, 0.1250f }),	// -> TFL (ch1)
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, r2o2, 0, 0, r2o8, 0.1250f, 0.3750f }),	// -> TFR (ch2)
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, r2o2, 0, r2o8, 0.3750f, 0.1250f }),// -> TBL (ch3)
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, 0, r2o2, r2o8, 0.1250f, 0.3750f }),// -> TBR (ch4)
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, 0, 0, 0, r2o8, 0, 0 }),				// -> BFL (ch5)
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, r2o2, 0, 0, r2o8, 0, 0 }),				// -> BFR (ch6)
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, r2o2, 0, r2o8, 0, 0 }),			// -> BBL (ch7)
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, 0, r2o2, r2o8, 0, 0 }),			// -> BBR (ch8)
					}
				},
				{
					"5.0.2_C", "M1Spatial",
					//L  C  R  Ls Rs Lts Rts
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, 0, 0, 0, 0.3750f, 0.1250f }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, r2o2, 0, 0, 0.1250f, 0.3750f }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, r2o2, 0, 0.3750f, 0.1250f }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, 0, r2o2, 0.1250f, 0.3750f }),
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, r2o2, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, r2o2, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, 0, r2o2, 0, 0 }),
					}
				},
				{
					"5.0.4_C", "M1Spatial",
					//Equivelent to `Auro 9`
					//L  C  R  Ls Rs FLts FRts BLts BRts
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, 0, 0, 0, 0.7280f, 0.1250f, 0.1250f, 0.0210f }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, r2o2, 0, 0, 0.1250f, 0.7280f, 0.0210f, 0.1250f }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, r2o2, 0, 0.1250f, 0.0210f, 0.7280f, 0.1250f }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, 0, r2o2, 0.0210f, 0.1250f, 0.1250f, 0.7280f }),
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, r2o2, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, r2o2, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, 0, r2o2, 0, 0, 0, 0 }),
					}
				},
				{
					"5.0.5_C", "M1Spatial",
					// `5.0.5_C` is equivelent to `Auro 10`
					//FL  FC  FR  Ls Rs TFL TFR TBL TBR Above
					// TODO: normalize spread from bottom to top for FL,FC,FR,Ls,Rs
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, 0, 0, 0, 1, 0, 0, 0, r2o4 }),	// -> TFL
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, r2o2, 0, 0, 0, 1, 0, 0, r2o4 }),	// -> TFR
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, 1, 0, r2o4 }),		// -> TBL
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, r2o2, 0, 0, 0, 1, r2o4 }),		// -> TBR
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, 0, 0, 0, 0, 0, 0, 0, 0 }),		// -> BFL
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, r2o2, 0, 0, 0, 0, 0, 0, 0 }),		// -> BFR
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, 0, 0, 0 }),			// -> BBL
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, r2o2, 0, 0, 0, 0, 0 }),			// -> BBR
					}
				},
				{
					"5.1.4_C", "M1Spatial",
					//L  C  R  Ls Rs LFE FLts FRts BLts BRts
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, 0, 0, 0, r2o8, 0.7280f, 0.1250f, 0.1250f, 0.0210f }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, r2o2, 0, 0, r2o8, 0.1250f, 0.7280f, 0.0210f, 0.1250f }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, r2o2, 0, r2o8, 0.1250f, 0.0210f, 0.7280f, 0.1250f }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, 0, r2o2, r2o8, 0.0210f, 0.1250f, 0.1250f, 0.7280f }),
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, 0, 0, 0, r2o8, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, r2o2, 0, 0, r2o8, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, r2o2, 0, r2o8, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20, 0, 0, r2o2, r2o8, 0, 0, 0, 0 }),
					}
				},
				{
					"5.1.4_C_SMPTE", "M1Spatial",
					//L  R  C  LFE Ls Rs FLts FRts BLts BRts
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, r2o4, r2o8, 0, 0, 0.7280f, 0.1250f, 0.1250f, 0.0210f }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, r2o4, r2o8, 0, 0, 0.1250f, 0.7280f, 0.0210f, 0.1250f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o20, r2o8, r2o2, 0, 0.1250f, 0.0210f, 0.7280f, 0.1250f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o20, r2o8, 0, r2o2, 0.0210f, 0.1250f, 0.1250f, 0.7280f }),
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, r2o4, r2o8, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, r2o4, r2o8, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o20, r2o8, r2o2, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o20, r2o8, 0, r2o2, 0, 0, 0, 0 }),
					}
				},
				// -> 5.x.x
				{
					"M1Horizon", "5.0_M",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, r2o4, r2o4 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1 }),
					}
				},
				{
					"M1Horizon", "5.0_C",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1 }),
					}
				},
				{
					"M1Horizon", "5.0_S",
					// FL, FR, BL, BR
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0 }),					// -> L
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o2, 0, 0 }),			// -> C
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0 }),					// -> R
						Mach1TranscodeChannel::Coeffs({ 0.258155, 0, 0.741845, 0 }),	// -> Ls
						Mach1TranscodeChannel::Coeffs({ 0, 0.258155, 0, 0.741845 }),	// -> Rs
					}
				},
				{
					"M1Horizon", "5.1_M",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, r2o4, r2o4 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8 }),
					}
				},
				{
					"M1Horizon", "5.1_C",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8 }),
					}
				},
				{
					"M1Horizon", "5.1_S",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.258155, 0, 0.741845, 0 }),	// -> Ls
						Mach1TranscodeChannel::Coeffs({ 0, 0.258155, 0, 0.741845 }),	// -> Rs
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8 }),
					}
				},
				{
					"M1Horizon", "5.1_M_SMPTE",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1 }),
					}
				},
				{
					"M1Horizon", "5.1_M_Dts",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8 }),
					}
				},
				{
					"M1Spatial", "5.0_M",
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2 }),
					}
				},
				{
					"M1SpatialS", "5.0_M",
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),
					}
				},
				{
					"M1Spatial", "5.1_M",
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2 }),
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 }),
					}
				},
				{
					"M1SpatialS", "5.1_M",
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o2, r2o2 }),
					}
				},
				{
					"M1Spatial", "5.1_C",
					{
						// TFL, TFR, TBL, TBR, BFL, BFR, BBL, BBR
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),					// -> L
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0 }),				// -> C
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),					// -> R
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0 }),					// -> Ls
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2 }),      				// -> Rs
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 }),	// -> LFE
					}
				},
				{
					"M1SpatialS", "5.1_C",
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, 0, 0 }),
					}
				},
				{
					"M1Spatial", "5.1_S",
					{
						// TFL, TFR, TBL, TBR, BFL, BFR, BBL, BBR
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),						// -> L
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0 }),					// -> C
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),						// -> R
						Mach1TranscodeChannel::Coeffs({ 0, 0.129078, 0, 0.370922, 0, 0.129078, 0, 0.370922 }),	// -> Ls
						Mach1TranscodeChannel::Coeffs({ 0.129078, 0, 0.370922, 0, 0.129078, 0, 0.370922, 0 }),	// -> Rs
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 }),		// -> LFE
					}
				},
				{
					"M1SpatialS", "5.1_S",
					{
						// TFL, TFR, TBL, TBR, BFL, BFR, BBL, BBR
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1, 0 }),							// -> L
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0, 0, 0 }),					// -> C
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1 }),							// -> R
						Mach1TranscodeChannel::Coeffs({ 0, 0.129078, 0, 0.370922, 0, 0.129078, 0, 0.370922, 0, 0 }),	// -> Ls
						Mach1TranscodeChannel::Coeffs({ 0.129078, 0, 0.370922, 0, 0.129078, 0, 0.370922, 0, 0, 0 }),	// -> Rs
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, 0, 0 }),		// -> LFE
					}
				},
				{
					"M1Spatial", "5.1_R",
					{
						// TFL, TFR, TBL, TBR, BFL, BFR, BBL, BBR
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),						// -> L
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0 }),					// -> C
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),						// -> R
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.404988, 0.095012, 0, 0, 0.404988, 0.095012 }),	// -> Ls
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.095012, 0.404988, 0, 0, 0.095012, 0.404988 }),	// -> Rs
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 }),		// -> LFE
					}
				},
				{
					"M1SpatialS", "5.1_R",
					{
						// TFL, TFR, TBL, TBR, BFL, BFR, BBL, BBR
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1, 0 }),							// -> L
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0, 0, 0 }),					// -> C
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1 }),							// -> R
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.404988, 0.095012, 0, 0, 0.404988, 0.095012, 0, 0 }),	// -> Ls
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.095012, 0.404988, 0, 0, 0.095012, 0.404988, 0, 0 }),	// -> Rs
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, 0, 0 }),		// -> LFE
					}
				},
				{
					"M1Spatial", "5.1_C_SMPTE",
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2 }),
					}
				},
				{
					"M1SpatialS", "5.1_C_SMPTE",
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o2, r2o2 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),
					}
				},
				{
					"M1Spatial", "5.1_C_Dts",
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2 }),
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 }),
					}
				},
				{
					"M1SpatialS", "5.1_C_Dts",
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o2, r2o2 }),
					}
				},
				{
					"M1Spatial", "5.0.2_C",
					//1  2  3  4  5  6  7  8
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2 }),
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, r2o2, 0, 0, 0, 0, 0 }),//Lts
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, r2o2, 0, 0, 0, 0 }),//Rts 
					}
				},
				{
					"M1SpatialS", "5.0.2_C",
					//1  2  3  4  5  6  7  8
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, r2o2, 0, 0, 0, 0, 0, 0, 0 }),//Lts
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, r2o2, 0, 0, 0, 0, 0, 0 }),//Rts
					}
				},
				{
					"M1Spatial", "5.1.2_C",
					//1  2  3  4  5  6  7  8
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2 }),
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, r2o2, 0, 0, 0, 0, 0 }),//Lts
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, r2o2, 0, 0, 0, 0 }),//Rts
					}
				},
				{
					"M1SpatialS", "5.1.2_C",
					//1  2  3  4  5  6  7  8
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o2, r2o2 }),
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, r2o2, 0, 0, 0, 0, 0, 0, 0 }),//Lts
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, r2o2, 0, 0, 0, 0, 0, 0 }),//Rts 
					}
				},
				{
					"M1Spatial", "5.0.4_C",
					//1  2  3  4  5  6  7  8
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2 }),
						Mach1TranscodeChannel::Coeffs({ 0.7280f, 0.1250f, 0.1250f, 0.0210f, 0, 0, 0, 0 }),//FLts
						Mach1TranscodeChannel::Coeffs({ 0.1250f, 0.7280f, 0.0210f, 0.1250f, 0, 0, 0, 0 }),//FRts
						Mach1TranscodeChannel::Coeffs({ 0.1250f, 0.0210f, 0.7280f, 0.1250f, 0, 0, 0, 0 }),//BLts
						Mach1TranscodeChannel::Coeffs({ 0.0210f, 0.1250f, 0.1250f, 0.7280f, 0, 0, 0, 0 }),//BRts
					}
				},
				{
					"M1SpatialS", "5.0.4_C",
					//1  2  3  4  5  6  7  8
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.7280f, 0.1250f, 0.1250f, 0.0210f, 0, 0, 0, 0, 0, 0 }),//FLts
						Mach1TranscodeChannel::Coeffs({ 0.1250f, 0.7280f, 0.0210f, 0.1250f, 0, 0, 0, 0, 0, 0 }),//FRts
						Mach1TranscodeChannel::Coeffs({ 0.1250f, 0.0210f, 0.7280f, 0.1250f, 0, 0, 0, 0, 0, 0 }),//BLts
						Mach1TranscodeChannel::Coeffs({ 0.0210f, 0.1250f, 0.1250f, 0.7280f, 0, 0, 0, 0, 0, 0 }),//BRts
					}
				},
				{
					"M1Spatial", "5.0.5_C",
					// `5.0.5_C` is equivelent to `Auro 10`
					//1  2  3  4  5  6  7  8
					// TODO: normalize spread from bottom to top for FL,FC,FR,Ls,Rs
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),// -> FL
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0 }),// -> FC
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),// -> FR
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0 }),// -> Ls
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2 }),// -> Rs
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0, 0, 0, 0, 0 }),// -> TFL
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0, 0, 0 }),// -> TFR
						Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0, 0, 0, 0, 0 }),// -> TBL
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1, 0, 0, 0, 0 }),// -> TBR
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, r2o4, r2o4, 0, 0, 0, 0 }),// -> Above
					}
				},
				{
					"M1Spatial", "5.1.4_C",
					//1  2  3  4  5  6  7  8
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.7280f, 0.1250f, 0.1250f, 0.0210f, 0, 0, 0, 0 }),//FLts
						Mach1TranscodeChannel::Coeffs({ 0.1250f, 0.7280f, 0.0210f, 0.1250f, 0, 0, 0, 0 }),//FRts
						Mach1TranscodeChannel::Coeffs({ 0.1250f, 0.0210f, 0.7280f, 0.1250f, 0, 0, 0, 0 }),//BLts
						Mach1TranscodeChannel::Coeffs({ 0.0210f, 0.1250f, 0.1250f, 0.7280f, 0, 0, 0, 0 }),//BRts
					}
				},
				{
					"M1SpatialS", "5.1.4_C",
					//1  2  3  4  5  6  7  8
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, r2o2 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, r2o4, r2o4 }),
						Mach1TranscodeChannel::Coeffs({ 0.7280f, 0.1250f, 0.1250f, 0.0210f, 0, 0, 0, 0, 0, 0 }),//FLts
						Mach1TranscodeChannel::Coeffs({ 0.1250f, 0.7280f, 0.0210f, 0.1250f, 0, 0, 0, 0, 0, 0 }),//FRts
						Mach1TranscodeChannel::Coeffs({ 0.1250f, 0.0210f, 0.7280f, 0.1250f, 0, 0, 0, 0, 0, 0 }),//BLts
						Mach1TranscodeChannel::Coeffs({ 0.0210f, 0.1250f, 0.1250f, 0.7280f, 0, 0, 0, 0, 0, 0 }),//BRts
					}
				},
				{
					"M1Spatial", "5.1.4_C_SMPTE",
					//1  2  3  4  5  6  7  8
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2 }),
						Mach1TranscodeChannel::Coeffs({ 0.7280f, 0.1250f, 0.1250f, 0.0210f, 0, 0, 0, 0 }),//FLts
						Mach1TranscodeChannel::Coeffs({ 0.1250f, 0.7280f, 0.0210f, 0.1250f, 0, 0, 0, 0 }),//FRts
						Mach1TranscodeChannel::Coeffs({ 0.1250f, 0.0210f, 0.7280f, 0.1250f, 0, 0, 0, 0 }),//BLts
						Mach1TranscodeChannel::Coeffs({ 0.0210f, 0.1250f, 0.1250f, 0.7280f, 0, 0, 0, 0 }),//BRts 
					}
				},
				{
					"M1SpatialS", "5.1.4_C_SMPTE",
					//1  2  3  4  5  6  7  8
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, r2o2 }),
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, r2o4, r2o4 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.7280f, 0.1250f, 0.1250f, 0.0210f, 0, 0, 0, 0, 0, 0 }),//FLts
						Mach1TranscodeChannel::Coeffs({ 0.1250f, 0.7280f, 0.0210f, 0.1250f, 0, 0, 0, 0, 0, 0 }),//FRts
						Mach1TranscodeChannel::Coeffs({ 0.1250f, 0.0210f, 0.7280f, 0.1250f, 0, 0, 0, 0, 0, 0 }),//BLts
						Mach1TranscodeChannel::Coeffs({ 0.0210f, 0.1250f, 0.1250f, 0.7280f, 0, 0, 0, 0, 0, 0 }),//BRts
					}
				},
				// --- 6.x.x Configurations --- //
				// -> M1Horizon / Quad_M
				{
					"6.0_M", "M1Horizon",
					// L, C, R, Ls, Rs, Cs
					{
						Mach1TranscodeChannel::Coeffs({ 1, r2o4, 0, 0, 0, 0 }),	// -> FL
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, 1, 0, 0, 0 }),	// -> FR
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, 0, 1, 0, r2o2 }),// -> BL
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, 0, 0, 1, r2o2 }),// -> BR
					}
				},
				// -> M1Spatial
				{
					"6.0_M", "M1Spatial",
					// L, C, R, Ls, Rs, Cs
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o8, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, r2o2, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, 0, r2o2, 0, r2o4 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, 0, 0, r2o2, r2o4 }),
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o8, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, r2o2, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, 0, r2o2, 0, r2o4 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, 0, 0, r2o2, r2o4 }),
					}
				},
				// -> 6.x.x
				{
					"M1Horizon", "6.0_M",
					// FL, FR, BL, BR
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, r2o4, r2o4 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, r2o2 }),// -> Cs
					}
				},
				{
					"M1Spatial", "6.0_M",
					// TFL, TFR, BFL, BFR, BFL, BFR, BBL, BBR
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),					// -> L
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 }),	// -> C
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),					// -> R
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0 }),					// -> Ls
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2 }),					// -> Rs
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o4, r2o4, 0, 0, r2o4, r2o4 }),				// -> Cs
					}
				},
				// --- 7.x.x Configurations --- //
				// -> M1Spatial
				{
					"7.0_M", "M1Spatial",
					//L, C, R, Lss, Rss, Lsr, Rsr, LFE
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o8, 0, r2o4, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, r2o2, 0, r2o4, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, 0, r2o4, 0, r2o2, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, 0, 0, r2o4, 0, r2o2 }),
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o8, 0, r2o4, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, r2o2, 0, r2o4, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, 0, r2o4, 0, r2o2, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, 0, 0, r2o4, 0, r2o2 }),
					}
				},
				{
					"7.0_C", "M1Spatial",
					//L, C, R, Lss, Rss, Lsr, Rsr with front facing
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, 0, r2o4, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, r2o2, 0, r2o4, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o4, 0, r2o2, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, r2o4, 0, r2o2 }),
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, 0, r2o4, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, r2o2, 0, r2o4, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o4, 0, r2o2, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, r2o4, 0, r2o2 }),
					}
				},
				{
					"7.0_M_SDDS", "M1Spatial",
					//L, Lc, C, Rc, R, Ls, Rs
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, r2o8, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o8, r2o4, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o8, 0, 0, r2o2, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o8, 0, 0, 0, r2o2 }),
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, r2o8, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o8, r2o4, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o8, 0, 0, r2o2, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o8, 0, 0, 0, r2o2 }),
					}
				},
				{
					"7.0_C_SDDS", "M1Spatial",
					//L, Lc, C, Rc, R, Ls, Rs
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, r2o4, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o4, r2o4, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, r2o2, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, r2o2 }),
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, r2o4, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o4, r2o4, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, r2o2, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, r2o2 }),
					}
				},
				{
					"7.0_C_3D", "M1Spatial",
					//TFL TFR CF TBC BFC BFL BFR
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, r2o4, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, r2o4, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o4, 0, r2o2, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o4, 0, r2o2, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0 }),
					}
				},
				{
					"7.0_C_SIM", "M1Spatial",
					//L, C, R, Lss, Rss, Lsr, Rsr with front facing
					//Channel Divergence = 70%
					//Center Channel Divergence = 50%
					{
						Mach1TranscodeChannel::Coeffs({ 0.361250, 0.213388, 0.063750, 0.248744, 0.001256, 0.063750, 0.011250 }),
						Mach1TranscodeChannel::Coeffs({ 0.063750, 0.213388, 0.361250, 0.001256, 0.248744, 0.011250, 0.063750 }),
						Mach1TranscodeChannel::Coeffs({ 0.063750, 0.036612, 0.011250, 0.248744, 0.001256, 0.361250, 0.063750 }),
						Mach1TranscodeChannel::Coeffs({ 0.011250, 0.036612, 0.063750, 0.001256, 0.248744, 0.063750, 0.361250 }),
						Mach1TranscodeChannel::Coeffs({ 0.361250, 0.213388, 0.063750, 0.248744, 0.001256, 0.063750, 0.011250 }),
						Mach1TranscodeChannel::Coeffs({ 0.063750, 0.213388, 0.361250, 0.001256, 0.248744, 0.011250, 0.063750 }),
						Mach1TranscodeChannel::Coeffs({ 0.063750, 0.036612, 0.011250, 0.248744, 0.001256, 0.361250, 0.063750 }),
						Mach1TranscodeChannel::Coeffs({ 0.011250, 0.036612, 0.063750, 0.001256, 0.248744, 0.063750, 0.361250 }),
					}
				},
				{
					"7.1_M", "M1Spatial",
					//L, C, R, Lss, Rss, Lsr, Rsr, LFE
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o8, 0, r2o4, 0, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, r2o2, 0, r2o4, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, 0, r2o4, 0, r2o2, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, 0, 0, r2o4, 0, r2o2, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o8, 0, r2o4, 0, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, r2o2, 0, r2o4, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, 0, r2o4, 0, r2o2, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, 0, 0, r2o4, 0, r2o2, r2o8 }),
					}
				},
				{
					"7.1_C", "M1Spatial",
					//L, C, R, Lss, Rss, Lsr, Rsr, LFE with front facing
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, 0, r2o4, 0, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, r2o2, 0, r2o4, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o4, 0, r2o2, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, r2o4, 0, r2o2, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, 0, r2o4, 0, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, r2o2, 0, r2o4, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o4, 0, r2o2, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, r2o4, 0, r2o2, r2o8 }),
					}
				},
				{
					"7.1_M_SDDS", "M1Spatial",
					//L, Lc, C, Rc, R, Ls, Rs, LFE
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, r2o8, 0, 0, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o8, r2o4, r2o2, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o8, 0, 0, r2o2, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o8, 0, 0, 0, r2o2, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, r2o8, 0, 0, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o8, r2o4, r2o2, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o8, 0, 0, r2o2, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o8, 0, 0, 0, r2o2, r2o8 }),
					}
				},
				{
					"7.1_C_SDDS", "M1Spatial",
					//L, Lc, C, Rc, R, Ls, Rs, LFE
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, r2o4, 0, 0, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o4, r2o4, r2o2, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, r2o2, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, r2o2, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, r2o4, 0, 0, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o4, r2o4, r2o2, 0, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, r2o2, 0, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, r2o2, r2o8 }),
					}
				},
				{
					"7.1_C_SIM", "M1Spatial",
					//L, C, R, Lss, Rss, Lsr, Rsr, LFE with front facing
					//Channel Divergence = 70%
					//Center Channel Divergence = 50%
					{
						Mach1TranscodeChannel::Coeffs({ 0.361250, 0.213388, 0.063750, 0.248744, 0.001256, 0.063750, 0.011250, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0.063750, 0.213388, 0.361250, 0.001256, 0.248744, 0.011250, 0.063750, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0.063750, 0.036612, 0.011250, 0.248744, 0.001256, 0.361250, 0.063750, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0.011250, 0.036612, 0.063750, 0.001256, 0.248744, 0.063750, 0.361250, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0.361250, 0.213388, 0.063750, 0.248744, 0.001256, 0.063750, 0.011250, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0.063750, 0.213388, 0.361250, 0.001256, 0.248744, 0.011250, 0.063750, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0.063750, 0.036612, 0.011250, 0.248744, 0.001256, 0.361250, 0.063750, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0.011250, 0.036612, 0.063750, 0.001256, 0.248744, 0.063750, 0.361250, r2o8 }),
					}
				},
				{
					"7.0.2_C", "M1Spatial",
					//L, C, R, Lss, Rss, Lsr, Rsr, Lts, Rts
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, r2o4, 0, 0, 0, 0, 0.0236619f, 0.0078859f }),   // -> TFL (ch1)
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, r2o4, 0, 0, 0, 0, 0.0078859f, 0.0236619f }),   // -> TFR (ch2)
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, r2o2, 0, 0.0236619f, 0.0078859f }),      // -> TBL (ch3)
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, r2o2, 0.0078859f, 0.0236619f }),      // -> TBR (ch4)
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, r2o4, 0, 0, 0, 0, 0, 0 }),                     // -> BFL (ch5)
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, r2o4, 0, 0, 0, 0, 0, 0 }),                     // -> BFR (ch6)
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, r2o2, 0, 0, 0 }),                        // -> BBL (ch7)
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, r2o2, 0, 0 }),                        // -> BBR (ch8)
					}
				},
				{
					"7.1.2_M", "M1Spatial",
						//L, R, C, LFE, Lss, Rss, Lrs, Rrs, Ltm, Rtm
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, r2o8, 0, 0, 0, 0, 0, 0.0236619f, 0.0078859f }),   	// -> TFL (ch1)
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, r2o8, 0, 0, 0, 0, 0, 0.0078859f, 0.0236619f }),   	// -> TFR (ch2)
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o8, 0, 0, 0, r2o2, 0, 0.0236619f, 0.0078859f }),   	// -> TBL (ch3)
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o8, 0, 0, 0, 0, r2o2, 0.0078859f, 0.0236619f }),   	// -> TBR (ch4)
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, r2o8, 0, 0, 0, 0, 0, 0, 0 }),                     	// -> BFL (ch5)
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, r2o8, 0, 0, 0, 0, 0, 0, 0 }),                     	// -> BFR (ch6)
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o8, 0, 0, 0, r2o2, 0, 0, 0 }),                     	// -> BBL (ch7)
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o8, 0, 0, 0, 0, r2o2, 0, 0 }),						// -> BBR (ch8)
					}
				},
				{
					"7.1.2_C", "M1Spatial",
						//L, R, C, LFE, Lss, Rss, Lrs, Rrs, Ltm, Rtm
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, r2o4, 0, 0, 0, 0, 0, 0.0236619f, 0.0078859f }),   // -> TFL (ch1)
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, r2o4, 0, 0, 0, 0, 0, 0.0078859f, 0.0236619f }),   // -> TFR (ch2)
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, r2o2, 0, 0.0236619f, 0.0078859f }),      // -> TBL (ch3)
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, r2o2, 0.0078859f, 0.0236619f }),      // -> TBR (ch4)
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, r2o4, 0, 0, 0, 0, 0, 0, 0 }),                     // -> BFL (ch5)
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, r2o4, 0, 0, 0, 0, 0, 0, 0 }),                     // -> BFR (ch6)
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, r2o2, 0, 0, 0 }),                        // -> BBL (ch7)
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, r2o2, 0, 0 }),                        // -> BBR (ch8)
					}
				},
				{
					"7.1.2_C", "M1SpatialS",
						//L, R, C, LFE, Lss, Rss, Lrs, Rrs, Ltm, Rtm
					{
						Mach1TranscodeChannel::Coeffs({ r2o4, 0, 0, 0, 0, 0, 0, 0, 0.0236619f, 0.0078859f }),   // -> TFL (ch1)
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, 0, 0, 0, 0, 0, 0, 0.0078859f, 0.0236619f }),   // -> TFR (ch2)
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, r2o2, 0, 0.0236619f, 0.0078859f }),   // -> TBL (ch3)
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, r2o2, 0.0078859f, 0.0236619f }),   // -> TBR (ch4)
						Mach1TranscodeChannel::Coeffs({ r2o4, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),                     // -> BFL (ch5)
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, 0, 0, 0, 0, 0, 0, 0, 0 }),                     // -> BFR (ch6)
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, r2o2, 0, 0, 0 }),                     // -> BBL (ch7)
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, r2o2, 0, 0 }),                     // -> BBR (ch8)
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, r2o2, r2o4, 0, 0, 0, 0, 0, 0 }),               // -> StereoL (ch9)
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, r2o2, r2o4, 0, 0, 0, 0, 0, 0 }),               // -> StereoR (ch10)
					}
				},
				{
					"7.0.4_C", "M1Spatial",
					//L, C, R, Lss, Rss, Lsr, Rsr, FLts, FRts, BLts, BRts
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, 0, r2o4, 0, 0, 0, 0.7280f, 0.1250f, 0.1250f, 0.0210f }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, r2o2, 0, r2o4, 0, 0, 0.1250f, 0.7280f, 0.0210f, 0.1250f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o4, 0, r2o2, 0, 0.1250f, 0.0210f, 0.7280f, 0.1250f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, r2o4, 0, r2o2, 0.0210f, 0.1250f, 0.1250f, 0.7280f }),
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, 0, r2o4, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, r2o2, 0, r2o4, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o4, 0, r2o2, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, r2o4, 0, r2o2, 0, 0, 0, 0 }),
					}
				},
				{
					"7.1.4_C", "M1Spatial",
					//L, C, R, Lss, Rss, Lsr, Rsr, LFE, FLts, FRts, BLts, BRts
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, 0, r2o4, 0, 0, 0, r2o8, 0.7280f, 0.1250f, 0.1250f, 0.0210f }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, r2o2, 0, r2o4, 0, 0, r2o8, 0.1250f, 0.7280f, 0.0210f, 0.1250f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o4, 0, r2o2, 0, r2o8, 0.1250f, 0.0210f, 0.7280f, 0.1250f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, r2o4, 0, r2o2, r2o8, 0.0210f, 0.1250f, 0.1250f, 0.7280f }),
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o4, 0, r2o4, 0, 0, 0, r2o8, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, r2o2, 0, r2o4, 0, 0, r2o8, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o4, 0, r2o2, 0, r2o8, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, r2o4, 0, r2o2, r2o8, 0, 0, 0, 0 }),
					}
				},
				// -> 7.x.x
				{
					"M1Spatial", "7.0_C",
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, 0, r2o4, 0, r2o4, 0, r2o4 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2 }),
					}
				},
				{
					"M1SpatialS", "7.0_C",
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),
					}
				},
				{
					"M1Spatial", "7.0_C_SDDS",
					//1  2  3  4  5  6  7  8
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),//L
						Mach1TranscodeChannel::Coeffs({ 0.375f, 0.125f, 0, 0, 0.375f, 0.125f, 0, 0 }),//Lc
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0 }),//C
						Mach1TranscodeChannel::Coeffs({ 0.125f, 0.375f, 0, 0, 0.125f, 0.375f, 0, 0 }),//Rc
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),//R
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0 }),//Ls
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2 }),//Rs
					}
				},
				{
					"M1SpatialS", "7.0_C_SDDS",
					//1  2  3  4  5  6  7  8  9  10
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0 }),//L
						Mach1TranscodeChannel::Coeffs({ 0.375f, 0.125f, 0, 0, 0.375f, 0.125f, 0, 0, r2o2, 0 }),//Lc
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0, 0, 0 }),//C
						Mach1TranscodeChannel::Coeffs({ 0.125f, 0.375f, 0, 0, 0.125f, 0.375f, 0, 0, 0, r2o2 }),//Rc
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, r2o2 }),//R
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),//Ls
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),//Rs 
					}
				},
				{
					"M1Spatial", "7.0_C_3D",
					//1  2  3  4  5  6  7  8
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0, 0, 0, 0, 0 }),				// -> TFL
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0, 0, 0 }),				// -> TFR
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0 }),	// -> CF
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, r2o2, 0, 0, 0, 0 }),		// -> TBC
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, r2o2, r2o2, 0, 0 }),		// -> BFC
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 1, 0 }),				// -> BFL
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 1 }),				// -> BFR
					}
				},
				{
					"M1SpatialS", "7.0_C_3D",
					//1  2  3  4  5  6  7  8  9  10
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0, 0, 0, 0, 0, 1, 0 }),				// -> TFL
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0, 0, 0, 0, 1 }),				// -> TFR
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0, 0, 0 }),	// -> CF
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, r2o2, 0, 0, 0, 0, 0, 0 }),			// -> TBC
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, r2o2, r2o2, 0, 0, 0, 0 }),			// -> BFC
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }),				// -> BFL
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 }),				// -> BFR
					}
				},
				{
					"M1Spatial", "7.1_M",
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, 0, r2o4, 0, r2o4, 0, r2o4 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2 }),
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 }),
					}
				},
				{
					"M1SpatialS", "7.1_M",
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o2, r2o2 }),
					}
				},
				{
					"M1Spatial", "7.1_C",
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, 0, r2o4, 0, r2o4, 0, r2o4 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2 }),
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 }),
					}
				},
				{
					"M1SpatialS", "7.1_C",
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o2, r2o2 }),
					}
				},
				{
					"M1Spatial", "7.1_C_SDDS",
					//1  2  3  4  5  6  7  8
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),//L
						Mach1TranscodeChannel::Coeffs({ 0.375f, 0.125f, 0, 0, 0.375f, 0.125f, 0, 0 }),//Lc
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0 }),//C
						Mach1TranscodeChannel::Coeffs({ 0.125f, 0.375f, 0, 0, 0.125f, 0.375f, 0, 0 }),//Rc
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),//R
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0 }),//Ls
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2 }),//Rs
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 }),//LFE
					}
				},
				{
					"M1SpatialS", "7.1_C_SMDDS",
					//1  2  3  4  5  6  7  8
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0 }),//L
						Mach1TranscodeChannel::Coeffs({ 0.375f, 0.125f, 0, 0, 0.375f, 0.125f, 0, 0, r2o2, 0 }),//Lc
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0, 0, 0 }),//C
						Mach1TranscodeChannel::Coeffs({ 0.125f, 0.375f, 0, 0, 0.125f, 0.375f, 0, 0, 0, r2o2 }),//Rc
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, r2o2 }),//R
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),//Ls
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),//Rs
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o2, r2o2 }),//LFE
					}
				},
				{
					"M1Spatial", "7.0.2_M",
					//1  2  3  4  5  6  7  8
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),//L
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 }),//C
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),//R
						Mach1TranscodeChannel::Coeffs({ r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0 }),//Lss
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, 0, r2o4, 0, r2o4, 0, r2o4 }),//Rss
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0 }),//Lsr
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2 }),//Rsr
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, r2o2, 0, 0, 0, 0, 0 }),//Lts
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, r2o2, 0, 0, 0, 0 }),//Rts
					}
				},
				{
					"M1SpatialS", "7.0.2_M",
					//1  2  3  4  5  6  7  8  9  10
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1, 0 }),//L
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, 0, 0 }),//C
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1 }),//R
						Mach1TranscodeChannel::Coeffs({ r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0, 0, 0 }),//Lss
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0, 0 }),//Rss
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),//Lsr
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),//Rsr
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, r2o2, 0, 0, 0, 0, 0, 0, 0 }),//Lts
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, r2o2, 0, 0, 0, 0, 0, 0 }),//Rts
					}
				},
				{
					"M1Spatial", "7.1.2_M",
					//1  2  3  4  5  6  7  8
					{
						Mach1TranscodeChannel::Coeffs({ r2o4, 0, 0, 0, r2o2, 0, 0, 0 }),//L
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, 0, 0, 0, r2o2, 0, 0 }),//R
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),//C
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),//LFE
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),//Lss
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),//Rss
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o4, 0, 0, 0, r2o2, 0 }),//Lsr
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o4, 0, 0, 0, r2o2 }),//Rsr
						Mach1TranscodeChannel::Coeffs({ 0.0236619f, 0.0078859f, 0.0236619f, 0.0078859f, 0, 0, 0, 0 }),//Lts
						Mach1TranscodeChannel::Coeffs({ 0.0078859f, 0.0236619f, 0.0078859f, 0.0236619f, 0, 0, 0, 0 }),//Rts 
					}
				},
				{
					"M1SpatialS", "7.1.2_M",
					//1  2  3  4  5  6  7  8  9  10
					{
						Mach1TranscodeChannel::Coeffs({ r2o4, 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0 }),//L
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, 0, 0, 0, r2o2, 0, 0, 0, r2o2 }),//R
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),//C
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),//LFE
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),//Lss
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),//Rss
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o4, 0, 0, 0, r2o2, 0, 0, 0 }),//Lsr
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o4, 0, 0, 0, r2o2, 0 ,0 }),//Rsr
						Mach1TranscodeChannel::Coeffs({ 0.0236619f, 0.0078859f, 0.0236619f, 0.0078859f, 0, 0, 0, 0, 0, 0 }),//Lts
						Mach1TranscodeChannel::Coeffs({ 0.0078859f, 0.0236619f, 0.0078859f, 0.0236619f, 0, 0, 0, 0, 0, 0 }),//Rts
					}
				},
				{
					"M1Spatial", "7.1.2_C",
					//1  2  3  4  5  6  7  8
					{
						Mach1TranscodeChannel::Coeffs({ r2o4, 0, 0, 0, r2o2, 0, 0, 0 }),//L
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, 0, 0, 0, r2o2, 0, 0 }),//R
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0 }),//C
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),//LFE
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),//Lss
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),//Rss
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o4, 0, 0, 0, r2o2, 0 }),//Lsr
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o4, 0, 0, 0, r2o2 }),//Rsr
						Mach1TranscodeChannel::Coeffs({ 0.0236619f, 0.0078859f, 0.0236619f, 0.0078859f, 0, 0, 0, 0 }),//Lts
						Mach1TranscodeChannel::Coeffs({ 0.0078859f, 0.0236619f, 0.0078859f, 0.0236619f, 0, 0, 0, 0 }),//Rts 
					}
				},
				{
					"M1SpatialS", "7.1.2_C",
					//1  2  3  4  5  6  7  8  9  10
					{
						Mach1TranscodeChannel::Coeffs({ r2o4, 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0 }),//L
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, 0, 0, 0, r2o2, 0, 0, 0, r2o2 }),//R
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, r2o2, r2o2 }),//C
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),//LFE
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),//Lss
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),//Rss
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o4, 0, 0, 0, r2o2, 0, 0, 0 }),//Lsr
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o4, 0, 0, 0, r2o2, 0 ,0 }),//Rsr
						Mach1TranscodeChannel::Coeffs({ 0.0236619f, 0.0078859f, 0.0236619f, 0.0078859f, 0, 0, 0, 0, 0, 0 }),//Lts
						Mach1TranscodeChannel::Coeffs({ 0.0078859f, 0.0236619f, 0.0078859f, 0.0236619f, 0, 0, 0, 0, 0, 0 }),//Rts
					}
				},
				{
					"M1Spatial", "7.0.4_M",
					//1  2  3  4  5  6  7  8
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),//L
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 }),//C
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),//R
						Mach1TranscodeChannel::Coeffs({ r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0 }),//Lss
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, 0, r2o4, 0, r2o4, 0, r2o4 }),//Rss
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0 }),//Lsr
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2 }),//Rsr
						Mach1TranscodeChannel::Coeffs({ 0.7280f, 0.1250f, 0.1250f, 0.0210f, 0, 0, 0, 0 }),//FLts
						Mach1TranscodeChannel::Coeffs({ 0.1250f, 0.7280f, 0.0210f, 0.1250f, 0, 0, 0, 0 }),//FRts
						Mach1TranscodeChannel::Coeffs({ 0.1250f, 0.0210f, 0.7280f, 0.1250f, 0, 0, 0, 0 }),//BLts
						Mach1TranscodeChannel::Coeffs({ 0.0210f, 0.1250f, 0.1250f, 0.7280f, 0, 0, 0, 0 }),//BRts
					}
				},
				{
					"M1SpatialS", "7.0.4_M",
					//1  2  3  4  5  6  7  8
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1, 0 }),//L
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, 0, 0 }),//C
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1 }),//R
						Mach1TranscodeChannel::Coeffs({ r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0, 0, 0 }),//Lss
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0, 0 }),//Rss
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),//Lsr
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),//Rsr
						Mach1TranscodeChannel::Coeffs({ 0.7280f, 0.1250f, 0.1250f, 0.0210f, 0, 0, 0, 0, 0, 0 }),//FLts
						Mach1TranscodeChannel::Coeffs({ 0.1250f, 0.7280f, 0.0210f, 0.1250f, 0, 0, 0, 0, 0, 0 }),//FRts
						Mach1TranscodeChannel::Coeffs({ 0.1250f, 0.0210f, 0.7280f, 0.1250f, 0, 0, 0, 0, 0, 0 }),//BLts
						Mach1TranscodeChannel::Coeffs({ 0.0210f, 0.1250f, 0.1250f, 0.7280f, 0, 0, 0, 0, 0, 0 }),//BRts
					}
				},
				{
					"M1Spatial", "7.1.4_M",
					//1  2  3  4  5  6  7  8
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),//L
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 }),//C
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),//R
						Mach1TranscodeChannel::Coeffs({ r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0 }),//Lss
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, 0, r2o4, 0, r2o4, 0, r2o4 }),//Rss
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0 }),//Lsr
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2 }),//Rsr
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 }),//LFE
						Mach1TranscodeChannel::Coeffs({ 0.7280f, 0.1250f, 0.1250f, 0.0210f, 0, 0, 0, 0 }),//FLts
						Mach1TranscodeChannel::Coeffs({ 0.1250f, 0.7280f, 0.0210f, 0.1250f, 0, 0, 0, 0 }),//FRts
						Mach1TranscodeChannel::Coeffs({ 0.1250f, 0.0210f, 0.7280f, 0.1250f, 0, 0, 0, 0 }),//BLts
						Mach1TranscodeChannel::Coeffs({ 0.0210f, 0.1250f, 0.1250f, 0.7280f, 0, 0, 0, 0 }),//BRts
					}
				},
				{
					"M1SpatialS", "7.1.4_M",
					//1  2  3  4  5  6  7  8
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1, 0 }),//L
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, 0, 0 }),//C
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1 }),//R
						Mach1TranscodeChannel::Coeffs({ r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0, 0, 0 }),//Lss
						Mach1TranscodeChannel::Coeffs({ 0, r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0, 0 }),//Rss
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),//Lsr
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),//Rsr
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o2, r2o2 }),//LFE
						Mach1TranscodeChannel::Coeffs({ 0.7280f, 0.1250f, 0.1250f, 0.0210f, 0, 0, 0, 0, 0, 0 }),//FLts
						Mach1TranscodeChannel::Coeffs({ 0.1250f, 0.7280f, 0.0210f, 0.1250f, 0, 0, 0, 0, 0, 0 }),//FRts
						Mach1TranscodeChannel::Coeffs({ 0.1250f, 0.0210f, 0.7280f, 0.1250f, 0, 0, 0, 0, 0, 0 }),//BLts
						Mach1TranscodeChannel::Coeffs({ 0.0210f, 0.1250f, 0.1250f, 0.7280f, 0, 0, 0, 0, 0, 0 }),//BRts
					}
				},
				// --- Other Surround Based Multichannel Configurations --- //
				// -> M1Horizon & M1Spatial
				{
					"1.0", "M1Spatial",
					{
						Mach1TranscodeChannel::Coeffs({ r2o8 }),
						Mach1TranscodeChannel::Coeffs({ r2o8 }),
						Mach1TranscodeChannel::Coeffs({ r2o8 }),
						Mach1TranscodeChannel::Coeffs({ r2o8 }),
						Mach1TranscodeChannel::Coeffs({ r2o8 }),
						Mach1TranscodeChannel::Coeffs({ r2o8 }),
						Mach1TranscodeChannel::Coeffs({ r2o8 }),
						Mach1TranscodeChannel::Coeffs({ r2o8 }),
					}
				},
				{
					"2.0_M", "M1Spatial",
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2 }),
						Mach1TranscodeChannel::Coeffs({ r2o2, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2 }),
						Mach1TranscodeChannel::Coeffs({ r2o2, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2 }),
						Mach1TranscodeChannel::Coeffs({ r2o2, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2 }),
					}
				},
				{
					"2.0_C", "M1Spatial",
					//fix ratio between front and back
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2 }),
						Mach1TranscodeChannel::Coeffs({ r2o20, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20 }),
						Mach1TranscodeChannel::Coeffs({ r2o2, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o2 }),
						Mach1TranscodeChannel::Coeffs({ r2o20, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o20 }),
					}
				},
				{
					"3.0_LCR", "M1Spatial",
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o8, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, r2o2 }),
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, r2o8 }),
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o8, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, r2o2 }),
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, r2o8 }),
					}
				},
		/*
				{
					"9.0_M", "M1Spatial",
					// TODO: Determine channel order
					{
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
					}
				},
				{
					"9.1_M", "M1Spatial",
					// TODO: Determine channel order
					{
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
					}
				},
		*/
				{
					"9.1.4_M", "M1Spatial",
					// TODO: Determine channel order
					{
						Mach1TranscodeChannel::Coeffs({ }),
						Mach1TranscodeChannel::Coeffs({ }),
						Mach1TranscodeChannel::Coeffs({ }),
						Mach1TranscodeChannel::Coeffs({ }),
						Mach1TranscodeChannel::Coeffs({ }),
						Mach1TranscodeChannel::Coeffs({ }),
						Mach1TranscodeChannel::Coeffs({ }),
						Mach1TranscodeChannel::Coeffs({ }),
					}
				},
				// -> Other Surround
				{
					"M1Spatial", "1.0",
					{
						Mach1TranscodeChannel::Coeffs({ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 }),
					}
				},
				{
					"M1Spatial", "2.0_M",
					{
						Mach1TranscodeChannel::Coeffs({ r2o8, 0, r2o8, 0, r2o8, 0, r2o8, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, 0, r2o8, 0, r2o8, 0, r2o8 }),
					}
				},
				{
					"M1SpatialS", "2.0_M",
					{
						Mach1TranscodeChannel::Coeffs({ r2o8, 0, r2o8, 0, r2o8, 0, r2o8, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, 0, r2o8, 0, r2o8, 0, r2o8, 0, 1 }),
					}
				},
				{
					"M1Spatial", "2.0_C",
					{
						Mach1TranscodeChannel::Coeffs({ r2o8, 0, r2o8, 0, r2o8, 0, r2o8, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, 0, r2o8, 0, r2o8, 0, r2o8 }),
					}
				},
				{
					"M1SpatialS", "2.0_C",
					{
						Mach1TranscodeChannel::Coeffs({ r2o8, 0, r2o8, 0, r2o8, 0, r2o8, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, r2o8, 0, r2o8, 0, r2o8, 0, r2o8, 0, 1 }),
					}
				},
		/*
				{
					"M1Spatial", "9.0_M",
					// TODO: Determine channel order
					//1  2  3  4  5  6  7  8
					{
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),
					}
				},
				{
					"M1SpatialS", "9.0_M",
					// TODO: Determine channel order
					//1  2  3  4  5  6  7  8  9  10
					{
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
					}
				},
				{
					"M1Spatial", "9.1_M",
					// TODO: Determine channel order
					//1  2  3  4  5  6  7  8
					{
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),
					}
				},
				{
					"M1SpatialS", "9.1_M",
					// TODO: Determine channel order
					//1  2  3  4  5  6  7  8  9  10
					{
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
					}
				},
				{
					"M1Spatial", "9.1.4_M",
					// TODO: Determine channel order
						//1  2  3  4  5  6  7  8
					{
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),
					}
				},
				{
					"M1SpatialS", "9.1.4_M",
					// TODO: Determine channel order
						//1  2  3  4  5  6  7  8 SL SR
					{
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
					}
				},
		*/
				{
					"M1Horizon", "Octahedron_M",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0 }),// -> FL
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0 }),// -> FR
						Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0 }),// -> BL
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1 }),// -> BR
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, r2o4, r2o4 }),// -> Above
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, r2o4, r2o4 }),// -> Below
					}
				},
				{
					"M1Spatial", "Octahedron_M",
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),// -> FL
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),// -> FR
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, 0, 0, 0, r2o2, 0 }),// -> BL
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, r2o2, 0, 0, 0, r2o2 }),// -> BR
						Mach1TranscodeChannel::Coeffs({ r2o4, r2o4, r2o4, r2o4, 0, 0, 0, 0 }),// -> Above
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, r2o4, r2o4, r2o4, r2o4 }),// -> Below
					}
				},
				{
					"M1Spatial", "16.0_M",
					{
						Mach1TranscodeChannel::Coeffs({ oor8, 0, 0, 0, oor8, 0, 0, 0 }),//mid -45
						Mach1TranscodeChannel::Coeffs({ oor16, oor16, 0, 0, oor16, oor16, 0, 0 }),//mid 0
						Mach1TranscodeChannel::Coeffs({ 0, oor8, 0, 0, 0, oor8, 0, 0 }),//mid 45
						Mach1TranscodeChannel::Coeffs({ 0, oor16, 0, oor16, 0, oor16, 0, oor16 }),//mid 90
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, oor8, 0, 0, 0, oor8 }),//mid 135
						Mach1TranscodeChannel::Coeffs({ 0, 0, oor16, oor16, 0, 0, oor16, oor16 }),//mid 180
						Mach1TranscodeChannel::Coeffs({ 0, 0, oor8, 0, 0, 0, oor8, 0 }),//mid -135
						Mach1TranscodeChannel::Coeffs({ oor16, 0, oor16, 0, oor16, 0, oor16, 0 }),//mid -90
						Mach1TranscodeChannel::Coeffs({ oor4, 0, 0, 0, 0, 0, 0, 0 }),//top -45
						Mach1TranscodeChannel::Coeffs({ 0, oor4, 0, 0, 0, 0, 0, 0 }),//top 45
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, oor4, 0, 0, 0, 0 }),//top 135
						Mach1TranscodeChannel::Coeffs({ 0, 0, oor4, 0, 0, 0, 0, 0 }),//top -135
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, oor4, 0, 0, 0 }),//bottom -45
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, oor4, 0, 0 }),//bottom 45
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, oor4 }),//bottom 135
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, oor4, 0 }),//bottom -135
					}
				},
				{
					"M1Spatial", "22.0_M",
					// TODO: Determine channel order
					{
						Mach1TranscodeChannel::Coeffs({ r2o2, 0, 0, 0, r2o2, 0, 0, 0 }),//L
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),//C
						Mach1TranscodeChannel::Coeffs({ 0, r2o2, 0, 0, 0, r2o2, 0, 0 }),//R
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),//Lsurround
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),//Rsurround
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),//Lside
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),//Rear
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),//Rside
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0, 0, 0, 0, 0 }),//AboveLeft
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0, 0, 0 }),//AboveRight
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),//LeftSide
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),//RightSide
						Mach1TranscodeChannel::Coeffs({ r2o2, r2o2, 0, 0, 0, 0, 0, 0 }),//AboveFront
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),//AboveLeftSurround
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),//AboveRightSurround
						Mach1TranscodeChannel::Coeffs({ 0, 0, r2o2, r2o2, 0, 0, 0, 0 }),//AboveRear
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),//AboveLeftSide
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),//Above
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0 }),//AboveRightSide
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 1, 0, 0, 0 }),//LowLeft
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 1, 0, 0 }),//LowRight
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, r2o2, r2o2, 0, 0 }),//LowCenter
					}
				},
			};						
		}
	};
};