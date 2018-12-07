#include "ofxAudioDecoder.h"

//----------
bool ofxAudioDecoder::load(ofSoundBuffer & buffer, string filename, size_t framesToRead) {
	AudioDecoder audioDecoder(ofToDataPath(filename));
	if (audioDecoder.open() != AUDIODECODER_OK) {
		ofLogError() << "Failed to load " << filename;
		return false;
	}
	
	auto numSamples = audioDecoder.numSamples();
	auto numChannels = audioDecoder.channels();
	auto sampleRate = audioDecoder.sampleRate();
	
	buffer.setSampleRate(sampleRate);
	buffer.allocate(numSamples / numChannels, numChannels);
	
	//if user asked for 0 samples
	if(framesToRead == 0) {
		//we interpet that as wanting to read whole file
		framesToRead = numSamples / numChannels;
	}
	unsigned long int samplesToRead = framesToRead * numChannels;
	unsigned long int readBufferSize = 512 * numChannels;
	
	unsigned long int curSample = 0;
	auto & rawSamples = buffer.getBuffer();
	while(curSample < samplesToRead) {
		unsigned long int  remainingSamples = MIN(readBufferSize, samplesToRead - curSample);
		unsigned long int  samplesRead = audioDecoder.read(remainingSamples, &rawSamples[curSample]);
		curSample += samplesRead;
		if(samplesRead < readBufferSize) {
			break;
		}
	}
	
	ofLogVerbose() << "Read " << curSample << " of " << numSamples;
	return true;
}

//----------
bool ofxAudioDecoder::load(string filename, size_t framesToRead) {
	return ofxAudioDecoder::load(this->buffer, filename, framesToRead);
}

//----------
int ofxAudioDecoder::getNumChannels() const {
	return this->buffer.getNumChannels();
}

//----------
int ofxAudioDecoder::getSampleRate() const {
	return this->buffer.getSampleRate();
}

//----------
long ofxAudioDecoder::getNumSamples() const {
	return this->buffer.size();
}

//----------
int ofxAudioDecoder::getNumFrames() const {
	return this->buffer.getNumFrames();
}

//----------
const ofSoundBuffer & ofxAudioDecoder::getBuffer() const {
	return this->buffer;
}

//----------
const vector<float> & ofxAudioDecoder::getRawSamples() const {
	return this->buffer.getBuffer();
}