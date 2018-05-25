#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "M1Encode.h"
#include "Mach1Decode.h"

#define RENDERING_SCALE 200

class ofApp : public ofBaseApp{

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

    ofxImGui::Gui gui;

    ofSoundPlayer playerL, playerR;

    // Encoder
    
    M1Encode m1Encode;
    Mach1Decode m1Decode;
    
    int inputKind = 0; // 0 = MONO, 1 = STEREO, 2 = QUAD, 3 = AFORMAT, 4 = BFORMAT
    int outputKind = 1; // 0 = 4ch, 1 = 8ch
    float rotation = 0, diverge = 0.5, pitch = 0;
    
    float sRotation, sSpread = 0.5;
    bool autoOrbit = true;
    
    // Decoder
    
    float decoderRotationY = 0, decoderRotationP = 0, decoderRotationR = 0;
    
    // UI
    
    
    bool enableMouse = true;
    bool enableIsotropicEncode = true;
    
    ofEasyCam camera;
};
