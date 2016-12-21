
#ifndef BaseAudioTest_h
#define BaseAudioTest_h

#include "M1DSPAlgorithms.h"

class BaseAudioTest {
public:
    BaseAudioTest() {
        
    }

	virtual void setPosition(float seconds) = 0;
	virtual float getPosition() = 0;

	virtual void audioOut(float * output, int bufferSize, int nChannels) = 0;

	virtual void play() = 0;
	virtual void pause() = 0;

	virtual void draw() = 0;
	virtual void update() {};
    
    virtual void drawOverlay() = 0;
    
    virtual void setOverallVolume(float volume) = 0;
    
    virtual void keyPressed(int key) {};
    virtual void mousePressed(int x, int y) {};
    
    float angleX, angleY, angleZ;

};

#endif /* BaseAudioTest_h */
