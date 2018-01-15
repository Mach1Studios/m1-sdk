#pragma once

#include "ofMain.h"

#include <audiodecoder.h>

class ofxAudioDecoder {
public:
	static bool load(ofSoundBuffer & buffer, string filePath, size_t framesToRead = 0);
protected:
	ofSoundBuffer buffer;

public:
	bool load(string filename, size_t framesToRead = 0);
	
	int getNumChannels() const;
	int getSampleRate() const;
	int getNumSamples() const;
	int getNumFrames() const;

	const ofSoundBuffer & getBuffer() const;
	const vector<float> & getRawSamples() const;
};

