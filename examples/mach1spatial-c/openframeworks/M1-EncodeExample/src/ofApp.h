#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "Mach1Encode.h"
#include "Mach1Decode.h"
#include "ofxAudioDecoder.h"

#define RENDERING_SCALE 200

class ofApp : public ofBaseApp{
	ofSoundStream soundStream;
	ofxAudioDecoder player;
	unsigned long int pos;

	std::vector<float> decoded;
	std::mutex mtx;
	std::vector<float> volumes;
	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

	void audioOut(float * output, int bufferSize, int nChannels);

	ofxImGui::Gui gui;

	// Encoder

	Mach1Encode m1Encode;

	int inputKind = 0; // 0 = MONO, 1 = STEREO, 2 = QUAD, 3 = AFORMAT, 4 = BFORMAT
	int outputKind = 1; // 0 = 4ch, 1 = 8ch
	float rotation = 0, diverge = 0.5, pitch = 0;

	float sRotation, sSpread = 0.5;
	bool autoOrbit = true;

	// Decoder

	Mach1Decode m1Decode;

	float decoderRotationY = 0, decoderRotationP = 0, decoderRotationR = 0;

	// UI

	bool enableMouse = true;
	bool enableIsotropicEncode = true;

	ofEasyCam camera;
};
