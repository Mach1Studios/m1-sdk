//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#pragma once

#ifndef Mach1TranscodeCore_h
#define Mach1TranscodeCore_h

#include <math.h>
#include <stddef.h>
#include <vector>
#include <map>
#include "M1DSP/M1DSPFilters.h"
#include "M1DSP/M1DSPUtilities.h"
#include "Mach1Point3DCore.h"

class SpatialSoundMatrix {
	std::vector<std::vector<float>> data;

public:
	SpatialSoundMatrix(std::vector<std::vector<float>> data)
	{
		this->data = data;
	}

	std::vector<std::vector<float>>& getData()
	{
		return data;
	}
};

namespace Mach1TranscodeConstantsInternal {
	const float r2 = (float)sqrt(2.0);
	//const float r3 = (float)sqrt(3.0);
	const float oor2 = (float)(1.0 / sqrt(2.0));
	//const float oor3 = (float)(1.0 / sqrt(3.0));
	const float oor4 = (float)(1.0 / sqrt(4.0));
	const float oor8 = (float)(1.0 / sqrt(8.0));
	const float oor16 = (float)(1.0 / sqrt(16.0));
	//const float oo2r2 = (float)(1.0 / (2.0 * sqrt(2.0)));
	//const float r3or2 = (float)(sqrt(3.0) / sqrt(2.0));
	const float r3o2 = (float)sqrt(3.0) / 2.0f;
	const float r2o2 = (float)sqrt(2.0) / 2.0f;
	const float r2o3 = (float)sqrt(2.0) / 3.0f;
	const float r2o4 = (float)sqrt(2.0) / 4.0f;
	//const float r2o6 = (float)sqrt(2.0) / 6.0f;
	const float r2o8 = (float)sqrt(2.0) / 8.0f;
	//const float r2o12 = (float)sqrt(2.0) / 12.0f;
	const float r2o20 = (float)sqrt(2.0) / 20.0f;
	//const float oo8 = (float)1.0f / 8.0f;


