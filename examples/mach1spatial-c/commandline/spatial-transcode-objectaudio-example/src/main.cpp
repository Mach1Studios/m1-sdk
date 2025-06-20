//  Mach1 Spatial SDK
//  Copyright © 2017-2021 Mach1. All rights reserved.

/*
 This example is for reference for how Mach1Spatial Coefficients from Mach1Transcode API
 could be used on audio streams and buffers.

 Order of Operations:
 1. Setup Input and Output formats (and paths)
 2. Call `processConversionPath()` to setup the conversion for processing
 3. Call `processConversion()` to execute the conversion and return coeffs per buffer/sample per channel
 4. Apply to buffer/samples per channel in file rendering or audio mixer
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
#include "Mach1AudioTimeline.h"

#include "sndfile.hh"
#include "CmdOption.h"
#include "yaml/Yaml.hpp"
#include "pugixml.hpp"
#include "bw64/bw64.hpp"
#include "adm_metadata.h"

std::vector<Mach1AudioObject> audioObjects;
std::vector<Mach1Point3D> keypoints;

Mach1Point3D* callbackPointsSampler(long long sample, int& n) {
	keypoints.resize(audioObjects.size());

	for (int i = 0; i < audioObjects.size(); i++) {
		std::vector<Mach1KeyPoint> kp = audioObjects[i].getKeyPoints();

		keypoints[i] = kp[0].point;
		for (int j = 0; j < kp.size(); j++) {
			if (kp[j].sample >= sample) {
				keypoints[i] = kp[j].point;
				continue;
			}
		}
	}

	n = keypoints.size();
	return keypoints.data();
}

using namespace std;

vector<string> &split(const string &s, char delim, vector<string> &elems) {
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

string convertToString(char* a, int size) {
	int i;
	string s = "";
	for (i = 0; i < size; i++) {
		s = s + a[i];
	}
	return s;
}

void printHelp() {
    std::cout << "spatial-transcode-objectaudio-example -- light command line example conversion tool" << std::endl;
    std::cout << "note: for a complete transcoding tool use `m1-transcode` from the `executables` directory" << std::endl;
    std::cout << std::endl;
    std::cout << "usage: ./m1-transcode -in-file test_s8.wav -in-fmt M1Spatial -out-file test_b.wav -out-fmt ACNSN3D -out-file-chans 0" << std::endl;
    std::cout << "usage: ./m1-transcode -in-file test_s8.wav -in-fmt M1Spatial -out-file 7_1_2-ADM.wav -out-fmt 7.1.2_M -write-metada -out-file-chans 0" << std::endl;
    std::cout << "all boolean argument flags should be used before the end of the command to ensure it is captured" << std::endl;
	std::cout << std::endl;
	std::cout << "  -help                    - list command line options" << std::endl;
	std::cout << "  -in-folder <folder>      - input folder: folder for audio files" << std::endl;
	std::cout << "  -in-file <filename>      - input file: put quotes around sets of files" << std::endl;
	std::cout << "  -in-file-meta <filename> - input meta file: only for Atmos" << std::endl;
	std::cout << "  -in-fmt   <fmt>          - input format: see supported formats below" << std::endl;
	std::cout << "  -in-json  <json>         - input json: for input custom json Mach1Transcode templates" << std::endl;
	std::cout << "  -out-file <filename>     - output file. full name for single file or name stem for file sets" << std::endl;
	std::cout << "  -out-fmt  <fmt>          - output format: see supported formats below" << std::endl;
	std::cout << "  -write-metadata       	- write channel-bed ADM metadata for supported formats" << std::endl;
	std::cout << std::endl;
	std::cout << "  Channel-Bed Formats Supported: ADM" << std::endl;
	std::cout << "    5.1.4_M (Film / Pro Tools default) - L C R Lss Rss Lsr Rsr FLts FRts BLts BRts [ADM Metadata Supported]" << std::endl;
	std::cout << "    5.1.4_C (Film / Pro Tools default) - L C R Lss Rss Lsr Rsr FLts FRts BLts BRts [ADM Metadata Supported]" << std::endl;
	std::cout << "    7.1.2_M (Film / Pro Tools default) - L C R Lss Rss Lsr Rsr LFE Lts Rts [ADM Metadata Supported]" << std::endl;
	std::cout << "    7.1.2_C (Film / Pro Tools default) - L R C LFE Lss Rss Lsr Rsr Lts Rts [ADM Metadata Supported]" << std::endl;
	std::cout << "    7.1.4_M (Film / Pro Tools default) - L C R Lss Rss Lsr Rsr LFE FLts FRts BLts BRts [ADM Metadata Supported]" << std::endl;
	std::cout << "    7.1.4_C (Film / Pro Tools default) - L C R Lss Rss Lsr Rsr LFE FLts FRts BLts BRts [ADM Metadata Supported]" << std::endl;
	std::cout << std::endl;
}

struct audiofileInfo {
	int format;
	int sampleRate;
	int numberOfChannels;
	float duration;
};

audiofileInfo printFileInfo(SndfileHandle file) {
	audiofileInfo inputFileInfo;
	std::cout << "Sample Rate:        " << file.samplerate() << std::endl;
	inputFileInfo.sampleRate = file.samplerate();
	int format = file.format() & 0xffff;
	if (format == SF_FORMAT_PCM_16) std::cout << "Bit Depth:          16" << std::endl;
	if (format == SF_FORMAT_PCM_24) std::cout << "Bit Depth:          24" << std::endl;
	if (format == SF_FORMAT_FLOAT)  std::cout << "Bit Depth:          32" << std::endl;
	inputFileInfo.format = format;
	std::cout << "Channels:           " << file.channels() << std::endl;
	inputFileInfo.numberOfChannels = file.channels();
	std::cout << "Length (sec):     " << (float)file.frames() / (float)file.samplerate() << std::endl;
	inputFileInfo.duration = (float)file.frames() / (float)file.samplerate();
	std::cout << std::endl;

	return inputFileInfo;
}

std::string prepareAdmMetadata(const char* admString, float duration, int sampleRate, int format) {
    // Used to find duration in time of input file
    // to correctly edit the ADM metadata and add the appropriate
    // `end` and `duration` times.
    std::string s(admString);
    std::string searchDurationString("hh:mm:ss.fffff");
    size_t pos = s.find(searchDurationString);
    
    int seconds, minutes, hours;
    std::string hoursString, minutesString, secondsString;
    seconds = duration;
    minutes = seconds / 60;
    hours = minutes / 60;
    
    if ((int)hours < 100) {
        hoursString = (int(hours) < 10) ? "0" + std::to_string(int(hours)) : std::to_string(int(hours));
    } else {
        // file duration too long?
        // TODO: handle case for when input is over 99 hours
    }
    minutesString = (int(minutes%60) < 10) ? "0" + std::to_string(int(minutes%60)) : std::to_string(int(minutes%60));
    secondsString = (int((seconds+1)%60) < 10) ? "0" + std::to_string(int((seconds+1)%60)) : std::to_string(int((seconds+1)%60));
    
    std::vector<size_t> positions;
    // Repeat till end is reached
    while(pos != std::string::npos){
        // Add position to the vector
        positions.push_back(pos);
        // Get the next occurrence from the current position
        pos = s.find(searchDurationString, pos + searchDurationString.size());
    }

    for (size_t pos : positions){
        if (pos != s.npos){
            s.replace(pos, searchDurationString.length(), hoursString+":"+minutesString+":"+secondsString+".00000");
        }
    }

    std::cout << "Detected Duration:  " << duration << std::endl;
    std::cout << "Duration Timecode:  " << hoursString << ":" << minutesString << ":" << secondsString << ".00000" << std::endl;
    
    // set metadata for samplerate
    std::string searchSampleRateString("__SAMPLERATE__");
    size_t srPos = s.find(searchSampleRateString);
    // Repeat till end is reached
    while(srPos != std::string::npos){
        if (srPos != s.npos) {
            s.replace(srPos, searchDurationString.length(), std::to_string(sampleRate));
        }
        // Get the next occurrence from the current position
        srPos = s.find(searchSampleRateString, srPos + searchSampleRateString.size());
    }
    std::cout << "Detected SampleRate:  " << std::to_string(sampleRate) << std::endl;
    
    // set metadata for bitdepth
    std::string searchBitDepthString("__BITDEPTH__");
    size_t bdPos = s.find(searchBitDepthString);
    // Repeat till end is reached
    while(bdPos != std::string::npos){
        if (bdPos != s.npos) {
            s.replace(bdPos, searchBitDepthString.length(), std::to_string(format));
        }
        // Get the next occurrence from the current position
        bdPos = s.find(searchBitDepthString, bdPos + searchBitDepthString.size());
    }
    std::cout << "Detected BitDepth:  " << std::to_string(format) << std::endl;
    
    return s;
}

// ---------------------------------------------------------
#define BUFFERLEN 512

class SndFileWriter {
	std::unique_ptr<bw64::Bw64Writer> outBw64;
	SndfileHandle outSnd;
	int channels;

	enum SNDFILETYPE {
		SNDFILETYPE_BW64,
		SNDFILETYPE_SND
	} type;

public:
	void open(std::string outfilestr, int sampleRate, int channels, int format) {
		outSnd = SndfileHandle(outfilestr, SFM_WRITE, format, channels, (int)sampleRate);
		this->channels = channels;
		type = SNDFILETYPE_SND;
	}

  void open(std::string outfilestr, int sampleRate, int channels, int format, bw64::ChnaChunk chnaChunkAdm, bw64::AxmlChunk axmlChunkAdm) {
      // TODO: make variable of samplerate and bitdepth based on input
      outBw64 = bw64::writeFile(outfilestr, channels, sampleRate, format, std::make_shared<bw64::ChnaChunk>(chnaChunkAdm), std::make_shared<bw64::AxmlChunk>(axmlChunkAdm));
      this->channels = channels;
      type = SNDFILETYPE_BW64;
  }

	bool isOpened() {
		if (type == SNDFILETYPE_SND) {
			return outSnd.error() == 0;
		} else {
			return true;
		}
	}

	void setClip() {
		if (type == SNDFILETYPE_SND) {
			outSnd.command(SFC_SET_CLIPPING, NULL, SF_TRUE);
		}
	}

	void printInfo() {
		if (type == SNDFILETYPE_SND) {
			printFileInfo(outSnd);
		}
	}

	void setString(int str_type, const char* str) {
		if (type == SNDFILETYPE_SND) {
			outSnd.setString(str_type, str);
		}
	}

	void write(float* buf, int frames) {
		if (type == SNDFILETYPE_SND) {
			outSnd.write(buf, frames*channels);
		} else {
			outBw64->write(buf, frames);
			outBw64->framesWritten();
		}
	}
};

int main(int argc, char* argv[])
{
	Mach1AudioTimeline m1audioTimeline;
	Mach1Transcode<float> m1transcode;
	m1transcode.setCustomPointsSamplerCallback(callbackPointsSampler);

	// locals for cmd line parameters
	bool fileOut = false;
	bool useAudioTimeline = false; // adm, atmos formats
	float masterGain = 1.0f; // in level, not dB
	char* infolder = NULL;
	char* infilename = NULL;
	char* inFmtStr = NULL;
	int inFmt;
	char* outfilename = NULL;
	std::string md_outfilename = "";
	char* outFmtStr = NULL;
	int outFmt;
	int outFileChans;
	int channels;
	bool writeMetadata = false;

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
		|| argc == 1)
	{
		printHelp();
		return 0;
	}

	// flag for writing ADM metadata to audiofile if supported
	pStr = getCmdOption(argv, argv + argc, "-write-metadata");
	if (pStr != NULL)
	{
		writeMetadata = true;
	}

	// input file name
	pStr = getCmdOption(argv, argv + argc, "-in-file");
	if (pStr && (strlen(pStr) > 0)) {
		infilename = pStr;
	} else {
		cerr << "Please specify an input file" << std::endl;
		return -1;
	}

	// input format
	pStr = getCmdOption(argv, argv + argc, "-in-fmt");
	if (pStr && (strlen(pStr) > 0)) {
		inFmtStr = pStr;

		if (strcmp(inFmtStr, "ADM") == 0) {
			m1transcode.setInputFormat(m1transcode.getFormatFromString("CustomPoints"));
			m1audioTimeline.parseADM(infilename);
			useAudioTimeline = true;
		} else if (strcmp(inFmtStr, "Atmos") == 0) {
			char* pStr = getCmdOption(argv, argv + argc, "-in-file-meta");
			if (pStr && (strlen(pStr) > 0)) {
				m1transcode.setInputFormat(m1transcode.getFormatFromString("CustomPoints"));
				m1audioTimeline.parseAtmos(infilename, pStr);
				useAudioTimeline = true;
			} else {
				cerr << "Please specify an input meta file" << std::endl;
				return -1;
			}
		} else {
			bool foundInFmt = false;
			inFmt = m1transcode.getFormatFromString(inFmtStr);
			if (inFmt > 1) { // if format int is 0 or -1 (making it invalid)
				foundInFmt = true;
			}
		}
	} else {
		std::cout << "Please select a valid input format" << std::endl;
		return -1;
	}

	// input folder
	if (useAudioTimeline) {
		pStr = getCmdOption(argv, argv + argc, "-in-folder");
		if (pStr && (strlen(pStr) > 0)) {
			infolder = pStr;
		} else {
			cerr << "Please specify an input folder for audio files" << std::endl;
			return -1;
		}
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

	// check output formats
	bool foundOutFmt = false;
	outFmt = m1transcode.getFormatFromString(outFmtStr);
	if (outFmt > 1) { // if format int is 0 or -1 (making it invalid)
		foundOutFmt = true;
	} else {
		std::cout << "Please select a valid output format" << std::endl;
		return -1;
	}

	pStr = getCmdOption(argv, argv + argc, "-out-file-chans");
	if (pStr != NULL)
		outFileChans = atoi(pStr);
	else
		outFileChans = 0;
	if (!((outFileChans == 0) || (outFileChans == 1) || (outFileChans == 2))) {
		std::cout << "Please select 0, 1, or 2, zero meaning a single, multichannel output file" << std::endl;
		return -1;
	}
	std::cout << std::endl;

	//=================================================================
	// initialize inputs, outputs and components
	//

	// -- input file ---------------------------------------
	// determine number of input files
	SndfileHandle *infile[Mach1TranscodeMAXCHANS];
	vector<string> fNames;
	audiofileInfo inputInfo;

	if (useAudioTimeline) {
		for (int i = 0; i < audioObjects.size(); i++) {
			std::string filename = std::string(infolder) + "/" + audioObjects[i].getName() + ".wav";
			fNames.push_back(filename);
		}
	} else {
        // already pushed back the first index of the string from the split command above
	}

	size_t numInFiles = fNames.size();
	for (int i = 0; i < numInFiles; i++) {
		infile[i] = new SndfileHandle(fNames[i].c_str());
		if (infile[i] && (infile[i]->error() == 0)) {
			// print input file stats
			std::cout << "Input File:         " << fNames[i] << std::endl;
			inputInfo = printFileInfo(*infile[i]);
			sampleRate = (long)infile[i]->samplerate();
			//            int inChannels = 0;
			//            for (int i = 0; i < numInFiles; i++)
			//                inChannels += infile[i]->channels();
			//            parseFile(*infile[i], inChannels);
		} else {
			cerr << "Error: opening in-file: " << fNames[i] << std::endl;
			return -1;
		}
	}

	std::cout << "Master Gain:        " << m1transcode.level2db(masterGain) << "dB" << std::endl;
	std::cout << std::endl;

	for (int i = 0; i < numInFiles; i++) {
		infile[i]->seek(0, 0); // rewind input
	}

	// -- setup
	m1transcode.setInputFormat(inFmt);
	m1transcode.setOutputFormat(outFmt);

	// first init of TT points
	if (useAudioTimeline) {
		audioObjects = m1audioTimeline.getAudioObjects();
		std::vector<Mach1Point3D> points;
		for (int i = 0; i < audioObjects.size(); i++) {
			Mach1KeyPoint keypoint = audioObjects[i].getKeyPoints()[0];
			points.push_back(keypoint.point);
		}
		m1transcode.setInputFormatCustomPoints(points);
	}

	// -- output file(s) --------------------------------------

	channels = m1transcode.getOutputNumChannels();
	SndFileWriter outfiles[Mach1TranscodeMAXCHANS];
	int actualOutFileChannels = outFileChans == 0 ? channels : outFileChans;

	if (actualOutFileChannels == 0) {
		std::cout << "Output channels count is 0!" << std::endl;
		return -1;
	}

	int numOutFiles = channels / actualOutFileChannels;

	for (int i = 0; i < Mach1TranscodeMAXCHANS; i++) {
		memset(inBuffers[i], 0, sizeof(inBuffers[i]));
		memset(outBuffers[i], 0, sizeof(outBuffers[i]));
	}

	//=================================================================
	//  print intermediate formats path
	//
	if (!m1transcode.processConversionPath()) {
		printf("Can't found conversion between formats!");
		return -1;
	} else {
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

	vector<vector<float>> matrix = m1transcode.getMatrixConversion();

	//=================================================================
	//  main sound loop
	//
	int inChannels = 0;
	for (int i = 0; i < numInFiles; i++)
		inChannels += infile[i]->channels();
	sf_count_t numBlocks = infile[0]->frames() / BUFFERLEN; // files must be the same length
	totalSamples = 0;
	float peak = 0.0f;

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
            printf(outfilestr, "%s_%0d.wav", outfilename, i);
		} else {
			strcpy(outfilestr, outfilename);
		}

        /*
          Section for writing ADM based metadata to output
          */
         if (writeMetadata) {
             // Setup empty metadata chunks
             bw64::ChnaChunk chnaChunkAdm;
             std::string axmlChunkAdmCorrectedString;
             int bitDepth;
             if (inputInfo.format == SF_FORMAT_PCM_16){
                 bitDepth = 16;
             } else if (inputInfo.format == SF_FORMAT_PCM_24) {
                 bitDepth = 24;
             } else if (inputInfo.format == SF_FORMAT_FLOAT) {
                 bitDepth = 32;
             }
             
             // TODO: remove the hardcoded `adm_metadata.h` file and write inline instructions for creating the metadata to scale for all formats
             if (outFmt == m1transcode.getFormatFromString("M1Spatial")){
                 // setup `chna` metadata chunk
                 /// Creates a description of an 8 objects
                 std::vector<ChannelDescType> channelDescType = { {3}, {3}, {3}, {3}, {3}, {3}, {3}, {3} };
                 chnaChunkAdm = fillChnaChunkADMDesc(channelDescType);
                 if (chnaChunkAdm.audioIds().size() != actualOutFileChannels){
                     std::cout << "ERROR: Issue writing `chna` metadata chunk due to mismatching channel count" << std::endl;
                     break;
                 }
                 // setup `axml` metadata chunk
                 axmlChunkAdmCorrectedString = prepareAdmMetadata(axml_m1spatial_ChunkAdmString, inputInfo.duration, inputInfo.sampleRate, bitDepth).c_str();
                 bw64::AxmlChunk axmlChunkAdmCorrected(axmlChunkAdmCorrectedString);
                 outfiles[i].open(outfilestr, inputInfo.sampleRate, actualOutFileChannels, bitDepth, chnaChunkAdm, axmlChunkAdmCorrected);
             }
             else if (outFmt == m1transcode.getFormatFromString("7.1.2_M") || outFmt == m1transcode.getFormatFromString("7.1.2_C") || outFmt == m1transcode.getFormatFromString("7.1.2_S")){
                 // setup `chna` metadata chunk
                 /// Creates a description of an 7.1.2 channel bed
                 std::vector<ChannelDescType> channelDescType = { {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1} };
                 chnaChunkAdm = fillChnaChunkADMDesc(channelDescType);
                 if (chnaChunkAdm.audioIds().size() != actualOutFileChannels){
                     std::cout << "ERROR: Issue writing `chna` metadata chunk due to mismatching channel count" << std::endl;
                     break;
                 }
                 // setup `axml` metadata chunk
                 axmlChunkAdmCorrectedString = prepareAdmMetadata(axml_7_1_2_ChunkAdmString, inputInfo.duration, inputInfo.sampleRate, bitDepth).c_str();
                 bw64::AxmlChunk axmlChunkAdmCorrected(axmlChunkAdmCorrectedString);
                 outfiles[i].open(outfilestr, inputInfo.sampleRate, actualOutFileChannels, bitDepth, chnaChunkAdm, axmlChunkAdmCorrected);
             }
             else if (outFmt == m1transcode.getFormatFromString("5.1.4_M") || outFmt == m1transcode.getFormatFromString("5.1.4_C") || outFmt == m1transcode.getFormatFromString("5.1.4_S")){
                 // setup `chna` metadata chunk
                 /// Creates a description of an 5.1 channel bed + 4 object bed
                 std::vector<ChannelDescType> channelDescType = { {1}, {1}, {1}, {1}, {1}, {1}, {3}, {3}, {3}, {3} };
                 chnaChunkAdm = fillChnaChunkADMDesc(channelDescType);
                 if (chnaChunkAdm.audioIds().size() != actualOutFileChannels){
                     std::cout << "ERROR: Issue writing `chna` metadata chunk due to mismatching channel count" << std::endl;
                     break;
                 }
                 // setup `axml` metadata chunk
                 axmlChunkAdmCorrectedString = prepareAdmMetadata(axml_5_1_4_ChunkAdmString, inputInfo.duration, inputInfo.sampleRate, bitDepth).c_str();
                 bw64::AxmlChunk axmlChunkAdmCorrected(axmlChunkAdmCorrectedString);
                 outfiles[i].open(outfilestr, inputInfo.sampleRate, actualOutFileChannels, bitDepth, chnaChunkAdm, axmlChunkAdmCorrected);
             }
             else if (outFmt == m1transcode.getFormatFromString("7.1.4_M") || outFmt == m1transcode.getFormatFromString("7.1.4_C") || outFmt == m1transcode.getFormatFromString("7.1.4_S")){
                 // setup `chna` metadata chunk
                 /// Creates a description of an 7.1 channel bed + 4 object bed
                 std::vector<ChannelDescType> channelDescType = { {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {3}, {3}, {3}, {3} };
                 chnaChunkAdm = fillChnaChunkADMDesc(channelDescType);
                 if (chnaChunkAdm.audioIds().size() != actualOutFileChannels){
                     std::cout << "ERROR: Issue writing `chna` metadata chunk due to mismatching channel count" << std::endl;
                     break;
                 }
                 // setup `axml` metadata chunk
                 axmlChunkAdmCorrectedString = prepareAdmMetadata(axml_7_1_4_ChunkAdmString, inputInfo.duration, inputInfo.sampleRate, bitDepth).c_str();
                 bw64::AxmlChunk axmlChunkAdmCorrected(axmlChunkAdmCorrectedString);
                 outfiles[i].open(outfilestr, inputInfo.sampleRate, actualOutFileChannels, bitDepth, chnaChunkAdm, axmlChunkAdmCorrected);
             }
         }
         else {
             outfiles[i].open(outfilestr, (int)sampleRate, actualOutFileChannels, format);
         }

		if (outfiles[i].isOpened()) {
			// set clipping mode
			outfiles[i].setClip();
			// output file stats
			std::cout << "Output File:        " << outfilestr << std::endl;
			outfiles[i].printInfo();
		} else {
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
	std::cout << std::endl;

	// get start samples for all objects (ADM format)
	std::vector<long long> startSampleForAudioObject;
	for (int i = 0; i < audioObjects.size(); i++) {
		startSampleForAudioObject.push_back(audioObjects[i].getKeyPoints()[0].sample);
	}

	for (int i = 0; i <= numBlocks; i++) {
		// read next buffer from each infile
		sf_count_t firstBuf = 0;
		float(*inBuf)[Mach1TranscodeMAXCHANS][BUFFERLEN] = (float(*)[Mach1TranscodeMAXCHANS][BUFFERLEN])&(inBuffers[0][0]);

		for (int file = 0; file < numInFiles; file++) {
			sf_count_t thisChannels = infile[file]->channels();

			// first fill buffer with zeros
			for (int j = 0; j < BUFFERLEN; j++)
				for (int k = 0; k < thisChannels; k++) {
					(*inBuf)[firstBuf + k][j] = 0;
				}

			int startSample = 0;
			if (useAudioTimeline) {
				startSample = startSampleForAudioObject[file];
			}
			if (totalSamples + BUFFERLEN >= startSample) {
				sf_count_t framesToRead = thisChannels * BUFFERLEN;

				// cut samples if the beginning of the offset does not match with the beginning of the buffer
				sf_count_t offset = 0;
				if (startSample + BUFFERLEN < totalSamples && totalSamples < startSample) {
					offset = startSample - totalSamples;
					framesToRead = BUFFERLEN + totalSamples - startSample;
				}

				sf_count_t framesReaded = infile[file]->read(fileBuffer, framesToRead);
				sf_count_t samplesRead = framesReaded / thisChannels;
				// demultiplex into process buffers
				float *ptrFileBuffer = fileBuffer;
				for (int j = 0; j < samplesRead; j++) {
					for (int k = 0; k < thisChannels; k++) {
							(*inBuf)[firstBuf + k][offset + j] = *ptrFileBuffer++;
					}
				}
			}
			firstBuf += thisChannels;
		}
		totalSamples += BUFFERLEN;

		/*
		 `processConversion()` is called after `processConversionPath() has been called and set at least once!
		 */
		m1transcode.processConversion(inPtrs, outPtrs, (int)BUFFERLEN);
		m1transcode.processMasterGain(outPtrs, (int)BUFFERLEN, masterGain);

		// multiplex to output channels with master gain
		float *ptrFileBuffer = fileBuffer;
		float(*outBuf)[Mach1TranscodeMAXCHANS][BUFFERLEN] = (float(*)[Mach1TranscodeMAXCHANS][BUFFERLEN])&(outBuffers[0][0]);
		for (int file = 0; file < numOutFiles; file++) {
			for (int j = 0; j < BUFFERLEN; j++) {
				for (int k = 0; k < actualOutFileChannels; k++) {
					*ptrFileBuffer++ = (*outBuf)[(file*actualOutFileChannels) + k][j];
				}
			}
		}

		// write to outfile
		for (int j = 0; j < numOutFiles; j++) {
			outfiles[j].write(fileBuffer + (j*actualOutFileChannels*BUFFERLEN), BUFFERLEN);
		}
	}
	// print time played
	std::cout << "Length (sec):     " << (float)totalSamples / (float)sampleRate << std::endl;
	return 0;
}
