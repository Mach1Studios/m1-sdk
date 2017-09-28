#pragma once

#include <math.h>
#include <stddef.h>

class MatrixConvert
{
public:
	static enum FmtType {
		FuMa,
		ACNSN3D,
		Square,
		SquareS,
		Square8,
		Cube,
		CubeS,
        Cube16,
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
        TBE
	} Formats;

	MatrixConvert();
    int getNumChannels(int fmt);
    int convert(int inFmt, float** inBufs, int outFmt, float** outBufs, int numSamples);
    
private:
	const static int NUMFMTS = 21;
	const static int MAXCHANS = 16;

	const float r2 = (float)sqrt(2.0);
	const float r3 = (float)sqrt(3.0);
	const float oor2 = (float)(1.0 / sqrt(2.0));
	const float oo2r2 = (float)(1.0 / (2.0 * sqrt(2.0)));
	const float oor3 = (float)(1.0 / sqrt(3.0));
	const float r3or2 = (float)(sqrt(3.0) / sqrt(2.0));
	const float r3o2 = (float)sqrt(3.0) / 2.0f;
    const float r2o2 = (float)sqrt(2.0) / 2.0f;
    const float r2o4 = (float)sqrt(2.0) / 4.0f;
    const float r2o6 = (float)sqrt(2.0) / 6.0f;
    const float r2o8 = (float)sqrt(2.0) / 8.0f;
    const float r2o12 = (float)sqrt(2.0) / 12.0f;
    const float r2o20 = (float)sqrt(2.0) / 20.0f;
    const float oo8 = (float)1.0f / 8.0f;

    // column down = inputs
    // rows left = outputs
    
	// --- FuMa ---
	float FuMa2ACNSN3D[4][4] =
	{ { r2, 0, 0, 0 },
  	  { 0, 0, 1, 0 },
	  { 0, 0, 0, 1 },
	  { 0, 1, 0, 0 } };
	// -- ACN/SN3D ---
	float ACNSN3D2FuMa[4][4] =
	{ { oor2, 0, 0, 0 },
	  { 0, 0, 0, 1 },
	  { 0, 1, 0, 0 },
	  { 0, 0, 1, 0 } };

	// --- Square ---
	float Square2FuMa[4][4] =
	{ { oor2, oor2, oor2, oor2 },
	  { oor2, -oor2, oor2, -oor2 },
	  { oor2, oor2, -oor2, -oor2 },
	  { 0, 0, 0, 0 } };
	float Square2ACNSN3D[4][4] =
	{ { 1, 1, 1, 1 },
	  { oor2, -oor2, -oor2, oor2 },
	  { 0, 0, 0, 0 },
	  { oor2, oor2, -oor2, -oor2 } };
	float Square2Square8[8][4] =
	{ { 1, 0, 0, 0 },
	  { 0, 1, 0, 0 },
	  { 0, 0, 1, 0 },
	  { 1, 0, 0, 0 },
	  { 0, 0, 0, 1 },
	  { 0, 0, 1, 0 },
	  { 0, 1, 0, 0 },
	  { 0, 0, 0, 1 } };
	float Square2Cube[8][4] =
	{ { 0.5f, 0, 0, 0 },
  	  { 0, 0.5f, 0, 0 },
	  { 0, 0, 0.5f, 0 },
	  { 0, 0, 0, 0.5f },
	  { 0.5f, 0, 0, 0 },
	  { 0, 0.5f, 0, 0 },
	  { 0, 0, 0.5f, 0 },
	  { 0, 0, 0, 0.5f } };
	float Square2Cube16[16][4] =
	{ { 0.5f, 0, 0, 0 },
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
	  { 0, 0, 0, 0.5f } };

