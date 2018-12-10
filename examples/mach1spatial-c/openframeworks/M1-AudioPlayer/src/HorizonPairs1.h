#include "BaseAudioTest.h"


class HorizonPairsOne: public BaseAudioTest {
public:
    HorizonPairsOne() {
        spherePoints[0] = ofPoint(200, -200, 0);
        spherePoints[1] = ofPoint(200, 200, 0);
        spherePoints[2] = ofPoint(-200, -200, 0);
        spherePoints[3] = ofPoint(-200, 200, 0);
        
        spherePoints[4] = ofPoint(200, -200, 200);
        spherePoints[5] = ofPoint(200, 200, 200);
        spherePoints[6] = ofPoint(-200, -200, 200);
        spherePoints[7] = ofPoint(-200, 200, 200);
        
        //
        
        volumes.resize(4);
        
        for (int i = 0; i < 4; i++){
            playersLeft[0].load("1/1.wav"); playersLeft[0].setLoop(true);
            playersLeft[1].load("1/3.wav"); playersLeft[1].setLoop(true);
            playersLeft[2].load("1/5.wav"); playersLeft[2].setLoop(true);
            playersLeft[3].load("1/7.wav"); playersLeft[3].setLoop(true);
            playersLeft[i].setPan(-1);
            playersRight[0].load("1/2.wav"); playersRight[0].setLoop(true);
            playersRight[1].load("1/4.wav"); playersRight[1].setLoop(true);
            playersRight[2].load("1/6.wav"); playersRight[2].setLoop(true);
            playersRight[3].load("1/8.wav"); playersRight[3].setLoop(true);
            playersRight[i].setPan(1);
        }
        
        //Mach1 Decode Setup
        //Setup the correct angle convention for orientation Euler input angles
        mach1Decode.setPlatformType(Mach1PlatformOfEasyCam);
        //Setup the expected spatial audio mix format for decoding
        mach1Decode.setDecodeAlgoType(Mach1DecodeAlgoHorizonPairs);
        //Setup for the safety filter speed:
        //1.0 = no filter | 0.1 = slow filter
        mach1Decode.setFilterSpeed(0.95f);
    }
    
    void update() {
        // Handling audio
        volumes = audioMixAlgorithm(angleY, angleX, angleZ);

        for (int i = 0; i < 4; i++) {
            playersLeft[i].setVolume(volumes[i * 2] * overallVolume);
            playersRight[i].setVolume(volumes[i * 2 + 1] * overallVolume);
        }
    }
    
    
    void draw() {
        
        // Restart offset fix
        if (scheduleRestart > 0) scheduleRestart--;
        if (scheduleRestart == 0) {
            restart();
            scheduleRestart--;
        }
        
        ofSetLineWidth(4);
        ofSetCircleResolution(48);
        for (int i = 0; i < 8; i++) {
            //            ofSetColor(200, 0, 0); // 1
            //            ofDrawSphere(spherePoints[i].x, spherePoints[i].y - 20, spherePoints[i].z, volumes[i * 2] * 18 + 2);
            //            ofSetColor(0, 0, 200);
            //            ofDrawSphere(spherePoints[i].x, spherePoints[i].y + 20, spherePoints[i].z, volumes[i * 2 + 1] * 18 + 2);
            
        }
        
        
        ofDisableLighting();
        for (int i = 0; i < 8; i++) {
            ofSetColor(255);
            //            ofDrawBitmapString(ofToString(i), spherePoints[i].x, spherePoints[i].y, spherePoints[i].z);
        }
        
        ofSetColor(0, 100, 0);
        ofNoFill();
        ofDrawCircle(0, 0, 100, 150);
        ofFill();
    }
    
    void drawOverlay() {
        ofDrawBitmapStringHighlight("Please turn Mach1 IMU on before launching this application", 10, 20);
        ofDrawBitmapStringHighlight("Sideload into app by right clicking app", 10, 40);
        ofDrawBitmapStringHighlight("and Showing Package Contents", 10, 60);
        ofDrawBitmapStringHighlight("Directory: M1-AudioPlayer/Contents/Resources/1/*.wav", 10, 100);
        ofDrawBitmapStringHighlight("Instructions:", ofGetWidth() - 500, 20);
        ofDrawBitmapStringHighlight("-Press 'spacebar' to play", ofGetWidth() - 500, 40);
        ofDrawBitmapStringHighlight("-Use the Yaw,Pitch,Roll sliders to", ofGetWidth() - 500, 60);
        ofDrawBitmapStringHighlight("simulate different head orientations", ofGetWidth() - 500, 80);
        
        // Player controls
        ofSetColor(255);
        ofLine(30, ofGetHeight() - 30, ofGetWidth() - 170 - 60, ofGetHeight() - 30);
        ofFill();
        ofCircle(ofLerp(30, ofGetWidth() - 170 - 60, playersLeft[0].getPosition()), ofGetWidth() - 170 - 60 , 20);
        
        ofCircle(30 + playersLeft[0].getPosition() * (ofGetWidth() - 200 - 60), ofGetHeight() - 30, 10);
    
        ofSetColor(255);
        for (int i = 0; i < volumes.size(); i++) {
//            ofDrawBitmapStringHighlight(" > " + ofToString(volumes[i], 3, 3), ofGetWidth() - 500, 120 + i * 40);
        }
    }
    
    void setOverallVolume(float volume) {
        overallVolume = volume;
        
        //    ofSetColor(255, 0, 0);
        //    ofDrawBitmapString("Red means left", 20, ofGetHeight() - 250);
        //    ofSetColor(0, 0, 255);
        //    ofDrawBitmapString("Blue means right", 20, ofGetHeight() - 230);
        
    }
    
    virtual void mousePressed(int x, int y) {
        // Player controls
        if ((x > 30 && (x < (ofGetWidth() - 200 - 60)) ) && (y > (ofGetHeight() - 70))) {
            for (int i = 0; i < 8; i++) {
                playersLeft[i].setPosition((((float)x - 30) / (ofGetWidth() - 200 - 60)));
                playersRight[i].setPosition((((float)x - 30) / (ofGetWidth() - 200 - 60)));
            }
        }
    };
    
    //
    
    ofPoint spherePoints[8];
    std::vector<float> volumes;
    float overallVolume = 0;
    float coefficients[8];
    ofSoundPlayer playersLeft[4];
    ofSoundPlayer playersRight[4];
    
    //////////////
    Mach1Decode mach1Decode;
    
    std::vector<float> audioMixAlgorithm(float X, float Y, float Z) {
        mach1Decode.beginBuffer();
        return mach1Decode.decode(X, Y, Z);
        mach1Decode.endBuffer();
    }
    
    int scheduleRestart = 30;
    
    void restart() {
        for (int i = 0; i < 4; i++) {
            playersLeft[i].setPosition(0);
            playersRight[i].setPosition(0);
        }
    }
    
    void keyPressed(int key) {
        if (key == ' ') {
            playersLeft[0].play();
            playersRight[0].play();
            playersLeft[1].play();
            playersRight[1].play();
            playersLeft[2].play();
            playersRight[2].play();
            playersLeft[3].play();
            playersRight[3].play();
            restart();
        }
    }
    
};
