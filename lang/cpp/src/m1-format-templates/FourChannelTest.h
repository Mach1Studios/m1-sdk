#include "BaseAudioTest.h"

class FourChannelTest: public BaseAudioTest {
public:
    FourChannelTest() {
        for (int i = 0; i < 4; i++){
            playersLeft[0].load("4ChannelAudio/1/1.wav"); playersLeft[0].setLoop(true);
            playersLeft[1].load("4ChannelAudio/1/2.wav"); playersLeft[1].setLoop(true);
            playersLeft[2].load("4ChannelAudio/1/3.wav"); playersLeft[2].setLoop(true);
            playersLeft[3].load("4ChannelAudio/1/4.wav"); playersLeft[3].setLoop(true);
            playersLeft[i].setPan(-1);
            playersRight[0].load("4ChannelAudio/1/1.wav"); playersRight[0].setLoop(true);
            playersRight[1].load("4ChannelAudio/1/2.wav"); playersRight[1].setLoop(true);
            playersRight[2].load("4ChannelAudio/1/3.wav"); playersRight[2].setLoop(true);
            playersRight[3].load("4ChannelAudio/1/4.wav"); playersRight[3].setLoop(true);
            playersRight[i].setPan(1);
        }
//        for (int i = 0; i < 4; i++) {
//            ofLog() << "loading " << "4ChannelAudio/1/" + ofToString(i + 1) + ".wav";
//            playersLeft[i].load("4ChannelAudio/1/" + ofToString(i + 1) + ".wav"); playersLeft[0].setLoop(true); playersLeft[0].play();
//            playersLeft[i].setPan(0);
//            playersRight[i].load("4ChannelAudio/1/" + ofToString(i + 1) + ".wav"); playersRight[0].setLoop(true); playersRight[0].play();
//            playersRight[i].setPan(1);
//        }
    }
    
    void update() {
        // Handling audio 
        
        volumes = audioMixAlgorithm(angleX, angleY, angleZ);
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
        
        
        //

        ofSetLineWidth(4);
        ofSetCircleResolution(48);
        
        ofSetColor(200);
        ofSetLineWidth(2);
        
        ofSetColor(200, 0, 0); // 1
        ofDrawSphere(200, -180, 100, volumes[0] * 15 + 5);
        ofSetColor(0, 0, 200);
        ofDrawSphere(200, -220, 100, volumes[1] * 15 + 5);

        ofSetColor(200, 0, 0); // 2
        ofDrawSphere(200, 220, 100, volumes[2] * 15 + 5);
        ofSetColor(0, 0, 200);
        ofDrawSphere(200, 180, 100, volumes[3] * 15 + 5);
        
        ofSetColor(200, 0, 0); // 3
        ofDrawSphere(-200, -180, 100, volumes[4] * 15 + 5);
        ofSetColor(0, 0, 200);
        ofDrawSphere(-200, -220, 100, volumes[5] * 15 + 5);
        
        ofSetColor(200, 0, 0); // 4
        ofDrawSphere(-200, 180, 100, volumes[6] * 15 + 5);
        ofSetColor(0, 0, 200);
        ofDrawSphere(-200, 220, 100, volumes[7] * 15 + 5);
        
        ofDisableLighting();
        
        
        
        ofSetColor(255);
        ofDrawBitmapString("1", 200, -200, 100);
        ofSetColor(255);
        ofDrawBitmapString("2", 200, 200, 100);
        ofSetColor(255);
        ofDrawBitmapString("3", -200, -200, 100);
        ofSetColor(255);
        ofDrawBitmapString("4", -200, 200, 100);
        
 
        ofSetColor(0, 100, 0);
        ofNoFill();
            ofDrawCircle(0, 0, 100, 150);
        
        ofFill();
    }
    
    void drawOverlay() {
        ofDrawBitmapStringHighlight("Four channel test", 20, ofGetHeight() - 70);

        // Player controls
        ofSetColor(255);
        ofLine(30, ofGetHeight() - 30, ofGetWidth() - 170 - 60, ofGetHeight() - 30);
        ofFill();
        ofCircle(ofLerp(30, ofGetWidth() - 170 - 60, playersLeft[0].getPosition()), ofGetWidth() - 170 - 60 , 20);
        
        ofCircle(30 + playersLeft[0].getPosition() * (ofGetWidth() - 200 - 60), ofGetHeight() - 30, 10);
        
        
        ofSetColor(255, 0, 0);
        ofDrawBitmapString("Red means left", 20, ofGetHeight() - 250);
        ofSetColor(0, 0, 255);
        ofDrawBitmapString("Blue means right", 20, ofGetHeight() - 230);
    }
  
    void setOverallVolume(float volume) {
        overallVolume = volume;
    }
    
    virtual void mousePressed(int x, int y) {
        // Player controls
        if ((x > 30 && (x < (ofGetWidth() - 200 - 60)) ) && (y > (ofGetHeight() - 70))) {
            for (int i = 0; i < 4; i++) {
                playersLeft[i].setPosition((((float)x - 30) / (ofGetWidth() - 200 - 60)));
                playersRight[i].setPosition((((float)x - 30) / (ofGetWidth() - 200 - 60)));
            }
        }
    };

    
    float overallVolume = 0;
    float coefficients[4];
    std::vector<float> volumes;
    ofSoundPlayer playersLeft[4];
    ofSoundPlayer playersRight[4];
    
    //////////////
    
    std::vector<float> audioMixAlgorithm(float X, float Y, float Z) {
        float volumes[8];
        coefficients[0] = 1. - std::min(1., std::min((float)360. - Y, Y) / 90.);
        coefficients[1] = 1. - std::min(1., std::abs((float)90. - Y) / 90.);
        coefficients[2] = 1. - std::min(1., std::abs((float)180. - Y) / 90.);
        coefficients[3] = 1. - std::min(1., std::abs((float)270. - Y) / 90.);
        
        
        std::vector<float> result;
        result.resize(8);
        result[0] = coefficients[0]; // 1 left
        result[1] = coefficients[3]; //   right
        result[2] = coefficients[1]; // 2 left
        result[3] = coefficients[0]; //   right
        result[4] = coefficients[3]; // 3 left
        result[5] = coefficients[2]; //   right
        result[6] = coefficients[2]; // 4 left
        result[7] = coefficients[1]; //   right
        return result;
    }
    

    //////////////
    
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
    

private:
    
};
