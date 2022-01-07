//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.
//
// References: 
// - York ambisonics: https://www.york.ac.uk/sadie-project/ambidec.html

#pragma once

#ifndef Mach1TranscodeFormats_h
#define Mach1TranscodeFormats_h

#include <math.h>
#include <stddef.h>
#include <vector>
#include <map>

namespace Mach1TranscodeFormats {
	enum FormatType {
		Empty = (int)0,
		FuMa,
		ACNSN3D,
		M1Horizon,
		M1HorizonS,
		M1HorizonPairs,
		M1Spatial,
		M1SpatialS,
		M1SpatialPairs,
		M1SpatialPlus,
		M1SpatialPlusPlus,
		M1SpatialExtended,
		M1SpatialExtendedPlus,
		Mono,
		Stereo,
		LCR,
		FiveOh,
		FiveOneFilm,
		FiveOneFilm_Cinema,
		FiveOneSmpte,
		FiveOneDts,
		SixOh,
		SevenOnePt,
		SevenOnePt_Cinema,
		ACNSN3DO2A,
		FuMaO2A,
		TBE,
		ACNSN3DO3A,
		FuMaO3A,
		SevenOneSDDS,
		SevenZeroSDDS,
		SevenOneTwo,
		SevenZeroTwo,
		NineOne,
		NineZero,
		Stereo_Cinema,
		FiveOneTwo,
		FiveZeroTwo,
		FiveOneFour,
		FiveZeroFour,
		SevenOneFour,
		SevenZeroFour,
		SevenZero_Cinema,
		M1SpatialFaces,
		NineOneFour,
		NineOneSix,
		MarcoSixteen,
		CustomPoints,
		ACNSN3DmaxRE1oa,
		ACNSN3DmaxRE2oa,
		ACNSN3DmaxRE3oa,
		ACNSN3DmaxRE4oa,
		ACNSN3DmaxRE5oa,
		ACNSN3DmaxRE6oa,
		ACNSN3DmaxRE7oa,
		DolbyAtmosSevenOneTwo,
		FiveOneFourSMPTE
	};
};

class Mach1SpatialSoundMatrix {
	std::vector<std::vector<float>> data;

public:
	Mach1SpatialSoundMatrix()
	{

	}

	Mach1SpatialSoundMatrix(std::vector<std::vector<float>> data)
	{
		this->data = data;
	}

	std::vector<std::vector<float>>& getData()
	{
		return data;
	}
};

struct Mach1TranscodeMatrix {
	Mach1TranscodeFormats::FormatType formatFrom;
	Mach1TranscodeFormats::FormatType formatTo;
	Mach1SpatialSoundMatrix matrix;
};

namespace Mach1TranscodeConstants {
	const float r2 = (float)sqrt(2.0);
	const float r3 = (float)sqrt(3.0);
	const float oor2 = (float)(1.0 / sqrt(2.0));
	const float oor3 = (float)(1.0 / sqrt(3.0));
	const float oor4 = (float)(1.0 / sqrt(4.0));
	const float oor8 = (float)(1.0 / sqrt(8.0));
	const float oor16 = (float)(1.0 / sqrt(16.0));
	const float oo2r2 = (float)(1.0 / (2.0 * sqrt(2.0)));
	const float r3or2 = (float)(sqrt(3.0) / sqrt(2.0));
	const float r3o2 = (float)sqrt(3.0) / 2.0f;
	const float r2o2 = (float)sqrt(2.0) / 2.0f;
	const float r2o3 = (float)sqrt(2.0) / 3.0f;
	const float r2o4 = (float)sqrt(2.0) / 4.0f;
	const float r2o6 = (float)sqrt(2.0) / 6.0f;
	const float r2o8 = (float)sqrt(2.0) / 8.0f;
	const float r2o12 = (float)sqrt(2.0) / 12.0f;
	const float r2o20 = (float)sqrt(2.0) / 20.0f;
	const float oo8 = (float)1.0f / 8.0f;