	const SpatialSoundMatrix FuMa2ACNSN3D(std::vector<std::vector<float>>
	{   { r2, 0, 0, 0 },  //W
		{ 0, 0, 1, 0 },   //Y
		{ 0, 0, 0, 1 },   //Z
		{ 0, 1, 0, 0 } }); //X
	const SpatialSoundMatrix FuMaO2A2ACNSN3DO2A(std::vector<std::vector<float>>
	{   { r2, 0, 0, 0, 0, 0, 0, 0, 0 },    //W
		{ 0, 0, 1, 0, 0, 0, 0, 0, 0 },     //X
		{ 0, 0, 0, 1, 0, 0, 0, 0, 0 },     //Y
		{ 0, 1, 0, 0, 0, 0, 0, 0, 0 },     //Z
		{ 0, 0, 0, 0, 0, 0, 0, 0, r3o2 },  //
		{ 0, 0, 0, 0, 0, 0, r3o2, 0, 0 },  //
		{ 0, 0, 0, 0, 1, 0, 0, 0, 0 },     //
		{ 0, 0, 0, 0, 0, r3o2, 0, 0, 0 },  //
		{ 0, 0, 0, 0, 0, 0, 0, r3o2, 0 } });//
	const SpatialSoundMatrix FuMaO3A2ACNSN3DO3A(std::vector<std::vector<float>>
	{   { r2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },            //W
		{ 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },             //X
		{ 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },             //Y
		{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },             //Z
		{ 0, 0, 0, 0, 0, 0, 0, 0, r3o2, 0, 0, 0, 0, 0, 0, 0 },          //
		{ 0, 0, 0, 0, 0, 0, r3o2, 0, 0, 0, 0, 0, 0, 0, 0, 0 },          //
		{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },             //
		{ 0, 0, 0, 0, 0, r3o2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },          //
		{ 0, 0, 0, 0, 0, 0, 0, r3o2, 0, 0, 0, 0, 0, 0, 0, 0 },          //
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.790569f },     //
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.745356f, 0, 0 },     //
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.843274f, 0, 0, 0, 0 },     //
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },             //
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.843274f, 0, 0, 0, 0, 0 },     //
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.745356f, 0, 0, 0 },     //
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.790569f, 0 } });   //
	// -- ACN/SN3D ---
	const SpatialSoundMatrix ACNSN3D2FuMa(std::vector<std::vector<float>>
	{   { oor2, 0, 0, 0 }, //W
		{ 0, 0, 0, 1 },    //X
		{ 0, 1, 0, 0 },    //Y
		{ 0, 0, 1, 0 } });  //Z
	const SpatialSoundMatrix ACNSN3DO2A2FuMaO2A(std::vector<std::vector<float>>
	{   { oor2, 0, 0, 0, 0, 0, 0, 0, 0 },  //W Fuma Output
		{ 0, 0, 0, 1, 0, 0, 0, 0, 0 },     //X
		{ 0, 1, 0, 0, 0, 0, 0, 0, 0 },     //Y
		{ 0, 0, 1, 0, 0, 0, 0, 0, 0 },     //Z
		{ 0, 0, 0, 0, 0, 0, 1, 0, 0 },     //
		{ 0, 0, 0, 0, 0, 0, 0, 1.154701f, 0 },     //
		{ 0, 0, 0, 0, 0, 1.154701f, 0, 0, 0 },     //
		{ 0, 0, 0, 0, 0, 0, 0, 0, 1.154701f },     //
		{ 0, 0, 0, 0, 1.154701f, 0, 0, 0, 0 } });   //
	const SpatialSoundMatrix ACNSN3DO3A2FuMaO3A(std::vector<std::vector<float>>
	{   { oor2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  //W Fuma Output
		{ 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     //X
		{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     //Y
		{ 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     //Z
		{ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     //
		{ 0, 0, 0, 0, 0, 0, 0, 1.154701f, 0, 0, 0, 0, 0, 0, 0, 0 },     //
		{ 0, 0, 0, 0, 0, 1.154701f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     //
		{ 0, 0, 0, 0, 0, 0, 0, 0, 1.154701f, 0, 0, 0, 0, 0, 0, 0 },     //
		{ 0, 0, 0, 0, 1.154701f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },     //
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 },             //
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.185854f, 0, 0 },     //
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.185854f, 0, 0, 0, 0 },     //
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.341641f, 0 },     //
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.341641f, 0, 0, 0, 0, 0 },     //
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.264911f },     //
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.264911f, 0, 0, 0, 0, 0, 0 } });   //

	// --- From M1Horizon ---
	const SpatialSoundMatrix M1Horizon2FuMa(std::vector<std::vector<float>>
	{   { oor2, oor2, oor2, oor2 },
		{ oor2, -oor2, oor2, -oor2 },
		{ oor2, oor2, -oor2, -oor2 },
		{ 0, 0, 0, 0 } });
	const SpatialSoundMatrix M1Horizon2ACNSN3D(std::vector<std::vector<float>>
	{   { 1, 1, 1, 1 },
		{ oor2, -oor2, -oor2, oor2 },
		{ 0, 0, 0, 0 },
		{ oor2, oor2, -oor2, -oor2 } });
	const SpatialSoundMatrix M1Horizon2ACNSN3DO2A(std::vector<std::vector<float>>
	{   {  1.0000f, 1.0000f, 1.0000f, 1.0000f},
		{ 0.7071f,  0.7071f, -0.7071f, -0.7071f },
		{ 0.0000f,  0.0000f,  0.0000f,  0.0000f },
		{ 0.7071f, -0.7071f, -0.7071f,  0.7071f },
		{ 0.8660f, -0.8660f,  0.8660f, -0.8660f },
		{ 0.0000f,  0.0000f,  -0.0000f,  -0.0000f },
		{ -0.5000f, -0.5000f, -0.5000f, -0.5000f },
		{ 0.0000f,  -0.0000f,  -0.0000f,  0.0000f },
		{ 0.0000f, -0.0000f,  0.0000f, -0.0000f } });
	const SpatialSoundMatrix M1Horizon2ACNSN3DO3A(std::vector<std::vector<float>>
	{   {  1.0000f, 1.0000f, 1.0000f, 1.0000f},
		{ 0.7071f,  0.7071f, -0.7071f, -0.7071f },
		{ 0.0000f,  0.0000f,  0.0000f,  0.0000f },
		{ 0.7071f, -0.7071f, -0.7071f,  0.7071f },
		{ 0.8660f, -0.8660f,  0.8660f, -0.8660f },
		{ 0.0000f,  0.0000f,  -0.0000f,  -0.0000f },
		{ -0.5000f, -0.5000f, -0.5000f, -0.5000f },
		{ 0.0000f,  -0.0000f,  -0.0000f,  0.0000f },
		{ 0.0000f, -0.0000f,  0.0000f, -0.0000f },
		{ 0.5590f,  0.5590f, -0.5590f, -0.5590f },
		{ 0.0000f,  -0.0000f,  0.0000f,  -0.0000f },
		{ -0.4330f, -0.4330f,  0.4330f,  0.4330f },
		{ -0.0000f,  -0.0000f,  -0.0000f,  -0.0000f },
		{ -0.4330f,  0.4330f,  0.4330f, -0.4330f },
		{ 0.0000f,  -0.0000f,  0.0000f,  -0.0000f },
		{ -0.5590f,  0.5590f,  0.5590f, -0.5590f } });
	const SpatialSoundMatrix ACNSN3D2M1Horizon(std::vector<std::vector<float>>
	{   {  0.5000f, 0.3536f, 0.0000f, 0.3536f},
		{ 0.5000f,  0.3536f,  0.0000f, -0.3536f },
		{ 0.5000f, -0.3536f,  0.0000f, -0.3536f },
		{ 0.5000f, -0.3536f,  0.0000f,  0.3536f } });
	const SpatialSoundMatrix ACNSN3DO2A2M1Horizon(std::vector<std::vector<float>>
	{   {  0.3727f, 0.3953f, 0.0000f, 0.3953f, 0.1614f, 0.0000f, -0.0932f, 0.0000f, 0.0000f},
		{ 0.3727f,  0.3953f,  0.0000f, -0.3953f, -0.1614f,  0.0000f, -0.0932f,  -0.0000f, -0.0000f },
		{ 0.3727f, -0.3953f,  0.0000f, -0.3953f,  0.1614f,  -0.0000f, -0.0932f,  -0.0000f,  0.0000f },
		{ 0.3727f, -0.3953f,  0.0000f,  0.3953f, -0.1614f,  -0.0000f, -0.0932f,  0.0000f, -0.0000f } });
	const SpatialSoundMatrix ACNSN3DO3A2M1Horizon(std::vector<std::vector<float>>
	{   {  0.3123f, 0.3975f, 0.0000f, 0.3975f, 0.2705f, 0.0000f, -0.1562f, 0.0000f, 0.0000f, 0.0349f, 0.0000f, -0.0270f, -0.0000f, -0.0270f, 0.0000f, -0.0349f},
		{ 0.3123f,  0.3975f,  0.0000f, -0.3975f, -0.2705f,  0.0000f, -0.1562f,  -0.0000f, -0.0000f,  0.0349f,  -0.0000f, -0.0270f,  -0.0000f,  0.0270f,  -0.0000f,  0.0349f },
		{ 0.3123f, -0.3975f,  0.0000f, -0.3975f,  0.2705f,  -0.0000f, -0.1562f,  -0.0000f,  0.0000f, -0.0349f,  0.0000f,  0.0270f,  -0.0000f,  0.0270f,  0.0000f,  0.0349f },
		{ 0.3123f, -0.3975f,  0.0000f,  0.3975f, -0.2705f,  -0.0000f, -0.1562f,  0.0000f, -0.0000f, -0.0349f,  -0.0000f,  0.0270f,  -0.0000f, -0.0270f,  -0.0000f, -0.0349f } });
	const SpatialSoundMatrix M1Horizon2M1HorizonPairs(std::vector<std::vector<float>>
	{   { 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 0, 1 },
		{ 0, 0, 0, 1 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 1, 0 },
		{ 1, 0, 0, 0 } });
	const SpatialSoundMatrix M1Horizon2M1Spatial(std::vector<std::vector<float>>
	{   { oor2, 0, 0, 0 },
		{ 0, oor2, 0, 0 },
		{ 0, 0, oor2, 0 },
		{ 0, 0, 0, oor2 },
		{ oor2, 0, 0, 0 },
		{ 0, oor2, 0, 0 },
		{ 0, 0, oor2, 0 },
		{ 0, 0, 0, oor2 } });
	const SpatialSoundMatrix M1Horizon2M1SpatialPairs(std::vector<std::vector<float>>
	{   { 0.5f, 0, 0, 0 },
		{ 0, 0.5f, 0, 0 },
		{ 0, 0, 0.5f, 0 },
		{ 0.5f, 0, 0, 0 },
		{ 0, 0, 0, 0.5f },
		{ 0, 0, 0.5f, 0 },
		{ 0, 0.5f, 0, 0 },
		{ 0, 0, 0, 0.5f },
		{ 0.5f, 0, 0, 0 },
		{ 0, 0.5f, 0, 0 },
		{ 0, 0, 0.5f, 0 },
		{ 0.5f, 0, 0, 0 },
		{ 0, 0, 0, 0.5f },
		{ 0, 0, 0.5f, 0 },
		{ 0, 0.5f, 0, 0 },
		{ 0, 0, 0, 0.5f } });
	const SpatialSoundMatrix M1Horizon2FiveOh(std::vector<std::vector<float>>
	{   { 1, 0, 0, 0 },
		{ r2o4, r2o4, r2o4, r2o4 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 } });
	const SpatialSoundMatrix M1Horizon2FiveOneFilm(std::vector<std::vector<float>>
	{   { 1, 0, 0, 0 },
		{ r2o4, r2o4, r2o4, r2o4 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 },
		{ r2o8, r2o8, r2o8, r2o8 } });
	const SpatialSoundMatrix M1Horizon2FiveOneFilm_Cinema(std::vector<std::vector<float>>
	{   { 1, 0, 0, 0 },
		{ r2o2, r2o2, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 },
		{ r2o8, r2o8, r2o8, r2o8 } });
	const SpatialSoundMatrix M1Horizon2FiveOneSmpte(std::vector<std::vector<float>>
	{   { 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ r2o2, r2o2, 0, 0 },
		{ r2o8, r2o8, r2o8, r2o8 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 } });
	const SpatialSoundMatrix M1Horizon2FiveOneDts(std::vector<std::vector<float>>
	{   { 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 },
		{ r2o2, r2o2, 0, 0 },
		{ r2o8, r2o8, r2o8, r2o8 } });
	const SpatialSoundMatrix M1Horizon2SixOh(std::vector<std::vector<float>>
	{   { 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 } });

	// --- Surround To M1Horizon ---
	const SpatialSoundMatrix FiveOh2M1Horizon(std::vector<std::vector<float>>
		//L, C, R, Ls, Rs
	{   { 1, r2o4, 0, 0, 0 },
		{ 0, r2o4, 1, 0, 0 },
		{ 0, r2o4, 0, 1, 0 },
		{ 0, r2o4, 0, 0, 1 } });
	const SpatialSoundMatrix FiveOneFilm2M1Horizon(std::vector<std::vector<float>>
		//L, C, R, Ls, Rs, LFE
	{   { 1, r2o4, 0, 0, 0, r2o8 },
		{ 0, r2o4, 1, 0, 0, r2o8 },
		{ 0, r2o4, 0, 1, 0, r2o8 },
		{ 0, r2o4, 0, 0, 1, r2o8 } });
	const SpatialSoundMatrix FiveOneFilm_Cinema2M1Horizon(std::vector<std::vector<float>>
	{   { 1, r2o4, 0, 0, 0, r2o8 },
		{ 0, r2o4, 1, 0, 0, r2o8 },
		{ 0, r2o20, 0, 1, 0, r2o8 },
		{ 0, r2o20, 0, 0, 1, r2o8 } });
	const SpatialSoundMatrix FiveOneSmpte2M1Horizon(std::vector<std::vector<float>>
	{   { 1, 0, r2o4, r2o8, 0, 0 },
		{ 0, 1, r2o4, r2o8, 0, 0 },
		{ 0, 0, r2o4, r2o8, 1, 0 },
		{ 0, 0, r2o4, r2o8, 0, 1 } });
	const SpatialSoundMatrix FiveOneDts2M1Horizon(std::vector<std::vector<float>>
	{   { 1, 0, 0, 0, r2o4, r2o8 },
		{ 0, 1, 0, 0, r2o4, r2o8 },
		{ 0, 0, 1, 0, r2o4, r2o8 },
		{ 0, 0, 0, 1, r2o4, r2o8 } });
	const SpatialSoundMatrix SixOh2M1Horizon(std::vector<std::vector<float>> // ?
	{   { 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0 } });

	// --- M1Horizon ( std::vector<std::vector<float>>M1Horizon) + Stereo ( std::vector<std::vector<float>>S) ---
	const SpatialSoundMatrix M1HorizonS2FuMa(std::vector<std::vector<float>>
	{   { oor2, oor2, oor2, oor2, oor2, oor2 },
		{ oor2, -oor2, oor2, -oor2, 0, 0 },
		{ oor2, oor2, -oor2, -oor2, 0, 0 },
		{ 0, 0, 0, 0, 0, 0 } });
	const SpatialSoundMatrix M1HorizonS2ACNSN3D(std::vector<std::vector<float>>
	{   { 1, 1, 1, 1, 1, 1 },
		{ oor2, -oor2, -oor2, oor2, 0, 0 },
		{ 0, 0, 0, 0, 0, 0 },
		{ oor2, oor2, -oor2, -oor2, 0, 0 } });
	const SpatialSoundMatrix M1HorizonS2M1Horizon(std::vector<std::vector<float>>
	{   { 1, 0, 0, 0, 0.25f, 0.25f },
		{ 0, 1, 0, 0, 0.25f, 0.25f },
		{ 0, 0, 1, 0, 0.25f, 0.25f },
		{ 1, 0, 0, 0, 0.25f, 0.25f } });
	const SpatialSoundMatrix M1HorizonS2M1HorizonPairs(std::vector<std::vector<float>>
	{   { 1, 0, 0, 0, 1, 0 },
		{ 0, 1, 0, 0, 0, 1 },
		{ 0, 1, 0, 0, 1, 0 },
		{ 0, 0, 0, 1, 0, 1 },
		{ 0, 0, 0, 1, 1, 0 },
		{ 0, 0, 1, 0, 0, 1 },
		{ 0, 0, 1, 0, 1, 0 },
		{ 1, 0, 0, 0, 0, 1 } });
	const SpatialSoundMatrix M1HorizonS2M1Spatial(std::vector<std::vector<float>>
	{   { 0.5f, 0, 0, 0, 0.125f, 0.125f },
		{ 0, 0.5f, 0, 0, 0.125f, 0.125f },
		{ 0, 0, 0.5f, 0, 0.125f, 0.125f },
		{ 0, 0, 0, 0.5f, 0.125f, 0.125f },
		{ 0.5f, 0, 0, 0, 0.125f, 0.125f },
		{ 0, 0.5f, 0, 0, 0.125f, 0.125f },
		{ 0, 0, 0.5f, 0, 0.125f, 0.125f },
		{ 0, 0, 0, 0.5f, 0.125f, 0.125f } });
	const SpatialSoundMatrix M1HorizonS2M1SpatialPairs(std::vector<std::vector<float>>
	{   { 0.5f, 0, 0, 0, 1, 0 },
		{ 0, 0.5f, 0, 0, 0, 1 },
		{ 0, 0, 0.5f, 0, 1, 0 },
		{ 0.5f, 0, 0, 0, 0, 1 },
		{ 0, 0, 0, 0.5f, 1, 0 },
		{ 0, 0, 0.5f, 0, 0, 1 },
		{ 0, 0.5f, 0, 0, 1, 0 },
		{ 0, 0, 0, 0.5f, 0, 1 },
		{ 0.5f, 0, 0, 0, 1, 0 },
		{ 0, 0.5f, 0, 0, 0, 1 },
		{ 0, 0, 0.5f, 0, 1, 0 },
		{ 0.5f, 0, 0, 0, 0, 1 },
		{ 0, 0, 0, 0.5f, 1, 0 },
		{ 0, 0, 0.5f, 0, 0, 1 },
		{ 0, 0.5f, 0, 0, 1, 0 },
		{ 0, 0, 0, 0.5f, 0, 1 } });
	//const SpatialSoundStandart M1HorizonPairs2FuMa( std::vector<std::vector<float>>
	//{   { oo2r2, oo2r2, oo2r2, oo2r2, oo2r2, oo2r2, oo2r2, oo2r2 },
	//  { oo2r2, oo2r2, oo2r2, -oo2r2,- oo2r2, oo2r2, -oo2r2, -oo2r2 },
	//  { oo2r2, -oo2r2, -oo2r2, oo2r2, -oo2r2, -oo2r2, oo2r2, -oo2r2 },
	//  { 0, 0, 0, 0 } });

	// --- M1Spatial ( std::vector<std::vector<float>>M1Spatial) ---
	const SpatialSoundMatrix M1Spatial2M1Spatial(std::vector<std::vector<float>> // requested for conversions of number of files
	{   { 1, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 1, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 1, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 1, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 1, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 1, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 1, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 1 } });
	const SpatialSoundMatrix M1Spatial2FuMa(std::vector<std::vector<float>>
	{   { oor2, oor2, oor2, oor2, oor2, oor2, oor2, oor2 },
		{ 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f },
		{ 0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f },
		{ oor2, oor2, oor2, oor2, -oor2, -oor2, -oor2, -oor2 } });
	const SpatialSoundMatrix M1Spatial2FuMaO2A(std::vector<std::vector<float>>
	{   { oor2, oor2, oor2, oor2, oor2, oor2, oor2, oor2 }, //W
		{ 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f }, //X
		{ 0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f }, //Y
		{ oor2, oor2, oor2, oor2, -oor2, -oor2, -oor2, -oor2 }, //Z
		{ 0, 0, 0, 0, 0, 0, 0, 0 }, //R
		{ 0, 0, 0, 0, 0, 0, 0, 0 }, //S
		{ 0, 0, 0, 0, 0, 0, 0, 0 }, //T
		{ 0, 0, 0, 0, 0, 0, 0, 0 }, //U
		{ 0, 0, 0, 0, 0, 0, 0, 0 } }); //V
	const SpatialSoundMatrix M1Spatial2TBE(std::vector<std::vector<float>>
	{   { 0.488603f, 0.488603f, 0.488603f, 0.488603f, 0.488603f, 0.488603f, 0.488603f, 0.488603f },
		{ -0.281971f, 0.281971f, -0.281971f, 0.281971f, -0.281971f, 0.281971f, -0.281971f, 0.281971f },
		{ 0.281971f, 0.281971f, -0.281971f, -0.281971f, 0.281971f, 0.281971f, -0.281971f, -0.281971f },
		{ 0.282343f, 0.282343f, 0.282343f, 0.282343f, -0.282343f, -0.282343f, -0.282343f, -0.282343f },
		{ 4.339019f, -1.590494f, 5.0602722f, 2.414620f, 4.339019f, -1.590494f, 5.0602722f, 2.414620f },
		{ -0.363863f, 0.363863f, 0.363863f, -0.363863f, -0.363863f, 0.363863f, 0.363863f, -0.363863f },
		{ -0.364343f, 0.364343f, -0.364343f, 0.364343f, 0.364343f, -0.364343f, 0.364343f, -0.364343f },
		{ 0.364343f, 0.364343f, -0.364343f, -0.364342f, -0.364343f, -0.364343f, 0.364343f, 0.364343f } });
	//oFOA
	//    const SpatialSoundStandart M1Spatial2ACNSN3D( std::vector<std::vector<float>>
	//    {   { oor3, oor3, oor3, oor3, oor3, oor3, oor3, oor3 }, //oor3 = 0.5773f
	//        { r3o2, -r3o2, r3o2, -r3o2, r3o2, -r3o2, r3o2, -r3o2 }, // r3o2 = 0.8660f
	//        { r3or2, r3or2, r3or2, r3or2, -r3or2, -r3or2, -r3or2, -r3or2 }, // r3or2 = 1.2247f
	//        { r3o2, r3o2, -r3o2, -r3o2, r3o2, r3o2, -r3o2, -r3o2 } });
	//nFOA
	const SpatialSoundMatrix M1Spatial2ACNSN3D(std::vector<std::vector<float>>
	{   {  1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f},
		{ 0.5000f, -0.5000f,  0.5000f, -0.5000f,  0.5000f, -0.5000f, -0.5000f,  0.5000f },
		{ 0.7071f,  0.7071f,  0.7071f,  0.7071f, -0.7071f, -0.7071f, -0.7071f, -0.7071f },
		{ 0.5000f,  0.5000f, -0.5000f, -0.5000f,  0.5000f,  0.5000f, -0.5000f, -0.5000f } });
	//oSOA
	//    const SpatialSoundStandart M1Spatial2ACNSN3DO2A( std::vector<std::vector<float>>
	//    {   { oor3, oor3, oor3, oor3, oor3, oor3, oor3, oor3 },
	//        { r3o2, -r3o2, r3o2, -r3o2, r3o2, -r3o2, r3o2, -r3o2 },
	//        { r3or2, r3or2, r3or2, r3or2, -r3or2, -r3or2, -r3or2, -r3or2 },
	//        { r3o2, r3o2, -r3o2, -r3o2, r3o2, r3o2, -r3o2, -r3o2 },
	//        {  0.4330f, -0.4330f,  0.4330f, -0.4330f,  0.4330f, -0.4330f,  0.4330f, -0.4330f},
	//        {  0.6124f,  0.6124f, -0.6124f, -0.6124f, -0.6124f, -0.6124f,  0.6124f,  0.6124f},
	//        {  0.2500f,  0.2500f,  0.2500f,  0.2500f,  0.2500f,  0.2500f,  0.2500f,  0.2500f},
	//        {  0.6124f, -0.6124f, -0.6124f,  0.6124f, -0.6124f,  0.6124f,  0.6124f, -0.6124f},
	//        {  0.0000f, -0.0000f,  0.0000f, -0.0000f,  0.0000f, -0.0000f,  0.0000f, -0.0000f} });
	//nSOA
	const SpatialSoundMatrix M1Spatial2ACNSN3DO2A(std::vector<std::vector<float>>
	{   {  1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f},
		{ 0.5000f, -0.5000f,  0.5000f, -0.5000f,  0.5000f, -0.5000f, -0.5000f,  0.5000f },
		{ 0.7071f,  0.7071f,  0.7071f,  0.7071f, -0.7071f, -0.7071f, -0.7071f, -0.7071f },
		{ 0.5000f,  0.5000f, -0.5000f, -0.5000f,  0.5000f,  0.5000f, -0.5000f, -0.5000f },
		{ 0.4330f, -0.4330f,  0.4330f, -0.4330f,  0.4330f, -0.4330f,  0.4330f, -0.4330f },
		{ 0.6124f,  0.6124f, -0.6124f, -0.6124f, -0.6124f, -0.6124f,  0.6124f,  0.6124f },
		{ 0.2500f,  0.2500f,  0.2500f,  0.2500f,  0.2500f,  0.2500f,  0.2500f,  0.2500f },
		{ 0.6124f, -0.6124f, -0.6124f,  0.6124f, -0.6124f,  0.6124f,  0.6124f, -0.6124f },
		{ 0.0000f,  0.0000f,  0.0000f,  0.0000f,  0.0000f,  0.0000f,  0.0000f,  0.0000f } });
	const SpatialSoundMatrix M1Spatial2ACNSN3DO3A(std::vector<std::vector<float>>
	{   {  1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f},
		{ 0.5000f, -0.5000f,  0.5000f, -0.5000f,  0.5000f, -0.5000f,  0.5000f, -0.5000f },
		{ 0.7071f,  0.7071f,  0.7071f,  0.7071f, -0.7071f, -0.7071f, -0.7071f, -0.7071f },
		{ 0.5000f,  0.5000f, -0.5000f, -0.5000f,  0.5000f,  0.5000f, -0.5000f, -0.5000f },
		{ 0.4330f, -0.4330f,  0.4330f, -0.4330f,  0.4330f, -0.4330f,  0.4330f, -0.4330f },
		{ 0.6124f,  0.6124f, -0.6124f, -0.6124f, -0.6124f, -0.6124f,  0.6124f,  0.6124f },
		{ 0.2500f,  0.2500f,  0.2500f,  0.2500f,  0.2500f,  0.2500f,  0.2500f,  0.2500f },
		{ 0.6124f, -0.6124f, -0.6124f,  0.6124f, -0.6124f,  0.6124f,  0.6124f, -0.6124f },
		{ 0.0000f,  0.0000f,  0.0000f,  0.0000f,  0.0000f,  0.0000f,  0.0000f,  0.0000f },
		{ 0.1976f,  0.1976f, -0.1976f, -0.1976f,  0.1976f,  0.1976f, -0.1976f, -0.1976f },
		{ 0.6847f, -0.6847f,  0.6847f, -0.6847f, -0.6847f,  0.6847f, -0.6847f,  0.6847f },
		{ 0.4593f,  0.4593f, -0.4593f, -0.4593f,  0.4593f,  0.4593f, -0.4593f, -0.4593f },
		{ -0.1768f, -0.1768f, -0.1768f, -0.1768f,  0.1768f,  0.1768f,  0.1768f,  0.1768f },
		{ 0.4593f, -0.4593f, -0.4593f,  0.4593f,  0.4593f, -0.4593f, -0.4593f,  0.4593f },
		{ 0.0000f,  0.0000f,  0.0000f,  0.0000f,  0.0000f,  0.0000f,  0.0000f,  0.0000f },
		{ -0.1976f,  0.1976f,  0.1976f, -0.1976f, -0.1976f,  0.1976f,  0.1976f, -0.1976f } });
	const SpatialSoundMatrix ACNSN3D2M1Spatial(std::vector<std::vector<float>>
	{   {  0.5000f, 0.2500f, 0.3536f, 0.2500f},
		{ 0.5000f,  0.2500f,  0.3536f, -0.2500f },
		{ 0.5000f, -0.2500f,  0.3536f, -0.2500f },
		{ 0.5000f, -0.2500f,  0.3536f,  0.2500f },
		{ 0.5000f,  0.2500f, -0.3536f,  0.2500f },
		{ 0.5000f,  0.2500f, -0.3536f, -0.2500f },
		{ 0.5000f, -0.2500f, -0.3536f, -0.2500f },
		{ 0.5000f, -0.2500f, -0.3536f,  0.2500f } });
	const SpatialSoundMatrix ACNSN3DO2A2M1Spatial(std::vector<std::vector<float>>
	{   {  0.3727f, 0.2795f, 0.3953f, 0.2795f, 0.0807f, 0.1141f, 0.0466f, 0.1141f, 0.0000f},
		{ 0.3727f,  0.2795f,  0.3953f, -0.2795f, -0.0807f,  0.1141f,  0.0466f, -0.1141f, -0.0000f },
		{ 0.3727f, -0.2795f,  0.3953f, -0.2795f,  0.0807f, -0.1141f,  0.0466f, -0.1141f,  0.0000f },
		{ 0.3727f, -0.2795f,  0.3953f,  0.2795f, -0.0807f, -0.1141f,  0.0466f,  0.1141f, -0.0000f },
		{ 0.3727f,  0.2795f, -0.3953f,  0.2795f,  0.0807f, -0.1141f,  0.0466f, -0.1141f,  0.0000f },
		{ 0.3727f,  0.2795f, -0.3953f, -0.2795f, -0.0807f, -0.1141f,  0.0466f,  0.1141f, -0.0000f },
		{ 0.3727f, -0.2795f, -0.3953f, -0.2795f,  0.0807f,  0.1141f,  0.0466f,  0.1141f,  0.0000f },
		{ 0.3727f, -0.2795f, -0.3953f,  0.2795f, -0.0807f,  0.1141f,  0.0466f, -0.1141f, -0.0000f } });
	const SpatialSoundMatrix ACNSN3DO3A2M1Spatial(std::vector<std::vector<float>>
	{   {  0.3123f, 0.2811f, 0.3975f, 0.2811f, 0.1352f, 0.1913f, 0.0781f, 0.1913f, 0.0000f, 0.0123f, 0.0428f, 0.0287f, -0.0110f, 0.0287f, 0.0000f, -0.0123f},
		{ 0.3123f,  0.2811f,  0.3975f, -0.2811f, -0.1352f,  0.1913f,  0.0781f, -0.1913f, -0.0000f,  0.0123f, -0.0428f,  0.0287f, -0.0110f, -0.0287f, -0.0000f,  0.0123f },
		{ 0.3123f, -0.2811f,  0.3975f, -0.2811f,  0.1352f, -0.1913f,  0.0781f, -0.1913f,  0.0000f, -0.0123f,  0.0428f, -0.0287f, -0.0110f, -0.0287f,  0.0000f,  0.0123f },
		{ 0.3123f, -0.2811f,  0.3975f,  0.2811f, -0.1352f, -0.1913f,  0.0781f,  0.1913f, -0.0000f, -0.0123f, -0.0428f, -0.0287f, -0.0110f,  0.0287f, -0.0000f, -0.0123f },
		{ 0.3123f,  0.2811f, -0.3975f,  0.2811f,  0.1352f, -0.1913f,  0.0781f, -0.1913f,  0.0000f,  0.0123f, -0.0428f,  0.0287f,  0.0110f,  0.0287f, -0.0000f, -0.0123f },
		{ 0.3123f,  0.2811f, -0.3975f, -0.2811f, -0.1352f, -0.1913f,  0.0781f,  0.1913f, -0.0000f,  0.0123f,  0.0428f,  0.0287f,  0.0110f, -0.0287f,  0.0000f,  0.0123f },
		{ 0.3123f, -0.2811f, -0.3975f, -0.2811f,  0.1352f,  0.1913f,  0.0781f,  0.1913f,  0.0000f, -0.0123f, -0.0428f, -0.0287f,  0.0110f, -0.0287f, -0.0000f,  0.0123f },
		{ 0.3123f, -0.2811f, -0.3975f,  0.2811f, -0.1352f,  0.1913f,  0.0781f, -0.1913f, -0.0000f, -0.0123f,  0.0428f, -0.0287f,  0.0110f,  0.0287f,  0.0000f, -0.0123f } });
	const SpatialSoundMatrix TBE2M1Spatial(std::vector<std::vector<float>>
	{   { 0.682217f, -0.590557f, 0.590557f, 0.591337f, 0, -0.152414f, -0.152615f, 0.152615f },
		{ 0.682217f, 0.590557f, 0.590557f, 0.591337f, 0, 0.152414f, 0.152615f, 0.152615f },
		{ 0.682217f, -0.590557f, -0.590557f, 0.591337f, 0, 0.152414f, -0.152615f, -0.152615f },
		{ 0.682217f, 0.590557f, -0.590557f, 0.591337f, 0, -0.152414f, 0.152615f, -0.152615f },
		{ 0.682217f, -0.590557f, 0.590557f, -0.591337f, 0, -0.152414f, 0.152615f, -0.152615f },
		{ 0.682217f, 0.590557f, 0.590557f, -0.591337f, 0, 0.152414f, -0.152615f, -0.152615f },
		{ 0.682217f, -0.590557f, -0.590557f, -0.591337f, 0, 0.152414f, 0.152615f, 0.152615f },
		{ 0.682217f, 0.590557f, -0.590557f, -0.591337f, 0, -0.152414f, -0.152615f, 0.152615f } });
	//inverse
	//    {   { 1.465809f, -1.693316f, 1.693316f, 1.691083f, 0, -6.561077f, -6.552435f, 6.552435f },
	//        { 1.465809f, 1.693316f, 1.693316f, 1.691083f, 0, 6.561077f, 6.552435f, 6.552435f },
	//        { 1.465809f, -1.693316f, -1.693316f, 1.691083f, 0, 6.561077f, -6.552435f, -6.552435f },
	//        { 1.465809f, 1.693316f, -1.693316f, 1.691083f, 0, -6.561077f, 6.552435f, -6.552435f },
	//        { 1.465809f, -1.693316f, 1.693316f, -1.691083f, 0, -6.561077f, 6.552435f, -6.552435f },
	//        { 1.465809f, 1.693316f, 1.693316f, -1.691083f, 0, 6.561077f, -6.552435f, -6.552435f },
	//        { 1.465809f, -1.693316f, -1.693316f, -1.691083f, 0, 6.561077f, 6.552435f, 6.552435f },
	//        { 1.465809f, 1.693316f, -1.693316f, -1.691083f, 0, -6.561077f, -6.552435f, 6.552435f } });
	const SpatialSoundMatrix M1Spatial2M1Horizon(std::vector<std::vector<float>>
	{   { oor2, 0, 0, 0, oor2, 0, 0, 0 },
		{ 0, oor2, 0, 0, 0, oor2, 0, 0 },
		{ 0, 0, oor2, 0, 0, 0, oor2, 0 },
		{ 0, 0, 0, oor2, 0, 0, 0, oor2 } });
	const SpatialSoundMatrix M1Spatial2M1HorizonPairs(std::vector<std::vector<float>>
	{   { oor2, 0, 0, 0, oor2, 0, 0, 0 }, //000L
		{ 0, oor2, 0, 0, 0, oor2, 0, 0 }, //000R
		{ 0, oor2, 0, 0, 0, oor2, 0, 0 }, //090L
		{ 0, 0, 0, oor2, 0, 0, 0, oor2 }, //090R
		{ 0, 0, 0, oor2, 0, 0, 0, oor2 }, //180L
		{ 0, 0, oor2, 0, 0, 0, oor2, 0 }, //180R
		{ 0, 0, oor2, 0, 0, 0, oor2, 0 }, //270L
		{ oor2, 0, 0, 0, oor2, 0, 0, 0 } }); //270R
	const SpatialSoundMatrix M1Spatial2M1SpatialFaces(std::vector<std::vector<float>>
		//Fc, Lc, Bc, Rc, Tc, Bc
	{   { r2o3, r2o3, 0, 0, r2o3, r2o3, 0, 0 },
		{ r2o3, 0, r2o3, 0, r2o3, 0, r2o3, 0 },
		{ 0, 0, r2o3, r2o3, 0, 0, r2o3, r2o3 },
		{ 0, r2o3, 0, r2o3, 0, r2o3, 0, r2o3 },
		{ r2o3, r2o3, r2o3, r2o3, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, r2o3, r2o3, r2o3, r2o3 } });
	const SpatialSoundMatrix M1Spatial2M1SpatialPairs(std::vector<std::vector<float>> //TODO: fix this
	{   { 1, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 1, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 1, 0, 0, 0, 0, 0 },
		{ 1, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 1, 0, 0, 0, 0 },
		{ 0, 0, 1, 0, 0, 0, 0, 0 },
		{ 0, 1, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 1, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 1, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 1, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 1, 0 },
		{ 0, 0, 0, 0, 1, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 0, 0, 0, 0, 0, 0, 1, 0 },
		{ 0, 0, 0, 0, 0, 1, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 1 } });

	// --- M1Spatial + Stereo ( std::vector<std::vector<float>>S) ---
	const SpatialSoundMatrix M1SpatialS2FuMa(std::vector<std::vector<float>>
	{   { oor2, oor2, oor2, oor2, oor2, oor2, oor2, oor2, 1, 1 },
		{ 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0, 0 },
		{ 0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0, 0 },
		{ oor2, oor2, oor2, oor2, -oor2, -oor2, -oor2, -oor2, 0, 0 } });
	const SpatialSoundMatrix M1SpatialS2ACNSN3D(std::vector<std::vector<float>>
	{   { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0, 0 },
		{ oor2, oor2, oor2, oor2, -oor2, -oor2, -oor2, -oor2, 0, 0 },
		{ 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0, 0 } });
	const SpatialSoundMatrix M1SpatialS2M1Horizon(std::vector<std::vector<float>>
	{   { 0.5f, 0, 0, 0, 0.5f, 0, 0, 0, 0.25f, 0.25f },
		{ 0, 0.5f, 0, 0, 0, 0.5f, 0, 0, 0.25f, 0.25f },
		{ 0, 0, 0.5f, 0, 0, 0, 0.5f, 0, 0.25f, 0.25f },
		{ 0, 0, 0, 0.5f, 0, 0, 0, 0.5f, 0.25f, 0.25f } });
	const SpatialSoundMatrix M1SpatialS2M1HorizonPairs(std::vector<std::vector<float>>
	{   { 0.5f, 0, 0, 0, 0.5f, 0, 0, 0, 1, 0 },
		{ 0, 0.5f, 0, 0, 0, 0.5f, 0, 0, 0, 1 },
		{ 0, 0.5f, 0, 0, 0, 0.5f, 0, 0, 1, 0 },
		{ 0, 0, 0, 0.5f, 0, 0, 0, 0.5f, 0, 1 },
		{ 0, 0, 0, 0.5f, 0, 0, 0, 0.5f, 1, 0 },
		{ 0, 0, 0.5f, 0, 0, 0, 0.5f, 0, 0, 1 },
		{ 0, 0, 0.5f, 0, 0, 0, 0.5f, 0, 1, 0 },
		{ 0.5f, 0, 0, 0, 0.5f, 0, 0, 0, 0, 1 } });
	const SpatialSoundMatrix M1SpatialS2M1Spatial(std::vector<std::vector<float>>
	{   { 1, 0, 0, 0, 0, 0, 0, 0, 0.125f, 0.125f },
		{ 0, 1, 0, 0, 0, 0, 0, 0, 0.125f, 0.125f },
		{ 0, 0, 1, 0, 0, 0, 0, 0, 0.125f, 0.125f },
		{ 0, 0, 0, 1, 0, 0, 0, 0, 0.125f, 0.125f },
		{ 0, 0, 0, 0, 1, 0, 0, 0, 0.125f, 0.125f },
		{ 0, 0, 0, 0, 0, 1, 0, 0, 0.125f, 0.125f },
		{ 0, 0, 0, 0, 0, 0, 1, 0, 0.125f, 0.125f },
		{ 0, 0, 0, 0, 0, 0, 0, 1, 0.125f, 0.125f } });
	const SpatialSoundMatrix M1SpatialS2M1SpatialPairs(std::vector<std::vector<float>>
	{   { 1, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
		{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 0, 0, 1, 0, 0, 0, 0, 0, 1, 0 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 0, 0, 0, 1, 0, 0, 0, 0, 1, 0 },
		{ 0, 0, 1, 0, 0, 0, 0, 0, 0, 1 },
		{ 0, 1, 0, 0, 0, 0, 0, 0, 1, 0 },
		{ 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 },
		{ 0, 0, 0, 0, 1, 0, 0, 0, 1, 0 },
		{ 0, 0, 0, 0, 0, 1, 0, 0, 0, 1 },
		{ 0, 0, 0, 0, 0, 0, 1, 0, 1, 0 },
		{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },
		{ 0, 0, 0, 0, 0, 0, 0, 1, 1, 0 },
		{ 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 },
		{ 0, 0, 0, 0, 0, 1, 0, 0, 1, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 1, 0, 1 },
	});

	// --- Surround to M1Spatial ---;
	const SpatialSoundMatrix Mono2M1Spatial(std::vector<std::vector<float>>
	{   { r2o8 },
		{ r2o8 },
		{ r2o8 },
		{ r2o8 },
		{ r2o8 },
		{ r2o8 },
		{ r2o8 },
		{ r2o8 } });
	const SpatialSoundMatrix Stereo2M1Spatial(std::vector<std::vector<float>>
	{   { r2o2, 0 },
		{ 0, r2o2 },
		{ r2o2, 0 },
		{ 0, r2o2 },
		{ r2o2, 0 },
		{ 0, r2o2 },
		{ r2o2, 0 },
		{ 0, r2o2 } });
	const SpatialSoundMatrix Stereo_Cinema2M1Spatial(std::vector<std::vector<float>>
		//fix ratio between front and back
	{   { r2o2, 0 },
		{ 0, r2o2 },
		{ r2o20, 0 },
		{ 0, r2o20 },
		{ r2o2, 0 },
		{ 0, r2o2 },
		{ r2o20, 0 },
		{ 0, r2o20 } });
	const SpatialSoundMatrix LCR2M1Spatial(std::vector<std::vector<float>>
	{   { r2o2, r2o8, 0 },
		{ 0, r2o8, r2o2 },
		{ r2o8, r2o8, 0 },
		{ 0, r2o8, r2o8 },
		{ r2o2, r2o8, 0 },
		{ 0, r2o8, r2o2 },
		{ r2o8, r2o8, 0 },
		{ 0, r2o8, r2o8 }, });
	const SpatialSoundMatrix FiveOh2M1Spatial(std::vector<std::vector<float>>
		//L, C , R, Ls, Rs
	{   { r2o2, r2o8, 0, 0, 0 },
		{ 0, r2o8, r2o2, 0, 0 },
		{ 0, r2o8, 0, r2o2, 0 },
		{ 0, r2o8, 0, 0, r2o2 },
		{ r2o2, r2o8, 0, 0, 0 },
		{ 0, r2o8, r2o2, 0, 0 },
		{ 0, r2o8, 0, r2o2, 0 },
		{ 0, r2o8, 0, 0, r2o2 } });
	const SpatialSoundMatrix FiveOneFilm2M1Spatial(std::vector<std::vector<float>>
		//L, C, R, Ls, Rs, LFE
	{   { r2o2, r2o8, 0, 0, 0, r2o8 },
		{ 0, r2o8, r2o2, 0, 0, r2o8 },
		{ 0, r2o8, 0, r2o2, 0, r2o8 },
		{ 0, r2o8, 0, 0, r2o2, r2o8 },
		{ r2o2, r2o8, 0, 0, 0, r2o8 },
		{ 0, r2o8, r2o2, 0, 0, r2o8 },
		{ 0, r2o8, 0, r2o2, 0, r2o8 },
		{ 0, r2o8, 0, 0, r2o2, r2o8 } });
	const SpatialSoundMatrix FiveOneFilm_Cinema2M1Spatial(std::vector<std::vector<float>>
		//L, C, R, Ls, Rs, LFE
	{   { r2o2, r2o4, 0, 0, 0, r2o8 },
		{ 0, r2o4, r2o2, 0, 0, r2o8 },
		{ 0, r2o20, 0, r2o2, 0, r2o8 },
		{ 0, r2o20, 0, 0, r2o2, r2o8 },
		{ r2o2, r2o4, 0, 0, 0, r2o8 },
		{ 0, r2o4, r2o2, 0, 0, r2o8 },
		{ 0, r2o20, 0, r2o2, 0, r2o8 },
		{ 0, r2o20, 0, 0, r2o2, r2o8 } });
	const SpatialSoundMatrix FiveOneSmpte2M1Spatial(std::vector<std::vector<float>>
		//L, R, C, LFE, Ls, Rs
	{   { r2o2, 0, r2o8, r2o8, 0, 0 },
		{ 0, r2o2, r2o8, r2o8, 0, 0 },
		{ 0, 0, r2o8, r2o8, r2o2, 0 },
		{ 0, 0, r2o8, r2o8, 0, r2o2 },
		{ r2o2, 0, r2o8, r2o8, 0, 0 },
		{ 0, r2o2, r2o8, r2o8, 0, 0 },
		{ 0, 0, r2o8, r2o8, r2o2, 0 },
		{ 0, 0, r2o8, r2o8, 0, r2o2 } });
	const SpatialSoundMatrix FiveOneDts2M1Spatial(std::vector<std::vector<float>>
		//L, R, Ls, Rs, C, LFE
	{   { r2o2, 0, 0, 0, r2o8, r2o8 },
		{ 0, r2o2, 0, 0, r2o8, r2o8 },
		{ 0, 0, r2o2, 0, r2o8, r2o8 },
		{ 0, 0, 0, r2o2, r2o8, r2o8 },
		{ r2o2, 0, 0, 0, r2o8, r2o8 },
		{ 0, r2o2, 0, 0, r2o8, r2o8 },
		{ 0, 0, r2o2, 0, r2o8, r2o8 },
		{ 0, 0, 0, r2o2, r2o8, r2o8 } });
	const SpatialSoundMatrix SixOh2M1Spatial(std::vector<std::vector<float>> //BLANK
	{   { 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0 } });
	const SpatialSoundMatrix SevenOnePt2M1Spatial(std::vector<std::vector<float>>
		//L, C, R, Lss, Rss, Lsr, Rsr, LFE
	{   { r2o2, r2o8, 0, r2o4, 0, 0, 0, r2o8 },
		{ 0, r2o8, r2o2, 0, r2o4, 0, 0, r2o8 },
		{ 0, r2o8, 0, r2o4, 0, r2o2, 0, r2o8 },
		{ 0, r2o8, 0, 0, r2o4, 0, r2o2, r2o8 },
		{ r2o2, r2o8, 0, r2o4, 0, 0, 0, r2o8 },
		{ 0, r2o8, r2o2, 0, r2o4, 0, 0, r2o8 },
		{ 0, r2o8, 0, r2o4, 0, r2o2, 0, r2o8 },
		{ 0, r2o8, 0, 0, r2o4, 0, r2o2, r2o8 } });
	const SpatialSoundMatrix SevenOnePt_Cinema2M1Spatial(std::vector<std::vector<float>>
		//L, C, R, Lss, Rss, Lsr, Rsr, LFE ( std::vector<std::vector<float>>with front facing
	{   { r2o2, r2o4, 0, r2o4, 0, 0, 0, r2o8 },
		{ 0, r2o4, r2o2, 0, r2o4, 0, 0, r2o8 },
		{ 0, 0, 0, r2o4, 0, r2o2, 0, r2o8 },
		{ 0, 0, 0, 0, r2o4, 0, r2o2, r2o8 },
		{ r2o2, r2o4, 0, r2o4, 0, 0, 0, r2o8 },
		{ 0, r2o4, r2o2, 0, r2o4, 0, 0, r2o8 },
		{ 0, 0, 0, r2o4, 0, r2o2, 0, r2o8 },
		{ 0, 0, 0, 0, r2o4, 0, r2o2, r2o8 } });
	const SpatialSoundMatrix SevenZero_Cinema2M1Spatial(std::vector<std::vector<float>>
		//L, C, R, Lss, Rss, Lsr, Rsr ( std::vector<std::vector<float>>with front facing
	{   { r2o2, r2o4, 0, r2o4, 0, 0, 0 },
		{ 0, r2o4, r2o2, 0, r2o4, 0, 0 },
		{ 0, 0, 0, r2o4, 0, r2o2, 0 },
		{ 0, 0, 0, 0, r2o4, 0, r2o2 },
		{ r2o2, r2o4, 0, r2o4, 0, 0, 0 },
		{ 0, r2o4, r2o2, 0, r2o4, 0, 0 },
		{ 0, 0, 0, r2o4, 0, r2o2, 0 },
		{ 0, 0, 0, 0, r2o4, 0, r2o2 } });
	const SpatialSoundMatrix SevenOneSDDS2M1Spatial(std::vector<std::vector<float>>
		//L, Lc, C, Rc, R, Ls, Rs, LFE
	{   { r2o2, r2o4, r2o8, 0, 0, 0, 0, r2o8 },
		{ 0, 0, r2o8, r2o4, r2o2, 0, 0, r2o8 },
		{ 0, 0, r2o8, 0, 0, r2o2, 0, r2o8 },
		{ 0, 0, r2o8, 0, 0, 0, r2o2, r2o8 },
		{ r2o2, r2o4, r2o8, 0, 0, 0, 0, r2o8 },
		{ 0, 0, r2o8, r2o4, r2o2, 0, 0, r2o8 },
		{ 0, 0, r2o8, 0, 0, r2o2, 0, r2o8 },
		{ 0, 0, r2o8, 0, 0, 0, r2o2, r2o8 } });
	const SpatialSoundMatrix SevenZeroSDDS2M1Spatial(std::vector<std::vector<float>>
		//L, Lc, C, Rc, R, Ls, Rs
	{   { r2o2, r2o4, r2o8, 0, 0, 0, 0 },
		{ 0, 0, r2o8, r2o4, r2o2, 0, 0 },
		{ 0, 0, r2o8, 0, 0, r2o2, 0 },
		{ 0, 0, r2o8, 0, 0, 0, r2o2 },
		{ r2o2, r2o4, r2o8, 0, 0, 0, 0 },
		{ 0, 0, r2o8, r2o4, r2o2, 0, 0 },
		{ 0, 0, r2o8, 0, 0, r2o2, 0 },
		{ 0, 0, r2o8, 0, 0, 0, r2o2 } });
	const SpatialSoundMatrix SevenOneTwo2M1Spatial(std::vector<std::vector<float>>
		//L, C, R, Lss, Rss, Lsr, Rsr, LFE, Lts, Rts
	{   { r2o2, r2o4, 0, r2o4, 0, 0, 0, r2o8, 0.3750f, 0.1250f },
		{ 0, r2o4, r2o2, 0, r2o4, 0, 0, r2o8, 0.1250f, 0.3750f },
		{ 0, 0, 0, r2o4, 0, r2o2, 0, r2o8, 0.3750f, 0.1250f },
		{ 0, 0, 0, 0, r2o4, 0, r2o2, r2o8, 0.1250f, 0.3750f },
		{ r2o2, r2o4, 0, r2o4, 0, 0, 0, r2o8, 0, 0 },
		{ 0, r2o4, r2o2, 0, r2o4, 0, 0, r2o8, 0, 0 },
		{ 0, 0, 0, r2o4, 0, r2o2, 0, r2o8, 0, 0 },
		{ 0, 0, 0, 0, r2o4, 0, r2o2, r2o8, 0, 0 } });
	const SpatialSoundMatrix SevenZeroTwo2M1Spatial(std::vector<std::vector<float>>
		//L, C, R, Lss, Rss, Lsr, Rsr, Lts, Rts
	{   { r2o2, r2o4, 0, r2o4, 0, 0, 0, 0.3750f, 0.1250f },
		{ 0, r2o4, r2o2, 0, r2o4, 0, 0, 0.1250f, 0.3750f },
		{ 0, 0, 0, r2o4, 0, r2o2, 0, 0.3750f, 0.1250f },
		{ 0, 0, 0, 0, r2o4, 0, r2o2, 0.1250f, 0.3750f },
		{ r2o2, r2o4, 0, r2o4, 0, 0, 0, 0, 0 },
		{ 0, r2o4, r2o2, 0, r2o4, 0, 0, 0, 0 },
		{ 0, 0, 0, r2o4, 0, r2o2, 0, 0, 0 },
		{ 0, 0, 0, 0, r2o4, 0, r2o2, 0, 0 } });
	const SpatialSoundMatrix NineOne2M1Spatial(std::vector<std::vector<float>>
	{   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } });
	const SpatialSoundMatrix NineZero2M1Spatial(std::vector<std::vector<float>>
	{   { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0 } });
	const SpatialSoundMatrix FiveOneTwo2M1Spatial(std::vector<std::vector<float>>
		//L  C  R  Ls Rs LFE Lts Rts
	{   { r2o2, r2o4, 0, 0, 0, r2o8, 0.3750f, 0.1250f },
		{ 0, r2o4, r2o2, 0, 0, r2o8, 0.1250f, 0.3750f },
		{ 0, r2o20, 0, r2o2, 0, r2o8, 0.3750f, 0.1250f },
		{ 0, r2o20, 0, 0, r2o2, r2o8, 0.1250f, 0.3750f },
		{ r2o2, r2o4, 0, 0, 0, r2o8, 0, 0 },
		{ 0, r2o4, r2o2, 0, 0, r2o8, 0, 0 },
		{ 0, r2o20, 0, r2o2, 0, r2o8, 0, 0 },
		{ 0, r2o20, 0, 0, r2o2, r2o8, 0, 0 } });
	const SpatialSoundMatrix FiveZeroTwo2M1Spatial(std::vector<std::vector<float>>
		//L  C  R  Ls Rs Lts Rts
	{   { r2o2, r2o4, 0, 0, 0, 0.3750f, 0.1250f },
		{ 0, r2o4, r2o2, 0, 0, 0.1250f, 0.3750f },
		{ 0, r2o20, 0, r2o2, 0, 0.3750f, 0.1250f },
		{ 0, r2o20, 0, 0, r2o2, 0.1250f, 0.3750f },
		{ r2o2, r2o4, 0, 0, 0, 0, 0 },
		{ 0, r2o4, r2o2, 0, 0, 0, 0 },
		{ 0, r2o20, 0, r2o2, 0, 0, 0 },
		{ 0, r2o20, 0, 0, r2o2, 0, 0 } });
	const SpatialSoundMatrix FiveOneFour2M1Spatial(std::vector<std::vector<float>>
		//L  C  R  Ls Rs LFE FLts FRts BLts BRts
	{   { r2o2, r2o4, 0, 0, 0, r2o8, 0.7280f, 0.1250f, 0.1250f, 0.0210f },
		{ 0, r2o4, r2o2, 0, 0, r2o8, 0.1250f, 0.7280f, 0.0210f, 0.1250f },
		{ 0, r2o20, 0, r2o2, 0, r2o8, 0.1250f, 0.0210f, 0.7280f, 0.1250f },
		{ 0, r2o20, 0, 0, r2o2, r2o8, 0.0210f, 0.1250f, 0.1250f, 0.7280f },
		{ r2o2, r2o4, 0, 0, 0, r2o8, 0, 0, 0, 0 },
		{ 0, r2o4, r2o2, 0, 0, r2o8, 0, 0, 0, 0 },
		{ 0, r2o20, 0, r2o2, 0, r2o8, 0, 0, 0, 0 },
		{ 0, r2o20, 0, 0, r2o2, r2o8, 0, 0, 0, 0 } });
	const SpatialSoundMatrix FiveZeroFour2M1Spatial(std::vector<std::vector<float>>
		//L  C  R  Ls Rs FLts FRts BLts BRts
	{   { r2o2, r2o4, 0, 0, 0, 0.7280f, 0.1250f, 0.1250f, 0.0210f },
		{ 0, r2o4, r2o2, 0, 0, 0.1250f, 0.7280f, 0.0210f, 0.1250f },
		{ 0, r2o20, 0, r2o2, 0, 0.1250f, 0.0210f, 0.7280f, 0.1250f },
		{ 0, r2o20, 0, 0, r2o2, 0.0210f, 0.1250f, 0.1250f, 0.7280f },
		{ r2o2, r2o4, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, r2o4, r2o2, 0, 0, 0, 0, 0, 0 },
		{ 0, r2o20, 0, r2o2, 0, 0, 0, 0, 0 },
		{ 0, r2o20, 0, 0, r2o2, 0, 0, 0, 0 } });
	const SpatialSoundMatrix SevenOneFour2M1Spatial(std::vector<std::vector<float>>
		//L, C, R, Lss, Rss, Lsr, Rsr, LFE, FLts, FRts, BLts, BRts
	{   { r2o2, r2o4, 0, r2o4, 0, 0, 0, r2o8, 0.7280f, 0.1250f, 0.1250f, 0.0210f },
		{ 0, r2o4, r2o2, 0, r2o4, 0, 0, r2o8, 0.1250f, 0.7280f, 0.0210f, 0.1250f },
		{ 0, 0, 0, r2o4, 0, r2o2, 0, r2o8, 0.1250f, 0.0210f, 0.7280f, 0.1250f },
		{ 0, 0, 0, 0, r2o4, 0, r2o2, r2o8, 0.0210f, 0.1250f, 0.1250f, 0.7280f },
		{ r2o2, r2o4, 0, r2o4, 0, 0, 0, r2o8, 0, 0, 0, 0 },
		{ 0, r2o4, r2o2, 0, r2o4, 0, 0, r2o8, 0, 0, 0, 0 },
		{ 0, 0, 0, r2o4, 0, r2o2, 0, r2o8, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, r2o4, 0, r2o2, r2o8, 0, 0, 0, 0 } });
	const SpatialSoundMatrix SevenZeroFour2M1Spatial(std::vector<std::vector<float>>
		//L, C, R, Lss, Rss, Lsr, Rsr, FLts, FRts, BLts, BRts
	{   { r2o2, r2o4, 0, r2o4, 0, 0, 0, 0.7280f, 0.1250f, 0.1250f, 0.0210f },
		{ 0, r2o4, r2o2, 0, r2o4, 0, 0, 0.1250f, 0.7280f, 0.0210f, 0.1250f },
		{ 0, 0, 0, r2o4, 0, r2o2, 0, 0.1250f, 0.0210f, 0.7280f, 0.1250f },
		{ 0, 0, 0, 0, r2o4, 0, r2o2, 0.0210f, 0.1250f, 0.1250f, 0.7280f },
		{ r2o2, r2o4, 0, r2o4, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, r2o4, r2o2, 0, r2o4, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, r2o4, 0, r2o2, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, r2o4, 0, r2o2, 0, 0, 0, 0 } });

	const SpatialSoundMatrix NineOneFour2M1Spatial(std::vector<std::vector<float>> // ?
	{
		{}
	});
	const SpatialSoundMatrix NineOneSix2M1Spatial(std::vector<std::vector<float>> // ?
	{
		{ }
	});
	const SpatialSoundMatrix M1SpatialFaces2M1Spatial(std::vector<std::vector<float>>
		// FC, LC, BC, RC, TC, BC
	{   { r2o4, r2o4, 0, 0, r2o4, 0 },
		{ r2o4, 0, 0, r2o4, r2o4, 0 },
		{ 0, r2o4, r2o4, 0, r2o4, 0 },
		{ 0, 0, r2o4, r2o4, r2o4, 0 },
		{ r2o4, r2o4, 0, 0, 0, r2o4 },
		{ r2o4, 0, 0, r2o4, 0, r2o4 },
		{ 0, r2o4, r2o4, 0, 0, r2o4 },
		{ 0, 0, r2o4, r2o4, 0, r2o4 } });

	// --- M1Spatial && M1Spatial+Stereo to Surround ---;
	//TODO: fix front to rear ratio
	const SpatialSoundMatrix M1Spatial2Stereo(std::vector<std::vector<float>>
	{   { r2o8, 0, r2o8, 0, r2o8, 0, r2o8, 0 },
		{ 0, r2o8, 0, r2o8, 0, r2o8, 0, r2o8 } });
	const SpatialSoundMatrix M1SpatialS2Stereo(std::vector<std::vector<float>>
	{   { r2o8, 0, r2o8, 0, r2o8, 0, r2o8, 0, 1, 0 },
		{ 0, r2o8, 0, r2o8, 0, r2o8, 0, r2o8, 0, 1 } });

	const SpatialSoundMatrix M1Spatial2Stereo_Cinema(std::vector<std::vector<float>>
	{   { r2o8, 0, r2o8, 0, r2o8, 0, r2o8, 0 },
		{ 0, r2o8, 0, r2o8, 0, r2o8, 0, r2o8 } });
	const SpatialSoundMatrix M1SpatialS2Stereo_Cinema(std::vector<std::vector<float>>
	{   { r2o8, 0, r2o8, 0, r2o8, 0, r2o8, 0, 1, 0 },
		{ 0, r2o8, 0, r2o8, 0, r2o8, 0, r2o8, 0, 1 } });

	const SpatialSoundMatrix M1Spatial2FiveOh(std::vector<std::vector<float>>
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0 },
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 },
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0 },
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0 },
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2 } });
	const SpatialSoundMatrix M1SpatialS2FiveOh(std::vector<std::vector<float>>
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1, 0 },
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, 0, 0 },
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1 },
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 },
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 } });

	const SpatialSoundMatrix M1Spatial2FiveOneFilm(std::vector<std::vector<float>>
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0 },
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 },
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0 },
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0 },
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2 },
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 } });
	const SpatialSoundMatrix M1SpatialS2FiveOneFilm(std::vector<std::vector<float>>
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1, 0 },
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, 0, 0 },
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1 },
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 },
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 },
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o2, r2o2 } });

	const SpatialSoundMatrix M1Spatial2FiveOneFilm_Cinema(std::vector<std::vector<float>>
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0 },
		{ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0 },
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0 },
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0 },
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2 },
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 } });
	const SpatialSoundMatrix M1SpatialS2FiveOneFilm_Cinema(std::vector<std::vector<float>>
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1, 0 },
		{ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0 },
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1 },
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 },
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 },
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o2, r2o2 } });

	const SpatialSoundMatrix M1Spatial2FiveOneSmpte(std::vector<std::vector<float>>
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0 },
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0 },
		{ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0 },
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 },
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0 },
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2 } });
	const SpatialSoundMatrix M1SpatialS2FiveOneSmpte(std::vector<std::vector<float>>
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1, 0 },
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1 },
		{ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0, 0, 0 },
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o2, r2o2 },
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 },
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 } });

	const SpatialSoundMatrix M1Spatial2FiveOneDts(std::vector<std::vector<float>>
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0 },
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0 },
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0 },
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2 },
		{ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0 },
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 } });
	const SpatialSoundMatrix M1SpatialS2FiveOneDts(std::vector<std::vector<float>>
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1, 0 },
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1 },
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 },
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 },
		{ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0, 0, 0 },
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o2, r2o2 } });

	const SpatialSoundMatrix M1Spatial2SevenOnePt(std::vector<std::vector<float>>
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0 },
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 },
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0 },
		{ r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0 },
		{ 0, r2o4, 0, r2o4, 0, r2o4, 0, r2o4 },
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0 },
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2 },
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 } });
	const SpatialSoundMatrix M1SpatialS2SevenOnePt(std::vector<std::vector<float>>
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1, 0 },
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, 0, 0 },
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1 },
		{ r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0, 0, 0 },
		{ 0, r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0, 0 },
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 },
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 },
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o2, r2o2 } });

	const SpatialSoundMatrix M1Spatial2SevenOnePt_Cinema(std::vector<std::vector<float>>
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0 },
		{ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0 },
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0 },
		{ r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0 },
		{ 0, r2o4, 0, r2o4, 0, r2o4, 0, r2o4 },
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0 },
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2 },
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 } });
	const SpatialSoundMatrix M1SpatialS2SevenOnePt_Cinema(std::vector<std::vector<float>>
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1, 0 },
		{ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0, 0, 0 },
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1 },
		{ r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0, 0, 0 },
		{ 0, r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0, 0 },
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 },
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 },
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o2, r2o2 } });

	const SpatialSoundMatrix M1Spatial2SevenZero_Cinema(std::vector<std::vector<float>>
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0 },
		{ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0 },
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0 },
		{ r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0 },
		{ 0, r2o4, 0, r2o4, 0, r2o4, 0, r2o4 },
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0 },
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2 } });
	const SpatialSoundMatrix M1SpatialS2SevenZero_Cinema(std::vector<std::vector<float>>
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1, 0 },
		{ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0, 0, 0 },
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1 },
		{ r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0, 0, 0 },
		{ 0, r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0, 0 },
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 },
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 } });

	const SpatialSoundMatrix M1Spatial2SevenOneSDDS(std::vector<std::vector<float>>
		//1  2  3  4  5  6  7  8
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0 }, //L
		{ 0.375f, 0.125f, 0, 0, 0.375f, 0.125f, 0, 0 }, //Lc
		{ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0 }, //C
		{ 0.125f, 0.375f, 0, 0, 0.125f, 0.375f, 0, 0 }, //Rc
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0 }, //R
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0 }, //Ls
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2 }, //Rs
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 } }); //LFE
	const SpatialSoundMatrix M1SpatialS2SevenOneSDDS(std::vector<std::vector<float>>
		//1  2  3  4  5  6  7  8
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0 }, //L
		{ 0.375f, 0.125f, 0, 0, 0.375f, 0.125f, 0, 0, r2o2, 0 }, //Lc
		{ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0, 0, 0 }, //C
		{ 0.125f, 0.375f, 0, 0, 0.125f, 0.375f, 0, 0, 0, r2o2 }, //Rc
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, r2o2 }, //R
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 }, //Ls
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 }, //Rs
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o2, r2o2 } }); //LFE

	const SpatialSoundMatrix M1Spatial2SevenZeroSDDS(std::vector<std::vector<float>>
		//1  2  3  4  5  6  7  8
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0 }, //L
		{ 0.375f, 0.125f, 0, 0, 0.375f, 0.125f, 0, 0 }, //Lc
		{ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0 }, //C
		{ 0.125f, 0.375f, 0, 0, 0.125f, 0.375f, 0, 0 }, //Rc
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0 }, //R
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0 }, //Ls
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2 } }); //Rs
	const SpatialSoundMatrix M1SpatialS2SevenZeroSDDS(std::vector<std::vector<float>>
		//1  2  3  4  5  6  7  8  9  10
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0 }, //L
		{ 0.375f, 0.125f, 0, 0, 0.375f, 0.125f, 0, 0, r2o2, 0 }, //Lc
		{ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0, 0, 0 }, //C
		{ 0.125f, 0.375f, 0, 0, 0.125f, 0.375f, 0, 0, 0, r2o2 }, //Rc
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, r2o2 }, //R
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 }, //Ls
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 } }); //Rs

	const SpatialSoundMatrix M1Spatial2SevenOneTwo(std::vector<std::vector<float>>
		//1  2  3  4  5  6  7  8
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0 },//L
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 },//C
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0 },//R
		{ r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0 },//Lss
		{ 0, r2o4, 0, r2o4, 0, r2o4, 0, r2o4 },//Rss
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0 },//Lsr
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2 },//Rsr
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 },//LFE
		{ r2o2, 0, r2o2, 0, 0, 0, 0, 0 },//Lts
		{ 0, r2o2, 0, r2o2, 0, 0, 0, 0 } });//Rts
	const SpatialSoundMatrix M1SpatialS2SevenOneTwo(std::vector<std::vector<float>>
		//1  2  3  4  5  6  7  8  9  10
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1, 0 },//L
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, 0, 0 },//C
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1 },//R
		{ r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0, 0, 0 },//Lss
		{ 0, r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0, 0 },//Rss
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 },//Lsr
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 },//Rsr
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o2, r2o2 },//LFE
		{ r2o2, 0, r2o2, 0, 0, 0, 0, 0, 0, 0 },//Lts
		{ 0, r2o2, 0, r2o2, 0, 0, 0, 0, 0, 0 } });//Rts

	const SpatialSoundMatrix M1Spatial2SevenZeroTwo(std::vector<std::vector<float>>
		//1  2  3  4  5  6  7  8
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0 },//L
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 },//C
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0 },//R
		{ r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0 },//Lss
		{ 0, r2o4, 0, r2o4, 0, r2o4, 0, r2o4 },//Rss
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0 },//Lsr
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2 },//Rsr
		{ r2o2, 0, r2o2, 0, 0, 0, 0, 0 },//Lts
		{ 0, r2o2, 0, r2o2, 0, 0, 0, 0 } });//Rts
	const SpatialSoundMatrix M1SpatialS2SevenZeroTwo(std::vector<std::vector<float>>
		//1  2  3  4  5  6  7  8  9  10
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1, 0 },//L
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, 0, 0 },//C
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1 },//R
		{ r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0, 0, 0 },//Lss
		{ 0, r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0, 0 },//Rss
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 },//Lsr
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 },//Rsr
		{ r2o2, 0, r2o2, 0, 0, 0, 0, 0, 0, 0 },//Lts
		{ 0, r2o2, 0, r2o2, 0, 0, 0, 0, 0, 0 } });//Rts

	const SpatialSoundMatrix M1Spatial2NineOne(std::vector<std::vector<float>>
		//1  2  3  4  5  6  7  8
	{   { 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 } });
	const SpatialSoundMatrix M1SpatialS2NineOne(std::vector<std::vector<float>>
		//1  2  3  4  5  6  7  8  9  10
	{   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } });

	const SpatialSoundMatrix M1Spatial2NineZero(std::vector<std::vector<float>>
		//1  2  3  4  5  6  7  8
	{   { 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 } });
	const SpatialSoundMatrix M1SpatialS2NineZero(std::vector<std::vector<float>>
		//1  2  3  4  5  6  7  8  9  10
	{   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } });

	const SpatialSoundMatrix M1Spatial2FiveOneTwo(std::vector<std::vector<float>>
		//1  2  3  4  5  6  7  8
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0 },
		{ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0 },
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0 },
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0 },
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2 },
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 },
		{ r2o2, 0, r2o2, 0, 0, 0, 0, 0 },//Lts
		{ 0, r2o2, 0, r2o2, 0, 0, 0, 0 } });//Rts
	const SpatialSoundMatrix M1SpatialS2FiveOneTwo(std::vector<std::vector<float>>
		//1  2  3  4  5  6  7  8
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1, 0 },
		{ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0, 0, 0 },
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1 },
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 },
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 },
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o2, r2o2 },
		{ r2o2, 0, r2o2, 0, 0, 0, 0, 0, 0, 0 },//Lts
		{ 0, r2o2, 0, r2o2, 0, 0, 0, 0, 0, 0 } });//Rts

	const SpatialSoundMatrix M1Spatial2FiveZeroTwo(std::vector<std::vector<float>>
		//1  2  3  4  5  6  7  8
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0 },
		{ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0 },
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0 },
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0 },
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2 },
		{ r2o2, 0, r2o2, 0, 0, 0, 0, 0 },//Lts
		{ 0, r2o2, 0, r2o2, 0, 0, 0, 0 } });//Rts
	const SpatialSoundMatrix M1SpatialS2FiveZeroTwo(std::vector<std::vector<float>>
		//1  2  3  4  5  6  7  8
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1, 0 },
		{ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0, 0, 0 },
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1 },
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 },
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 },
		{ r2o2, 0, r2o2, 0, 0, 0, 0, 0, 0, 0 },//Lts
		{ 0, r2o2, 0, r2o2, 0, 0, 0, 0, 0, 0 } });//Rts

	const SpatialSoundMatrix M1Spatial2FiveOneFour(std::vector<std::vector<float>>
		//1  2  3  4  5  6  7  8
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0 },
		{ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0 },
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0 },
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0 },
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2 },
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 },
		{ 0.7280f, 0.1250f, 0.1250f, 0.0210f, 0, 0, 0, 0 },//FLts
		{ 0.1250f, 0.7280f, 0.0210f, 0.1250f, 0, 0, 0, 0 },//FRts
		{ 0.1250f, 0.0210f, 0.7280f, 0.1250f, 0, 0, 0, 0 },//BLts
		{ 0.0210f, 0.1250f, 0.1250f, 0.7280f, 0, 0, 0, 0 } });//BRts
	const SpatialSoundMatrix M1SpatialS2FiveOneFour(std::vector<std::vector<float>>
		//1  2  3  4  5  6  7  8
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1, 0 },
		{ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0, 0, 0 },
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1 },
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 },
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 },
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o2, r2o2 },
		{ 0.7280f, 0.1250f, 0.1250f, 0.0210f, 0, 0, 0, 0, 0, 0 },//FLts
		{ 0.1250f, 0.7280f, 0.0210f, 0.1250f, 0, 0, 0, 0, 0, 0 },//FRts
		{ 0.1250f, 0.0210f, 0.7280f, 0.1250f, 0, 0, 0, 0, 0, 0 },//BLts
		{ 0.0210f, 0.1250f, 0.1250f, 0.7280f, 0, 0, 0, 0, 0, 0 } });//BRts

	const SpatialSoundMatrix M1Spatial2FiveZeroFour(std::vector<std::vector<float>>
		//1  2  3  4  5  6  7  8
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0 },
		{ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0 },
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0 },
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0 },
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2 },
		{ 0.7280f, 0.1250f, 0.1250f, 0.0210f, 0, 0, 0, 0 },//FLts
		{ 0.1250f, 0.7280f, 0.0210f, 0.1250f, 0, 0, 0, 0 },//FRts
		{ 0.1250f, 0.0210f, 0.7280f, 0.1250f, 0, 0, 0, 0 },//BLts
		{ 0.0210f, 0.1250f, 0.1250f, 0.7280f, 0, 0, 0, 0 } });//BRts
	const SpatialSoundMatrix M1SpatialS2FiveZeroFour(std::vector<std::vector<float>>
		//1  2  3  4  5  6  7  8
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1, 0 },
		{ r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0, 0, 0 },
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1 },
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 },
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 },
		{ 0.7280f, 0.1250f, 0.1250f, 0.0210f, 0, 0, 0, 0, 0, 0 },//FLts
		{ 0.1250f, 0.7280f, 0.0210f, 0.1250f, 0, 0, 0, 0, 0, 0 },//FRts
		{ 0.1250f, 0.0210f, 0.7280f, 0.1250f, 0, 0, 0, 0, 0, 0 },//BLts
		{ 0.0210f, 0.1250f, 0.1250f, 0.7280f, 0, 0, 0, 0, 0, 0 } });//BRts

	const SpatialSoundMatrix M1Spatial2SevenOneFour(std::vector<std::vector<float>>
		//1  2  3  4  5  6  7  8
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0 },//L
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 },//C
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0 },//R
		{ r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0 },//Lss
		{ 0, r2o4, 0, r2o4, 0, r2o4, 0, r2o4 },//Rss
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0 },//Lsr
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2 },//Rsr
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 },//LFE
		{ 0.7280f, 0.1250f, 0.1250f, 0.0210f, 0, 0, 0, 0 },//FLts
		{ 0.1250f, 0.7280f, 0.0210f, 0.1250f, 0, 0, 0, 0 },//FRts
		{ 0.1250f, 0.0210f, 0.7280f, 0.1250f, 0, 0, 0, 0 },//BLts
		{ 0.0210f, 0.1250f, 0.1250f, 0.7280f, 0, 0, 0, 0 } });//BRts
	const SpatialSoundMatrix M1SpatialS2SevenOneFour(std::vector<std::vector<float>>
		//1  2  3  4  5  6  7  8
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1, 0 },//L
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, 0, 0 },//C
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1 },//R
		{ r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0, 0, 0 },//Lss
		{ 0, r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0, 0 },//Rss
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 },//Lsr
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 },//Rsr
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o2, r2o2 },//LFE
		{ 0.7280f, 0.1250f, 0.1250f, 0.0210f, 0, 0, 0, 0, 0, 0 },//FLts
		{ 0.1250f, 0.7280f, 0.0210f, 0.1250f, 0, 0, 0, 0, 0, 0 },//FRts
		{ 0.1250f, 0.0210f, 0.7280f, 0.1250f, 0, 0, 0, 0, 0, 0 },//BLts
		{ 0.0210f, 0.1250f, 0.1250f, 0.7280f, 0, 0, 0, 0, 0, 0 } });//BRts

	const SpatialSoundMatrix M1Spatial2SevenZeroFour(std::vector<std::vector<float>>
		//1  2  3  4  5  6  7  8
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0 },//L
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 },//C
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0 },//R
		{ r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0 },//Lss
		{ 0, r2o4, 0, r2o4, 0, r2o4, 0, r2o4 },//Rss
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0 },//Lsr
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2 },//Rsr
		{ 0.7280f, 0.1250f, 0.1250f, 0.0210f, 0, 0, 0, 0 },//FLts
		{ 0.1250f, 0.7280f, 0.0210f, 0.1250f, 0, 0, 0, 0 },//FRts
		{ 0.1250f, 0.0210f, 0.7280f, 0.1250f, 0, 0, 0, 0 },//BLts
		{ 0.0210f, 0.1250f, 0.1250f, 0.7280f, 0, 0, 0, 0 } });//BRts
	const SpatialSoundMatrix M1SpatialS2SevenZeroFour(std::vector<std::vector<float>>
		//1  2  3  4  5  6  7  8
	{   { r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1, 0 },//L
		{ r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, 0, 0 },//C
		{ 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0, 1 },//R
		{ r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0, 0, 0 },//Lss
		{ 0, r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0, 0 },//Rss
		{ 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0, 0 },//Lsr
		{ 0, 0, 0, r2o2, 0, 0, 0, r2o2, 0, 0 },//Rsr
		{ 0.7280f, 0.1250f, 0.1250f, 0.0210f, 0, 0, 0, 0, 0, 0 },//FLts
		{ 0.1250f, 0.7280f, 0.0210f, 0.1250f, 0, 0, 0, 0, 0, 0 },//FRts
		{ 0.1250f, 0.0210f, 0.7280f, 0.1250f, 0, 0, 0, 0, 0, 0 },//BLts
		{ 0.0210f, 0.1250f, 0.1250f, 0.7280f, 0, 0, 0, 0, 0, 0 } });//BRts

	const SpatialSoundMatrix M1Spatial2NineOneFour(std::vector<std::vector<float>> // ?
		//1  2  3  4  5  6  7  8
	{   { 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 } });
	const SpatialSoundMatrix M1SpatialS2NineOneFour(std::vector<std::vector<float>> // ?
		//1  2  3  4  5  6  7  8
	{   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } });

	const SpatialSoundMatrix M1Spatial2NineOneSix(std::vector<std::vector<float>> // ?
		//1  2  3  4  5  6  7  8
	{   { 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 } });
	const SpatialSoundMatrix M1SpatialS2NineOneSix(std::vector<std::vector<float>> // ?
		//1  2  3  4  5  6  7  8
	{   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } });

	const SpatialSoundMatrix M1Spatial2MarcoSixteen(std::vector<std::vector<float>>
		//1  2  3  4  5  6  7  8
	{   { oor8, 0, 0, 0, oor8, 0, 0, 0 }, //mid -45
		{ oor16, oor16, 0, 0, oor16, oor16, 0, 0 }, //mid 0
		{ 0, oor8, 0, 0, 0, oor8, 0, 0 }, //mid 45
		{ 0, oor16, 0, oor16, 0, oor16, 0, oor16 }, //mid 90
		{ 0, 0, 0, oor8, 0, 0, 0, oor8 }, //mid 135
		{ 0, 0, oor16, oor16, 0, 0, oor16, oor16 }, //mid 180
		{ 0, 0, oor8, 0, 0, 0, oor8, 0 }, //mid -135
		{ oor16, 0, oor16, 0, oor16, 0, oor16, 0 }, //mid -90
		{ oor4, 0, 0, 0, 0, 0, 0, 0 }, //top -45
		{ 0, oor4, 0, 0, 0, 0, 0, 0 }, //top 45
		{ 0, 0, 0, oor4, 0, 0, 0, 0 }, //top 135
		{ 0, 0, oor4, 0, 0, 0, 0, 0 }, //top -135
		{ 0, 0, 0, 0, oor4, 0, 0, 0 }, //bottom -45
		{ 0, 0, 0, 0, 0, oor4, 0, 0 }, //bottom 45
		{ 0, 0, 0, 0, 0, 0, 0, oor4 }, //bottom 135
		{ 0, 0, 0, 0, 0, 0, oor4, 0 } }); //bottom -135
};

