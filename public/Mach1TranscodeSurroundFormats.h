//  Mach1 Spatial SDK
//  Copyright © 2017-2022 Mach1. All rights reserved.
//
// TODO: 
// - 5.0.6/Auro-11: FL, FC, FR, Ls, Rs, TFL, TFC, TFR, TBL, TBR, Above
// - 7.0.6/Auro-13: FL, FC, FR, Lss, Rss, Lsr, Rsr, TFL, TFC, TFR, TBL, TBR, Above
// - 8.0.2/THX-10.0: 30degL, C, 30degR, 60degL, 60degR, Ls, BC, Rs, TLs, TRs
// - 10.0.2/THX-12.0: 30degL, C, 30degR, 60degL, 60degR, Ls, BC, Rs, TLs, TRs, 90degL, 90degR
// - 22.2: https://en.wikipedia.org/wiki/22.2_surround_sound | FL, FR, FC, LFE1, BL, BR, FLc, FRc, BC, LFE2, SiL (side left), SiR, TpFL, TpFR, TpFC, TpC, TpBL, TpBR, TpSiL, TpSiR, TpBC, BtFC, BtFL, BtFR

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

 /*
  * Mach1 Spatial Best Practices: 
  * - C / S / R surround configurations should use Mach1Spatial-12 as a minimum to correctly handle the transcoding of a dedicated Center channel
  * - M or SIM surround configurations could be retained within lower Mach1Spatial-4 / Mach1Spatial-8 containers
  */