	const std::map<Mach1TranscodeFormats::FormatType, const char*> FormatNames = {
	{ Mach1TranscodeFormats::Empty, "None" },
	{ Mach1TranscodeFormats::FuMa, "FuMa" },
	{ Mach1TranscodeFormats::ACNSN3D, "ACNSN3D" },
	{ Mach1TranscodeFormats::M1Horizon, "M1Horizon" },
	{ Mach1TranscodeFormats::M1HorizonS, "M1Horizon+S" },
	{ Mach1TranscodeFormats::M1HorizonPairs, "M1HorizonPairs" },
	{ Mach1TranscodeFormats::M1Spatial, "M1Spatial" },
	{ Mach1TranscodeFormats::M1SpatialS, "M1Spatial+S" },
	{ Mach1TranscodeFormats::M1SpatialPairs, "M1SpatialPairs" },
	{ Mach1TranscodeFormats::M1SpatialPlus, "M1SpatialPlus" },
	{ Mach1TranscodeFormats::M1SpatialPlusPlus, "M1SpatialPlusPlus" },
	{ Mach1TranscodeFormats::M1SpatialExtended, "M1SpatialExtended" },
	{ Mach1TranscodeFormats::M1SpatialExtendedPlus, "M1SpatialExtendedPlus" },
	{ Mach1TranscodeFormats::Mono, "Mono" },
	{ Mach1TranscodeFormats::Stereo, "Stereo" },
	{ Mach1TranscodeFormats::LCR, "LCR" },
	{ Mach1TranscodeFormats::FiveOh, "FiveOh" },
	{ Mach1TranscodeFormats::FiveOneFilm, "FiveOneFilm" },
	{ Mach1TranscodeFormats::FiveOneFilm_Cinema, "FiveOneFilm_Cinema" },
	{ Mach1TranscodeFormats::FiveOneSmpte, "FiveOneSmpte" },
	{ Mach1TranscodeFormats::FiveOneDts, "FiveOneDts" },
	{ Mach1TranscodeFormats::SixOh, "SixOh" },
	{ Mach1TranscodeFormats::SevenOnePt, "SevenOnePt" },
	{ Mach1TranscodeFormats::SevenOnePt_Cinema, "SevenOnePt_Cinema" },
	{ Mach1TranscodeFormats::ACNSN3DO2A, "ACNSN3DO2A" },
	{ Mach1TranscodeFormats::FuMaO2A, "FuMaO2A" },
	{ Mach1TranscodeFormats::TBE, "TBE" },
	{ Mach1TranscodeFormats::ACNSN3DO3A, "ACNSN3DO3A" },
	{ Mach1TranscodeFormats::FuMaO3A, "FuMaO3A" },
	{ Mach1TranscodeFormats::SevenOneSDDS, "SevenOneSDDS" },
	{ Mach1TranscodeFormats::SevenZeroSDDS, "SevenZeroSDDS" },
	{ Mach1TranscodeFormats::SevenOneTwo, "SevenOneTwo" },
	{ Mach1TranscodeFormats::SevenZeroTwo, "SevenZeroTwo" },
	{ Mach1TranscodeFormats::NineOne, "NineOne" },
	{ Mach1TranscodeFormats::NineZero, "NineZero" },
	{ Mach1TranscodeFormats::Stereo_Cinema, "Stereo_Cinema" },
	{ Mach1TranscodeFormats::FiveOneTwo, "FiveOneTwo" },
	{ Mach1TranscodeFormats::FiveZeroTwo, "FiveZeroTwo" },
	{ Mach1TranscodeFormats::FiveOneFour, "FiveOneFour" },
	{ Mach1TranscodeFormats::FiveOneFourSMPTE, "FiveOneFourSMPTE" },
	{ Mach1TranscodeFormats::FiveZeroFour, "FiveZeroFour" },
	{ Mach1TranscodeFormats::SevenOneFour, "SevenOneFour" },
	{ Mach1TranscodeFormats::SevenZeroFour, "SevenZeroFour" },
	{ Mach1TranscodeFormats::SevenZero_Cinema, "SevenZero_Cinema" },
	{ Mach1TranscodeFormats::M1SpatialFaces, "M1SpatialFaces" },
	{ Mach1TranscodeFormats::NineOneFour, "NineOneFour" },
	{ Mach1TranscodeFormats::NineOneSix, "NineOneSix" },
	{ Mach1TranscodeFormats::MarcoSixteen, "MarcoSixteen" },
	{ Mach1TranscodeFormats::CustomPoints, "CustomPoints" },
	{ Mach1TranscodeFormats::ACNSN3DmaxRE1oa, "ACNSN3DmaxRE1oa" },
	{ Mach1TranscodeFormats::ACNSN3DmaxRE2oa, "ACNSN3DmaxRE2oa" },
	{ Mach1TranscodeFormats::ACNSN3DmaxRE3oa, "ACNSN3DmaxRE3oa" },
	{ Mach1TranscodeFormats::ACNSN3DmaxRE4oa, "ACNSN3DmaxRE4oa" },
	{ Mach1TranscodeFormats::ACNSN3DmaxRE5oa, "ACNSN3DmaxRE5oa" },
	{ Mach1TranscodeFormats::ACNSN3DmaxRE6oa, "ACNSN3DmaxRE6oa" },
	{ Mach1TranscodeFormats::ACNSN3DmaxRE7oa, "ACNSN3DmaxRE7oa" },
	{ Mach1TranscodeFormats::DolbyAtmosSevenOneTwo, "DolbyAtmosSevenOneTwo" },
	};