namespace Mach1TranscodeFormats {

	const enum FormatType {
		Empty = 0,
		FuMa,
		ACNSN3D,
		M1Horizon,
		M1HorizonS,
		M1HorizonPairs,
		M1Spatial,
		M1SpatialS,
		M1SpatialPairs,
		Mono, //
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
		TTPoints,
	};
};


namespace Mach1TranscodeConstants {

	const std::map<Mach1TranscodeFormats::FormatType, char*> FormatNames = {
		{ Mach1TranscodeFormats::Empty, "None" },
		{ Mach1TranscodeFormats::FuMa, "FuMa" },
		{ Mach1TranscodeFormats::ACNSN3D, "ACNSN3D" },
		{ Mach1TranscodeFormats::M1Horizon, "M1Horizon" },
		{ Mach1TranscodeFormats::M1HorizonS, "M1HorizonS" },
		{ Mach1TranscodeFormats::M1HorizonPairs, "M1HorizonPairs" },
		{ Mach1TranscodeFormats::M1Spatial, "M1Spatial" },
		{ Mach1TranscodeFormats::M1SpatialS, "M1SpatialS" },
		{ Mach1TranscodeFormats::M1SpatialPairs, "M1SpatialPairs" },
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
		{ Mach1TranscodeFormats::FiveZeroFour, "FiveZeroFour" },
		{ Mach1TranscodeFormats::SevenOneFour, "SevenOneFour" },
		{ Mach1TranscodeFormats::SevenZeroFour, "SevenZeroFour" },
		{ Mach1TranscodeFormats::SevenZero_Cinema, "SevenZero_Cinema" },
		{ Mach1TranscodeFormats::M1SpatialFaces, "M1SpatialFaces" },
		{ Mach1TranscodeFormats::NineOneFour, "NineOneFour" },
		{ Mach1TranscodeFormats::NineOneSix, "NineOneSix" },
		{ Mach1TranscodeFormats::MarcoSixteen, "MarcoSixteen" },
		{ Mach1TranscodeFormats::TTPoints, "TT Points" }
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
		{ Mach1TranscodeFormats::FiveZeroFour, 9 },
		{ Mach1TranscodeFormats::SevenOneFour, 12 },
		{ Mach1TranscodeFormats::SevenZeroFour, 11 },
		{ Mach1TranscodeFormats::SevenZero_Cinema, 7 },
		{ Mach1TranscodeFormats::M1SpatialFaces, 6 },
		{ Mach1TranscodeFormats::NineOneFour, 14 },
		{ Mach1TranscodeFormats::NineOneSix, 16 },
		{ Mach1TranscodeFormats::MarcoSixteen, 16 }
	};

