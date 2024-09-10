//  Mach1 Spatial SDK
//  Copyright © 2017-2021 Mach1. All rights reserved.

/*
 This example is for reference for how Mach1Spatial Coefficients from Mach1Transcode API
 could be used on audio streams and buffers.

 Usage: Set starting orientation with `-yaw` `-pitch` `-roll` flags in command or use `w`/`a`/`s`/`d`
 during playback to update the orientation.

 Order of Operations:
 1. Setup Input and Output formats (and paths)
 2. Call `processConversionPath()` to setup the conversion for processing
 3. Call `processConversion()` to execute the conversion and return coeffs per buffer/sample per channel
 4. Apply to buffer/samples per channel along with Mach1Decode for realtime playback
 */


/*
for windows, add these definitions to the project settings:
__WINDOWS_ASIO__;__WINDOWS_WASAPI__;_CRT_SECURE_NO_WARNINGS
*/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#if defined(_WIN32)
#include <time.h>
#include <windows.h>
#include <conio.h>
#define _TIMESPEC_DEFINED
#else
#include <sys/time.h>
#include <unistd.h>
#include <termios.h>
#endif

#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <time.h>
#include <thread>
#include <chrono>

#include "Mach1Transcode.h"
#include "Mach1Decode.h"
#include "M1DSP/M1DSPUtilities.h"
#include "sndfile.hh"
#include "CmdOption.h"
#include "rtaudio/RtAudio.h"

#define DELTA_ANGLE 0.0174533 // equivalent of 1 degrees in radians
#define DELTA_VALUE 1.0 // used for incrementing in degrees directly
#ifndef PI
#define PI 3.14159265358979323846
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

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
    std::cout << "note: for a complete transcoding tool use `m1-transcode`." << std::endl;
    std::cout << std::endl;
    std::cout << "usage: -in-file test_FiveOneFilm.wav -in-fmt 5.1_C -out-file test_b.wav -out-file-chans 0 -yaw 90.0 -pitch 15.0 -roll 0.0" << std::endl;
    std::cout << std::endl;
    std::cout << "  -help                 - list command line options" << std::endl;
    std::cout << "  -in-file  <filename>  - input file: put quotes around sets of files" << std::endl;
    std::cout << "  -in-fmt   <fmt>       - input format: see supported formats below" << std::endl;
    std::cout << "  -in-json  <json>      - input json: for input custom json Mach1Transcode templates" << std::endl;
    std::cout << "  -normalize            - two pass normalize absolute peak to zero dBFS" << std::endl;
    std::cout << "  -master-gain <#>      - final output gain in dB like -3 or 2.3" << std::endl;
    std::cout << "  -spatial-downmix <#>  - compare top vs. bottom of the input soundfield, if difference is less than the set threshold (float) output format will be Mach1Spatial-4" << std::endl;
    std::cout << "  -yaw <#>              - Angle for decoded output's yaw orientation in float" << std::endl;
    std::cout << "  -pitch <#>            - Angle for decoded output's pitch orientation in float" << std::endl;
    std::cout << "  -roll <#>             - Angle for decoded output's roll orientation in float" << std::endl;
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

size_t numInFiles;
SndfileHandle *infile[Mach1TranscodeMAXCHANS];
int inChannels = 0;

// multiplexed buffers
float fileBuffer[Mach1TranscodeMAXCHANS * BUFFERLEN];

// process buffers
float inBuffers[Mach1TranscodeMAXCHANS][BUFFERLEN];
float *inPtrs[Mach1TranscodeMAXCHANS];
float outBuffers[Mach1TranscodeMAXCHANS][BUFFERLEN];
float *outPtrs[Mach1TranscodeMAXCHANS];

// Mach1Transcode variables & objects
Mach1Transcode m1transcode;
int inFmt;
int outFmt;
float corrThreshold = 0.1; // 10% difference in signal or less will auto downmix
std::vector< std::vector<float> > conversionMatrix;
std::vector<float> transcodeToDecodeCoeffs;

