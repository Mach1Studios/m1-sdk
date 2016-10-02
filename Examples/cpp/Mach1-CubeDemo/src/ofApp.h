#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "ofxWatermark.h"

#include "ArduinoWatcher.h"

#include "BaseAudioTest.h"
#include "Audio1.h"
#include "Audio2.h"


#define SETTINGS_TOOLBAR_WIDTH 200

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void serialAngleUpdate(float serialY, float serialP);
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
    
    ofSerial serial;
    
    int selectedTest = 0;
    std::vector<BaseAudioTest*> tests;
    
    // Simulation
    
    ofPoint simulationAngles = ofPoint(0);
    void updateSimulationAngles();
    
    float angleX, angleY, angleZ;

    ArduinoWatcher arduinoWatcher;
    bool initializedController = false;
    
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
    
    vector<unsigned char> queueBuffer;
    vector<unsigned char> queueBackupDebug;
    
    int lastY = 0, lastP = 0;
    float pitchAngle = 0., pitchAngleClamp = 0.;
    
    bool getNewDataFromQueue(int & Y, int & P) {
        
        
        unsigned char newData[4];
        
        int idx = -1;
        for (int i = 0; i < queueBuffer.size(); i++) {
            if ((queueBuffer[i] == 255) && (idx < 0)) {
                if (((queueBuffer.size() - idx > 4))) idx = i;
                else ofLog() << queueBuffer.size() << " too close to " << idx;
            }
        }
        
        if ((idx < queueBuffer.size() - 4)) {
            //            ofLog() << "found 255 at " << idx << " / " << queueBuffer.size();
            //            newData[0] = queueBuffer[idx];
            int Yp, Pp;
            
            newData[0] = queueBuffer[idx + 1];
            newData[1] = queueBuffer[idx + 2];
            newData[2] = queueBuffer[idx + 3];
            
            
            decode(newData, Yp, Pp);
            Y = Yp;
            P = Pp;
            
            
            queueBuffer.erase(queueBuffer.begin() + idx, queueBuffer.begin() + idx + 4);
            return true;
        } else if (idx < 0) {
            ofLog() << "not found suitable of " << queueBuffer.size() << " symbols";
            queueBuffer.clear();
            return false;
        } else {
            queueBuffer.clear();
            return false;
        }
        
    }
    
    void encode(int Y, int P, unsigned char* x) {
        
        unsigned char Ypart2 = Y / 254;
        unsigned char Yc[2] = {0, 0};
        Yc[0] = (Y - (int)Ypart2 * 254);
        Yc[1] = Ypart2;
        
        unsigned char Ppart2 = P / 254;
        unsigned char Pc[2] = {0, 0};
        Pc[0] = (unsigned char)((P - (int)Ppart2 * 254));
        Pc[1] = Ppart2;
        
        
        unsigned char multipliers = (Ypart2 << 4) | Ppart2;
        
        x[0] = Yc[0];
        x[1] = Pc[0];
        x[2] = multipliers;
        
    }
    
    void decode(unsigned char* input, int & Y, int & P) {
        char multipliers = input[2];
        
        char Ypart2 = (multipliers & 0b11110000) >> 4;
        char Ppart2 = (multipliers & 0b00001111);
        
        //			ofLog() << "decoding Y = " << ((int)Ypart2 * 254 + (int)input[0]) << " from " << (int)input[0] << ";" << (int)Ypart2;
        //			ofLog() << "decoding P = " << ((int)Ppart2 * 254 + (int)input[1]) << " from " << (int)input[1] << ";" << (int)Ppart2;
        
        Y = ((int)Ypart2 * 254 + (int)input[0]);
        P = ((int)Ppart2 * 254 + (int)input[1]);
        
        
        
    }
    
};