	const int MAXCHANS = 16;
	const int NUMFMTS = 44;

	const std::map<std::pair<Mach1TranscodeFormats::FormatType, Mach1TranscodeFormats::FormatType>, const SpatialSoundMatrix*> FormatMatrix = {
		{std::make_pair(Mach1TranscodeFormats::FuMa, Mach1TranscodeFormats::ACNSN3D), &Mach1TranscodeConstantsInternal::FuMa2ACNSN3D},
		{std::make_pair(Mach1TranscodeFormats::FuMaO2A, Mach1TranscodeFormats::ACNSN3DO2A), &Mach1TranscodeConstantsInternal::FuMaO2A2ACNSN3DO2A},
		{std::make_pair(Mach1TranscodeFormats::FuMaO3A, Mach1TranscodeFormats::ACNSN3DO3A), &Mach1TranscodeConstantsInternal::FuMaO3A2ACNSN3DO3A},
		{std::make_pair(Mach1TranscodeFormats::ACNSN3D, Mach1TranscodeFormats::FuMa), &Mach1TranscodeConstantsInternal::ACNSN3D2FuMa},
		{std::make_pair(Mach1TranscodeFormats::ACNSN3DO2A, Mach1TranscodeFormats::FuMaO2A), &Mach1TranscodeConstantsInternal::ACNSN3DO2A2FuMaO2A},
		{std::make_pair(Mach1TranscodeFormats::ACNSN3DO3A, Mach1TranscodeFormats::FuMaO3A), &Mach1TranscodeConstantsInternal::ACNSN3DO3A2FuMaO3A},
		{std::make_pair(Mach1TranscodeFormats::M1Horizon, Mach1TranscodeFormats::FuMa), &Mach1TranscodeConstantsInternal::M1Horizon2FuMa},
		{std::make_pair(Mach1TranscodeFormats::M1Horizon, Mach1TranscodeFormats::ACNSN3D), &Mach1TranscodeConstantsInternal::M1Horizon2ACNSN3D},
		{std::make_pair(Mach1TranscodeFormats::M1Horizon, Mach1TranscodeFormats::ACNSN3DO2A), &Mach1TranscodeConstantsInternal::M1Horizon2ACNSN3DO2A},
		{std::make_pair(Mach1TranscodeFormats::M1Horizon, Mach1TranscodeFormats::ACNSN3DO3A), &Mach1TranscodeConstantsInternal::M1Horizon2ACNSN3DO3A},
		{std::make_pair(Mach1TranscodeFormats::ACNSN3D, Mach1TranscodeFormats::M1Horizon), &Mach1TranscodeConstantsInternal::ACNSN3D2M1Horizon},
		{std::make_pair(Mach1TranscodeFormats::ACNSN3DO2A, Mach1TranscodeFormats::M1Horizon), &Mach1TranscodeConstantsInternal::ACNSN3DO2A2M1Horizon},
		{std::make_pair(Mach1TranscodeFormats::ACNSN3DO3A, Mach1TranscodeFormats::M1Horizon), &Mach1TranscodeConstantsInternal::ACNSN3DO3A2M1Horizon},
		{std::make_pair(Mach1TranscodeFormats::M1Horizon, Mach1TranscodeFormats::M1HorizonPairs), &Mach1TranscodeConstantsInternal::M1Horizon2M1HorizonPairs},
		{std::make_pair(Mach1TranscodeFormats::M1Horizon, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::M1Horizon2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::M1Horizon, Mach1TranscodeFormats::M1SpatialPairs), &Mach1TranscodeConstantsInternal::M1Horizon2M1SpatialPairs},
		{std::make_pair(Mach1TranscodeFormats::M1Horizon, Mach1TranscodeFormats::FiveOh), &Mach1TranscodeConstantsInternal::M1Horizon2FiveOh},
		{std::make_pair(Mach1TranscodeFormats::M1Horizon, Mach1TranscodeFormats::FiveOneFilm), &Mach1TranscodeConstantsInternal::M1Horizon2FiveOneFilm},
		{std::make_pair(Mach1TranscodeFormats::M1Horizon, Mach1TranscodeFormats::FiveOneFilm_Cinema), &Mach1TranscodeConstantsInternal::M1Horizon2FiveOneFilm_Cinema},
		{std::make_pair(Mach1TranscodeFormats::M1Horizon, Mach1TranscodeFormats::FiveOneSmpte), &Mach1TranscodeConstantsInternal::M1Horizon2FiveOneSmpte},
		{std::make_pair(Mach1TranscodeFormats::M1Horizon, Mach1TranscodeFormats::FiveOneDts), &Mach1TranscodeConstantsInternal::M1Horizon2FiveOneDts},
		{std::make_pair(Mach1TranscodeFormats::M1Horizon, Mach1TranscodeFormats::SixOh), &Mach1TranscodeConstantsInternal::M1Horizon2SixOh},
		{std::make_pair(Mach1TranscodeFormats::FiveOh, Mach1TranscodeFormats::M1Horizon), &Mach1TranscodeConstantsInternal::FiveOh2M1Horizon},
		{std::make_pair(Mach1TranscodeFormats::FiveOneFilm, Mach1TranscodeFormats::M1Horizon), &Mach1TranscodeConstantsInternal::FiveOneFilm2M1Horizon},
		{std::make_pair(Mach1TranscodeFormats::FiveOneFilm_Cinema, Mach1TranscodeFormats::M1Horizon), &Mach1TranscodeConstantsInternal::FiveOneFilm_Cinema2M1Horizon},
		{std::make_pair(Mach1TranscodeFormats::FiveOneSmpte, Mach1TranscodeFormats::M1Horizon), &Mach1TranscodeConstantsInternal::FiveOneSmpte2M1Horizon},
		{std::make_pair(Mach1TranscodeFormats::FiveOneDts, Mach1TranscodeFormats::M1Horizon), &Mach1TranscodeConstantsInternal::FiveOneDts2M1Horizon},
		{std::make_pair(Mach1TranscodeFormats::SixOh, Mach1TranscodeFormats::M1Horizon), &Mach1TranscodeConstantsInternal::SixOh2M1Horizon},
		{std::make_pair(Mach1TranscodeFormats::M1HorizonS, Mach1TranscodeFormats::FuMa), &Mach1TranscodeConstantsInternal::M1HorizonS2FuMa},
		{std::make_pair(Mach1TranscodeFormats::M1HorizonS, Mach1TranscodeFormats::ACNSN3D), &Mach1TranscodeConstantsInternal::M1HorizonS2ACNSN3D},
		{std::make_pair(Mach1TranscodeFormats::M1HorizonS, Mach1TranscodeFormats::M1Horizon), &Mach1TranscodeConstantsInternal::M1HorizonS2M1Horizon},
		{std::make_pair(Mach1TranscodeFormats::M1HorizonS, Mach1TranscodeFormats::M1HorizonPairs), &Mach1TranscodeConstantsInternal::M1HorizonS2M1HorizonPairs},
		{std::make_pair(Mach1TranscodeFormats::M1HorizonS, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::M1HorizonS2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::M1HorizonS, Mach1TranscodeFormats::M1SpatialPairs), &Mach1TranscodeConstantsInternal::M1HorizonS2M1SpatialPairs},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::M1Spatial2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::FuMa), &Mach1TranscodeConstantsInternal::M1Spatial2FuMa},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::FuMaO2A), &Mach1TranscodeConstantsInternal::M1Spatial2FuMaO2A},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::TBE), &Mach1TranscodeConstantsInternal::M1Spatial2TBE},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::ACNSN3D), &Mach1TranscodeConstantsInternal::M1Spatial2ACNSN3D},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::ACNSN3DO2A), &Mach1TranscodeConstantsInternal::M1Spatial2ACNSN3DO2A},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::ACNSN3DO3A), &Mach1TranscodeConstantsInternal::M1Spatial2ACNSN3DO3A},
		{std::make_pair(Mach1TranscodeFormats::ACNSN3D, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::ACNSN3D2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::ACNSN3DO2A, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::ACNSN3DO2A2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::ACNSN3DO3A, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::ACNSN3DO3A2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::TBE, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::TBE2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::M1Horizon), &Mach1TranscodeConstantsInternal::M1Spatial2M1Horizon},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::M1HorizonPairs), &Mach1TranscodeConstantsInternal::M1Spatial2M1HorizonPairs},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::M1SpatialFaces), &Mach1TranscodeConstantsInternal::M1Spatial2M1SpatialFaces},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::M1SpatialPairs), &Mach1TranscodeConstantsInternal::M1Spatial2M1SpatialPairs},
		{std::make_pair(Mach1TranscodeFormats::M1SpatialS, Mach1TranscodeFormats::FuMa), &Mach1TranscodeConstantsInternal::M1SpatialS2FuMa},
		{std::make_pair(Mach1TranscodeFormats::M1SpatialS, Mach1TranscodeFormats::ACNSN3D), &Mach1TranscodeConstantsInternal::M1SpatialS2ACNSN3D},
		{std::make_pair(Mach1TranscodeFormats::M1SpatialS, Mach1TranscodeFormats::M1Horizon), &Mach1TranscodeConstantsInternal::M1SpatialS2M1Horizon},
		{std::make_pair(Mach1TranscodeFormats::M1SpatialS, Mach1TranscodeFormats::M1HorizonPairs), &Mach1TranscodeConstantsInternal::M1SpatialS2M1HorizonPairs},
		{std::make_pair(Mach1TranscodeFormats::M1SpatialS, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::M1SpatialS2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::M1SpatialS, Mach1TranscodeFormats::M1SpatialPairs), &Mach1TranscodeConstantsInternal::M1SpatialS2M1SpatialPairs},
		{std::make_pair(Mach1TranscodeFormats::Mono, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::Mono2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::Stereo, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::Stereo2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::Stereo_Cinema, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::Stereo_Cinema2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::LCR, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::LCR2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::FiveOh, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::FiveOh2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::FiveOneFilm, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::FiveOneFilm2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::FiveOneFilm_Cinema, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::FiveOneFilm_Cinema2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::FiveOneSmpte, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::FiveOneSmpte2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::FiveOneDts, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::FiveOneDts2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::SixOh, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::SixOh2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::SevenOnePt, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::SevenOnePt2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::SevenOnePt_Cinema, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::SevenOnePt_Cinema2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::SevenZero_Cinema, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::SevenZero_Cinema2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::SevenOneSDDS, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::SevenOneSDDS2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::SevenZeroSDDS, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::SevenZeroSDDS2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::SevenOneTwo, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::SevenOneTwo2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::SevenZeroTwo, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::SevenZeroTwo2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::NineOne, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::NineOne2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::NineZero, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::NineZero2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::FiveOneTwo, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::FiveOneTwo2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::FiveZeroTwo, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::FiveZeroTwo2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::FiveOneFour, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::FiveOneFour2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::FiveZeroFour, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::FiveZeroFour2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::SevenOneFour, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::SevenOneFour2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::SevenZeroFour, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::SevenZeroFour2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::NineOneFour, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::NineOneFour2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::NineOneSix, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::NineOneSix2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::M1SpatialFaces, Mach1TranscodeFormats::M1Spatial), &Mach1TranscodeConstantsInternal::M1SpatialFaces2M1Spatial},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::Stereo), &Mach1TranscodeConstantsInternal::M1Spatial2Stereo},
		{std::make_pair(Mach1TranscodeFormats::M1SpatialS, Mach1TranscodeFormats::Stereo), &Mach1TranscodeConstantsInternal::M1SpatialS2Stereo},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::Stereo_Cinema), &Mach1TranscodeConstantsInternal::M1Spatial2Stereo_Cinema},
		{std::make_pair(Mach1TranscodeFormats::M1SpatialS, Mach1TranscodeFormats::Stereo_Cinema), &Mach1TranscodeConstantsInternal::M1SpatialS2Stereo_Cinema},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::FiveOh), &Mach1TranscodeConstantsInternal::M1Spatial2FiveOh},
		{std::make_pair(Mach1TranscodeFormats::M1SpatialS, Mach1TranscodeFormats::FiveOh), &Mach1TranscodeConstantsInternal::M1SpatialS2FiveOh},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::FiveOneFilm), &Mach1TranscodeConstantsInternal::M1Spatial2FiveOneFilm},
		{std::make_pair(Mach1TranscodeFormats::M1SpatialS, Mach1TranscodeFormats::FiveOneFilm), &Mach1TranscodeConstantsInternal::M1SpatialS2FiveOneFilm},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::FiveOneFilm_Cinema), &Mach1TranscodeConstantsInternal::M1Spatial2FiveOneFilm_Cinema},
		{std::make_pair(Mach1TranscodeFormats::M1SpatialS, Mach1TranscodeFormats::FiveOneFilm_Cinema), &Mach1TranscodeConstantsInternal::M1SpatialS2FiveOneFilm_Cinema},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::FiveOneSmpte), &Mach1TranscodeConstantsInternal::M1Spatial2FiveOneSmpte},
		{std::make_pair(Mach1TranscodeFormats::M1SpatialS, Mach1TranscodeFormats::FiveOneSmpte), &Mach1TranscodeConstantsInternal::M1SpatialS2FiveOneSmpte},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::FiveOneDts), &Mach1TranscodeConstantsInternal::M1Spatial2FiveOneDts},
		{std::make_pair(Mach1TranscodeFormats::M1SpatialS, Mach1TranscodeFormats::FiveOneDts), &Mach1TranscodeConstantsInternal::M1SpatialS2FiveOneDts},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::SevenOnePt), &Mach1TranscodeConstantsInternal::M1Spatial2SevenOnePt},
		{std::make_pair(Mach1TranscodeFormats::M1SpatialS, Mach1TranscodeFormats::SevenOnePt), &Mach1TranscodeConstantsInternal::M1SpatialS2SevenOnePt},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::SevenOnePt_Cinema), &Mach1TranscodeConstantsInternal::M1Spatial2SevenOnePt_Cinema},
		{std::make_pair(Mach1TranscodeFormats::M1SpatialS, Mach1TranscodeFormats::SevenOnePt_Cinema), &Mach1TranscodeConstantsInternal::M1SpatialS2SevenOnePt_Cinema},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::SevenZero_Cinema), &Mach1TranscodeConstantsInternal::M1Spatial2SevenZero_Cinema},
		{std::make_pair(Mach1TranscodeFormats::M1SpatialS, Mach1TranscodeFormats::SevenZero_Cinema), &Mach1TranscodeConstantsInternal::M1SpatialS2SevenZero_Cinema},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::SevenOneSDDS), &Mach1TranscodeConstantsInternal::M1Spatial2SevenOneSDDS},
		{std::make_pair(Mach1TranscodeFormats::M1SpatialS, Mach1TranscodeFormats::SevenOneSDDS), &Mach1TranscodeConstantsInternal::M1SpatialS2SevenOneSDDS},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::SevenZeroSDDS), &Mach1TranscodeConstantsInternal::M1Spatial2SevenZeroSDDS},
		{std::make_pair(Mach1TranscodeFormats::M1SpatialS, Mach1TranscodeFormats::SevenZeroSDDS), &Mach1TranscodeConstantsInternal::M1SpatialS2SevenZeroSDDS},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::SevenOneTwo), &Mach1TranscodeConstantsInternal::M1Spatial2SevenOneTwo},
		{std::make_pair(Mach1TranscodeFormats::M1SpatialS, Mach1TranscodeFormats::SevenOneTwo), &Mach1TranscodeConstantsInternal::M1SpatialS2SevenOneTwo},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::SevenZeroTwo), &Mach1TranscodeConstantsInternal::M1Spatial2SevenZeroTwo},
		{std::make_pair(Mach1TranscodeFormats::M1SpatialS, Mach1TranscodeFormats::SevenZeroTwo), &Mach1TranscodeConstantsInternal::M1SpatialS2SevenZeroTwo},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::NineOne), &Mach1TranscodeConstantsInternal::M1Spatial2NineOne},
		{std::make_pair(Mach1TranscodeFormats::M1SpatialS, Mach1TranscodeFormats::NineOne), &Mach1TranscodeConstantsInternal::M1SpatialS2NineOne},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::NineZero), &Mach1TranscodeConstantsInternal::M1Spatial2NineZero},
		{std::make_pair(Mach1TranscodeFormats::M1SpatialS, Mach1TranscodeFormats::NineZero), &Mach1TranscodeConstantsInternal::M1SpatialS2NineZero},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::FiveOneTwo), &Mach1TranscodeConstantsInternal::M1Spatial2FiveOneTwo},
		{std::make_pair(Mach1TranscodeFormats::M1SpatialS, Mach1TranscodeFormats::FiveOneTwo), &Mach1TranscodeConstantsInternal::M1SpatialS2FiveOneTwo},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::FiveZeroTwo), &Mach1TranscodeConstantsInternal::M1Spatial2FiveZeroTwo},
		{std::make_pair(Mach1TranscodeFormats::M1SpatialS, Mach1TranscodeFormats::FiveZeroTwo), &Mach1TranscodeConstantsInternal::M1SpatialS2FiveZeroTwo},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::FiveOneFour), &Mach1TranscodeConstantsInternal::M1Spatial2FiveOneFour},
		{std::make_pair(Mach1TranscodeFormats::M1SpatialS, Mach1TranscodeFormats::FiveOneFour), &Mach1TranscodeConstantsInternal::M1SpatialS2FiveOneFour},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::FiveZeroFour), &Mach1TranscodeConstantsInternal::M1Spatial2FiveZeroFour},
		{std::make_pair(Mach1TranscodeFormats::M1SpatialS, Mach1TranscodeFormats::FiveZeroFour), &Mach1TranscodeConstantsInternal::M1SpatialS2FiveZeroFour},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::SevenOneFour), &Mach1TranscodeConstantsInternal::M1Spatial2SevenOneFour},
		{std::make_pair(Mach1TranscodeFormats::M1SpatialS, Mach1TranscodeFormats::SevenOneFour), &Mach1TranscodeConstantsInternal::M1SpatialS2SevenOneFour},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::SevenZeroFour), &Mach1TranscodeConstantsInternal::M1Spatial2SevenZeroFour},
		{std::make_pair(Mach1TranscodeFormats::M1SpatialS, Mach1TranscodeFormats::SevenZeroFour), &Mach1TranscodeConstantsInternal::M1SpatialS2SevenZeroFour},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::NineOneFour), &Mach1TranscodeConstantsInternal::M1Spatial2NineOneFour},
		{std::make_pair(Mach1TranscodeFormats::M1SpatialS, Mach1TranscodeFormats::NineOneFour), &Mach1TranscodeConstantsInternal::M1SpatialS2NineOneFour},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::NineOneSix), &Mach1TranscodeConstantsInternal::M1Spatial2NineOneSix},
		{std::make_pair(Mach1TranscodeFormats::M1SpatialS, Mach1TranscodeFormats::NineOneSix), &Mach1TranscodeConstantsInternal::M1SpatialS2NineOneSix},
		{std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::MarcoSixteen), &Mach1TranscodeConstantsInternal::M1Spatial2MarcoSixteen},

		{ std::make_pair(Mach1TranscodeFormats::TTPoints, Mach1TranscodeFormats::M1Horizon), NULL },
		{ std::make_pair(Mach1TranscodeFormats::TTPoints, Mach1TranscodeFormats::M1Spatial), NULL },

		{ std::make_pair(Mach1TranscodeFormats::M1Horizon, Mach1TranscodeFormats::TTPoints), NULL },
		{ std::make_pair(Mach1TranscodeFormats::M1Spatial, Mach1TranscodeFormats::TTPoints), NULL },
	};
};

