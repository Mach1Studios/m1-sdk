
#include "MatrixConvert.h"

//#ifdef _MSC_VER
//#define _CRT_SECURE_NO_WARNINGS
//#endif

#include <stdlib.h>

#include <cstring>
#include <iostream>
#include <vector>
#include <sstream>
#define _USE_MATH_DEFINES
#include <math.h>
#include "sndfile.hh"

#include "CmdOption.h"

using namespace std;

class Component
{
public:
	float** inBufs;
	float** outBufs;
};

float inline db2level(float db)
{
	return  (float)exp(M_LN10 * ((double)db / 20.0));
}

float inline level2db(float level)
{
	return (float)(20.0f * log((double)level) / M_LN10);
}

vector<string> &split(const string &s, char delim, vector<string> &elems) {
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

void printHelp()
{
    cout << "m1-transcode -- command line mach1 format conversion tool" << std::endl;
    cout << "ambisonics contributed by VVAudio: http://www.vvaudio.com/ " << std::endl;
	cout << std::endl;
	cout << "usage: fmtconvert -in-file test_b.wav -in-fmt FuMa -out-file test_s8.wav -out-fmt Cube -out-file-chans 1" << std::endl;
	cout << std::endl;
	cout << "  -help - list command line options" << std::endl;
	cout << "  -in-file  <filename>  - input file: put quotes around sets of files" << std::endl;
	cout << "  -in-fmt   <fmt>       - input format: see supported formats below" << std::endl;
	cout << "  -out-file <filename>  - output file. full name for single file or name stem for file sets" << std::endl;
	cout << "  -out-fmt  <fmt>       - output format: see supported formats below" << std::endl;
	cout << "  -out-file-chans <#>   - output file channels: 1, 2 or 0 (0 = multichannel)" << std::endl;
    cout << "  -normalize            - two pass normalize absolute peak to zero dBFS" << std::endl;
	cout << "  -master-gain <number> - final output gain in dB like -3 or 2.3" << std::endl;
	cout << std::endl;
	cout << "  Formats Supported:" << std::endl;
    cout << "    Stereo   - L & R spatialized" << std::endl;
    cout << "    Stereo_Cinema  - L & R spatialized, forward focus" << std::endl;
    cout << "    LCR      - L & R spatialized with C mono" << std::endl;
	cout << "    M1Horizon (Mach1 Horizon / Quad) - L R Ls Rs" << std::endl;
    cout << "    M1Horizon+S (Mach1 Horizon / Quad) - L R Ls Rs StereoL StereoR" << std::endl;
    cout << "    M1HorizonPairs (Mach1 Horizon / Quad-Binaural) - FrontPair, LeftPair, RearPair, RightPair" << std::endl;
    cout << "    M1Spatial (Mach1 Spatial) - Upper L R Ls Rs, Lower L R Ls Rs" << std::endl;
    cout << "    M1Spatial+S (Mach1 Spatial) - Upper L R Ls Rs, Lower L R Ls Rs, StereoL StereoR" << std::endl;
    cout << "    M1SpatialPairs (Mach1 Spatial Pairs) - Upper front, left, rear, right, pairs, then lower same" << std::endl;
    cout << "    M1SpatialFaces - Fc, Lc, Rc, Bc, Tc, Bc" << std::endl;
    cout << "    FiveOh   - L C R Ls Rs" << std::endl;
    cout << "    FiveOneFilm (Pro Tools default / C|24)  - L C R Ls Rs LFE" << std::endl;
    cout << "    FiveOneFilm_Cinema (Pro Tools default / C|24)  - L C R Ls Rs LFE, forward focus" << std::endl;
    cout << "    FiveOneSmpte (SMPTE/ITU for Dolby Digital (AC3) - L R C LFE Ls Rs" << std::endl;
    cout << "    FiveOneDts (DTS) - L R Ls Rs C LFE" << std::endl;
    //	cout << "    SixOh   - INCOMPLETE" << std::endl;
    cout << "    SevenOnePT (Pro Tools default) - L C R Lss Rss Lsr Rsr LFE" << std::endl;
    cout << "    SevenOnePT_Cinema (Pro Tools default) - L C R Lss Rss Lsr Rsr LFE, forward focus" << std::endl;
    cout << "    SevenZero_Cinema (Pro Tools default) - L C R Lss Rss Lsr Rsr, forward focus" << std::endl;
    cout << "    SevenOneSDDS (Sony SDDS) - L Lc C Rc R Ls Rs LFE" << std::endl;
    cout << "    SevenZeroSDDS (Sony SDDS) - L Lc C Rc R Ls Rs" << std::endl;
    cout << "    FiveOneTwo (Film / Pro Tools default) - L C R Lss Rss Lsr Rsr FLts FRts BLts BRts" << std::endl;
    cout << "    FiveZeroTwo (Film / Pro Tools default) - L C R Lss Rss Lsr Rsr FLts FRts BLts BRts" << std::endl;
    cout << "    FiveOneFour (Film / Pro Tools default) - L C R Lss Rss Lsr Rsr FLts FRts BLts BRts" << std::endl;
    cout << "    FiveZeroFour (Film / Pro Tools default) - L C R Lss Rss Lsr Rsr FLts FRts BLts BRts" << std::endl;
    cout << "    SevenOneTwo (Film / Pro Tools default) - L C R Lss Rss Lsr Rsr LFE Lts Rts" << std::endl;
    cout << "    SevenZeroTwo (Film / Pro Tools default) - L C R Lss Rss Lsr Rsr Lts Rts" << std::endl;
    cout << "    SevenOneFour (Film / Pro Tools default) - L C R Lss Rss Lsr Rsr LFE FLts FRts BLts BRts" << std::endl;
    cout << "    SevenZeroFour (Film / Pro Tools default) - L C R Lss Rss Lsr Rsr FLts FRts BLts BRts" << std::endl;
//    cout << "    SevenOneTwo (SMPTE) - L R C LFE Lss Rss Lsr Rsr Lts Rts" << std::endl;
//    cout << "    SevenZeroTwo (SMPTE) - L R C Lss Rss Lsr Rsr Lts Rts" << std::endl;
    cout << "    NineOne - " << std::endl;
    cout << "    NineZero - " << std::endl;
    cout << "    ACNSN3D  - 1st order B-format, ACN order and SN3D weighting" << std::endl;
    cout << "    FuMa     - 1st order B-format, Furse-Malham order and weighting" << std::endl;
    cout << "    ACNSN3DO2A   - 2nd order B-format, AmbiX ACN order and SN3D weighting" << std::endl;
    cout << "    FuMaO2A      - 2nd order B-format, Furse-Malham order and weighting, W, Y, Z, X, V, T, R, S, U" << std::endl;
    cout << "    TBE   - W, X, Y, Z, U, V, T, S" << std::endl;
    cout << "    ACNSN3DO3A   - 16 channel AmbiX" << std::endl;
    cout << "    FuMaO3A   - 3rd order B-format, W, Y, Z, X, V, T, R, S, U, Q, O, M, K, L, N, P" << std::endl;
	cout << std::endl;
}

void printFileInfo(SndfileHandle file)
{
	cout << "Sample Rate:        " << file.samplerate() << std::endl;
	int format = file.format() & 0xffff;
	if (format == SF_FORMAT_PCM_24) cout << "Bit Depth:          24" << std::endl;
	if (format == SF_FORMAT_PCM_16) cout << "Bit Depth:          16" << std::endl;
	if (format == SF_FORMAT_FLOAT)  cout << "Bit Depth:          32" << std::endl;
	cout << "Channels:           " << file.channels() << std::endl;
	//cout << "Length (sec):     " << (float)file.frames() / (float)file.samplerate() << std::endl;
	cout << std::endl;
}

// ---------------------------------------------------------

#define MAXBUFFERS (16)
#define BUFFERLEN (512)

int main(int argc, char* argv[])
{
	// locals for cmd line parameters
	bool fileOut = false;
	const char *emptyString = "";
	const char *dotString = ".";
	float masterGain = 1.0f; // in level, not dB
	bool normalize = false;
	char* infilename = NULL;
	char* inFmtStr = NULL;
	MatrixConvert::FmtType inFmt;
	char* outfilename = NULL;
	char* outFmtStr = NULL;
	MatrixConvert::FmtType outFmt;
	int outFileChans;
	int channels;
	sf_count_t totalSamples;
	long sampleRate;

	// multiplexed buffers
	float fileBuffer[16 * BUFFERLEN];

	// process buffers
	float inBuffers[MAXBUFFERS][BUFFERLEN];
	float *inPtrs[MAXBUFFERS];
	float outBuffers[MAXBUFFERS][BUFFERLEN];
	float *outPtrs[MAXBUFFERS];
	int inBufNdx;
	int outBufNdx; // max 8 buffers needed for cube or octogon
	for (int i = 0; i < MAXBUFFERS; i++)
	{
		inPtrs[i] = inBuffers[i];
		outPtrs[i] = outBuffers[i];
	}

	//=================================================================
	// read command line parameters 
	//

	char *pStr;
	if (cmdOptionExists(argv, argv + argc, "-h")
		|| cmdOptionExists(argv, argv + argc, "-help")
		|| cmdOptionExists(argv, argv + argc, "--help")
		|| argc == 1)
	{
		printHelp();
		return 0;
	}
	pStr = getCmdOption(argv, argv + argc, "-normalize");
	if (pStr != NULL)
	{
		normalize = true;
	}
	pStr = getCmdOption(argv, argv + argc, "-master-gain");
	if (pStr != NULL)
	{
		masterGain = (float)atof(pStr); // still in dB
		masterGain = db2level(masterGain);
	}

	// input file name and format
	pStr = getCmdOption(argv, argv + argc, "-in-file");
	if (pStr && (strlen(pStr) > 0))
	{
		infilename = pStr;
	}
	else
	{
		cerr << "Please specify an input file" << std::endl;
		return -1;
	}
	pStr = getCmdOption(argv, argv + argc, "-in-fmt");
	if (pStr && (strlen(pStr) > 0))
	{
		inFmtStr = pStr;
	}
	if (strcmp(inFmtStr, "FuMa") == 0)
		inFmt = MatrixConvert::FuMa;
	else if (strcmp(inFmtStr, "ACNSN3D") == 0)
		inFmt = MatrixConvert::ACNSN3D;
	else if (strcmp(inFmtStr, "M1Horizon") == 0)
		inFmt = MatrixConvert::M1Horizon;
    else if (strcmp(inFmtStr, "M1Horizon+S") == 0)
		inFmt = MatrixConvert::M1HorizonS;
	else if (strcmp(inFmtStr, "M1HorizonPairs") == 0)
		inFmt = MatrixConvert::M1HorizonPairs;
	else if (strcmp(inFmtStr, "M1Spatial") == 0)
		inFmt = MatrixConvert::M1Spatial;
	else if (strcmp(inFmtStr, "M1Spatial+S") == 0)
		inFmt = MatrixConvert::M1SpatialS;
	else if (strcmp(inFmtStr, "M1SpatialPairs") == 0)
		inFmt = MatrixConvert::M1SpatialPairs;
    else if (strcmp(inFmtStr, "Stereo") == 0)
        inFmt = MatrixConvert::Stereo;
    else if (strcmp(inFmtStr, "LCR") == 0)
        inFmt = MatrixConvert::LCR;
    else if (strcmp(inFmtStr, "FiveOh") == 0)
        inFmt = MatrixConvert::FiveOh;
    else if (strcmp(inFmtStr, "FiveOneFilm_Cinema") == 0)
        inFmt = MatrixConvert::FiveOneFilm_Cinema;
    else if (strcmp(inFmtStr, "FiveOneFilm") == 0)
        inFmt = MatrixConvert::FiveOneFilm;
    else if (strcmp(inFmtStr, "FiveOneSmpte") == 0)
        inFmt = MatrixConvert::FiveOneSmpte;
    else if (strcmp(inFmtStr, "FiveOneDts") == 0)
        inFmt = MatrixConvert::FiveOneDts;
    else if (strcmp(inFmtStr, "SevenOnePT") == 0)
        inFmt = MatrixConvert::SevenOnePt;
    else if (strcmp(inFmtStr, "SevenOnePT_Cinema") == 0)
	    inFmt = MatrixConvert::SevenOnePt_Cinema;
    else if (strcmp(inFmtStr, "TBE") == 0)
        inFmt = MatrixConvert::TBE;
    else if (strcmp(inFmtStr, "SevenOneSDDS") == 0)
        inFmt = MatrixConvert::SevenOneSDDS;
    else if (strcmp(inFmtStr, "SevenZeroSDDS") == 0)
        inFmt = MatrixConvert::SevenZeroSDDS;
    else if (strcmp(inFmtStr, "SevenOneTwo") == 0)
        inFmt = MatrixConvert::SevenOneTwo;
    else if (strcmp(inFmtStr, "SevenZeroTwo") == 0)
        inFmt = MatrixConvert::SevenZeroTwo;
    else if (strcmp(inFmtStr, "NineOne") == 0)
        inFmt = MatrixConvert::NineOne;
    else if (strcmp(inFmtStr, "NineZero") == 0)
        inFmt = MatrixConvert::NineZero;
    else if (strcmp(inFmtStr, "Stereo_Cinema") == 0)
        inFmt = MatrixConvert::Stereo_Cinema;
    else if (strcmp(inFmtStr, "FiveOneTwo") == 0)
        inFmt = MatrixConvert::FiveOneTwo;
    else if (strcmp(inFmtStr, "FiveZeroTwo") == 0)
        inFmt = MatrixConvert::FiveZeroTwo;
    else if (strcmp(inFmtStr, "FiveOneFour") == 0)
        inFmt = MatrixConvert::FiveOneFour;
    else if (strcmp(inFmtStr, "FiveZeroFour") == 0)
        inFmt = MatrixConvert::FiveZeroFour;
    else if (strcmp(inFmtStr, "SevenOneFour") == 0)
        inFmt = MatrixConvert::SevenOneFour;
    else if (strcmp(inFmtStr, "SevenZeroFour") == 0)
        inFmt = MatrixConvert::SevenZeroFour;
    else if (strcmp(inFmtStr, "SevenZero_Cinema") == 0)
        inFmt = MatrixConvert::SevenZero_Cinema;
    else if (strcmp(inFmtStr, "M1SpatialFaces") == 0)
        inFmt = MatrixConvert::M1SpatialFaces;
	else
	{
		cout << "Please select a valid input format" << std::endl;
		return -1;
	}


	// output file name and format
	pStr = getCmdOption(argv, argv + argc, "-out-file");
	if (pStr && (strlen(pStr) > 0))
	{
		fileOut = true;
		outfilename = pStr;
	}
	pStr = getCmdOption(argv, argv + argc, "-out-fmt");
	if (pStr && (strlen(pStr) > 0))
	{
		outFmtStr = pStr;
	}
	if (strcmp(outFmtStr, "FuMa") == 0)
		outFmt = MatrixConvert::FuMa;
	else if (strcmp(outFmtStr, "ACNSN3D") == 0)
		outFmt = MatrixConvert::ACNSN3D;
	else if (strcmp(outFmtStr, "M1Horizon") == 0)
		outFmt = MatrixConvert::M1Horizon;
	else if (strcmp(outFmtStr, "M1Horizon+S") == 0)
		outFmt = MatrixConvert::M1HorizonS;
	else if (strcmp(outFmtStr, "M1HorizonPairs") == 0)
		outFmt = MatrixConvert::M1HorizonPairs;
	else if (strcmp(outFmtStr, "M1Spatial") == 0)
		outFmt = MatrixConvert::M1Spatial;
	else if (strcmp(outFmtStr, "M1Spatial+S") == 0)
		outFmt = MatrixConvert::M1SpatialS;
	else if (strcmp(outFmtStr, "M1SpatialPairs") == 0)
		outFmt = MatrixConvert::M1SpatialPairs;
	else if (strcmp(outFmtStr, "ACNSN3DO2A") == 0)
		outFmt = MatrixConvert::ACNSN3DO2A;
	else if (strcmp(outFmtStr, "FuMaO2A") == 0)
		outFmt = MatrixConvert::FuMaO2A;
	else if (strcmp(outFmtStr, "FiveOneFilm") == 0)
        outFmt = MatrixConvert::FiveOneFilm;
    else if (strcmp(outFmtStr, "FiveOneFilm_Cinema") == 0)
        outFmt = MatrixConvert::FiveOneFilm_Cinema;
    else if (strcmp(outFmtStr, "FiveOneSmpte") == 0)
        outFmt = MatrixConvert::FiveOneSmpte;
    else if (strcmp(outFmtStr, "FiveOneDts") == 0)
        outFmt = MatrixConvert::FiveOneDts;
    else if (strcmp(outFmtStr, "SevenOnePt") == 0)
        outFmt = MatrixConvert::SevenOnePt;
    else if (strcmp(outFmtStr, "SevenOnePt_Cinema") == 0)
        outFmt = MatrixConvert::SevenOnePt_Cinema;
    else if (strcmp(outFmtStr, "TBE") == 0)
        outFmt = MatrixConvert::TBE;
    else if (strcmp(outFmtStr, "ACNSN3DO3A") == 0)
        outFmt = MatrixConvert::ACNSN3DO3A;
    else if (strcmp(outFmtStr, "FuMaO3A") == 0)
        outFmt = MatrixConvert::FuMaO3A;
    else if (strcmp(outFmtStr, "SevenOneSDDS") == 0)
        outFmt = MatrixConvert::SevenOneSDDS;
    else if (strcmp(outFmtStr, "SevenZeroSDDS") == 0)
        outFmt = MatrixConvert::SevenZeroSDDS;
    else if (strcmp(outFmtStr, "SevenOneTwo") == 0)
        outFmt = MatrixConvert::SevenOneTwo;
    else if (strcmp(outFmtStr, "SevenZeroTwo") == 0)
        outFmt = MatrixConvert::SevenZeroTwo;
    else if (strcmp(outFmtStr, "NineOne") == 0)
        outFmt = MatrixConvert::NineOne;
    else if (strcmp(outFmtStr, "NineZero") == 0)
        outFmt = MatrixConvert::NineZero;
    else if (strcmp(outFmtStr, "Stereo_Cinema") == 0)
        outFmt = MatrixConvert::Stereo_Cinema;
    else if (strcmp(outFmtStr, "FiveOneTwo") == 0)
        outFmt = MatrixConvert::FiveOneTwo;
    else if (strcmp(outFmtStr, "FiveZeroTwo") == 0)
        outFmt = MatrixConvert::FiveZeroTwo;
    else if (strcmp(outFmtStr, "FiveOneFour") == 0)
        outFmt = MatrixConvert::FiveOneFour;
    else if (strcmp(outFmtStr, "FiveZeroFour") == 0)
        outFmt = MatrixConvert::FiveZeroFour;
    else if (strcmp(outFmtStr, "SevenOneFour") == 0)
        outFmt = MatrixConvert::SevenOneFour;
    else if (strcmp(outFmtStr, "SevenZeroFour") == 0)
        outFmt = MatrixConvert::SevenZeroFour;
    else if (strcmp(outFmtStr, "SevenZero_Cinema") == 0)
        outFmt = MatrixConvert::SevenZero_Cinema;
    else if (strcmp(outFmtStr, "M1SpatialFaces") == 0)
        outFmt = MatrixConvert::M1SpatialFaces;
	else
	{
		cout << "Please select a valid output format" << std::endl;
		return -1;
	}
	pStr = getCmdOption(argv, argv + argc, "-out-file-chans");
	if (pStr != NULL)
		outFileChans = atoi(pStr);
	else
		outFileChans = 0;
	if (!((outFileChans == 0) || (outFileChans == 1) || (outFileChans == 2)))
	{
		cout << "Please select 0, 1, or 2, zero meaning a single, multichannel output file" << std::endl;
		return -1;
	}
	cout << std::endl;

	//=================================================================
	// initialize inputs, outputs and components 
	//

	// -- input file ---------------------------------------
	// determine number of input files
#define MAXINFILES (16)
	SndfileHandle *infile[MAXINFILES];
	vector<string> fNames;
	split(infilename, ' ', fNames);
	size_t numInFiles = fNames.size();
	for (int i = 0; i < numInFiles; i++)
	{
		infile[i] = new SndfileHandle(fNames[i]);
		if (infile[i] && (infile[i]->error() == 0))
		{
			// print input file stats
			cout << "Input File:         " << fNames[i] << std::endl;
			printFileInfo(*infile[i]);
			sampleRate = (long)infile[i]->samplerate();
		}
		else
		{
			cerr << "Error: opening in-file: " << fNames[i] << std::endl;
			return -1;
		}
	}

	cout << "Master Gain:        " << level2db(masterGain) << std::endl;
	cout << std::endl;

	inBufNdx = 0;
	outBufNdx = 0;
	MatrixConvert mc;

	infile[0]->seek(0, 0); // rewind input

	// -- output file(s) --------------------------------------
#define MAXOUTFILES (16)
	channels = mc.getNumChannels(outFmt);
	SndfileHandle outfiles[MAXOUTFILES];
	int actualOutFileChannels = outFileChans == 0 ? channels : outFileChans;
	int numOutFiles = channels / actualOutFileChannels;
	for (int i = 0; i < numOutFiles; i++)
	{
		const int format = infile[0]->format() & 0x1ffff; // convert waveformat-ex to waveformat
		char outfilestr[1024];
		if (numOutFiles > 1)
			sprintf_s(outfilestr, "%s_%0d.wav", outfilename, i);
		else
			strcpy_s(outfilestr, outfilename);
		outfiles[i] = SndfileHandle(outfilestr, SFM_WRITE, format, actualOutFileChannels, sampleRate);
		if (outfiles[i] && (outfiles[i].error() == 0))
		{
			// set clipping mode
			outfiles[i].command(SFC_SET_CLIPPING, NULL, SF_TRUE);
			// output file stats
			cout << "Output File:        " << outfilestr << std::endl;
			printFileInfo(outfiles[i]);
		}
		else
		{
			cerr << "Error: opening out-file: " << outfilestr << std::endl;
			return -1;
		}
	}
	cout << std::endl;

	for (int i = 0; i < MAXBUFFERS; i++)
	{
		memset(inBuffers[i], 0, sizeof(inBuffers[i]));
		memset(outBuffers[i], 0, sizeof(outBuffers[i]));
	}

	//=================================================================
	//  main sound loop 
	//

	// count total input channels
	int inChannels = 0;
	for (int i = 0; i < numInFiles; i++)
		inChannels += infile[i]->channels();
	sf_count_t numBlocks = infile[0]->frames() / BUFFERLEN; // files must be the same length
	totalSamples = 0;
	float peak = 0.0f;
	for (int pass = 1; pass <= (normalize ? 2 : 1); pass++)
	{
		// normalize
		if (normalize & (pass == 2))
		{
			cout << "Reducing gain by " << level2db(peak) << std::endl;
			masterGain /= peak;
			totalSamples = 0;
			for (int file = 0; file < numInFiles; file++)
				infile[file]->seek(0, SEEK_SET);
		}

		for (int i = 0; i <= numBlocks; i++)
		{
			// read next buffer from each infile
			sf_count_t samplesRead;
			sf_count_t firstBuf = 0;
			for (int file = 0; file < numInFiles; file++)
			{
				sf_count_t thisChannels = infile[file]->channels();
				sf_count_t framesRead = infile[file]->read(fileBuffer, thisChannels*BUFFERLEN);
				samplesRead = framesRead / thisChannels;
				// demultiplex into process buffers
				float *ptrFileBuffer = fileBuffer;
				float(*inBuf)[MAXBUFFERS][BUFFERLEN] = (float(*)[MAXBUFFERS][BUFFERLEN])&(inBuffers[0][0]);
				for (int j = 0; j < samplesRead; j++)
					for (int k = 0; k < thisChannels; k++)
						(*inBuf)[firstBuf + k][j] = *ptrFileBuffer++;
				firstBuf += thisChannels;
			}
			totalSamples += samplesRead;

			mc.convert(inFmt, inPtrs, outFmt, outPtrs, (int)samplesRead);

			if (normalize && (pass == 1))
			{
				// find max
				float *outBuf = (float(*))&(outBuffers[outBufNdx][0]);
				for (int i = 0; i < (numOutFiles*channels*samplesRead); i++)
				{
					float tmp = fabs(*outBuf++);
					if (tmp > peak)
						peak = tmp;
				}
			}
			else
			{
				// multiplex to output channels with master gain
				float *ptrFileBuffer = fileBuffer;
				float(*outBuf)[MAXBUFFERS][BUFFERLEN] = (float(*)[MAXBUFFERS][BUFFERLEN])&(outBuffers[outBufNdx][0]);
				for (int file = 0; file < numOutFiles; file++)
					for (int j = 0; j < samplesRead; j++)
						for (int k = 0; k < actualOutFileChannels; k++)
							*ptrFileBuffer++ = masterGain * (*outBuf)[(file*actualOutFileChannels) + k][j];

				// write to outfile
				for (int j = 0; j < numOutFiles; j++)
				{
					outfiles[j].write(fileBuffer + (j*actualOutFileChannels*samplesRead), actualOutFileChannels*samplesRead);
				}
			}
		}
	}
	// print time played
	cout << "Length (sec):     " << (float)totalSamples / (float)sampleRate << std::endl;

	return 0;
}
