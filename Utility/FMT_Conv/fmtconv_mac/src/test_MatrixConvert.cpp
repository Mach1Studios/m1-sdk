
#include "MatrixConvert.h"

#define MAXCHANNELS (16)
#define BUFLEN (1024)

int main(int argc, char* argv[])
{
	float inBufs[MAXCHANNELS][BUFLEN];
	float outBufs[MAXCHANNELS][BUFLEN];
	float* inPtrs[MAXCHANNELS];
	float* outPtrs[MAXCHANNELS];
	MatrixConvert mc;

	for (int i = 0; i < MAXCHANNELS; i++)
	{
		inPtrs[i] = inBufs[i];
		outPtrs[i] = outBufs[i];
	}
	inBufs[0][0] = 1.0f;
	mc.convert(MatrixConvert::FuMa, inPtrs, MatrixConvert::Square8, outPtrs, BUFLEN);

	return 0;
}
