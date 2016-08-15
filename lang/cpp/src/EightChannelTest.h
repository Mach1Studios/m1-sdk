#include "BaseAudioTest.h"

class EightChannelTest: public BaseAudioTest {
public:
    EightChannelTest() {
        spherePoints[0] = ofPoint(200, -200, 0);
        spherePoints[1] = ofPoint(200, 200, 0);
        spherePoints[2] = ofPoint(-200, -200, 0);
        spherePoints[3] = ofPoint(-200, 200, 0);

        spherePoints[4] = ofPoint(200, -200, 200);
        spherePoints[5] = ofPoint(200, 200, 200);
        spherePoints[6] = ofPoint(-200, -200, 200);
        spherePoints[7] = ofPoint(-200, 200, 200);
        
        //
        
        volumes.resize(8);
        
        for (int i = 0; i < 8; i++){
            playersLeft[0].load("8ChannelAudio/1/1.wav"); playersLeft[0].setLoop(true);
            playersLeft[1].load("8ChannelAudio/1/2.wav"); playersLeft[1].setLoop(true);
            playersLeft[2].load("8ChannelAudio/1/3.wav"); playersLeft[2].setLoop(true);
            playersLeft[3].load("8ChannelAudio/1/4.wav"); playersLeft[3].setLoop(true);
            playersLeft[4].load("8ChannelAudio/1/5.wav"); playersLeft[4].setLoop(true);
            playersLeft[5].load("8ChannelAudio/1/6.wav"); playersLeft[5].setLoop(true);
            playersLeft[6].load("8ChannelAudio/1/7.wav"); playersLeft[6].setLoop(true);
            playersLeft[7].load("8ChannelAudio/1/8.wav"); playersLeft[7].setLoop(true);
            playersLeft[i].setPan(-1);
            playersRight[0].load("8ChannelAudio/1/1.wav"); playersRight[0].setLoop(true);
            playersRight[1].load("8ChannelAudio/1/2.wav"); playersRight[1].setLoop(true);
            playersRight[2].load("8ChannelAudio/1/3.wav"); playersRight[2].setLoop(true);
            playersRight[3].load("8ChannelAudio/1/4.wav"); playersRight[3].setLoop(true);
            playersRight[4].load("8ChannelAudio/1/5.wav"); playersRight[4].setLoop(true);
            playersRight[5].load("8ChannelAudio/1/6.wav"); playersRight[5].setLoop(true);
            playersRight[6].load("8ChannelAudio/1/7.wav"); playersRight[6].setLoop(true);
            playersRight[7].load("8ChannelAudio/1/8.wav"); playersRight[7].setLoop(true);
            playersRight[i].setPan(1);
        }
    }
    
    void update() {
        // Handling audio
        
        volumes = audioMixAlgorithm(angleX, angleY, angleZ);
        
        for (int i = 0; i < 8; i++) {
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
            ofSetColor(200, 0, 0); // 1
            ofDrawSphere(spherePoints[i].x, spherePoints[i].y - 20, spherePoints[i].z, volumes[i * 2] * 18 + 2);
            ofSetColor(0, 0, 200);
            ofDrawSphere(spherePoints[i].x, spherePoints[i].y + 20, spherePoints[i].z, volumes[i * 2 + 1] * 18 + 2);
            
        }
        
        
        ofDisableLighting();
        for (int i = 0; i < 8; i++) {
            ofSetColor(255);
            ofDrawBitmapString(ofToString(i), spherePoints[i].x, spherePoints[i].y, spherePoints[i].z);
        }
    }
    
    void drawOverlay() {
        ofDrawBitmapStringHighlight("Eight channel test", 20, ofGetHeight() - 70);
    }

    void setOverallVolume(float volume) {
        overallVolume = volume;

    }
    
    //
    
    ofPoint spherePoints[8];
    std::vector<float> volumes;
    float overallVolume = 0;
    float coefficients[8];
    ofSoundPlayer playersLeft[8];
    ofSoundPlayer playersRight[8];

    //////////////
    
    std::vector<float> audioMixAlgorithm(float X, float Y, float Z) {
        float volumes[8];
        coefficients[0] = 1. - std::min(1., std::min((float)360. - Y, Y) / 90.);
        coefficients[1] = 1. - std::min(1., std::abs((float)90. - Y) / 90.);
        coefficients[2] = 1. - std::min(1., std::abs((float)180. - Y) / 90.);
        coefficients[3] = 1. - std::min(1., std::abs((float)270. - Y) / 90.);
        
        float tiltAngle = ofMap(Z, -90, 90, 0., 1.);
        float tiltHigh = cos(tiltAngle * (0.5*PI));
        float tiltLow = cos((1.0 - tiltAngle) * (0.5*PI));
        
        //ISSUE//
        //Able to kill stereo by making both pitch and tilt at max or min values together
        
        std::vector<float> result;
        result.resize(16);
        result[0] = coefficients[0] * tiltHigh; // 1 left
        result[1] = coefficients[3] * tiltHigh; //   right
        result[2] = coefficients[1] * tiltLow; // 2 left
        result[3] = coefficients[0] * tiltLow; //   right
        result[4] = coefficients[3] * tiltLow; // 3 left
        result[5] = coefficients[2] * tiltLow; //   right
        result[6] = coefficients[2] * tiltHigh; // 4 left
        result[7] = coefficients[1] * tiltHigh; //   right
        
        result[0 + 8] = coefficients[0] * tiltLow; // 1 left
        result[1 + 8] = coefficients[3] * tiltLow ; //   right
        result[2 + 8] = coefficients[1] * tiltHigh ; // 2 left
        result[3 + 8] = coefficients[0] * tiltHigh ; //   right
        result[4 + 8] = coefficients[3] * tiltHigh ; // 3 left
        result[5 + 8] = coefficients[2] * tiltHigh ; //   right
        result[6 + 8] = coefficients[2] * tiltLow ; // 4 left
        result[7 + 8] = coefficients[1] * tiltLow ; //   right
        
        float pitchAngle = ofMap(X, 90, -90, 0., 1.);
        float pitchHigherHalf = cos(pitchAngle * (0.5*PI));
        float pitchLowerHalf = cos((1.0 - pitchAngle) * (0.5*PI));
        //float pitchLowerHalf = 1 - pitchHigherHalf;
        
        for (int i = 0; i < 8; i++) {
            result[i] *= pitchLowerHalf;
            result[i + 8] *= pitchHigherHalf;
        }
        
        return result;
    }
    
    int scheduleRestart = 30;
    
    void restart() {
        for (int i = 0; i < 8; i++) {
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
            playersLeft[4].play();
            playersRight[4].play();
            playersLeft[5].play();
            playersRight[5].play();
            playersLeft[6].play();
            playersRight[6].play();
            playersLeft[7].play();
            playersRight[7].play();
            restart();
        }
    }

};