// Mach1Decode variables & objects
Mach1Decode m1Decode;
Mach1Point3D orientation;
float yaw = 0.0f;
float pitch = 0.0f;
float roll = 0.0f;
std::vector<float> m1Coeffs;

// locals for cmd line parameters
bool fileOut = false;
float masterGain = 1.0f; // in level, not dB
bool normalize = false;
char* infilename = NULL;
char* inFmtStr = NULL;
char* outfilename = NULL;
char* outFmtStr = NULL;
std::string md_outfilename = "";
int outFileChans;
int outFormatChannels;

sf_count_t totalSamplesRead;
long sampleRate;
sf_count_t numBlocksInInputAudio = 0;

// threading setup for handling key command inputs to Mach1Decode
static void updateMach1DecodeOrientation();
static void updateMach1Transcode();
static std::thread* threadUpdateMach1DecodeOrientation = nullptr;
static bool done = false;

// RtAudio playback reader.
int rtAudioPlayback( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
                     double streamTime, RtAudioStreamStatus status, void *userData )
{
    unsigned int i, c;
    double *stereoOutputBuffer = (double *) outputBuffer;
    double *lastValues = (double *) userData;
    if ( status )
    std::cout << "Stream underflow detected!" << std::endl;

    // read next buffer from each infile
    sf_count_t samplesRead;
    sf_count_t firstBuf = 0;
    for (int file = 0; file < numInFiles; file++){
        sf_count_t thisChannels = infile[file]->channels();
        sf_count_t framesRead = infile[file]->read(fileBuffer, thisChannels * BUFFERLEN);
        samplesRead = framesRead / thisChannels;
        // demultiplex into process buffers
        float *inputFileBufferPtr = fileBuffer;

		// play audio
		for (i = 0; i < nBufferFrames; i++) {
			stereoOutputBuffer[i * 2 + 0] = 0;
			stereoOutputBuffer[i * 2 + 1] = 0;
		}
		for (c = 0; c < inChannels; c++) {
			for (i = 0; i < nBufferFrames; i++) {
                // LEFT:
                stereoOutputBuffer[i * 2 + 0] += inputFileBufferPtr[i * thisChannels + c] * transcodeToDecodeCoeffs[2 * c + 0];
                // RIGHT:
                stereoOutputBuffer[i * 2 + 1] += inputFileBufferPtr[i * thisChannels + c] * transcodeToDecodeCoeffs[2 * c + 1];
			}
		}

        float(*inBuf)[Mach1TranscodeMAXCHANS][BUFFERLEN] = (float(*)[Mach1TranscodeMAXCHANS][BUFFERLEN])&(inBuffers[0][0]);
        for (int j = 0; j < samplesRead; j++)
            for (int k = 0; k < thisChannels; k++)
                (*inBuf)[firstBuf + k][j] = *inputFileBufferPtr++;
        firstBuf += thisChannels;
    }
    totalSamplesRead += samplesRead;

    // print current sample index and range
    //printf("%d / %d \n", totalSamplesRead, numBlocksInInputAudio * BUFFERLEN);
  return 0;
}

