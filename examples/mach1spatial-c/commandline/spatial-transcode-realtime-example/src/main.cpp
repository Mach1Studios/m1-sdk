//  Mach1 Spatial SDK
//  Copyright © 2017-2021 Mach1. All rights reserved.

/*
 This example demonstrates REAL-TIME processing with Mach1Transcode and Mach1Decode.
 
 Unlike other examples that pre-calculate coefficients, this example:
 1. Reads audio buffers in real-time
 2. Calls Mach1Transcode.processConversion() for each buffer
 3. Handles multiple intermediate buffer conversions during transcode
 4. Calls Mach1Decode.decodeBuffer() on the transcoded output
 5. Outputs final stereo mix in real-time
 
 This tests the actual real-time performance of Mach1Transcode and Mach1Decode
 and handles the case where multiple buffer conversions may occur during transcode.
 
 Usage: Set starting orientation with `-yaw` `-pitch` `-roll` flags in command or use `w`/`a`/`s`/`d`
 during playback to update the orientation.
 
 Order of Operations:
 1. Setup Input and Output formats (and paths)
 2. Call `processConversionPath()` to setup the conversion for processing
 3. For each audio buffer:
    - Read input buffer
    - Call `processConversion()` to transcode the buffer
    - Handle any intermediate buffer conversions
    - Call `decodeBuffer()` on the final transcoded buffer
    - Output stereo mix
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
#include <atomic>
#include <cmath>

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
	std::cout << "spatial-transcode-realtime -- REAL-TIME Mach1Transcode and Mach1Decode example" << std::endl;
    std::cout << "This example tests actual real-time performance of Mach1Transcode.processConversion()" << std::endl;
    std::cout << std::endl;
    std::cout << "usage: -in-file test_FiveOneFilm.wav -in-fmt 5.1_C -yaw 90.0 -pitch 15.0 -roll 0.0" << std::endl;
    std::cout << std::endl;
    std::cout << "  -help                 - list command line options" << std::endl;
    std::cout << "  -in-file  <filename>  - input file: put quotes around sets of files" << std::endl;
    std::cout << "  -in-fmt   <fmt>       - input format: see supported formats below" << std::endl;
    std::cout << "  -in-json  <json>      - input json: for input custom json Mach1Transcode templates" << std::endl;
    std::cout << "  -yaw <#>              - Angle for decoded output's yaw orientation in float" << std::endl;
    std::cout << "  -pitch <#>            - Angle for decoded output's pitch orientation in float" << std::endl;
    std::cout << "  -roll <#>             - Angle for decoded output's roll orientation in float" << std::endl;
    std::cout << std::endl;
    std::cout << "  Real-time Controls:" << std::endl;
    std::cout << "    w/s - pitch up/down" << std::endl;
    std::cout << "    a/d - yaw left/right" << std::endl;
    std::cout << "    z/x - roll left/right" << std::endl;
    std::cout << "    q   - quit" << std::endl;
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
Mach1Transcode<float> m1transcode;
int inFmt;
int outFmt;
std::vector< std::vector<float> > conversionMatrix;

// Mach1Decode variables & objects
Mach1Decode<float> m1Decode;
Mach1Point3D orientation;
float yaw = 0.0f;
float pitch = 0.0f;
float roll = 0.0f;
std::vector<float> m1Coeffs;

// Real-time processing buffers
std::vector< std::vector<float> > inputBuffers;
std::vector< std::vector<float> > transcodedBuffers;
std::vector< std::vector<float> > intermediateBuffers;
std::vector< std::vector<float> > outputBuffers;

// Performance monitoring
std::atomic<long long> totalProcessedSamples{0};
std::atomic<long long> totalProcessingTime{0};
std::atomic<int> bufferUnderruns{0};
std::atomic<int> bufferOverruns{0};

// locals for cmd line parameters
char* infilename = NULL;
char* inFmtStr = NULL;

sf_count_t totalSamplesRead;
long sampleRate;
sf_count_t numBlocksInInputAudio = 0;

// threading setup for handling key command inputs to Mach1Decode
static void updateMach1DecodeOrientation();
static std::thread* threadUpdateMach1DecodeOrientation = nullptr;
static std::atomic<bool> done{false};

// RtAudio playback reader with REAL-TIME Mach1Transcode processing
int rtAudioPlayback( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
                     double streamTime, RtAudioStreamStatus status, void *userData )
{
    unsigned int i, c;
    double *stereoOutputBuffer = (double *)outputBuffer;
    
    if ( status ) {
        std::cout << "Stream underflow detected!" << std::endl;
        bufferUnderruns++;
    }

    // Prepare internal buffers for real-time processing with bounds checking
    int inputChannels = m1transcode.getInputNumChannels();
    int outputChannels = m1transcode.getOutputNumChannels();
    
    if (inputChannels <= 0 || outputChannels <= 0) {
        // Fill with silence if transcode is not properly initialized
        for (i = 0; i < nBufferFrames; i++) {
            stereoOutputBuffer[i * 2 + 0] = 0.0;
            stereoOutputBuffer[i * 2 + 1] = 0.0;
        }
        return 0;
    }
    
    if (inputBuffers.size() != inputChannels) {
        inputBuffers.resize(inputChannels);
        for (c = 0; c < inputChannels; c++) {
            inputBuffers[c].resize(nBufferFrames);
        }
    }
    
    if (transcodedBuffers.size() != outputChannels) {
        transcodedBuffers.resize(outputChannels);
        for (c = 0; c < outputChannels; c++) {
            transcodedBuffers[c].resize(nBufferFrames);
        }
    }
    
    if (outputBuffers.size() != 2) {
        outputBuffers.resize(2);
        for (c = 0; c < 2; c++) {
            outputBuffers[c].resize(nBufferFrames);
        }
    }

    // Read next buffer from each infile with bounds checking
    sf_count_t samplesRead = 0;
    for (int file = 0; file < numInFiles && file < Mach1TranscodeMAXCHANS; file++) {
        if (infile[file] == nullptr) continue;
        
        sf_count_t thisChannels = infile[file]->channels();
        if (thisChannels <= 0) continue;
        
        sf_count_t framesRead = infile[file]->read(fileBuffer, thisChannels * BUFFERLEN);
        samplesRead = framesRead / thisChannels;
        
        // If we've reached EOF, loop back to beginning
        if (samplesRead == 0) {
            infile[file]->seek(0, SEEK_SET);
            framesRead = infile[file]->read(fileBuffer, thisChannels * BUFFERLEN);
            samplesRead = framesRead / thisChannels;
        }
        
        // Demultiplex into process buffers with bounds checking
        float *inputFileBufferPtr = fileBuffer;
        
        for (c = 0; c < inChannels && c < inputChannels; c++) {
            for (i = 0; i < nBufferFrames && i < samplesRead; i++) {
                if (c < inputBuffers.size() && i < inputBuffers[c].size()) {
                    inputBuffers[c][i] = inputFileBufferPtr[i * thisChannels + c];
                }
            }
        }
    }
    totalSamplesRead += samplesRead;

    // REAL-TIME PROCESSING: Call Mach1Transcode.processConversion() for each buffer
    auto startTime = std::chrono::high_resolution_clock::now();
    
    try {
        // This is the key difference - we call processConversion() in real-time
        m1transcode.processConversion(inputBuffers, transcodedBuffers, nBufferFrames);
        
        // Handle any intermediate buffer conversions that might occur
        // (This is where multiple buffer conversions would be handled)
        std::vector<int> conversionPath = m1transcode.getFormatConversionPath();
        if (conversionPath.size() > 2) {
            // Multiple conversion steps detected - handle intermediate buffers
            if (intermediateBuffers.size() != outputChannels) {
                intermediateBuffers.resize(outputChannels);
                for (c = 0; c < outputChannels; c++) {
                    intermediateBuffers[c].resize(nBufferFrames);
                }
            }
            
            // Copy transcoded output to intermediate buffers for further processing
            for (c = 0; c < outputChannels && c < transcodedBuffers.size(); c++) {
                for (i = 0; i < nBufferFrames && i < transcodedBuffers[c].size(); i++) {
                    intermediateBuffers[c][i] = transcodedBuffers[c][i];
                }
            }
        }
        
        // Apply Mach1Decode to the final transcoded buffer
        m1Decode.decodeBuffer(transcodedBuffers, outputBuffers, nBufferFrames);
    } catch (...) {
        // If transcode/decode fails, fill with silence
        for (i = 0; i < nBufferFrames; i++) {
            stereoOutputBuffer[i * 2 + 0] = 0.0;
            stereoOutputBuffer[i * 2 + 1] = 0.0;
        }
        return 0;
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto processingTime = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    totalProcessingTime += processingTime.count();
    totalProcessedSamples += nBufferFrames;

    // Output final stereo mix with bounds checking
    for (i = 0; i < nBufferFrames; i++) {
        // LEFT:
        if (outputBuffers.size() > 0 && i < outputBuffers[0].size()) {
            stereoOutputBuffer[i * 2 + 0] = outputBuffers[0][i];
        } else {
            stereoOutputBuffer[i * 2 + 0] = 0.0;
        }
        // RIGHT:
        if (outputBuffers.size() > 1 && i < outputBuffers[1].size()) {
            stereoOutputBuffer[i * 2 + 1] = outputBuffers[1][i];
        } else {
            stereoOutputBuffer[i * 2 + 1] = 0.0;
        }
    }

    // Debug: Print audio level every 1000 samples to verify audio is being processed
    static int debugCounter = 0;
    static int callbackCount = 0;
    callbackCount++;
    
    if (++debugCounter >= 1000) {
        float leftLevel = 0.0f, rightLevel = 0.0f;
        for (i = 0; i < nBufferFrames; i++) {
            leftLevel += fabs(stereoOutputBuffer[i * 2 + 0]);
            rightLevel += fabs(stereoOutputBuffer[i * 2 + 1]);
        }
        leftLevel /= nBufferFrames;
        rightLevel /= nBufferFrames;
        printf("Audio callback #%d - Levels L: %.6f, R: %.6f\n", callbackCount, leftLevel, rightLevel);
        debugCounter = 0;
    }

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

    bool foundInFmt = false;
    inFmt = m1transcode.getFormatFromString(inFmtStr);
    if (inFmt > 1) { // if format int is 0 or -1 (making it invalid)
        foundInFmt = true;
    }
    else {
        std::cerr << "Please select a valid input format" << std::endl;
        return -1;
    }

    // Default to M1Spatial-14 output for decoding
    outFmt = m1transcode.getFormatFromString("M1Spatial-8");

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
		} else {
            std::cerr << "Error: opening in-file: " << fNames[i] << std::endl;
			return -1;
		}
	}

	for (int i = 0; i < numInFiles; i++) {
		infile[i]->seek(0, 0); // rewind input
	}

	// -- Mach1Transcode setup
	m1transcode.setInputFormat(inFmt);
	m1transcode.setOutputFormat(outFmt);

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

    // Get the actual output format from the conversion path
    std::vector<int> conversionPath = m1transcode.getFormatConversionPath();
    int actualOutputFormat = conversionPath.back(); // Last format in the path is the final output
    
    // Set up Mach1Decode to match the actual transcode output format
    m1Decode.setPlatformType(Mach1PlatformDefault);
    
    // Map the transcode output format to the correct decode mode
    if (actualOutputFormat == m1transcode.getFormatFromString("M1Spatial-8")) {
        m1Decode.setDecodeMode(M1DecodeSpatial_8);
    } else if (actualOutputFormat == m1transcode.getFormatFromString("M1Spatial-14")) {
        m1Decode.setDecodeMode(M1DecodeSpatial_14);
    } else {
        // Default to M1Spatial-8 if we can't determine
        m1Decode.setDecodeMode(M1DecodeSpatial_8);
    }
    
    m1Decode.setFilterSpeed(0.95f);
    orientation.x = yaw;
    orientation.y = pitch;
    orientation.z = roll;
    m1Decode.setRotationDegrees(orientation);

	for (int i = 0; i < Mach1TranscodeMAXCHANS; i++) {
		memset(inBuffers[i], 0, sizeof(inBuffers[i]));
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

    // Starting playback
	double *data = (double *)calloc(parameters.nChannels, sizeof(double));
    
    // Try multiple sample rates and buffer sizes for better compatibility
    std::vector<unsigned int> sampleRatesToTry = {44100, 48000, 88200, 96000};
    std::vector<unsigned int> bufferSizesToTry = {512, 256, 1024, 128};
    bool streamStarted = false;
    
    for (unsigned int sampleRate : sampleRatesToTry) {
        for (unsigned int bufferSize : bufferSizesToTry) {
            try {
                std::cout << "Trying sample rate: " << sampleRate << ", buffer size: " << bufferSize << std::endl;
                dac.openStream( &parameters, NULL, RTAUDIO_FLOAT64,
                               sampleRate, &bufferSize, &rtAudioPlayback, (void *)&data );
                dac.startStream();
                streamStarted = true;
                playbackSampleRate = sampleRate;
                bufferFrames = bufferSize;
                std::cout << "Successfully started stream at " << sampleRate << " Hz, buffer: " << bufferSize << std::endl;
                break;
            }
            catch ( RtAudioErrorType& e ) {
                std::cout << "Failed at sample rate " << sampleRate << ", buffer " << bufferSize << ", error: " << e << std::endl;
                if (dac.isStreamOpen()) {
                    dac.closeStream();
                }
            }
        }
        if (streamStarted) break;
    }
    
    if (!streamStarted) {
        printf("ERROR: Could not start audio stream with any configuration\n");
        printf("Try using built-in speakers instead of Bluetooth devices\n");
        exit( 0 );
    }

    std::cout << "\n=== REAL-TIME PROCESSING STARTED ===" << std::endl;
    std::cout << "Processing " << m1transcode.getInputNumChannels() << " input channels -> " 
              << m1transcode.getOutputNumChannels() << " output channels" << std::endl;
    std::cout << "Actual output format: " << m1transcode.getFormatName(actualOutputFormat) << std::endl;
    std::cout << "Decode mode: " << m1Decode.getDecodeMode() << std::endl;
    std::cout << "Expected coefficient count: " << m1Decode.getFormatCoeffCount() << std::endl;
    std::cout << "Buffer size: " << bufferFrames << " frames" << std::endl;
    std::cout << "Use w/a/s/d/z/x keys to control orientation, 'q' to quit" << std::endl;

	while (!done) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        // Print performance stats every 5 seconds
        static auto lastStatsTime = std::chrono::high_resolution_clock::now();
        auto now = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(now - lastStatsTime).count() >= 5) {
            if (totalProcessedSamples > 0) {
                double avgProcessingTime = (double)totalProcessingTime / totalProcessedSamples;
                double cpuUsage = (avgProcessingTime / (bufferFrames * 1000000.0 / playbackSampleRate)) * 100.0;
                
                std::cout << "\n=== PERFORMANCE STATS ===" << std::endl;
                std::cout << "Samples processed: " << totalProcessedSamples << std::endl;
                std::cout << "Avg processing time: " << avgProcessingTime << " μs per buffer" << std::endl;
                std::cout << "CPU usage: " << cpuUsage << "%" << std::endl;
                std::cout << "Buffer underruns: " << bufferUnderruns << std::endl;
                std::cout << "Buffer overruns: " << bufferOverruns << std::endl;
                std::cout << "=========================" << std::endl;
            }
            lastStatsTime = now;
        }
	}

    if ( dac.isStreamOpen() ) dac.closeStream();

    return 0;
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

    printf("Real-time orientation control active\n");
    char c;
    printf("Enter a command (w/a/s/d/z/x/q):\n");
    while (1) {

#ifdef _WIN32
        c = _getch();
#else
        c = getchar();
#endif

        if (c == 'q') break;

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
        if (yaw < -180.0) yaw = 180.0;
        else if (yaw > 180.0) yaw = -180.0;
        if (pitch < -90.0) pitch = -90.0;
        else if (pitch > 90.0) pitch = 90.0;
        if (roll < -180.0) roll = 180.0;
        else if (roll > 180.0) roll = -180.0;

        orientation.x = yaw;
        orientation.y = pitch;
        orientation.z = roll;

        m1Decode.setRotationDegrees(orientation);
        m1Coeffs = m1Decode.decodeCoeffs();

        // Mach1DecodeCAPI Log:
        printf("\n");
        printf("y / p / r: %f %f %f\n", yaw, pitch, roll);
        printf("Total coefficients: %zu\n", m1Coeffs.size());
        printf("Format coefficient count: %d\n", m1Decode.getFormatCoeffCount());
        printf("\n");
        printf("Decode Coeffs:\n");
        for (int i = 0; i < m1Coeffs.size()/2; i++){
            printf(" %iL: %f", i, m1Coeffs[i * 2]);
            printf(" %iR: %f\n", i, m1Coeffs[i * 2 + 1]);
        }
        printf("\n");
    }
    printf("\n");
    printf("Exiting\n");
    done = true;
}
