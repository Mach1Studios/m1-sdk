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
	buffer.allocate(numSamples / numChannels + 4096, numChannels);
	
	//if user asked for 0 samples
	if(framesToRead == 0) {
		//we interpet that as wanting to read whole file
		framesToRead = numSamples / numChannels;
	}
	int samplesToRead = framesToRead * numChannels;
	int readBufferSize = 512 * numChannels;
	
	int curSample = 0;
	vector<float>& rawSamples = buffer.getBuffer();
    
    long int s = rawSamples.size();
    
	while(curSample < samplesToRead) {
		int remainingSamples = MIN(readBufferSize, samplesToRead - curSample);
		int samplesRead = audioDecoder.read(remainingSamples, &rawSamples[curSample]);
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
int ofxAudioDecoder::getNumSamples() const {
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