int main(int argc, char* argv[])
{
    RtAudio dac;
    if ( dac.getDeviceCount() < 1 ) {
    std::cout << "\nNo audio devices found!\n";
    exit( 0 );
    }
    RtAudio::StreamParameters parameters;
	parameters.deviceId = dac.getDefaultOutputDevice();
	parameters.nChannels = 2;
    parameters.firstChannel = 0;
    unsigned int playbackSampleRate = 44100;
    unsigned int bufferFrames = BUFFERLEN;

    // Initializing process buffers
	for (int i = 0; i < Mach1TranscodeMAXCHANS; i++) {
		inPtrs[i] = inBuffers[i];
		outPtrs[i] = outBuffers[i];
	}

    // create thread for reading key command updates for Mach1Decode
	threadUpdateMach1DecodeOrientation = new std::thread(updateMach1DecodeOrientation);
	threadUpdateMach1DecodeOrientation->detach();

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
     yaw orientation angle for decoded stereo output
     Range: Signed float degrees [-180->180]
     */
    pStr = getCmdOption(argv, argv + argc, "-yaw");
    if (pStr != NULL) {
        yaw = atof(pStr);
    }
    if (yaw < -180.0f || yaw > 180.0f) {
        std::cerr << "Please use -180.0 to 180.0 for yaw angle range" << std::endl;
        return -1;
    }
    /*
     pitch orientation angle for decoded stereo output
     Range: Signed float degrees [-90->90]
     */
    pStr = getCmdOption(argv, argv + argc, "-pitch");
    if (pStr != NULL) {
        pitch = atof(pStr);
    }
    if (pitch < -90.0f || pitch > 90.0f) {
        std::cerr << "Please use -90.0 to 90.0 for pitch angle range" << std::endl;
        return -1;
    }
    /*
     roll orientation angle for decoded stereo output
     Range: Signed float degrees [-90->90]
     */
    pStr = getCmdOption(argv, argv + argc, "-roll");
    if (pStr != NULL) {
        roll = atof(pStr);
    }
    if (roll < -180.0f || roll > 180.0f) {
        std::cerr << "Please use -90.0 to 90.0 for roll angle range" << std::endl;
        return -1;
    }

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
		if (strcmp(inFmtStr, "CustomPoints") == 0) {
			pStr = getCmdOption(argv, argv + argc, "-in-json");
			if (pStr && (strlen(pStr) > 0)) {
                std::ifstream file(pStr);
                std::string strJson((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
				m1transcode.setInputFormatCustomPointsJson((char*)strJson.c_str());
			}
		}
	}
    pStr = getCmdOption(argv, argv + argc, "-out-fmt");
    if (pStr && (strlen(pStr) > 0)) {
        outFmtStr = pStr;
        if (strcmp(outFmtStr, "CustomPoints") == 0) {
            pStr = getCmdOption(argv, argv + argc, "-out-json");
            if (pStr && (strlen(pStr) > 0)) {
                std::ifstream file(pStr);
                std::string strJson((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                m1transcode.setOutputFormatCustomPointsJson((char*)strJson.c_str());
            }
        }
    }

	bool foundInFmt = false;
	inFmt = m1transcode.getFormatFromString(inFmtStr);
    if (inFmt > 1) { // if format int is 0 or -1 (making it invalid)
		foundInFmt = true;
	} else {
        std::cerr << "Please select a valid input format" << std::endl;
		return -1;
	}

    // default to 14ch output for decoding
    // TODO: expand to search for other M1 formats and apply to decoder too
    outFmt = m1transcode.getFormatFromString("M1Spatial-14");

	//=================================================================
	// initialize inputs, outputs and components
	//

	// -- input file ---------------------------------------
	// determine number of input files
    std::vector<std::string> fNames;
	split(infilename, ' ', fNames);
	numInFiles = fNames.size();
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

	// -- Mach1Transcode setup
	m1transcode.setInputFormat(inFmt);
	m1transcode.setOutputFormat(outFmt);

    // -- Mach1Decode setup
    m1Decode.setPlatformType(Mach1PlatformDefault);
    m1Decode.setDecodeAlgoType(Mach1DecodeAlgoSpatial_14); // TODO: allow other M1 output from transcoder and apply here too
    m1Decode.setFilterSpeed(0.95f);
    orientation.x = yaw;
    orientation.y = pitch;
    orientation.z = roll;
    m1Decode.setRotationDegrees(orientation);

	for (int i = 0; i < Mach1TranscodeMAXCHANS; i++) {
		memset(inBuffers[i], 0, sizeof(inBuffers[i]));
	}

	//=================================================================
	//  print intermediate formats path
	//
	if (!m1transcode.processConversionPath()) {
		printf("No applicable conversion between selected input & output formats!");
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

    // Return matrix of coeffs for conversion for further customization or tweaking
    conversionMatrix = m1transcode.getMatrixConversion();

	//=================================================================
	//  main sound loop
	//

    for (int i = 0; i < numInFiles; i++) {
		inChannels += infile[i]->channels();
    }
    numBlocksInInputAudio = infile[0]->frames() / BUFFERLEN; // files must be the same length
	totalSamplesRead = 0;

	updateMach1Transcode();

    // Starting playback
	double *data = (double *)calloc(parameters.nChannels, sizeof(double));
    try {
        dac.openStream( &parameters, NULL, RTAUDIO_FLOAT64,
                       playbackSampleRate, &bufferFrames, &rtAudioPlayback, (void *)&data );
        dac.startStream();
    }
    catch ( RtAudioErrorType& e ) {
        printf("ERROR: RTAudio returned: %u", e);
        exit( 0 );
    }

	while (!done) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

    if ( dac.isStreamOpen() ) dac.closeStream();

    return 0;
}

void updateMach1Transcode()
{
	m1Coeffs = m1Decode.decodeCoeffs();
	transcodeToDecodeCoeffs.resize(inChannels * 2);
	for (int c = 0; c < inChannels; c++) {
		float thisInputToLeftChannel = 0;
		for (int i = 0; i < 8; i++) {
			float conversionMatrixCoeff = conversionMatrix[i][c];
			thisInputToLeftChannel += m1Coeffs[2 * i + 0] * conversionMatrixCoeff;
		}

		float thisInputToRightChannel = 0;
		for (int i = 0; i < 8; i++) {
			float conversionMatrixCoeff = conversionMatrix[i][c];
			thisInputToRightChannel += m1Coeffs[2 * i + 1] * conversionMatrixCoeff;
		}
		transcodeToDecodeCoeffs[c * 2 + 0] = thisInputToLeftChannel;
		transcodeToDecodeCoeffs[c * 2 + 1] = thisInputToRightChannel;
	}
}

// Thread for handling key inputs for updating orientation to Mach1Decode
static void updateMach1DecodeOrientation()
{
    /* Allow Terminal to input chars without "Enter" */
#ifndef _WIN32
    struct termios info;
    tcgetattr(0, &info);
    info.c_lflag &= ~ICANON;
    info.c_cc[VMIN] = 1;
    info.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &info);
#endif

    printf("In the run thread\n");
    char c;
    printf("Enter a command:\n");
    while (1) {

#ifdef _WIN32
        c = _getch();
#else
        c = getchar();
#endif

        if (c == 'q') return;

        // delete entered character
        printf("\b");
        switch (c) {
            case 'd':
                yaw += DELTA_VALUE;
                break;
            case 'a':
                yaw -= DELTA_VALUE;
                break;
            case 'w':
                pitch += DELTA_VALUE;
                break;
            case 's':
                pitch -= DELTA_VALUE;
                break;
            case 'x':
                roll += DELTA_VALUE;
                break;
            case 'z':
                roll -= DELTA_VALUE;
                break;
            default:
                printf("Input not recognized.\n");
        }

        // check that the values are in proper range
        if (yaw < 0.0) yaw = 360.0;
        else if (yaw > 360.0) yaw = 0.0;
        if (pitch < -90.0) pitch = -90.0;
        else if (pitch > 90.0) pitch = 90.0;
        if (roll < -90.0) roll = -90.0;
        else if (roll > 90.0) roll = 90.0;

        orientation.x = yaw;
        orientation.y = pitch;
        orientation.z = roll;

        m1Decode.setRotationDegrees(orientation);
        m1Coeffs = m1Decode.decodeCoeffs();

		updateMach1Transcode();

        // Mach1DecodeCAPI Log:
        printf("\n");
        printf("y / p / r: %f %f %f\n", yaw, pitch, roll);
        printf("\n");
        printf("Decode Coeffs:\n");
        for (int i = 0; i < (m1Coeffs.size()-2)/2; i++){
            printf(" %iL: %f", i, m1Coeffs[i * 2]);
            printf(" %iR: %f\n", i, m1Coeffs[i * 2 + 1]);
        }
        printf("Headlock Stereo Coeffs:\n");
        printf("%f %f\n", m1Coeffs[m1Decode.getFormatCoeffCount()-2], m1Coeffs[m1Decode.getFormatCoeffCount()-1]);
        printf("\n");
    }
    printf("\n");
    printf("Exiting\n");
    done = true;
}
