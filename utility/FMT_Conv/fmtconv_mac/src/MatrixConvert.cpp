
#include <math.h>
#include "MatrixConvert.h"

MatrixConvert::MatrixConvert()
{
}

int MatrixConvert::getNumChannels(int fmt)
{
	return fmtChannels[fmt];
}

int MatrixConvert::convert(int inFmt, float** inBufs, int outFmt, float** outBufs, int numSamples)
{
	int inChans = fmtChannels[inFmt];
	int outChans = fmtChannels[outFmt];
	float* M = fmtMatrix[inFmt][outFmt];
		
	// if null then nothing to do
	// might want to copy ins to outs here
	if (M == NULL)
		return -1;

	// copy pointers to local
	float* inPtrs[MAXCHANS];
	for (int i=0; i<inChans; i++)
		inPtrs[i] = inBufs[i];
	float* outPtrs[MAXCHANS];
	for (int i=0; i<outChans; i++)
		outPtrs[i] = outBufs[i];

	// matrix mult each sample
	float ins[MAXCHANS];
	for (int n = 0; n < numSamples; n++)
	{
		float* m = M;
		for (int i = 0; i < inChans; i++)
			ins[i] = *inPtrs[i]++;
		for (int j = 0; j < outChans; j++)
		{
			float out = 0;
			for (int i = 0; i < inChans; i++)
				out += *m++ * ins[i];
			*outPtrs[j]++ = out;
		}
	}

	return 0;
}


