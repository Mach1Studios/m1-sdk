#include "ofApp.h"

void ofApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate(120);
	loadFile("demo.mp3");

	soundStream.listDevices();
	soundStream.setDeviceID(0);
	soundStream.setup(this, NUM_CHANNELS, 0, SAMPLE_RATE, STREAM_BUFFER_SIZE, 2);
}

void ofApp::update() {

}

void ofApp::draw() {
	ofBackground(0);

	ofPushStyle();
	ofPushMatrix();
	{
		ofEnableBlendMode(OF_BLENDMODE_ADD);
		ofScale(10 * (float)ofGetWidth() / audio.getNumFrames(), ofGetHeight() / 2);
		ofTranslate(0, 1);

		ofSetColor(ofColor::red);
		left.drawWireframe();

		ofSetColor(ofColor::green);
		right.drawWireframe();
	}
	ofPopMatrix();
	ofPopStyle();

	stringstream ss;
	ss << "Samples : " << audio.getNumSamples() << endl;
	ss << "Channels : " << audio.getNumChannels() << endl;
	ss << "Frames : " << audio.getNumFrames();
	ofDrawBitmapString(ss.str(), 10, 20);
}

void ofApp::dragEvent(ofDragInfo dragInfo) {
	loadFile(dragInfo.files[0]);
}

void ofApp::loadFile(string filename) {
	audio.load(filename);

	left.clear();
	right.clear();

	left.setMode(OF_PRIMITIVE_LINE_STRIP);
	right.setMode(OF_PRIMITIVE_LINE_STRIP);

	const vector<float>& rawSamples = audio.getRawSamples();
	int channels = audio.getNumChannels();
	int n = rawSamples.size();
	for (int c = 0; c < channels; c++) {
		for (int i = c; i < n; i += channels) {
			(c == 0 ? left : right).addVertex(ofVec2f(i / channels, rawSamples[i]));
		}
	}
}


//--------------------------------------------------------------
void ofApp::audioOut(float* output, int bufferSize, int nChannels) {
	 
		for (int i = 0; i < bufferSize; i++)
		{
			for (int j = 0; j < NUM_CHANNELS; j++)
			{
				output[i*NUM_CHANNELS + j] = audio.getRawSamples()[playPosition];
			}

			playPosition += audio.getNumChannels();
			if (playPosition >= audio.getNumSamples())
			{
				playPosition = 0;
			}
		}
	 
}


 