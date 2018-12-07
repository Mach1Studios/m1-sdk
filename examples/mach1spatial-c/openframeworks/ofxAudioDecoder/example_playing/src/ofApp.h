#pragma once

#include "ofMain.h"
#include "ofxAudioDecoder.h"

#define NUM_CHANNELS 2
#define SAMPLE_RATE 44100
#define STREAM_BUFFER_SIZE 512

class ofApp : public ofBaseApp {

 
public:
	void setup();
	void update();
	void draw();
	void dragEvent(ofDragInfo dragInfo);

	 
	void loadFile(string filename);
	

	// Audio
	void audioOut(float * output, int bufferSize, int nChannels);
	ofSoundStream soundStream;
	int playPosition = 0;

	ofxAudioDecoder audio;
	ofMesh left, right;
};