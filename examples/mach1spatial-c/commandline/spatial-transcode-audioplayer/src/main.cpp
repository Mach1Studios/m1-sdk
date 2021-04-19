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
#include "Mach1Decode.h"
#include "M1DSPUtilities.h"
#include "sndfile.hh"
#include "CmdOption.h"
#include "rtaudio/RtAudio.h"

typedef signed short MY_TYPE;
#define FORMAT RTAUDIO_SINT16

int inout( void *outputBuffer, void *inputBuffer, unsigned int /*nBufferFrames*/,
           double /*streamTime*/, RtAudioStreamStatus status, void *data )
{
  // Since the number of input and output channels is equal, we can do
  // a simple buffer copy operation here.
  if ( status ) std::cout << "Stream over/underflow detected." << std::endl; //TODO: remove since ins and outs wont be same?

  unsigned int *bytes = (unsigned int *) data;
  memcpy( outputBuffer, inputBuffer, *bytes );
  return 0;
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

std::string convertToString(char* a, int size)
{
	int i;
    std::string s = "";
	for (i = 0; i < size; i++) {
		s = s + a[i];
	}
	return s;
}

void printHelp()
{
	std::cout << "spatial-transcode-audio -- light command line example conversion tool" << std::endl;
    std::cout << "note: for a complete transcoding tool use `m1-transcode` from the `binaries/executables` directory" << std::endl;
    std::cout << std::endl;
    std::cout << "usage: fmtconv -in-file test_s8.wav -in-fmt M1Spatial -out-file test_b.wav -out-fmt ACNSN3D -out-file-chans 0" << std::endl;
    std::cout << std::endl;
    std::cout << "  -help                 - list command line options" << std::endl;
    std::cout << "  -in-file  <filename>  - input file: put quotes around sets of files" << std::endl;
    std::cout << "  -in-fmt   <fmt>       - input format: see supported formats below" << std::endl;
    std::cout << "  -in-json  <json>      - input json: for input custom json Mach1Transcode templates" << std::endl;
    std::cout << "  -out-file <filename>  - output file. full name for single file or name stem for file sets" << std::endl;
    std::cout << "  -out-fmt  <fmt>       - output format: see supported formats below" << std::endl;
    std::cout << "  -out-json  <json>     - output json: for output custom json Mach1Transcode templates" << std::endl;
    std::cout << "  -out-file-chans <#>   - output file channels: 1, 2 or 0 (0 = multichannel)" << std::endl;
    std::cout << "  -normalize            - two pass normalize absolute peak to zero dBFS" << std::endl;
    std::cout << "  -master-gain <#>      - final output gain in dB like -3 or 2.3" << std::endl;
    std::cout << "  -spatial-downmix <#>  - compare top vs. bottom of the input soundfield, if difference is less than the set threshold (float) output format will be Mach1 Horizon" << std::endl;
    std::cout << std::endl;
    std::cout << "  Formats Supported:" << std::endl;
    std::cout << "    Stereo   - L & R spatialized" << std::endl;
    std::cout << "    Stereo_Cinema  - L & R spatialized, forward focus" << std::endl;
    std::cout << "    LCR      - L & R spatialized with C mono" << std::endl;
    std::cout << "    M1Horizon (Mach1 Horizon / Quad) - L R Ls Rs" << std::endl;
    std::cout << "    M1Horizon+S (Mach1 Horizon / Quad) - L R Ls Rs StereoL StereoR" << std::endl;
    std::cout << "    M1HorizonPairs (Mach1 Horizon / Quad-Binaural) - FrontPair, LeftPair, RearPair, RightPair" << std::endl;
    std::cout << "    M1Spatial (Mach1 Spatial) - Upper L R Ls Rs, Lower L R Ls Rs" << std::endl;
    std::cout << "    M1Spatial+S (Mach1 Spatial) - Upper L R Ls Rs, Lower L R Ls Rs, StereoL StereoR" << std::endl;
    std::cout << "    M1SpatialPairs (Mach1 Spatial Pairs) - Upper front, left, rear, right, pairs, then lower same" << std::endl;
    std::cout << "    M1SpatialFaces - Fc, Lc, Rc, Bc, Tc, Bc" << std::endl;
    std::cout << "    FiveOh   - L C R Ls Rs" << std::endl;
    std::cout << "    FiveOneFilm (Pro Tools default / C|24)  - L C R Ls Rs LFE" << std::endl;
    std::cout << "    FiveOneFilm_Cinema (Pro Tools default / C|24)  - L C R Ls Rs LFE, forward focus" << std::endl;
    std::cout << "    FiveOneSmpte (SMPTE/ITU for Dolby Digital (AC3) - L R C LFE Ls Rs" << std::endl;

    std::cout << "    FiveOneDts (DTS) - L R Ls Rs C LFE" << std::endl;
    std::cout << "    SevenOnePt (Pro Tools default) - L C R Lss Rss Lsr Rsr LFE" << std::endl;
    std::cout << "    SevenOnePt_Cinema (Pro Tools default) - L C R Lss Rss Lsr Rsr LFE, forward focus" << std::endl;
    std::cout << "    SevenZero_Cinema (Pro Tools default) - L C R Lss Rss Lsr Rsr, forward focus" << std::endl;
    std::cout << "    SevenOneSDDS (Sony SDDS) - L Lc C Rc R Ls Rs LFE" << std::endl;
    std::cout << "    SevenZeroSDDS (Sony SDDS) - L Lc C Rc R Ls Rs" << std::endl;
    std::cout << "    FiveOneTwo (Film / Pro Tools default) - L C R Lss Rss Lsr Rsr FLts FRts BLts BRts" << std::endl;
    std::cout << "    FiveZeroTwo (Film / Pro Tools default) - L C R Lss Rss Lsr Rsr FLts FRts BLts BRts" << std::endl;
    std::cout << "    FiveOneFour (Film / Pro Tools default) - L C R Lss Rss Lsr Rsr FLts FRts BLts BRts" << std::endl;
    std::cout << "    FiveZeroFour (Film / Pro Tools default) - L C R Lss Rss Lsr Rsr FLts FRts BLts BRts" << std::endl;
    std::cout << "    SevenOneTwo (Film / Pro Tools default) - L C R Lss Rss Lsr Rsr LFE Lts Rts" << std::endl;
    std::cout << "    SevenZeroTwo (Film / Pro Tools default) - L C R Lss Rss Lsr Rsr Lts Rts" << std::endl;
    std::cout << "    SevenOneFour (Film / Pro Tools default) - L C R Lss Rss Lsr Rsr LFE FLts FRts BLts BRts" << std::endl;
    std::cout << "    SevenZeroFour (Film / Pro Tools default) - L C R Lss Rss Lsr Rsr FLts FRts BLts BRts" << std::endl;
    std::cout << "    SevenOneTwo (SMPTE) - L R C LFE Lss Rss Lsr Rsr Lts Rts [INCOMPLETE]" << std::endl;
    std::cout << "    SevenZeroTwo (SMPTE) - L R C Lss Rss Lsr Rsr Lts Rts [INCOMPLETE]" << std::endl;
    std::cout << "    ACNSN3D  - 1st order B-format, ACN order and SN3D weighting" << std::endl;
    std::cout << "    FuMa     - 1st order B-format, Furse-Malham order and weighting" << std::endl;
    std::cout << "    TBE   - W, X, Y, Z, U, V, T, S" << std::endl;
    std::cout << "    ACNSN3DO2A   - 2nd order B-format, AmbiX ACN order and SN3D weighting" << std::endl;
    std::cout << "    FuMaO2A      - 2nd order B-format, Furse-Malham order and weighting, W, Y, Z, X, V, T, R, S, U" << std::endl;
    std::cout << "    ACNSN3DO3A   - 16 channel AmbiX" << std::endl;
    std::cout << "    FuMaO3A   - 3rd order B-format, W, Y, Z, X, V, T, R, S, U, Q, O, M, K, L, N, P" << std::endl;
    std::cout << "    ACNSN3DmaxRE1oa   - 1st order, AmbiX ACN order and SN3D-maxRE from IEM AllRAD" << std::endl;
    std::cout << "    ACNSN3DmaxRE2oa   - 2nd order, AmbiX ACN order and SN3D-maxRE from IEM AllRAD" << std::endl;
    std::cout << "    ACNSN3DmaxRE3oa   - 3rd order, AmbiX ACN order and SN3D-maxRE from IEM AllRAD" << std::endl;
    std::cout << "    ACNSN3DmaxRE4oa   - 4th order, AmbiX ACN order and SN3D-maxRE from IEM AllRAD" << std::endl;
    std::cout << "    ACNSN3DmaxRE5oa   - 5th order, AmbiX ACN order and SN3D-maxRE from IEM AllRAD" << std::endl;
    std::cout << "    ACNSN3DmaxRE6oa   - 6th order, AmbiX ACN order and SN3D-maxRE from IEM AllRAD" << std::endl;
    std::cout << "    ACNSN3DmaxRE7oa   - 7th order, AmbiX ACN order and SN3D-maxRE from IEM AllRAD" << std::endl;
    std::cout << "    16.0   - 16 channel Surround 3D layout" << std::endl;
    std::cout << std::endl;
}

void printFileInfo(SndfileHandle file)
{
    std::cout << "Sample Rate:        " << file.samplerate() << std::endl;
	int format = file.format() & 0xffff;
    if (format == SF_FORMAT_PCM_16) std::cout << "Bit Depth:          16" << std::endl;
	if (format == SF_FORMAT_PCM_24) std::cout << "Bit Depth:          24" << std::endl;
	if (format == SF_FORMAT_FLOAT)  std::cout << "Bit Depth:          32" << std::endl;
    std::cout << "Channels:           " << file.channels() << std::endl;
	//cout << "Length (sec):     " << (float)file.frames() / (float)file.samplerate() << std::endl;
    std::cout << std::endl;
}

// ---------------------------------------------------------
#define BUFFERLEN 512

int main(int argc, char* argv[])
{
    // RtAudio setup
    // Set the same number of channels for both input and output.
    unsigned int audioOutputChannels = 2, fs, bufferBytes, oDevice = 0, iDevice = 0, iOffset = 0, oOffset = 0;
    unsigned int bufferFrames = 512;
    RtAudio::StreamParameters iParams, oParams;
    iParams.deviceId = iDevice;
    iParams.nChannels = audioOutputChannels;
    iParams.firstChannel = iOffset;
    oParams.deviceId = oDevice;
    oParams.nChannels = audioOutputChannels;
    oParams.firstChannel = oOffset;
    
    RtAudio adac;
    if ( adac.getDeviceCount() < 1 ) {
      std::cout << "\nNo audio devices found!\n";
      exit( 1 );
    }

    if ( iDevice == 0 )
      iParams.deviceId = adac.getDefaultInputDevice();
    if ( oDevice == 0 )
      oParams.deviceId = adac.getDefaultOutputDevice();
    
    // Mach1Transcode setup
	Mach1Transcode m1transcode;

	// locals for cmd line parameters
	bool fileOut = false;
	//TODO: inputGain = 1.0f; // in level, not db
	float masterGain = 1.0f; // in level, not dB
	bool normalize = false;
	char* infilename = NULL;
	char* inFmtStr = NULL;
	Mach1TranscodeFormatType inFmt;
	char* outfilename = NULL;
	std::string md_outfilename = "";
	char* outFmtStr = NULL;
	Mach1TranscodeFormatType outFmt;
	int outFileChans;
	int outFormatChannels;
    M1DSP::Utilities::CSpatialDownmixChecker spatialDownmixChecker;
	float corrThreshold = 0.1; // 10% difference in signal or less will auto downmix
    
	sf_count_t totalSamples;
	long sampleRate;

	// multiplexed buffers
	float fileBuffer[Mach1TranscodeMAXCHANS * BUFFERLEN];

	// process buffers
	float inBuffers[Mach1TranscodeMAXCHANS][BUFFERLEN];
	float *inPtrs[Mach1TranscodeMAXCHANS];
	float outBuffers[Mach1TranscodeMAXCHANS][BUFFERLEN];
	float *outPtrs[Mach1TranscodeMAXCHANS];
	for (int i = 0; i < Mach1TranscodeMAXCHANS; i++) {
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
		|| argc == 1) {
		printHelp();
		return 0;
	}
	pStr = getCmdOption(argv, argv + argc, "-normalize");
	if (pStr != NULL) {
		normalize = true;
	}
	pStr = getCmdOption(argv, argv + argc, "-master-gain");
	if (pStr != NULL) {
		masterGain = (float)atof(pStr); // still in dB
		masterGain = m1transcode.db2level(masterGain);
	}
	/*
	 flag for auto Mach1 Spatial downmixer
	 compares top/bottom to downmix to Horizon
	 TODO: scale to other formats
	 */
	// input file name and format
	pStr = getCmdOption(argv, argv + argc, "-in-file");
	if (pStr && (strlen(pStr) > 0)) {
		infilename = pStr;
	} else {
        std::cerr << "Please specify an input file" << std::endl;
		return -1;
	}
	pStr = getCmdOption(argv, argv + argc, "-in-fmt");
	if (pStr && (strlen(pStr) > 0)) {
		inFmtStr = pStr;
		if (strcmp(inFmtStr, "TTPoints") == 0) {
			pStr = getCmdOption(argv, argv + argc, "-in-json");
			if (pStr && (strlen(pStr) > 0)) {
                std::ifstream file(pStr);
                std::string strJson((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
				m1transcode.setInputFormatTTJson((char*)strJson.c_str());
			}
		}
	}

	bool foundInFmt = false;
	inFmt = m1transcode.getFormatFromString(inFmtStr);
	if (inFmt != Mach1TranscodeFormatType::Mach1TranscodeFormatEmpty) {
		foundInFmt = true;
	} else {
        std::cerr << "Please select a valid input format" << std::endl;
		return -1;
	}

	// output file name and format
	pStr = getCmdOption(argv, argv + argc, "-out-file");
	if (pStr && (strlen(pStr) > 0)) {
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
	if (pStr && (strlen(pStr) > 0)) {
		outFmtStr = pStr;
		if (strcmp(outFmtStr, "TTPoints") == 0) {
			pStr = getCmdOption(argv, argv + argc, "-out-json");
			if (pStr && (strlen(pStr) > 0)) {
                std::ifstream file(pStr);
                std::string strJson((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
				m1transcode.setOutputFormatTTJson((char*)strJson.c_str());
			}
		}
	}

    outFmt = Mach1TranscodeFormatType::Mach1TranscodeFormatM1Spatial;

	//=================================================================
	// initialize inputs, outputs and components
	//

	// -- input file ---------------------------------------
	// determine number of input files
	SndfileHandle *infile[Mach1TranscodeMAXCHANS];
    std::vector<std::string> fNames;
	split(infilename, ' ', fNames);
	size_t numInFiles = fNames.size();
	for (int i = 0; i < numInFiles; i++) {
		infile[i] = new SndfileHandle(fNames[i].c_str());
		if (infile[i] && (infile[i]->error() == 0)) {
			// print input file stats
            std::cout << "Input File:         " << fNames[i] << std::endl;
			printFileInfo(*infile[i]);
			sampleRate = (long)infile[i]->samplerate();
			//            int inChannels = 0;
			//            for (int i = 0; i < numInFiles; i++)
			//                inChannels += infile[i]->channels();
			//            parseFile(*infile[i], inChannels);
		} else {
            std::cerr << "Error: opening in-file: " << fNames[i] << std::endl;
			return -1;
		}
	}

    std::cout << "Master Gain:        " << m1transcode.level2db(masterGain) << "dB" << std::endl;
    std::cout << std::endl;

	for (int i = 0; i < numInFiles; i++) {
		infile[i]->seek(0, 0); // rewind input
	}

	// -- Transcode setup
	m1transcode.setInputFormat(inFmt);
	m1transcode.setOutputFormat(outFmt);
    
    // -- Mach1 Decode
    Mach1Decode m1decode;
    m1decode.setDecodeAlgoType(Mach1DecodeAlgoSpatial);

	// -- output file(s) --------------------------------------

	outFormatChannels = m1transcode.getOutputNumChannels();
	SndfileHandle outfiles[Mach1TranscodeMAXCHANS];
	int actualOutFileChannels = outFileChans == 0 ? outFormatChannels : outFileChans;

	if (actualOutFileChannels == 0) {
        std::cerr << "Output channels count is 0!" << std::endl;
		return -1;
	}

	int numOutFiles = outFormatChannels / actualOutFileChannels;

	for (int i = 0; i < Mach1TranscodeMAXCHANS; i++) {
		memset(inBuffers[i], 0, sizeof(inBuffers[i]));
		memset(outBuffers[i], 0, sizeof(outBuffers[i]));
	}

	//=================================================================
	//  print intermediate formats path
	//
	if (!m1transcode.processConversionPath()) {
		printf("No applicable conversion between selected input & output formats!");
		return -1;
	}
	else {
        std::vector<Mach1TranscodeFormatType> formatsConvertionPath = m1transcode.getFormatConversionPath();
		printf("Conversion Path:    ");
		for (int k = 0; k < formatsConvertionPath.size(); k++) {
            printf("%s", m1transcode.getFormatName(formatsConvertionPath[k]).c_str());
			if (k < formatsConvertionPath.size() - 1) {
				printf(" > ");
			}
		}
		printf("\r\n");
	}

    // Return matrix of coeffs for conversion for further customization or tweaking
    std::vector<std::vector<float>> matrix = m1transcode.getMatrixConversion();

	//=================================================================
	//  main sound loop
	// 
	int inChannels = 0;
    for (int i = 0; i < numInFiles; i++) {
		inChannels += infile[i]->channels();
    }
	sf_count_t numBlocks = infile[0]->frames() / BUFFERLEN; // files must be the same length
	totalSamples = 0;
	float peak = 0.0f;

    /*
     Multi-loop processing:
     - if normalization or spatial downmixer is used then the render will use 2 loops to process
     - otherwise only 1 loop used to process
     */
	for (int currentRenderLoop = 1, maxNumRenderLoops = normalize; currentRenderLoop <= maxNumRenderLoops; currentRenderLoop++) {
		if (currentRenderLoop == 2) {
			// normalize
			if (normalize) {
                std::cout << "Reducing gain by " << m1transcode.level2db(peak) << std::endl;
				masterGain /= peak;
			}

			totalSamples = 0;
            for (int file = 0; file < numInFiles; file++) {
				infile[file]->seek(0, SEEK_SET);
            }
		} 

		if (currentRenderLoop == maxNumRenderLoops){ // final loop of processing (prepares the output file)
			// init outfiles
			for (int i = 0; i < numOutFiles; i++) {
                //TODO: expand this out to other output types and better handling from printFileInfo()
                int format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
                int inputFormat = infile[0]->format() & 0xffff;
                if (inputFormat == SF_FORMAT_PCM_16) format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
                if (inputFormat == SF_FORMAT_PCM_24) format = SF_FORMAT_WAV | SF_FORMAT_PCM_24;
                if (inputFormat == SF_FORMAT_FLOAT)  format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;
				char outfilestr[1024];
                if (numOutFiles > 1) {
					sprintf(outfilestr, "%s_%0d.wav", outfilename, i);
                } else {
					strcpy(outfilestr, outfilename);
                }
                outfiles[i] = SndfileHandle(outfilestr, SFM_WRITE, format, actualOutFileChannels, (int)sampleRate);
				if (outfiles[i] && (outfiles[i].error() == 0)) {
					// set clipping mode
					outfiles[i].command(SFC_SET_CLIPPING, NULL, SF_TRUE);
					// output file stats
                    std::cout << "Output File:        " << outfilestr << std::endl;
					printFileInfo(outfiles[i]);
				}
				else
				{
                    std::cerr << "Error: opening out-file: " << outfilestr << std::endl;
					return -1;
				}
				if (outFmt == Mach1TranscodeFormatType::Mach1TranscodeFormatM1Spatial) {
					outfiles[i].setString(0x05, "mach1spatial-8");
				}
				else if (outFmt == Mach1TranscodeFormatType::Mach1TranscodeFormatM1Horizon) {
					outfiles[i].setString(0x05, "mach1horizon-4");
				}
				else if (outFmt == Mach1TranscodeFormatType::Mach1TranscodeFormatM1HorizonPairs) {
					outfiles[i].setString(0x05, "mach1horizon-8");
				}
			}
            std::cout << std::endl;
		}

		for (int i = 0; i <= numBlocks; i++) {
			// read next buffer from each infile
			sf_count_t samplesRead;
			sf_count_t firstBuf = 0;
			for (int file = 0; file < numInFiles; file++){
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

			if (currentRenderLoop == 1) {
				if (normalize) {
					// find max for first pass normalization
					peak = (std::max)(peak, m1transcode.processNormalization(outPtrs, (int)samplesRead));
				}
			}

			if (currentRenderLoop == maxNumRenderLoops) {
				m1transcode.processMasterGain(outPtrs, (int)samplesRead, masterGain);

				// multiplex to output channels with master gain
				float *ptrFileBuffer = fileBuffer;
				float(*outBuf)[Mach1TranscodeMAXCHANS][BUFFERLEN] = (float(*)[Mach1TranscodeMAXCHANS][BUFFERLEN]) & (outBuffers[0][0]);
                for (int file = 0; file < numOutFiles; file++) {
                    for (int j = 0; j < samplesRead; j++) {
                        for (int k = 0; k < actualOutFileChannels; k++) {
							*ptrFileBuffer++ = (*outBuf)[(file * actualOutFileChannels) + k][j];
                        }
                    }
                }

				// write to outfile
				for (int j = 0; j < numOutFiles; j++) {
					outfiles[j].write(fileBuffer +
                                      (j * actualOutFileChannels * samplesRead),
                                      actualOutFileChannels * samplesRead);
				}
			}
		}
	}
	// print time played
    std::cout << "Length (sec):     " << (float)totalSamples / (float)sampleRate << std::endl;
	return 0;
}
