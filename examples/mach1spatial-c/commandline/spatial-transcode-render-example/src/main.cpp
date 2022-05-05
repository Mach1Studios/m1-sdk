//  Mach1 Spatial SDK
//  Copyright © 2017-2021 Mach1. All rights reserved.

/*
 This example is for reference for how Mach1Spatial Coefficients from Mach1Transcode API
 could be used on audio streams and buffers.
 
 Order of Operations:
 1. Setup Input and Output formats (and paths)
 2. Call `processConversionPath()` to setup the conversion for processing
 3. Use `setSpatialDownmixer()` & `getSpatialDownmixerPossibility()` to downmix content to Mach1Horizon if top/bottom
    difference is less than correlation threshold
    Note: Afterwards reinitizalize setup of Input and Output formats
 4. Call `processConversion()` to execute the conversion and return coeffs per buffer/sample per channel
 5. Apply to buffer/samples per channel in file rendering or audio mixer
 */

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>

#include "Mach1Transcode.h"
#include "sndfile.hh"
#include "CmdOption.h"

using namespace std;

vector<string> &split(const string &s, char delim, vector<string> &elems) {
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

string convertToString(char* a, int size)
{
	int i;
	string s = "";
	for (i = 0; i < size; i++) {
		s = s + a[i];
	}
	return s;
}

void printHelp()
{
	cout << "spatial-transcode-audio -- light command line example conversion tool" << std::endl;
    cout << "note: for a complete transcoding tool use `m1-transcode` from the `executables` directory" << std::endl;
	cout << std::endl;
	cout << "usage: fmtconv -in-file test_s8.wav -in-fmt M1Spatial -out-file test_b.wav -out-fmt ACNSN3D -out-file-chans 0" << std::endl;
	cout << std::endl;
	cout << "  -help                 - list command line options" << std::endl;
	cout << "  -in-file  <filename>  - input file: put quotes around sets of files" << std::endl;
	cout << "  -in-fmt   <fmt>       - input format: see supported formats below" << std::endl;
    cout << "  -in-json  <json>      - input json: for input custom json Mach1Transcode templates" << std::endl;
	cout << "  -out-file <filename>  - output file. full name for single file or name stem for file sets" << std::endl;
	cout << "  -out-fmt  <fmt>       - output format: see supported formats below" << std::endl;
    cout << "  -out-json  <json>     - output json: for output custom json Mach1Transcode templates" << std::endl;
	cout << "  -out-file-chans <#>   - output file channels: 1, 2 or 0 (0 = multichannel)" << std::endl;
	cout << "  -normalize            - two pass normalize absolute peak to zero dBFS" << std::endl;
	cout << "  -master-gain <#>      - final output gain in dB like -3 or 2.3" << std::endl;
	cout << "  -spatial-downmix <#>  - compare top vs. bottom of the input soundfield, if difference is less than the set threshold (float) output format will be Mach1 Horizon" << std::endl;
	cout << std::endl;
    std::cout << "  Formats Supported: https://dev.mach1.tech/#formats-supported" << std::endl;
	cout << std::endl;
}

void printFileInfo(SndfileHandle file)
{
	cout << "Sample Rate:        " << file.samplerate() << std::endl;
	int format = file.format() & 0xffff;
    if (format == SF_FORMAT_PCM_16) cout << "Bit Depth:          16" << std::endl;
	if (format == SF_FORMAT_PCM_24) cout << "Bit Depth:          24" << std::endl;
	if (format == SF_FORMAT_FLOAT)  cout << "Bit Depth:          32" << std::endl;
	cout << "Channels:           " << file.channels() << std::endl;
	//cout << "Length (sec):     " << (float)file.frames() / (float)file.samplerate() << std::endl;
	cout << std::endl;
}

// ---------------------------------------------------------
#define BUFFERLEN 512

int main(int argc, char* argv[])
{
	Mach1Transcode m1transcode;

	// locals for cmd line parameters
	bool fileOut = false;
	//TODO: inputGain = 1.0f; // in level, not db
	float masterGain = 1.0f; // in level, not dB
	bool normalize = false;
	char* infilename = NULL;
	char* inFmtStr = NULL;
	int inFmt;
	char* outfilename = NULL;
	std::string md_outfilename = "";
	char* outFmtStr = NULL;
	int outFmt;
	int outFileChans;
	int channels;
	bool spatialDownmixerMode = false;

	float corrThreshold = 0.1;
	sf_count_t totalSamples;
	long sampleRate;

	// multiplexed buffers
	float fileBuffer[Mach1TranscodeMAXCHANS * BUFFERLEN];

	// process buffers
	float inBuffers[Mach1TranscodeMAXCHANS][BUFFERLEN];
	float *inPtrs[Mach1TranscodeMAXCHANS];
	float outBuffers[Mach1TranscodeMAXCHANS][BUFFERLEN];
	float *outPtrs[Mach1TranscodeMAXCHANS];
	for (int i = 0; i < Mach1TranscodeMAXCHANS; i++)
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
		masterGain = m1transcode.db2level(masterGain);
	}
	/*
	 flag for auto Mach1 Spatial downmixer
	 compares top/bottom to downmix to Horizon
	 TODO: scale to other formats
	 */
	pStr = getCmdOption(argv, argv + argc, "-spatial-downmix");
	if (pStr != NULL)
	{
		spatialDownmixerMode = true;
		corrThreshold = atof(pStr);

		m1transcode.setSpatialDownmixer(corrThreshold);
	}
	if (spatialDownmixerMode && (corrThreshold < 0.0 || corrThreshold > 1.0))
	{
		cout << "Please use 0.0 to 1.0 range for correlation threshold" << std::endl;
		return -1;
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
		if (strcmp(inFmtStr, "CustomPoints") == 0) {
			pStr = getCmdOption(argv, argv + argc, "-in-json");
			if (pStr && (strlen(pStr) > 0))
            {
                std::ifstream file(pStr);
                std::string strJson((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
				m1transcode.setInputFormatCustomPointsJson((char*)strJson.c_str());
			}
		}
	}

	bool foundInFmt = false;
	inFmt = m1transcode.getFormatFromString(inFmtStr);
    if (inFmt > 1) { // if format int is 0 or -1 (making it invalid)
		foundInFmt = true;
	}
	else {
		cout << "Please select a valid input format" << std::endl;
		return -1;
	}

	// output file name and format
	pStr = getCmdOption(argv, argv + argc, "-out-file");
	if (pStr && (strlen(pStr) > 0))
	{
		fileOut = true;
		outfilename = pStr;
		std::string FileExt = ".txt";
		int outfilename_size = strlen(outfilename);
		md_outfilename = convertToString(outfilename, outfilename_size);
		std::string Path, FileName;
		std::string::size_type found = md_outfilename.find_last_of(".");
		// if we found one of this symbols
		if (found != std::string::npos) {
			// path will be all symbols before found position
			Path = md_outfilename.substr(0, found);
		}
		else { // if we not found '.', path is empty
			Path.clear();
		}
		md_outfilename += FileExt;
	}
	pStr = getCmdOption(argv, argv + argc, "-out-fmt");
	if (pStr && (strlen(pStr) > 0))
	{
		outFmtStr = pStr;
		if (strcmp(outFmtStr, "CustomPoints") == 0) {
			pStr = getCmdOption(argv, argv + argc, "-out-json");
			if (pStr && (strlen(pStr) > 0))
			{
                std::ifstream file(pStr);
                std::string strJson((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
				m1transcode.setOutputFormatCustomPointsJson((char*)strJson.c_str());
			}
		}
	}

	bool foundOutFmt = false;
	outFmt = m1transcode.getFormatFromString(outFmtStr);
    if (outFmt > 1) { // if format int is 0 or -1 (making it invalid)
		foundOutFmt = true;
	}
	else {
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
	SndfileHandle *infile[Mach1TranscodeMAXCHANS];
	vector<string> fNames;
	split(infilename, ' ', fNames);
	size_t numInFiles = fNames.size();
	for (int i = 0; i < numInFiles; i++)
	{
		infile[i] = new SndfileHandle(fNames[i].c_str());
		if (infile[i] && (infile[i]->error() == 0))
		{
			// print input file stats
			cout << "Input File:         " << fNames[i] << std::endl;
			printFileInfo(*infile[i]);
			sampleRate = (long)infile[i]->samplerate();
			//            int inChannels = 0;
			//            for (int i = 0; i < numInFiles; i++)
			//                inChannels += infile[i]->channels();
			//            parseFile(*infile[i], inChannels);
		}
		else
		{
			cerr << "Error: opening in-file: " << fNames[i] << std::endl;
			return -1;
		}
	}

	cout << "Master Gain:        " << m1transcode.level2db(masterGain) << "dB" << std::endl;
    cout << std::endl;

	for (int i = 0; i < numInFiles; i++) {
		infile[i]->seek(0, 0); // rewind input
	}

	// -- setup 
	m1transcode.setInputFormat(inFmt);
	m1transcode.setOutputFormat(outFmt);

	// -- output file(s) --------------------------------------

	channels = m1transcode.getOutputNumChannels();
	SndfileHandle outfiles[Mach1TranscodeMAXCHANS];
	int actualOutFileChannels = outFileChans == 0 ? channels : outFileChans;

	if (actualOutFileChannels == 0) {
		cout << "Output channels count is 0!" << std::endl;
		return -1;
	}

	int numOutFiles = channels / actualOutFileChannels;

	for (int i = 0; i < Mach1TranscodeMAXCHANS; i++)
	{
		memset(inBuffers[i], 0, sizeof(inBuffers[i]));
		memset(outBuffers[i], 0, sizeof(outBuffers[i]));
	}

	//=================================================================
	//  print intermediate formats path
	//
	if (!m1transcode.processConversionPath()) {
		printf("Can't found conversion between formats!");
		return -1;
	}
	else {
        std::vector<int> formatsConvertionPath = m1transcode.getFormatConversionPath();
		printf("Conversion Path:    ");
		for (int k = 0; k < formatsConvertionPath.size(); k++) {
            printf("%s", m1transcode.getFormatName(formatsConvertionPath[k]).c_str());
			if (k < formatsConvertionPath.size() - 1) {
				printf(" > ");
			}
		}
		printf("\r\n");
	}

	vector< vector<float> > matrix = m1transcode.getMatrixConversion();

	//=================================================================
	//  main sound loop
	// 
	int inChannels = 0;
	for (int i = 0; i < numInFiles; i++)
		inChannels += infile[i]->channels();
	sf_count_t numBlocks = infile[0]->frames() / BUFFERLEN; // files must be the same length
	totalSamples = 0;
	float peak = 0.0f;

	for (int pass = 1, countPasses = ((normalize || spatialDownmixerMode) ? 2 : 1); pass <= countPasses; pass++)
	{
		if (pass == 2) {
			// Mach1 Spatial Downmixer
			// Triggered due to correlation of top vs bottom
			// being higher than threshold
			if (spatialDownmixerMode && outFmt == m1transcode.getFormatFromString("M1Spatial"))
			{
				if (m1transcode.getSpatialDownmixerPossibility())
				{
					vector<float> avgSamples = m1transcode.getAvgSamplesDiff();

					// reinitialize inputs and outputs
					outFmt = m1transcode.getFormatFromString("M1Horizon");
					m1transcode.setOutputFormat(outFmt);
					m1transcode.processConversionPath();

					channels = m1transcode.getOutputNumChannels();
					actualOutFileChannels = outFileChans == 0 ? channels : outFileChans;
					numOutFiles = channels / actualOutFileChannels;

					printf("Spatial Downmix:    ");
                    printf("%s\r\n", m1transcode.getFormatName(outFmt).c_str());
                    printf("Top/Bottom Soundfield Differences:\r\n%f %f %f %f\r\n", avgSamples[0], avgSamples[1], avgSamples[2], avgSamples[3], avgSamples[4], avgSamples[5], avgSamples[6], avgSamples[7] );
                    printf("\r\n");
				}
			}

			// normalize
			if (normalize)
			{
				cout << "Reducing gain by " << m1transcode.level2db(peak) << std::endl;
				masterGain /= peak;
			}

			totalSamples = 0;
			for (int file = 0; file < numInFiles; file++)
				infile[file]->seek(0, SEEK_SET);
		} 

		if (pass == countPasses) {
			// init outfiles
			for (int i = 0; i < numOutFiles; i++)
			{
                //TODO: expand this out to other output types and better handling from printFileInfo()
                int format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
                int inputFormat = infile[0]->format() & 0xffff;
                if (inputFormat == SF_FORMAT_PCM_16) format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
                if (inputFormat == SF_FORMAT_PCM_24) format = SF_FORMAT_WAV | SF_FORMAT_PCM_24;
                if (inputFormat == SF_FORMAT_FLOAT)  format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;
				char outfilestr[1024];
				if (numOutFiles > 1)
					sprintf(outfilestr, "%s_%0d.wav", outfilename, i);
				else
					strcpy(outfilestr, outfilename);
				outfiles[i] = SndfileHandle(outfilestr, SFM_WRITE, format, actualOutFileChannels, (int)sampleRate);
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
				if (outFmt == m1transcode.getFormatFromString("M1Spatial")) {
					outfiles[i].setString(0x05, "mach1spatial-8");
				}
				else if (outFmt == m1transcode.getFormatFromString("M1Horizon")) {
					outfiles[i].setString(0x05, "mach1horizon-4");
				}
				else if (outFmt == m1transcode.getFormatFromString("M1HorizonPairs")) {
					outfiles[i].setString(0x05, "mach1horizon-8");
				}
			}
			cout << std::endl;
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
				float(*inBuf)[Mach1TranscodeMAXCHANS][BUFFERLEN] = (float(*)[Mach1TranscodeMAXCHANS][BUFFERLEN])&(inBuffers[0][0]);
				for (int j = 0; j < samplesRead; j++)
					for (int k = 0; k < thisChannels; k++)
						(*inBuf)[firstBuf + k][j] = *ptrFileBuffer++;
				firstBuf += thisChannels;
			}
			totalSamples += samplesRead;

            /*
             `processConversion()` is called after `processConversionPath() has been called and set at least once!
             */
			m1transcode.processConversion(inPtrs, outPtrs, (int)samplesRead);

			if (pass == 1) {
				if (normalize)
				{
					// find max for first pass normalization
					peak = (std::max)(peak, m1transcode.processNormalization(outPtrs, (int)samplesRead));
				}
			}

			if (pass == countPasses)
			{
				m1transcode.processMasterGain(outPtrs, (int)samplesRead, masterGain);

				// multiplex to output channels with master gain
				float *ptrFileBuffer = fileBuffer;
				float(*outBuf)[Mach1TranscodeMAXCHANS][BUFFERLEN] = (float(*)[Mach1TranscodeMAXCHANS][BUFFERLEN])&(outBuffers[0][0]);
				for (int file = 0; file < numOutFiles; file++)
					for (int j = 0; j < samplesRead; j++)
						for (int k = 0; k < actualOutFileChannels; k++)
							*ptrFileBuffer++ = (*outBuf)[(file*actualOutFileChannels) + k][j];

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
