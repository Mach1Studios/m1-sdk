
#ifndef BaseAudioTest_h
#define BaseAudioTest_h

#include "Mach1Decode.h"
#include "ofxAudioDecoder.h"

class BaseAudioTest {
public:
    BaseAudioTest() {
		angleYaw = 0;
		anglePitch = 0;
		angleRoll = 0;

		speed = 0.9;
		pos = 0;
		isPlaying = false;
		perSample = false;
    }
    
    virtual void draw() = 0;
    virtual void update() {};
    
	virtual void drawOverlay() = 0;
	virtual float getPlayheadPosition() = 0;

    virtual void setOverallVolume(float volume) = 0;
    
    virtual void keyPressed(int key) {};
    virtual void mousePressed(int x, int y) {};
     
	virtual void audioOut(float * output, int bufferSize, int nChannels) {}; 

    Mach1Decode mach1Decode;

	float angleYaw, anglePitch, angleRoll;

	int pos;
	bool isPlaying;
	bool perSample;
	float speed;
};

#endif /* BaseAudioTest_h */