	const std::map<Mach1TranscodeFormats::FormatType, int> FormatChannels = {
		{ Mach1TranscodeFormats::Empty, 0 },
		{ Mach1TranscodeFormats::FuMa, 4 },
		{ Mach1TranscodeFormats::ACNSN3D, 4 },
		{ Mach1TranscodeFormats::M1Horizon, 4 },
		{ Mach1TranscodeFormats::M1HorizonS, 6 },
		{ Mach1TranscodeFormats::M1HorizonPairs, 8 },
		{ Mach1TranscodeFormats::M1Spatial, 8 },
		{ Mach1TranscodeFormats::M1SpatialS, 10 },
		{ Mach1TranscodeFormats::M1SpatialPairs, 16 },
		{ Mach1TranscodeFormats::M1SpatialPlus, 12 },
		{ Mach1TranscodeFormats::M1SpatialPlusPlus, 14 },
		{ Mach1TranscodeFormats::M1SpatialExtended, 16 },
		{ Mach1TranscodeFormats::M1SpatialExtendedPlus, 18 },
		{ Mach1TranscodeFormats::Mono, 1 },
		{ Mach1TranscodeFormats::Stereo, 2 },
		{ Mach1TranscodeFormats::LCR, 3 },
		{ Mach1TranscodeFormats::FiveOh, 5 },
		{ Mach1TranscodeFormats::FiveOneFilm, 6 },
		{ Mach1TranscodeFormats::FiveOneFilm_Cinema, 6 },
		{ Mach1TranscodeFormats::FiveOneSmpte, 6 },
		{ Mach1TranscodeFormats::FiveOneDts, 6 },
		{ Mach1TranscodeFormats::SixOh, 6 },
		{ Mach1TranscodeFormats::SevenOnePt, 8 },
		{ Mach1TranscodeFormats::SevenOnePt_Cinema, 8 },
		{ Mach1TranscodeFormats::ACNSN3DO2A, 9 },
		{ Mach1TranscodeFormats::FuMaO2A, 9 },
		{ Mach1TranscodeFormats::TBE, 8 },
		{ Mach1TranscodeFormats::ACNSN3DO3A, 16 },
		{ Mach1TranscodeFormats::FuMaO3A, 16 },
		{ Mach1TranscodeFormats::SevenOneSDDS, 8 },
		{ Mach1TranscodeFormats::SevenZeroSDDS, 7 },
		{ Mach1TranscodeFormats::SevenOneTwo, 10 },
		{ Mach1TranscodeFormats::SevenZeroTwo, 9 },
		{ Mach1TranscodeFormats::NineOne, 10 },
		{ Mach1TranscodeFormats::NineZero, 9 },
		{ Mach1TranscodeFormats::Stereo_Cinema, 2 },
		{ Mach1TranscodeFormats::FiveOneTwo, 8 },
		{ Mach1TranscodeFormats::FiveZeroTwo, 7 },
		{ Mach1TranscodeFormats::FiveOneFour, 10 },
		{ Mach1TranscodeFormats::FiveOneFourSMPTE, 10 },
		{ Mach1TranscodeFormats::FiveZeroFour, 9 },
		{ Mach1TranscodeFormats::SevenOneFour, 12 },
		{ Mach1TranscodeFormats::SevenZeroFour, 11 },
		{ Mach1TranscodeFormats::SevenZero_Cinema, 7 },
		{ Mach1TranscodeFormats::M1SpatialFaces, 6 },
		{ Mach1TranscodeFormats::NineOneFour, 14 },
		{ Mach1TranscodeFormats::NineOneSix, 16 },
		{ Mach1TranscodeFormats::MarcoSixteen, 16 },
		{ Mach1TranscodeFormats::ACNSN3DmaxRE1oa, 4 },
		{ Mach1TranscodeFormats::ACNSN3DmaxRE2oa, 9 },
		{ Mach1TranscodeFormats::ACNSN3DmaxRE3oa, 16 },
		{ Mach1TranscodeFormats::ACNSN3DmaxRE4oa, 25 },
		{ Mach1TranscodeFormats::ACNSN3DmaxRE5oa, 36 },
		{ Mach1TranscodeFormats::ACNSN3DmaxRE6oa, 49 },
		{ Mach1TranscodeFormats::ACNSN3DmaxRE7oa, 64 },
		{ Mach1TranscodeFormats::DolbyAtmosSevenOneTwo, 10 },
	};