	// --- Square+S ---
	float SquareS2FuMa[4][6] =
	{ { oor2, oor2, oor2, oor2, oor2, oor2 },
	  { oor2, -oor2, oor2, -oor2, 0, 0 },
	  { oor2, oor2, -oor2, -oor2, 0, 0 },
	  { 0, 0, 0, 0, 0, 0 } };
	float SquareS2ACNSN3D[4][6] =
	{ { 1, 1, 1, 1, 1, 1 },
	  { oor2, -oor2, -oor2, oor2, 0, 0 },
	  { 0, 0, 0, 0, 0, 0 },
	  { oor2, oor2, -oor2, -oor2, 0, 0 } };
	float SquareS2Square[4][6] =
	{ { 1, 0, 0, 0, 0.25f, 0.25f },
	  { 0, 1, 0, 0, 0.25f, 0.25f },
	  { 0, 0, 1, 0, 0.25f, 0.25f },
	  { 1, 0, 0, 0, 0.25f, 0.25f } };
	float SquareS2Square8[8][6] =
	{ { 1, 0, 0, 0, 1, 0 },
	  { 0, 1, 0, 0, 0, 1 },
	  { 0, 0, 1, 0, 1, 0 },
	  { 1, 0, 0, 0, 0, 1 },
	  { 0, 0, 0, 1, 1, 0 },
	  { 0, 0, 1, 0, 0, 1 },
	  { 0, 1, 0, 0, 1, 0 },
	  { 0, 0, 0, 1, 0, 1 } };
	float SquareS2Cube[8][6] =
	{ { 0.5f, 0, 0, 0, 0.125f, 0.125f },
	  { 0, 0.5f, 0, 0, 0.125f, 0.125f },
	  { 0, 0, 0.5f, 0, 0.125f, 0.125f },
	  { 0, 0, 0, 0.5f, 0.125f, 0.125f },
	  { 0.5f, 0, 0, 0, 0.125f, 0.125f },
	  { 0, 0.5f, 0, 0, 0.125f, 0.125f },
	  { 0, 0, 0.5f, 0, 0.125f, 0.125f },
	  { 0, 0, 0, 0.5f, 0.125f, 0.125f } };
	float SquareS2Cube16[16][6] =
	{ { 0.5f, 0, 0, 0, 1, 0 },
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
	  { 0, 0, 0, 0.5f, 0, 1 } };

	//float Square82FuMa[4][8] =
	//{ { oo2r2, oo2r2, oo2r2, oo2r2, oo2r2, oo2r2, oo2r2, oo2r2 },
	//  { oo2r2, oo2r2, oo2r2, -oo2r2,- oo2r2, oo2r2, -oo2r2, -oo2r2 },
	//  { oo2r2, -oo2r2, -oo2r2, oo2r2, -oo2r2, -oo2r2, oo2r2, -oo2r2 },
	//  { 0, 0, 0, 0 } };