//TODO: 10.2, 12.2, 11.1, 22.2
//10.2/12.2: https://en.wikipedia.org/wiki/10.2_surround_sound
//22.2: https://en.wikipedia.org/wiki/22.2_surround_sound
//FL, FR, FC, LFE1, BL, BR, FLc, FRc, BC, LFE2, SiL (side left), SiR, TpFL, TpFR, TpFC, TpC, TpBL, TpBR, TpSiL, TpSiR, TpBC, BtFC, BtFL, BtFR

class Mach1TranscodeCore
{
private:
	Mach1TranscodeFormats::FormatType inFmt;
	Mach1TranscodeFormats::FormatType outFmt;

	std::vector<Mach1Point3DCore> inTTPoints;
	std::vector<Mach1Point3DCore> outTTPoints;

	float *buffers[Mach1TranscodeConstants::MAXCHANS];
	int bufferSize;

	std::vector<Mach1TranscodeFormats::FormatType> formatsConvertionPath;
	
	M1DSP::Utilities::CSpatialDownmixChecker spatialDownmixChecker;

	std::vector<M1DSP::Filters::CFilterSimpleLP> lpFilters;
	std::vector<int> subChannelIndices;

	void convert(Mach1TranscodeFormats::FormatType inFmt, float** inBufs, Mach1TranscodeFormats::FormatType outFmt, float** outBufs, int numSamples);
	int getNumChannels(Mach1TranscodeFormats::FormatType fmt, bool isInput);

public:

