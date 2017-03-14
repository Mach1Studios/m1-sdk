#pragma once

#include <math.h>

class MatrixConvert
{
public:
	static enum {
		FuMa,
		ACNSN3D,
		Square,
		SquareS,
		Square8,
		Cube,
		CubeS,
		Cube16
	} Formats;

	MatrixConvert();
	int convert(int inFmt, float** inBufs, int outFmt, float** outBufs, int numSamples);

private:
	const static int NUMFMTS = 8;
	const static int MAXCHANS = 16;

	const float r2 = (float)sqrt(2.0);
	const float r3 = (float)sqrt(3.0);
	const float oor2 = (float)(1.0 / sqrt(2.0));
	const float oo2r2 = (float)(1.0 / (2.0 * sqrt(2.0)));
	const float oor3 = (float)(1.0 / sqrt(3.0));
	const float r3or2 = (float)(sqrt(3.0) / sqrt(2.0));
	const float r3o2 = (float)sqrt(3.0) / 2.0f;

	// --- FuMa ---
	float FuMa2ACNSN3D[4][4] =
	{ { r2, 0, 0, 0 },
  	  { 0, 0, r3, 0 },
	  { 0, 0, 0, r3 },
	  { 0, r3, 0, 0 } };
	// -- ACN/SN3D ---
	float ACNSN3D2FuMa[4][4] =
	{ { oor2, 0, 0, 0 },
	  { 0, 0, 0, oor3 },
	  { 0, oor3, 0, 0 },
	  { 0, 0, oor3, 0 } };

	// --- Square ---
	float Square2FuMa[4][4] =
	{ { oor2, oor2, oor2, oor2 },
	  { oor2, -oor2, oor2, -oor2 },
	  { oor2, oor2, -oor2, -oor2 },
	  { 0, 0, 0, 0 } };
	float Square2ACNSN3D[4][4] =
	{ { 1, 1, 1, 1 },
	  { r3*oor2, -r3*oor2, -r3*oor2, r3*oor2 },
	  { 0, 0, 0, 0 },
	  { r3*oor2, r3*oor2, -r3*oor2, -r3*oor2 } };
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
	  { r3*oor2, -r3*oor2, -r3*oor2, r3*oor2, 0, 0 },
	  { 0, 0, 0, 0, 0, 0 },
	  { r3*oor2, r3*oor2, -r3*oor2, -r3*oor2, 0, 0 } };
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
	float Cube2ACNSN3D[4][8] =
	{ { oor3, oor3, oor3, oor3, oor3, oor3, oor3, oor3 },
  	  { r3o2, -r3o2, r3o2, -r3o2, r3o2, -r3o2, r3o2, -r3o2 },
	  { r3or2, r3or2, r3or2, r3or2, -r3or2, -r3or2, -r3or2, -r3or2 },
	  { r3o2, r3o2, -r3o2, -r3o2, r3o2, r3o2, -r3o2, -r3o2 } };
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
	{ { oor3, oor3, oor3, oor3, oor3, oor3, oor3, oor3, 1, 1 },
	  { r3o2, -r3o2, r3o2, -r3o2, r3o2, -r3o2, r3o2, -r3o2, 0, 0 },
	  { r3or2, r3or2, r3or2, r3or2, -r3or2, -r3or2, -r3or2, -r3or2, 0, 0 },
	  { r3o2, r3o2, -r3o2, -r3o2, r3o2, r3o2, -r3o2, -r3o2, 0, 0 } };
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

	int fmtChannels[NUMFMTS] = { 4, 4, 4, 6, 8, 8, 10, 16 };
	float* fmtMatrix[NUMFMTS][NUMFMTS] =
	{ { NULL, &(FuMa2ACNSN3D[0][0]), NULL, NULL, NULL, NULL, NULL, NULL },
	  { &(ACNSN3D2FuMa[0][0]), NULL, NULL, NULL, NULL, NULL, NULL, NULL },
	  { &(Square2FuMa[0][0]), &(Square2ACNSN3D[0][0]), NULL, NULL, &(Square2Square8[0][0]), &(Square2Cube[0][0]), NULL, &(Square2Cube16[0][0]) },
	  { &(SquareS2FuMa[0][0]), &(SquareS2ACNSN3D[0][0]), &(SquareS2Square[0][0]), NULL, &(SquareS2Square8[0][0]), &(SquareS2Cube[0][0]), NULL, &(SquareS2Cube16[0][0]) },
	  { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
	  { &(Cube2FuMa[0][0]), &(Cube2ACNSN3D[0][0]), &(Cube2Square[0][0]), NULL, &(Cube2Square8[0][0]), NULL, NULL, &(Cube2Cube16[0][0]) },
	  { &(CubeS2FuMa[0][0]), &(CubeS2ACNSN3D[0][0]), &(CubeS2Square[0][0]), NULL, &(CubeS2Square8[0][0]), &(CubeS2Cube[0][0]), NULL, &(CubeS2Cube16[0][0]) },
	  { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL }
	};

};