	// --- Cube ---
	float Cube2FuMa [4][8] = 
	{ { oor2, oor2, oor2, oor2, oor2, oor2, oor2, oor2 },
	  { 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f },
	  { 0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f },
	  { oor2, oor2, oor2, oor2, -oor2, -oor2, -oor2, -oor2 } };
    float Cube2FuMaO2A[9][8] =
    {  { oor2, oor2, oor2, oor2, oor2, oor2, oor2, oor2 }, //W
	   { 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f }, //X
	   { 0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f }, //Y
	   { oor2, oor2, oor2, oor2, -oor2, -oor2, -oor2, -oor2 }, //Z
	   { 0, 0, 0, 0, 0, 0, 0, 0 }, //R
	   { 0, 0, 0, 0, 0, 0, 0, 0 }, //S
	   { 0, 0, 0, 0, 0, 0, 0, 0 }, //T
	   { 0, 0, 0, 0, 0, 0, 0, 0 }, //U
	   { 0, 0, 0, 0, 0, 0, 0, 0 } }; //V
    //nFOA
//	float Cube2ACNSN3D[4][8] =
//	{ { 1, 1, 1, 1, 1, 1, 1, 1 },
//	  { 0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f },
//	  { oor2, oor2, oor2, oor2, -oor2, -oor2, -oor2, -oor2 },
//	  { 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f } };
    //oFOA
    float Cube2ACNSN3D[4][8] =
    {   { oor3, oor3, oor3, oor3, oor3, oor3, oor3, oor3 },
        { r3o2, -r3o2, r3o2, -r3o2, r3o2, -r3o2, r3o2, -r3o2 },
        { r3or2, r3or2, r3or2, r3or2, -r3or2, -r3or2, -r3or2, -r3or2 },
        { r3o2, r3o2, -r3o2, -r3o2, r3o2, r3o2, -r3o2, -r3o2 } };
    float Cube2TBE[8][8] =
    {   { 1, 1, 1, 1, 1, 1, 1, 1 },
        { 0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f }, //invert because based on ISO?
        { 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f }, // channel order change w,x,y,z
        { oor2, oor2, oor2, oor2, -oor2, -oor2, -oor2, -oor2 },
        { 0, 0, 0, 0, 0, 0, 0, 0 },
        { -r2o12, r2o12, r2o12, -r2o12, -r2o12, r2o12, r2o12, -r2o12 }, //FRBL
        { -r2o12, r2o12, -r2o12, r2o12, r2o12, -r2o12, r2o12, -r2o12 }, //URDL
        { r2o12, r2o12, -r2o12, -r2o12, -r2o12, -r2o12, r2o12, r2o12 }}; //TFDB
    float Cube2ACNSN3DO2A[9][8] =
    {  { 1, 1, 1, 1, 1, 1, 1, 1 }, //W
	   { 0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f }, //X
	   { oor2, oor2, oor2, oor2, -oor2, -oor2, -oor2, -oor2 }, //Y
	   { 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f }, //Z
	   { -0.25f, -0.25f, -0.25f, -0.25f, -0.25f, -0.25f, -0.25f, -0.25f }, //R?
	   { 0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f }, //S
	   { -0.25f, -0.25f, 0.25f, 0.25f, -0.25f, -0.25f, 0.25f, 0.25f }, //T?
	   { 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f }, //U
	   { -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, -0.5f } }; //V
	float Cube2Square[4][8] =
	{ { 0.5f, 0, 0, 0, 0.5f, 0, 0, 0 },
	  { 0, 0.5f, 0, 0, 0, 0.5f, 0, 0 },
	  { 0, 0, 0.5f, 0, 0, 0, 0.5f, 0 },
	  { 0, 0, 0, 0.5f, 0, 0, 0, 0.5f } };
	float Cube2Square8[8][8] =
	{ { 0.5f, 0, 0, 0, 0.5f, 0, 0, 0 },
  	  { 0, 0.5f, 0, 0, 0, 0.5f, 0, 0 },
	  { 0, 0, 0.5f, 0, 0, 0, 0.5f, 0 },
	  { 0.5f, 0, 0, 0, 0.5f, 0, 0, 0 },
	  { 0, 0, 0, 0.5f, 0, 0, 0, 0.5f },
	  { 0, 0, 0.5f, 0, 0, 0, 0.5f, 0 },
	  { 0, 0.5f, 0, 0, 0, 0.5f, 0, 0 },
	  { 0, 0, 0, 0.5f, 0, 0, 0, 0.5f } };
	float Cube2Cube16[16][8] =
	{ { 1, 0, 0, 0, 0, 0, 0, 0 },
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
	  { 0, 0, 0, 0, 0, 0, 0, 1 } };