	const int MAXCHANS = 64;

	const std::vector<Mach1TranscodeMatrix> mach1TranscodeMatrices = {
		{
			Mach1TranscodeFormats::FuMa,
			Mach1TranscodeFormats::ACNSN3D,
			Mach1SpatialSoundMatrix({
				{ r2, 0, 0, 0 },  //W
				{ 0, 0, 1, 0 },   //Y
				{ 0, 0, 0, 1 },   //Z
				{ 0, 1, 0, 0 } //X
			})
		},
		{
			Mach1TranscodeFormats::FuMaO2A,
			Mach1TranscodeFormats::ACNSN3DO2A,
			Mach1SpatialSoundMatrix({
				{ r2, 0, 0, 0, 0, 0, 0, 0, 0 },    //W
				{ 0, 0, 1, 0, 0, 0, 0, 0, 0 },     //X
				{ 0, 0, 0, 1, 0, 0, 0, 0, 0 },     //Y
				{ 0, 1, 0, 0, 0, 0, 0, 0, 0 },     //Z
				{ 0, 0, 0, 0, 0, 0, 0, 0, r3o2 },  //
				{ 0, 0, 0, 0, 0, 0, r3o2, 0, 0 },  //
				{ 0, 0, 0, 0, 1, 0, 0, 0, 0 },     //
				{ 0, 0, 0, 0, 0, r3o2, 0, 0, 0 },  //
				{ 0, 0, 0, 0, 0, 0, 0, r3o2, 0 } //
			})
		},

		// ...

		{ Mach1TranscodeFormats::CustomPoints, Mach1TranscodeFormats::M1Horizon, Mach1SpatialSoundMatrix() },
		{ Mach1TranscodeFormats::CustomPoints, Mach1TranscodeFormats::M1Spatial, Mach1SpatialSoundMatrix() },
		{ Mach1TranscodeFormats::CustomPoints, Mach1TranscodeFormats::M1SpatialPlus, Mach1SpatialSoundMatrix() },
		{ Mach1TranscodeFormats::CustomPoints, Mach1TranscodeFormats::M1SpatialPlusPlus, Mach1SpatialSoundMatrix() },
		{ Mach1TranscodeFormats::CustomPoints, Mach1TranscodeFormats::M1SpatialExtended, Mach1SpatialSoundMatrix() },
		{ Mach1TranscodeFormats::CustomPoints, Mach1TranscodeFormats::M1SpatialExtendedPlus, Mach1SpatialSoundMatrix() },
		{ Mach1TranscodeFormats::CustomPoints, Mach1TranscodeFormats::M1Horizon, Mach1SpatialSoundMatrix() },
		{ Mach1TranscodeFormats::CustomPoints, Mach1TranscodeFormats::M1Horizon, Mach1SpatialSoundMatrix() },
		{ Mach1TranscodeFormats::CustomPoints, Mach1TranscodeFormats::M1Horizon, Mach1SpatialSoundMatrix() },

		{ Mach1TranscodeFormats::M1Horizon, Mach1TranscodeFormats::CustomPoints, Mach1SpatialSoundMatrix() },
		{ Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::CustomPoints, Mach1SpatialSoundMatrix() },
		{ Mach1TranscodeFormats::M1SpatialPlus, Mach1TranscodeFormats::CustomPoints, Mach1SpatialSoundMatrix() },
		{ Mach1TranscodeFormats::M1SpatialPlusPlus, Mach1TranscodeFormats::CustomPoints, Mach1SpatialSoundMatrix() },
		{ Mach1TranscodeFormats::M1SpatialExtended, Mach1TranscodeFormats::CustomPoints, Mach1SpatialSoundMatrix() },
		{ Mach1TranscodeFormats::M1SpatialExtendedPlus, Mach1TranscodeFormats::CustomPoints, Mach1SpatialSoundMatrix() },


	};

};

#endif