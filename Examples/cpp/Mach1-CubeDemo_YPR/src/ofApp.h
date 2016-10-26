#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "ofxWatermark.h"

#include "ArduinoWatcher.h"
#include "ArduinoDecoder.h"

#include "BaseAudioTest.h"
#include "Audio1.h"
#include "Audio2.h"


#define SETTINGS_TOOLBAR_WIDTH 200

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit() {
        delete pointLight;
        delete pointLight2;
        delete pointLight3;
        
        serial.close();
        arduinoWatcher->stopThread();
        ofSleepMillis(1000);
        delete arduinoWatcher;
    }
    
    void serialAngleUpdate(float serialY, float serialP, float serialR);
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
    
    bool setupFinished = false;
    
    ArduinoDecoder decoder;
    
    float a, b, c;
    
    ofSerial serial;
    
    int selectedTest = 0;
    std::vector<BaseAudioTest*> tests;
    
    // Simulation
    
    ofPoint simulationAngles = ofPoint(0);
    void updateSimulationAngles();
    
    float angleX, angleY, angleZ;

    ArduinoWatcher *arduinoWatcher;
    bool initializedController = false;
    
    // Visualizing
    
    ofPoint spectatorCam = ofPoint(0.25, 0.5); // spectator cam angle
    
    ofLight *pointLight;
    ofLight *pointLight2;
    ofLight *pointLight3;
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
    
    ofImage logo;
    ofxImGui gui;
    
    ofxWatermark watermark;
    
    //Map Utility
    static float mmap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp) {
        
        if (fabs(inputMin - inputMax) < __FLT_EPSILON__){
            return outputMin;
        } else {
            float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
            
            if( clamp ){
                if(outputMax < outputMin){
                    if( outVal < outputMax )outVal = outputMax;
                    else if( outVal > outputMin )outVal = outputMin;
                }else{
                    if( outVal > outputMax )outVal = outputMax;
                    else if( outVal < outputMin )outVal = outputMin;
                }
            }
            return outVal;
        }
    }
};