namespace Mach1TranscodeConstants {
	class MatricesSurround {
	public:
		static std::vector<Mach1TranscodeMatrix> getData() {
			return {
				{
					"2.0_M", "M1Spatial-4",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0.707106f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0.707106f }),
					}
				},
				{
					"M1Spatial-4", "2.0_M",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f }),
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f }),
					}
				},
				{
					"2.0_C", "M1Spatial-4",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0.070710f, 0, 0.707106f, 0, 0.070710f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0.070710f, 0, 0.707106f, 0, 0.070710f }),
					}
				},
				{
					"M1Spatial-4", "2.0_C",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0 }),
					}
				},
				{
					"3.0_LCR", "M1Spatial-4",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0.176776f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.353553f, 0.353553f, 0.353553f, 0.353553f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0.176776f }),
					}
				},
				{
					"5.0_M", "M1Horizon",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.353553f, 0.353553f, 0.353553f, 0.353553f }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1 }),
					}
				},
				{
					"M1Horizon", "5.0_M",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0.353553f, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.353553f, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.353553f, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.353553f, 0, 0, 1 }),
					}
				},
				{
					"5.1_M", "M1Horizon",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.353553f, 0.353553f, 0.353553f, 0.353553f }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ 0.176776f, 0.176776f, 0.176776f, 0.176776f }),
					}
				},
				{
					"M1Horizon", "5.1_M",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0.353553f, 0, 0, 0, 0.176776f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.353553f, 1, 0, 0, 0.176776f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.353553f, 0, 1, 0, 0.176776f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.353553f, 0, 0, 1, 0.176776f }),
					}
				},
				{
					"5.1_M_SMPTE", "M1Horizon",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.353553f, 0.353553f, 0.353553f, 0.353553f }),
						Mach1TranscodeChannel::Coeffs({ 0.176776f, 0.176776f, 0.176776f, 0.176776f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1 }),
					}
				},
				{
					"M1Horizon", "5.1_M_SMPTE",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0.707106f, 0.176776f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0.707106f, 0.176776f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.176776f, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.176776f, 0, 1 }),
					}
				},
				{
					"5.1_M_Dts", "M1Horizon",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ 0.353553f, 0.353553f, 0.353553f, 0.353553f }),
						Mach1TranscodeChannel::Coeffs({ 0.176776f, 0.176776f, 0.176776f, 0.176776f }),
					}
				},
				{
					"M1Horizon", "5.1_M_Dts",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0, 0.707106f, 0.176776f }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0.707106f, 0.176776f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0, 0, 0.176776f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1, 0, 0.176776f }),
					}
				},
				{
					"5.0_C_SIM", "M1Spatial-8",
					{
						Mach1TranscodeChannel::Coeffs({ 0.36125001f, 0.06375f, 0.06375f, 0.01125f, 0.36125001f, 0.06375f, 0.06375f, 0.01125f }),
						Mach1TranscodeChannel::Coeffs({ 0.213388f, 0.213388f, 0.036612f, 0.036612f, 0.213388f, 0.213388f, 0.036612f, 0.036612f }),
						Mach1TranscodeChannel::Coeffs({ 0.06375f, 0.36125001f, 0.01125f, 0.06375f, 0.06375f, 0.36125001f, 0.01125f, 0.06375f }),
						Mach1TranscodeChannel::Coeffs({ 0.06375f, 0.01125f, 0.36125001f, 0.06375f, 0.06375f, 0.01125f, 0.36125001f, 0.06375f }),
						Mach1TranscodeChannel::Coeffs({ 0.01125f, 0.06375f, 0.06375f, 0.36125001f, 0.01125f, 0.06375f, 0.06375f, 0.36125001f }),
					}
				},
				{
					"5.1_C_SIM", "M1Spatial-8",
					{
						Mach1TranscodeChannel::Coeffs({ 0.36125001f, 0.06375f, 0.06375f, 0.01125f, 0.36125001f, 0.06375f, 0.06375f, 0.01125f }),
						Mach1TranscodeChannel::Coeffs({ 0.213388f, 0.213388f, 0.036612f, 0.036612f, 0.213388f, 0.213388f, 0.036612f, 0.036612f }),
						Mach1TranscodeChannel::Coeffs({ 0.06375f, 0.36125001f, 0.01125f, 0.06375f, 0.06375f, 0.36125001f, 0.01125f, 0.06375f }),
						Mach1TranscodeChannel::Coeffs({ 0.06375f, 0.01125f, 0.36125001f, 0.06375f, 0.06375f, 0.01125f, 0.36125001f, 0.06375f }),
						Mach1TranscodeChannel::Coeffs({ 0.01125f, 0.06375f, 0.06375f, 0.36125001f, 0.01125f, 0.06375f, 0.06375f, 0.36125001f }),
						Mach1TranscodeChannel::Coeffs({ 0.176776f, 0.176776f, 0.176776f, 0.176776f, 0.176776f, 0.176776f, 0.176776f, 0.176776f }),
					}
				},
				{
					"5.0_C", "M1Spatial-12",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0 }),
					}
				},
				{
					"5.0_S", "M1Spatial-12",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.129078f, 0, 0.370922f, 0, 0.129078f, 0, 0.370922f, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.129078f, 0, 0.370922f, 0, 0.129078f, 0, 0.370922f, 0, 0, 0, 0 }),
					}
				},
				{
					"5.0_R", "M1Spatial-12",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0}),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.095012f, 0.40498799f, 0, 0, 0.095012f, 0.40498799f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.40498799f, 0.095012f, 0, 0, 0.40498799f, 0.095012f, 0, 0, 0, 0 }),
					}
				},
				{
					"5.1_C", "M1Spatial-12",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f }),
					}
				},
				{
					"M1Spatial-12", "5.1_C",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0.707106f, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0.707106f, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.117851f }),
					}
				},
				{
					"5.1_S", "M1Spatial-12",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.129078f, 0, 0.370922f, 0, 0.129078f, 0, 0.370922f, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.129078f, 0, 0.370922f, 0, 0.129078f, 0, 0.370922f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f }),
					}
				},
				{
					"M1Spatial-12", "5.1_S",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0.129078f, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0.129078f, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0.370922f, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.370922f, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0.129078f, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0.129078f, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0.370922f, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.370922f, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.117851f }),
					}
				},
				{
					"5.1_R", "M1Spatial-12",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.095012f, 0.40498799f, 0, 0, 0.095012f, 0.40498799f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.40498799f, 0.095012f, 0, 0, 0.40498799f, 0.095012f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f }),
					}
				},
				{
					"M1Spatial-12", "5.1_R",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.40498799f, 0.095012f, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.095012f, 0.40498799f, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.40498799f, 0.095012f, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.095012f, 0.40498799f, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.117851f }),
					}
				},
				{
					"5.0.2_C", "M1Spatial-14",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.375f, 0.125f, 0.375f, 0.125f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.125f, 0.375f, 0.125f, 0.375f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
					}
				},
				{
					"M1Spatial-14", "5.0.2_C",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0, 0.707106f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0.707106f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0.707106f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0.707106f, 0, 0.707106f }),
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0.707106f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.707106f, 0.707106f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0 }),
					}
				},
				{
					"5.1.2_C", "M1Spatial-14",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.101015f, 0.101015f, 0.101015f, 0.101015f, 0.101015f, 0.101015f, 0.101015f, 0.101015f, 0.101015f, 0.101015f, 0.101015f, 0.101015f, 0.101015f, 0.101015f }),
						Mach1TranscodeChannel::Coeffs({ 0.375f, 0.125f, 0.375f, 0.125f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.125f, 0.375f, 0.125f, 0.375f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
					}
				},
				{
					"M1Spatial-14", "5.1.2_C",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0, 0.117851f, 0.707106f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0.117851f, 0, 0.707106f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0.117851f, 0.707106f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0.707106f, 0.117851f, 0, 0.707106f }),
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0, 0.117851f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0.117851f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0.117851f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0.707106f, 0.117851f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0.117851f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.117851f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.117851f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.117851f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.117851f, 0.707106f, 0.707106f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.117851f, 0, 0 }),
					}
				},
				{
					"5.0.4_C", "M1Spatial-14",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.72799999f, 0.125f, 0.125f, 0.021f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.125f, 0.72799999f, 0.021f, 0.125f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.125f, 0.021f, 0.72799999f, 0.125f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.021f, 0.125f, 0.125f, 0.72799999f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
					}
				},
				{
					"M1Spatial-14", "5.0.4_C",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0, 0.72799999f, 0.125f, 0.125f, 0.021f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0.125f, 0.72799999f, 0.021f, 0.125f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0.125f, 0.021f, 0.72799999f, 0.125f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0.707106f, 0.021f, 0.125f, 0.125f, 0.72799999f }),
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0.707106f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.353553f, 0.353553f, 0.353553f, 0.353553f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
					}
				},
				{
					"5.0.5_C", "M1Spatial-14",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.353553f, 0.353553f, 0.353553f, 0.353553f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
					}
				},
				{
					"5.1.4_C", "M1Spatial-14",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.101015f, 0.101015f, 0.101015f, 0.101015f, 0.101015f, 0.101015f, 0.101015f, 0.101015f, 0.101015f, 0.101015f, 0.101015f, 0.101015f, 0.101015f, 0.101015f }),
						Mach1TranscodeChannel::Coeffs({ 0.72799999f, 0.125f, 0.125f, 0.021f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.125f, 0.72799999f, 0.021f, 0.125f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.125f, 0.021f, 0.72799999f, 0.125f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.021f, 0.125f, 0.125f, 0.72799999f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
					}
				},
				{
					"5.1.4_C_SMPTE", "M1Spatial-14",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.101015f, 0.101015f, 0.101015f, 0.101015f, 0.101015f, 0.101015f, 0.101015f, 0.101015f, 0.101015f, 0.101015f, 0.101015f, 0.101015f, 0.101015f, 0.101015f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.72799999f, 0.125f, 0.125f, 0.021f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.125f, 0.72799999f, 0.021f, 0.125f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.125f, 0.021f, 0.72799999f, 0.125f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.021f, 0.125f, 0.125f, 0.72799999f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
					}
				},
				{
					"M1Spatial-12", "5.0.4_C",
					// L, C, R, Ls, Rs, Hl, Hr, Hls, Hlr | Auro 9
					{
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0 }),
					}
				},
				{
					"M1Spatial-14", "5.0.5_C",
					// L, C, R, Ls, Rs, Hl, Hr, Hls, Hlr, TC | Auro 10
					{
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0 }),
					}
				},
				{
					"M1Spatial-14", "5.1.4_C",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0, 0.101015f, 0.72799999f, 0.125f, 0.125f, 0.021f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0.101015f, 0.125f, 0.72799999f, 0.021f, 0.125f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0.101015f, 0.125f, 0.021f, 0.72799999f, 0.125f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0.707106f, 0.101015f, 0.021f, 0.125f, 0.125f, 0.72799999f }),
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0, 0.101015f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0.101015f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0.101015f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0.707106f, 0.101015f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0.101015f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.101015f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.101015f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.101015f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.101015f, 0.353553f, 0.353553f, 0.353553f, 0.353553f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.101015f, 0, 0, 0, 0 }),
					}
				},
				{
					"6.0_M", "M1Horizon",
					// L, C, R, Ls, Cs, Rs
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.353553f, 0.353553f, 0.353553f, 0.353553f }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0.707106f }),
					}
				},
				{
					"M1Horizon", "6.0_M",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0.353553f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.353553f, 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.353553f, 0, 1, 0, 0.707106f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.353553f, 0, 0, 1, 0.707106f }),
					}
				},
				{
					"6.0_C", "M1Spatial-12",
					// L, C, R, Ls, Cs, Rs
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0 }),
					}
				},
				{
					"M1Spatial-12", "6.0_C",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.707106f }),
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.707106f }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0 }),
					}
				},
				{
					"Octahedron_M", "M1Spatial-14",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }),
					}
				},
				{
					"M1Spatial-14", "Octahedron_M",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 1 }),
					}
				},
				{
					"7.0_M", "M1Spatial-8", // TODO: Change to >M1Spatial-16
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.176776f, 0.176776f, 0.176776f, 0.176776f, 0.176776f, 0.176776f, 0.176776f, 0.176776f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.353553f, 0, 0.353553f, 0, 0.353553f, 0, 0.353553f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.353553f, 0, 0.353553f, 0, 0.353553f, 0, 0.353553f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0, 0, 0.707106f }),
					}
				},
				{
					"7.0_C", "M1Spatial-12",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0 }),
					}
				},
				{
					"M1Spatial-12", "7.0_C",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.707106f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0.707106f }),
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.707106f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0.707106f }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 1, 0, 0 }),
					}
				},
				{
					"7.0_M_SDDS", "M1Spatial-8",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.530328f, 0.176776f, 0, 0, 0.530328f, 0.176776f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.176776f, 0.176776f, 0.176776f, 0.176776f, 0.176776f, 0.176776f, 0.176776f, 0.176776f }),
						Mach1TranscodeChannel::Coeffs({ 0.176776f, 0.530328f, 0, 0, 0.176776f, 0.530328f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0, 0, 0.707106f }),
					}
				},
				{
					"7.0_C_SDDS", "M1Spatial-12",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.530328f, 0.176776f, 0, 0, 0.530328f, 0.176776f, 0, 0, 0, 0, 0, 0  }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.176776f, 0.530328f, 0, 0, 0.176776f, 0.530328f, 0, 0, 0, 0, 0, 0  }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0  }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0  }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0  }),
					}
				},
				{
					"M1Spatial-8", "7.0_C_SDDS", // TODO: Finish and fix
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0.375f, 0.353553f, 0.125f, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.125f, 0.353553f, 0.375f, 0.707106f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.707106f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0.707106f }),
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0.375f, 0.353553f, 0.125f, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.125f, 0.353553f, 0.375f, 0.707106f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.707106f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0.707106f }),
					}
				},
				{
					// HFL, HFR, CF, HR, LF, LFL, LFR
					"7.0_C_3D", "M1Spatial-12",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0.707106f, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0.707106f, 0.707106f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 }),
					}
				},
				{
					"M1Spatial-12", "7.0_C_3D",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0.707106f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0.707106f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0 }),
					}
				},
				{
					"7.0_C_SIM", "M1Spatial-8",
					{
						Mach1TranscodeChannel::Coeffs({ 0.36125001f, 0.06375f, 0.06375f, 0.01125f, 0.36125001f, 0.06375f, 0.06375f, 0.01125f }),
						Mach1TranscodeChannel::Coeffs({ 0.213388f, 0.213388f, 0.036612f, 0.036612f, 0.213388f, 0.213388f, 0.036612f, 0.036612f }),
						Mach1TranscodeChannel::Coeffs({ 0.06375f, 0.36125001f, 0.01125f, 0.06375f, 0.06375f, 0.36125001f, 0.01125f, 0.06375f }),
						Mach1TranscodeChannel::Coeffs({ 0.248744f, 0.001256f, 0.248744f, 0.001256f, 0.248744f, 0.001256f, 0.248744f, 0.001256f }),
						Mach1TranscodeChannel::Coeffs({ 0.001256f, 0.248744f, 0.001256f, 0.248744f, 0.001256f, 0.248744f, 0.001256f, 0.248744f }),
						Mach1TranscodeChannel::Coeffs({ 0.06375f, 0.01125f, 0.36125001f, 0.06375f, 0.06375f, 0.01125f, 0.36125001f, 0.06375f }),
						Mach1TranscodeChannel::Coeffs({ 0.01125f, 0.06375f, 0.06375f, 0.36125001f, 0.01125f, 0.06375f, 0.06375f, 0.36125001f }),
					}
				},
				{
					"7.1_M", "M1Spatial-12",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f }),
					}
				},
				{
					"M1Spatial-12", "7.1_M",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0.117851f, 0, 0, 0, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0.707106f, 0, 0, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0, 0, 0, 0.707106f, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0, 0, 0, 0, 0.707106f, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0.117851f, 0, 0, 0, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0.707106f, 0, 0, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0, 0, 0, 0.707106f, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0, 0, 0, 0, 0.707106f, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0, 0, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0, 1, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0, 0, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0, 0, 1, 0, 0.117851f }),
					}
				},
				{
					"7.1_C", "M1Spatial-12",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f }),
					}
				},
				{
					"M1Spatial-12", "7.1_C",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.707106f, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0.707106f, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.707106f, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0.707106f, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1, 0, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 1, 0, 0, 0.117851f }),
					}
				},
				{
					"7.1_M_SDDS", "M1Spatial-8",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.353553f, 0, 0, 0, 0.353553f, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.176776f, 0.176776f, 0.176776f, 0.176776f, 0.176776f, 0.176776f, 0.176776f, 0.176776f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.353553f, 0, 0, 0, 0.353553f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0, 0, 0.707106f }),
						Mach1TranscodeChannel::Coeffs({ 0.176776f, 0.176776f, 0.176776f, 0.176776f, 0.176776f, 0.176776f, 0.176776f, 0.176776f }),
					}
				},
				{
					"7.1_C_SDDS", "M1Spatial-12",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.530328f, 0.176776f, 0, 0, 0.530328f, 0.176776f, 0, 0, 0, 0, 0, 0  }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.176776f, 0.530328f, 0, 0, 0.176776f, 0.530328f, 0, 0, 0, 0, 0, 0  }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f }),
					}
				},
				{
					"M1Spatial-12", "7.1_C_SDDS",
					// L, Lc, C, Rc, R, Ls, Rs, LFE
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0.707106f, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.707106f, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0.707106f, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0.707106f, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.707106f, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0.707106f, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0, 0, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0.117851f }),
					}
				},
				{
					"7.1_C_SIM", "M1Spatial-8",
					// L, C, R, Lss, Rss, Lsr, Rsr, LFE, Lts, Rts
					{
						Mach1TranscodeChannel::Coeffs({ 0.36125001f, 0.06375f, 0.06375f, 0.01125f, 0.36125001f, 0.06375f, 0.06375f, 0.01125f }),
						Mach1TranscodeChannel::Coeffs({ 0.213388f, 0.213388f, 0.036612f, 0.036612f, 0.213388f, 0.213388f, 0.036612f, 0.036612f }),
						Mach1TranscodeChannel::Coeffs({ 0.06375f, 0.36125001f, 0.01125f, 0.06375f, 0.06375f, 0.36125001f, 0.01125f, 0.06375f }),
						Mach1TranscodeChannel::Coeffs({ 0.248744f, 0.001256f, 0.248744f, 0.001256f, 0.248744f, 0.001256f, 0.248744f, 0.001256f }),
						Mach1TranscodeChannel::Coeffs({ 0.001256f, 0.248744f, 0.001256f, 0.248744f, 0.001256f, 0.248744f, 0.001256f, 0.248744f }),
						Mach1TranscodeChannel::Coeffs({ 0.06375f, 0.01125f, 0.36125001f, 0.06375f, 0.06375f, 0.01125f, 0.36125001f, 0.06375f }),
						Mach1TranscodeChannel::Coeffs({ 0.01125f, 0.06375f, 0.06375f, 0.36125001f, 0.01125f, 0.06375f, 0.06375f, 0.36125001f }),
						Mach1TranscodeChannel::Coeffs({ 0.176776f, 0.176776f, 0.176776f, 0.176776f, 0.176776f, 0.176776f, 0.176776f, 0.176776f }),
					}
				},
				{
					"7.0.2_C", "M1Spatial-12",
					// L, C, R, Lss, Rss, Lsr, Rsr, Lts, Rts
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.0236619f, 0.0078859f, 0.0236619f, 0.0078859f, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.0078859f, 0.0236619f, 0.0078859f, 0.0236619f, 0, 0, 0, 0, 0, 0, 0, 0 }),
					}
				},
				{
					"M1Spatial-12", "7.0.2_M",
					// L, C, R, Lss, Rss, Lsr, Rsr, Lts, Rts
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0.117851f, 0, 0, 0, 0, 0, 0.707106f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0.707106f, 0, 0, 0, 0, 0, 0.707106f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0, 0, 0, 0.707106f, 0, 0.707106f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0, 0, 0, 0, 0.707106f, 0, 0.707106f }),
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0.117851f, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0.707106f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0, 0, 0, 0.707106f, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0, 0, 0, 0, 0.707106f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0, 0, 1, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0, 1, 0, 0, 0, 0, 0 }),
					}
				},
				{
					"7.1.2_M", "M1Spatial-12",
					// L, C, R, Lss, Rss, Lsr, Rsr, LFE, Lts, Rts
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0.0236619f, 0.0078859f, 0.0236619f, 0.0078859f, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.0078859f, 0.0236619f, 0.0078859f, 0.0236619f, 0, 0, 0, 0, 0, 0, 0, 0 }),
					}
				},
				{
					"M1Spatial-8", "7.1.2_M",
					// L, C, R, Lss, Rss, Lsr, Rsr, LFE, Lts, Rts
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0.117851f, 0, 0, 0, 0, 0, 0.117851f, 0.0236619f, 0.0078859f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0.707106f, 0, 0, 0, 0, 0.117851f, 0.0078859f, 0.0236619f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0, 0, 0, 0.707106f, 0, 0.117851f, 0.0236619f, 0.0078859f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0, 0, 0, 0, 0.707106f, 0.117851f, 0.0078859f, 0.0236619f }),
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0.117851f, 0, 0, 0, 0, 0, 0.117851f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0.707106f, 0, 0, 0, 0, 0.117851f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0, 0, 0, 0.707106f, 0, 0.117851f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0, 0, 0, 0, 0.707106f, 0.117851f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0, 0, 0, 0, 0, 0.117851f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0, 0, 1, 0, 0, 0.117851f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0, 0, 0, 0, 0, 0.117851f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0, 1, 0, 0, 0, 0.117851f, 0, 0 }),
					}
				},
				{
					"7.1.2_C", "M1Spatial-12",
					// L, C, R, Lss, Rss, Lsr, Rsr, LFE, Lts, Rts
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.0236619f, 0.0078859f, 0.0236619f, 0.0078859f, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.0078859f, 0.0236619f, 0.0078859f, 0.0236619f, 0, 0, 0, 0, 0, 0, 0, 0 }),
					}
				},
				{
					"M1Spatial-12", "7.1.2_C",
					// L, C, R, Lss, Rss, Lsr, Rsr, LFE, Lts, Rts
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0, 0, 0, 0.117851f, 0.0236619f, 0.0078859f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0, 0.117851f, 0.0078859f, 0.0236619f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.707106f, 0, 0.117851f, 0.0236619f, 0.0078859f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0.707106f, 0.117851f, 0.0078859f, 0.0236619f }),
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0, 0, 0, 0.117851f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0, 0.117851f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.707106f, 0, 0.117851f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0.707106f, 0.117851f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0, 0, 0.117851f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 1, 0, 0, 0.117851f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0.117851f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1, 0, 0, 0, 0.117851f, 0, 0 }),
					}
				},
				{
					"M1Spatial-12", "7.0.4_M",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0.117851f, 0, 0, 0, 0, 0, 0.72799999f, 0.125f, 0.125f, 0.021f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0.707106f, 0, 0, 0, 0, 0.125f, 0.72799999f, 0.021f, 0.125f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0, 0, 0, 0.707106f, 0, 0.125f, 0.021f, 0.72799999f, 0.125f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0, 0, 0, 0, 0.707106f, 0.021f, 0.125f, 0.125f, 0.72799999f }),
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0.176776f, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0.707106f, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0, 0, 0, 0, 0.707106f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0, 0, 1, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.117851f, 0, 1, 0, 0, 0, 0, 0, 0 }),
					}
				},
				{
					"7.0.4_C", "M1Spatial-12",
					// L, C, R, Lss, Rss, Lsr, Rsr, Ltf, Rtf, Ltr, Rtr
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.72799999f, 0.125f, 0.125f, 0.021f, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.125f, 0.72799999f, 0.021f, 0.125f, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.125f, 0.021f, 0.72799999f, 0.125f, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.021f, 0.125f, 0.125f, 0.72799999f, 0, 0, 0, 0, 0, 0, 0, 0 }),
					}
				},
				{
					"M1Spatial-12", "7.0.4_C",
					//L, C, R, Lss, Rss, Lsr, Rsr, FLts, FRts, BLts, BRts
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0, 0, 0, 0.7280f, 0.1250f, 0.1250f, 0.0210f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0, 0.1250f, 0.7280f, 0.0210f, 0.1250f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.707106f, 0, 0.1250f, 0.0210f, 0.7280f, 0.1250f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0.707106f, 0.0210f, 0.1250f, 0.1250f, 0.7280f }),
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0.707106f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 }),
					}
				},
				{
					"M1Spatial-12", "7.1.4_M",
					// L, C, R, Lss, Rss, Lsr, Rsr, LFE, Ltf, Rtf, Ltr, Rtr
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0.176776f, 0, 0, 0, 0, 0, 0.176776f, 0.72799999f, 0.125f, 0.125f, 0.021f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.176776f, 0.707106f, 0, 0, 0, 0, 0.176776f, 0.125f, 0.72799999f, 0.021f, 0.125f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.176776f, 0, 0, 0, 0.707106f, 0, 0.176776f, 0.125f, 0.021f, 0.72799999f, 0.125f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.176776f, 0, 0, 0, 0, 0.707106f, 0.176776f, 0.021f, 0.125f, 0.125f, 0.72799999f }),
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0.176776f, 0, 0, 0, 0, 0, 0.176776f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.176776f, 0.707106f, 0, 0, 0, 0, 0.176776f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.176776f, 0, 0, 0, 0.707106f, 0, 0.176776f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.176776f, 0, 0, 0, 0, 0.707106f, 0.176776f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0.176776f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 1, 0, 0, 0.176776f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0.176776f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1, 0, 0, 0, 0.176776f, 0, 0, 0, 0 }),
					}
				},
				{
					"7.1.4_C", "M1Spatial-12",
					// L, C, R, Lss, Rss, Lsr, Rsr, LFE, Ltf, Rtf, Ltr, Rtr
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f, 0.117851f }),
						Mach1TranscodeChannel::Coeffs({ 0.72799999f, 0.125f, 0.125f, 0.021f, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.125f, 0.72799999f, 0.021f, 0.125f, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.125f, 0.021f, 0.72799999f, 0.125f, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.021f, 0.125f, 0.125f, 0.72799999f, 0, 0, 0, 0, 0, 0, 0, 0 }),
					}
				},
				{
					"M1Spatial-12", "7.1.4_C",
					//L, C, R, Lss, Rss, Lsr, Rsr, LFE, FLts, FRts, BLts, BRts
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0, 0, 0, 0.117851f, 0.7280f, 0.1250f, 0.1250f, 0.0210f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0, 0.117851f, 0.1250f, 0.7280f, 0.0210f, 0.1250f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.707106f, 0, 0.117851f, 0.1250f, 0.0210f, 0.7280f, 0.1250f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0.707106f, 0.117851f, 0.0210f, 0.1250f, 0.1250f, 0.7280f }),
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0, 0, 0, 0.117851f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0, 0.117851f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.707106f, 0, 0.117851f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0.707106f, 0.117851f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0, 0, 0, 0.117851f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 1, 0, 0, 0, 0.117851f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0.117851f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1, 0, 0, 0, 0, 0.117851f, 0, 0, 0, 0 }),
					}
				},
				{
					"M1Spatial-8", "16.0_M",
					{
						Mach1TranscodeChannel::Coeffs({ 0.353553f, 0.250000f, 0, 0, 0, 0, 0, 0.250000f, 0.500000f, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.250000f, 0.353553f, 0.250000f, 0, 0, 0, 0, 0, 0.500000f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.250000f, 0.353553f, 0.250000f, 0, 0, 0, 0.500000f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.250000f, 0.353553f, 0.250000f, 0, 0, 0, 0, 0.500000f, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.353553f, 0.250000f, 0, 0, 0, 0, 0, 0.250000f, 0, 0, 0, 0, 0.500000f, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.250000f, 0.353553f, 0.250000f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.500000f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0.250000f, 0.353553f, 0.250000f, 0, 0, 0, 0, 0, 0, 0, 0.500000f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.250000f, 0.353553f, 0.250000f, 0, 0, 0, 0, 0, 0, 0, 0, 0.500000f, 0 }),
					}
				},
				{
					"M1Spatial-8", "22.0_M",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0.707106f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0.707106f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
					}
				},
			};
		}
	};
};