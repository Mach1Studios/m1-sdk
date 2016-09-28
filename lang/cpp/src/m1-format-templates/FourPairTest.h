#include "BaseAudioTest.h"

class FourPairTest: public BaseAudioTest {
public:
    FourPairTest() {
        players[0].load("4PairAudio/1/000.wav"); players[0].setLoop(true);
        players[1].load("4PairAudio/1/090.wav"); players[1].setLoop(true);
        players[2].load("4PairAudio/1/180.wav"); players[2].setLoop(true);
        players[3].load("4PairAudio/1/270.wav"); players[3].setLoop(true);
    }
    
    void update() {
        // Handling audio
        
        volumes = audioMixAlgorithm(angleX, angleY, angleZ);
        players[0].setVolume(volumes[0] * overallVolume);
        players[1].setVolume(volumes[1] * overallVolume);
        players[2].setVolume(volumes[2] * overallVolume);
        players[3].setVolume(volumes[3] * overallVolume);
        
        
    }
    
    void draw() {
        
         // Restart offset fix
         if (scheduleRestart > 0) scheduleRestart--;
         if (scheduleRestart == 0) {
             restart();
             scheduleRestart--;
         }
        
         //
        
         ofSetLineWidth(4);
         ofSetCircleResolution(48);
         
         ofSetColor(200);
         ofSetLineWidth(2);
         
         ofSetColor(0, 0, 200);
         ofDrawSphere(200, 0, 100, 5 + volumes[0] * 15);
         ofDrawSphere(0, 200, 100, 5 + volumes[1] * 15);
         ofDrawSphere(-200, 0, 100, 5 + volumes[2] * 15);
         ofDrawSphere(0, -200, 100, 5 + volumes[3] * 15);
         
         
         ofDisableLighting();
         
         ofSetColor(255);
         ofDrawBitmapString("00", 200, 0, 100);
         ofDrawBitmapString("090", 0, 200, 100);
         ofDrawBitmapString("180", -200, 0, 100);
         ofDrawBitmapString("270", 0, -200, 100);
        
        
         ofSetColor(0, 100, 0);
         ofNoFill();
         ofDrawCircle(0, 0, 100, 150);

         ofDrawLine(145, 0, 100, 155, 0, 100);
         ofFill();
    }
    
    void drawOverlay() {
        ofDrawBitmapStringHighlight("Four pair test", 20, ofGetHeight() - 70);
        
        float phaseSum;
        for (int i = 0; i < 4; i++) {
            phaseSum += (float)players[i].getPositionMS();
        }
        phaseSum /= 4;
        
        for (int i = 0; i < 4; i++) {
            float phaseOffset = phaseSum - players[i].getPositionMS();
            ofDrawBitmapStringHighlight(ofToString(phaseOffset), 20, 120 + i * 20);
        }
        
        // Player controls
        ofSetColor(255);
        ofLine(30, ofGetHeight() - 30, ofGetWidth() - 170 - 60, ofGetHeight() - 30);
        ofFill();
        ofCircle(ofLerp(30, ofGetWidth() - 170 - 60, players[0].getPosition()), ofGetWidth() - 170 - 60 , 20);
        
        ofCircle(30 + players[0].getPosition() * (ofGetWidth() - 200 - 60), ofGetHeight() - 30, 10);
        
    }
    
    void restart() {
        for (int i = 0; i < 4; i++) {
            players[i].setPosition(0);
        }
    }
    
    void setPosition(float position) {
        for (int i = 0; i < 4; i++) {
            players[i].setPosition(position);
        }
    }
    
    void keyPressed(int key) {
        if (key == ' ') {
            players[0].play();
            players[1].play();
            players[2].play();
            players[3].play();
            restart();
        }
    }

    
    void setOverallVolume(float volume) {
        overallVolume = volume;
    }
    
    virtual void mousePressed(int x, int y) {
        if ((x > 30 && (x < (ofGetWidth() - 200 - 60)) ) && (y > (ofGetHeight() - 70))) {
            for (int i = 0; i < 4; i++) {
                players[i].setPosition((((float)x - 30) / (ofGetWidth() - 200 - 60)));
            }
        }
    };

    
    float overallVolume = 0;
    std::vector<float> volumes;
    ofSoundPlayer players[4];
    
//////////////
    
    std::vector<float> audioMixAlgorithm(float X, float Y, float Z) {
        float volumes[4];
        volumes[0] = 1. - std::min(1., std::min((float)360. - Y, Y) / 90.);
        volumes[1] = 1. - std::min(1., std::abs((float)90. - Y) / 90.);
        volumes[2] = 1. - std::min(1., std::abs((float)180. - Y) / 90.);
        volumes[3] = 1. - std::min(1., std::abs((float)270. - Y) / 90.);
        
        std::vector<float> result;
        result.push_back(volumes[0]);
        result.push_back(volumes[1]);
        result.push_back(volumes[2]);
        result.push_back(volumes[3]);
        return result;
    }

    
//////////////
    
    int scheduleRestart = 30;
};
