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

 /*
  * Naming Conventions:
  * - `_2`		= Indicates an additional 2 channels of static or non-diegetic audio, or in this case stereo
  * - `Pairs`	= Indicates the spatial audio files are already doubled in sets of binauralized stereo files
  * - `_SIM`	= Indicates the soundfield is shrunken to a spherical projection to undiverge the input sources 
  * 			to make use of all the output channels and simulating a more realistic transcode
  */

namespace Mach1TranscodeConstants {
	class MatricesVector {
	public:
		static std::vector<Mach1TranscodeMatrix> getData() {
			return {
				{
					"M1Spatial-4", "M1Spatial-4", // AKA M1Horizon
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1 }),
					}
				},
				{
					"M1Spatial-4", "M1HorizonPairs", // AKA Quad-Binaural
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0, 0, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 1, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 1, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1, 1, 0, 0, 0 }),
					}
				},
				{
					"M1HorizonPairs", "M1Spatial-4", // AKA Quad-Binaural
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0 }),
					}
				},
				{
					"M1Spatial-4", "M1Spatial-4_2",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1, 0, 0 }),
					}
				},
				{
					"M1Spatial-4_2", "M1Spatial-4",
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
					"M1Spatial-4", "M1Spatial-8",
					{
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0, 0, 0.707106f, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.707106f, 0, 0, 0, 0.707106f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.707106f, 0, 0, 0, 0.707106f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0.707106f, 0, 0, 0, 0.707106f }),
					}
				},
				{
					"M1Spatial-8", "M1Spatial-4",
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
					"M1Spatial-8", "M1Spatial-8",
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
					"M1Spatial-6", "M1Spatial-8",
					{
						Mach1TranscodeChannel::Coeffs({ 0.353553f, 0.353553f, 0, 0, 0.353553f, 0.353553f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.353553f, 0, 0.353553f, 0, 0.353553f, 0, 0.353553f, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.353553f, 0.353553f, 0, 0, 0.353553f, 0.353553f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.353553f, 0, 0.353553f, 0, 0.353553f, 0, 0.353553f }),
						Mach1TranscodeChannel::Coeffs({ 0.353553f, 0.353553f, 0.353553f, 0.353553f, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0.353553f, 0.353553f, 0.353553f, 0.353553f }),
					}
				},
				{
					"M1Spatial-8", "M1Spatial-6",
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
					"M1Spatial-8", "M1Spatial-8_2",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 }),
					}
				},
				{
					"M1Spatial-8_2", "M1Spatial-8",
					// This downfolds headlocked stereo to headlocked mono
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
				{	// Implemented with 8 input channels directly outputting to the 32 channels without signal distribution included
					"M1Spatial-8", "M1Spatial-12",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 }),
					}
				},
				{
					"M1Spatial-12", "M1Spatial-8",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ 0.353553f, 0.353553f, 0, 0, 0.353553f, 0.353553f, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0.353553f, 0, 0.353553f, 0, 0.353553f, 0, 0.353553f }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0.353553f, 0.353553f, 0, 0, 0.353553f, 0.353553f }),
						Mach1TranscodeChannel::Coeffs({ 0.353553f, 0, 0.353553f, 0, 0.353553f, 0, 0.353553f, 0 }),
					}
				},
				{
					"M1Spatial-12", "M1Spatial-14",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 }),
					}
				},
				{
					"M1Spatial-14", "M1Spatial-12",
					{
						Mach1TranscodeChannel::Coeffs({ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }),
						Mach1TranscodeChannel::Coeffs({ 0.353553f, 0.353553f, 0.353553f, 0.353553f, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0.353553f, 0.353553f, 0.353553f, 0.353553f, 0, 0, 0, 0 }),
					}
				},
				{	// Implemented with 8 input channels directly outputting to the 32 channels without signal distribution included
					"M1Spatial-14", "M1Spatial-32",
					{
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0.707106f, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.707106f, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.707106f, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.707106f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.707106f }),
					}
				},
				{
					"M1Spatial-32", "M1Spatial-14", //TODO
					{
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
						Mach1TranscodeChannel::Coeffs({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
					}
				},
			};
		}
	};
};
