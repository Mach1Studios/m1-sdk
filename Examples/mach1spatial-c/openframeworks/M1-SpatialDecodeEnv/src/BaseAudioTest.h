
#ifndef BaseAudioTest_h
#define BaseAudioTest_h

#include "M1DSPAlgorithms.h"
#include "ofxAudioDecoder.h"


class BaseAudioTest {
public:
    BaseAudioTest() {
		angleX = 0;
		angleY = 0;
		angleZ = 0;

		speed = 0.2;
		pos = 0;
		isPlay = false;
		perSample = false;
    }
    
    virtual void draw() = 0;
    virtual void update() {};
    
    virtual void drawOverlay() = 0;
    
    virtual void setOverallVolume(float volume) = 0;
    
    virtual void keyPressed(int key) {};
    virtual void mousePressed(int x, int y) {};
     
	virtual void audioOut(float * output, int bufferSize, int nChannels) {}; 

	M1DSPAlgorithms m1DSPAlgorithms;
	
	float angleX, angleY, angleZ;

	int pos;
	bool isPlay;
	bool perSample;
	float speed;
};

#endif /* BaseAudioTest_h */