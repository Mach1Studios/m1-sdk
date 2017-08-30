#pragma once

#include "ofMain.h"
#include "ofxImGui.h"

//

#include "BaseAudioTest.h"
#include "IsotropicEightChannelTest.h"
#include "EightChannelTest.h"

#define SETTINGS_TOOLBAR_WIDTH 200

class ofApp : public ofBaseApp{
		ofSoundStream soundStream;
		ofImage logo;
		ofxImGui::Gui gui;
		int framerate;

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
    
    int selectedTest = 0;
    std::vector<BaseAudioTest*> tests;
    
    // Simulation
    
	ofPoint simulationAngles = ofPoint(0);
	void updateSimulationAngles();
    
    float angleX, angleY, angleZ;
    
    // Visualizing
    
    ofPoint spectatorCam = ofPoint(0.25, 0.5); // spectator cam angle
    
    ofLight pointLight;
    ofLight pointLight2;
    ofLight pointLight3;
    ofMaterial material;
    
    ofSpherePrimitive sphere;
    
    
    void drawSphere(float x, float y, float z, float scale, bool wireframe = false) {
        if (wireframe) {
            ofNoFill();
                sphere.setPosition(x, y, z);
                sphere.setRadius(scale);
                sphere.drawWireframe();
            ofFill();
        } else {
            sphere.setPosition(x, y, z);
            sphere.setRadius(scale);
            sphere.draw();
        }
    }

    
    // UI
    
    ofPoint spectatorCamStart, dragStart, anglesDragStart;

    bool dragginCamera;
};