	Mach1TranscodeCore();
	~Mach1TranscodeCore();

	int getInputNumChannels();
	int getOutputNumChannels();
	Mach1TranscodeFormats::FormatType getFormatFromString(char* str);
	char* getFormatName(void * M1obj, Mach1TranscodeFormats::FormatType fmt);

	float calcNormalization(float** bufs, int numSamples);
	void applyMasterGain(float** bufs, int numSamples, float masterGain = 1.0f);

	float db2level(float db);
	float level2db(float level);

	void setLFESub(std::vector<int> subChannelIndices, int sampleRate);
	void setSpatialDownmixer(float corrThreshold = 0.1);
	bool getSpatialDownmixerPossibility();

	void setInputFormat(Mach1TranscodeFormats::FormatType inFmt);
	void setInputFormatADM(char *inXml);
	void setInputFormatTTJson(char *inJson);
	void setInputFormatTTPoints(std::vector<Mach1Point3DCore> points);

	void setOutputFormat(Mach1TranscodeFormats::FormatType outFmt);
	void setOutputFormatTTPoints(std::vector<Mach1Point3DCore> points);

	bool computeConvertionPath();
	void convert(float** inBufs, float** outBufs, int numSamples);

	std::vector<Mach1TranscodeFormats::FormatType>& getFormatsConvertionPath();
};

#endif /* Mach1TranscodeCore_h */