	// --- Cube+S ---
	float CubeS2FuMa[4][10] =
	{ { oor2, oor2, oor2, oor2, oor2, oor2, oor2, oor2, 1, 1 },
	  { 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0, 0 },
	  { 0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0, 0 },
	  { oor2, oor2, oor2, oor2, -oor2, -oor2, -oor2, -oor2, 0, 0 } };
	float CubeS2ACNSN3D[4][10] =
	{ { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	  { 0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0, 0 },
	  { oor2, oor2, oor2, oor2, -oor2, -oor2, -oor2, -oor2, 0, 0 },
	  { 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0, 0 } };
	float CubeS2Square[4][10] =
	{ { 0.5f, 0, 0, 0, 0.5f, 0, 0, 0, 0.25f, 0.25f },
	  { 0, 0.5f, 0, 0, 0, 0.5f, 0, 0, 0.25f, 0.25f },
	  { 0, 0, 0.5f, 0, 0, 0, 0.5f, 0, 0.25f, 0.25f },
	  { 0, 0, 0, 0.5f, 0, 0, 0, 0.5f, 0.25f, 0.25f } };
	float CubeS2Square8[8][10] =
	{ { 0.5f, 0, 0, 0, 0.5f, 0, 0, 0, 1, 0 },
	  { 0, 0.5f, 0, 0, 0, 0.5f, 0, 0, 0, 1 },
	  { 0, 0, 0.5f, 0, 0, 0, 0.5f, 0, 1, 0 },
	  { 0.5f, 0, 0, 0, 0.5f, 0, 0, 0, 0, 1 },
	  { 0, 0, 0, 0.5f, 0, 0, 0, 0.5f, 1, 0 },
	  { 0, 0, 0.5f, 0, 0, 0, 0.5f, 0, 0, 1 },
	  { 0, 0.5f, 0, 0, 0, 0.5f, 0, 0, 1, 0 },
	  { 0, 0, 0, 0.5f, 0, 0, 0, 0.5f, 0, 1 } };
	float CubeS2Cube[8][10] =
	{ { 1, 0, 0, 0, 0, 0, 0, 0, 0.125f, 0.125f },
	  { 0, 1, 0, 0, 0, 0, 0, 0, 0.125f, 0.125f },
	  { 0, 0, 1, 0, 0, 0, 0, 0, 0.125f, 0.125f },
	  { 0, 0, 0, 1, 0, 0, 0, 0, 0.125f, 0.125f },
	  { 0, 0, 0, 0, 1, 0, 0, 0, 0.125f, 0.125f },
	  { 0, 0, 0, 0, 0, 1, 0, 0, 0.125f, 0.125f },
	  { 0, 0, 0, 0, 0, 0, 1, 0, 0.125f, 0.125f },
	  { 0, 0, 0, 0, 0, 0, 0, 1, 0.125f, 0.125f } };
	float CubeS2Cube16[16][10] =
	{ { 1, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
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
	};

    // --- Surround to M1Spatial ---;
    float Mono2Cube[1][8] =
    {   { r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 } };
    float Stereo2Cube[2][8] =
    {   { r2o2, 0, r2o2, 0, r2o2, 0, r2o2, 0 },
        { 0, r2o2, 0, r2o2, 0, r2o2, 0, r2o2 } };
    float LCR2Cube[8][3] =
    {   { 1, r2o8, 0 },
        { 0, r2o8, 1 },
        { r2o8, r2o8, 0 },
        { 0, r2o8, r2o8 },
        { 1, r2o8, 0 },
        { 0, r2o8, 1 },
        { r2o8, r2o8, 0 },
        { 0, r2o8, r2o8 },};
    float FiveOh2Cube[8][5] =
    {   { 1, r2o8, 0, 0, 0 },
        { 0, r2o8, 1, 0, 0 },
        { 0, r2o8, 0, 1, 0 },
        { 0, r2o8, 0, 0, 1 },
        { 1, r2o8, 0, 0, 0 },
        { 0, r2o8, 1, 0, 0 },
        { 0, r2o8, 0, 1, 0 },
        { 0, r2o8, 0, 0, 1 } };
    float FiveOneFilm2Cube[8][6] =
    {   { 1, r2o8, 0, 0, 0, r2o8 },
        { 0, r2o8, 1, 0, 0, r2o8 },
        { 0, r2o8, 0, 1, 0, r2o8 },
        { 0, r2o8, 0, 0, 1, r2o8 },
        { 1, r2o8, 0, 0, 0, r2o8 },
        { 0, r2o8, 1, 0, 0, r2o8 },
        { 0, r2o8, 0, 1, 0, r2o8 },
        { 0, r2o8, 0, 0, 1, r2o8 } };
    float FiveOneFilm_Cinema2Cube[8][6] =
    {   { 1, r2o4, 0, 0, 0, r2o8 },
        { 0, r2o4, 1, 0, 0, r2o8 },
        { 0, r2o20, 0, 1, 0, r2o8 },
        { 0, r2o20, 0, 0, 1, r2o8 },
        { 1, r2o4, 0, 0, 0, r2o8 },
        { 0, r2o4, 1, 0, 0, r2o8 },
        { 0, r2o20, 0, 1, 0, r2o8 },
        { 0, r2o20, 0, 0, 1, r2o8 } };
    float FiveOneSmpte2Cube[8][6] =
    {   { 1, 0, r2o8, r2o8, 0, 0 },
        { 0, 1, r2o8, r2o8, 0, 0 },
        { 0, 0, r2o8, r2o8, 1, 0 },
        { 0, 0, r2o8, r2o8, 0, 1 },
        { 1, 0, r2o8, r2o8, 0, 0 },
        { 0, 1, r2o8, r2o8, 0, 0 },
        { 0, 0, r2o8, r2o8, 1, 0 },
        { 0, 0, r2o8, r2o8, 0, 1 } };
    float FiveOneDts2Cube[8][6] =
    {   { 1, 0, 0, 0, r2o8, r2o8 },
        { 0, 1, 0, 0, r2o8, r2o8 },
        { 0, 0, 1, 0, r2o8, r2o8 },
        { 0, 0, 0, 1, r2o8, r2o8 },
        { 1, 0, 0, 0, r2o8, r2o8 },
        { 0, 1, 0, 0, r2o8, r2o8 },
        { 0, 0, 1, 0, r2o8, r2o8 },
        { 0, 0, 0, 1, r2o8, r2o8 } };
    float SixOh2Cube[8][6] = //BLANK
    {   { 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0 } };
    float SevenOnePt2Cube[8][8] =
    {   { 1, r2o8, 0, r2o4, 0, 0, 0, r2o8 },
        { 0, r2o8, 1, 0, r2o4, 0, 0, r2o8 },
        { 0, r2o8, 0, r2o4, 0, 1, 0, r2o8 },
        { 0, r2o8, 0, 0, r2o4, 0, 1, r2o8 },
        { 1, r2o8, 0, r2o4, 0, 0, 0, r2o8 },
        { 0, r2o8, 1, 0, r2o4, 0, 0, r2o8 },
        { 0, r2o8, 0, r2o4, 0, 1, 0, r2o8 },
        { 0, r2o8, 0, 0, r2o4, 0, 1, r2o8 } };
    float SevenOnePt_Cinema2Cube[8][8] =
    {   { 1, r2o4, 0, r2o4, 0, 0, 0, r2o8 },
        { 0, r2o4, 1, 0, r2o4, 0, 0, r2o8 },
        { 0, 0, 0, r2o4, 0, 1, 0, r2o8 },
        { 0, 0, 0, 0, r2o4, 0, 1, r2o8 },
        { 1, r2o4, 0, r2o4, 0, 0, 0, r2o8 },
        { 0, r2o4, 1, 0, r2o4, 0, 0, r2o8 },
        { 0, 0, 0, r2o4, 0, 1, 0, r2o8 },
        { 0, 0, 0, 0, r2o4, 0, 1, r2o8 } };
    
    // --- Surround to M1Spatial ---;
    float Cube2FiveOneFilm[6][8] =
    {   { 1, 0, 0, 0, 1, 0, 0, 0 },
        { r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 },
        { 0, 1, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 0, 0, 0, 1, 0 },
        { 0, 0, 0, 1, 0, 0, 0, 1 },
        { r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 } };
    float Cube2FiveOneFilm_Cinema[6][8] =
    {   { 1, 0, 0, 0, 1, 0, 0, 0 },
        { r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0 },
        { 0, 1, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 0, 0, 0, 1, 0 },
        { 0, 0, 0, 1, 0, 0, 0, 1 },
        { r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 } };
    float Cube2FiveOneSmpte[6][8] =
    {   { 1, 0, 0, 0, 1, 0, 0, 0 },
        { 0, 1, 0, 0, 0, 1, 0, 0 },
        { r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0 },
        { r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 },
        { 0, 0, 1, 0, 0, 0, 1, 0 },
        { 0, 0, 0, 1, 0, 0, 0, 1 } };
    float Cube2FiveOneDts[6][8] =
    {   { 1, 0, 0, 0, 1, 0, 0, 0 },
        { 0, 1, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 0, 0, 0, 1, 0 },
        { 0, 0, 0, 1, 0, 0, 0, 1 },
        { r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0 },
        { r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 } };
    float Cube2SevenOnePt[8][8] =
    {   { 1, 0, 0, 0, 1, 0, 0, 0 },
        { r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 },
        { 0, 1, 0, 0, 0, 1, 0, 0 },
        { r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0 },
        { 0, r2o4, 0, r2o4, 0, r2o4, 0, r2o4 },
        { 0, 0, 1, 0, 0, 0, 1, 0 },
        { 0, 0, 0, 1, 0, 0, 0, 1 },
        { r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 } };
    float Cube2SevenOnePt_Cinema[8][8] =
    {   { 1, 0, 0, 0, 1, 0, 0, 0 },
        { r2o4, r2o4, 0, 0, r2o4, r2o4, 0, 0 },
        { 0, 1, 0, 0, 0, 1, 0, 0 },
        { r2o4, 0, r2o4, 0, r2o4, 0, r2o4, 0 },
        { 0, r2o4, 0, r2o4, 0, r2o4, 0, r2o4 },
        { 0, 0, 1, 0, 0, 0, 1, 0 },
        { 0, 0, 0, 1, 0, 0, 0, 1 },
        { r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8, r2o8 } };
    float TBE2Cube[8][8] =
        //W         -L->+R     +F->-B    +U->-D         
    {   { 0.682217, -0.590557, 0.590557, 0.591337, 0, -0.152414, -0.152615, 0.152615 },
        { 0.682217, 0.590557, 0.590557, 0.591337, 0, 0.152414, 0.152615, 0.152615 },
        { 0.682217, -0.590557, -0.590557, 0.591337, 0, 0.152414, -0.152615, -0.152615 },
        { 0.682217, 0.590557, -0.590557, 0.591337, 0, -0.152414, 0.152615, -0.152615 },
        { 0.682217, -0.590557, 0.590557, -0.591337, 0, -0.152414, 0.152615, -0.152615 },
        { 0.682217, 0.590557, 0.590557, -0.591337, 0, 0.152414, -0.152615, -0.152615 },
        { 0.682217, -0.590557, -0.590557, -0.591337, 0, 0.152414, 0.152615, 0.152615 },
        { 0.682217, 0.590557, -0.590557, -0.591337, 0, -0.152414, -0.152615, 0.152615 } };
        // lower first column
    
	int fmtChannels[NUMFMTS] = { 4, 4, 4, 6, 8, 8, 10, 16, 2, 3, 5, 6, 6, 6, 6, 6, 8, 8, 9, 9, 8 };
	float* fmtMatrix[NUMFMTS][NUMFMTS] =
    {   { NULL, &(FuMa2ACNSN3D[0][0]), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        
        { &(ACNSN3D2FuMa[0][0]), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        
        { &(Square2FuMa[0][0]), &(Square2ACNSN3D[0][0]), NULL, NULL, &(Square2Square8[0][0]), &(Square2Cube[0][0]), NULL, &(Square2Cube16[0][0]), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        
        { &(SquareS2FuMa[0][0]), &(SquareS2ACNSN3D[0][0]), &(SquareS2Square[0][0]), NULL, &(SquareS2Square8[0][0]), &(SquareS2Cube[0][0]), NULL, &(SquareS2Cube16[0][0]), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        
        { &(Cube2FuMa[0][0]), &(Cube2ACNSN3D[0][0]), &(Cube2Square[0][0]), NULL, &(Cube2Square8[0][0]), NULL, NULL, &(Cube2Cube16[0][0]), NULL, &(Cube2FiveOneFilm[0][0]), &(Cube2FiveOneFilm_Cinema[0][0]), &(Cube2FiveOneSmpte[0][0]), &(Cube2FiveOneDts[0][0]), NULL, &(Cube2SevenOnePt[0][0]), &(Cube2SevenOnePt_Cinema[0][0]), &(Cube2ACNSN3DO2A[0][0]), &(Cube2ACNSN3DO2A[0][0]), &(Cube2TBE[0][0]) },
        
        { &(CubeS2FuMa[0][0]), &(CubeS2ACNSN3D[0][0]), &(CubeS2Square[0][0]), NULL, &(CubeS2Square8[0][0]), &(CubeS2Cube[0][0]), NULL, &(CubeS2Cube16[0][0]), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        
        { NULL, NULL, NULL, NULL, NULL, &(Stereo2Cube[0][0]), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        { NULL, NULL, NULL, NULL, NULL, &(LCR2Cube[0][0]), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        { NULL, NULL, NULL, NULL, NULL, &(FiveOh2Cube[0][0]), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        { NULL, NULL, NULL, NULL, NULL, &(FiveOneFilm2Cube[0][0]), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        { NULL, NULL, NULL, NULL, NULL, &(FiveOneFilm_Cinema2Cube[0][0]), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        { NULL, NULL, NULL, NULL, NULL, &(FiveOneSmpte2Cube[0][0]), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        { NULL, NULL, NULL, NULL, NULL, &(FiveOneDts2Cube[0][0]), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        { NULL, NULL, NULL, NULL, NULL, &(SixOh2Cube[0][0]), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        { NULL, NULL, NULL, NULL, NULL, &(SevenOnePt2Cube[0][0]), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        { NULL, NULL, NULL, NULL, NULL, &(SevenOnePt_Cinema2Cube[0][0]), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        { NULL, NULL, NULL, NULL, NULL, &(TBE2Cube[0][0]), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL }
    };

};
