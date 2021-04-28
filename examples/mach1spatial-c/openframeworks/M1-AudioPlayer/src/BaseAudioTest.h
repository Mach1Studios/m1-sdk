
#ifndef BaseAudioTest_h
#define BaseAudioTest_h

#include "Mach1Decode.h"

class BaseAudioTest {
public:
    BaseAudioTest() {
    }

    virtual void draw() = 0;
    virtual void update() {};
    virtual void drawOverlay() = 0;
    virtual void setOverallVolume(float volume) = 0;
    virtual void keyPressed(int key) {};
    virtual void mousePressed(int x, int y) {};

    float angleX, angleY, angleZ;
    Mach1Decode mach1Decode;   
};

#endif /* BaseAudioTest_h */
