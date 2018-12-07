#include "BaseAudioTest.h"

class SpatialTest2: public BaseAudioTest {
public:
    SpatialTest2() {
        spherePoints[0] = ofPoint(200, -200, 0);
        spherePoints[1] = ofPoint(200, 200, 0);
        spherePoints[2] = ofPoint(-200, -200, 0);
        spherePoints[3] = ofPoint(-200, 200, 0);
        
        spherePoints[4] = ofPoint(200, -200, 200);
        spherePoints[5] = ofPoint(200, 200, 200);
        spherePoints[6] = ofPoint(-200, -200, 200);
        spherePoints[7] = ofPoint(-200, 200, 200);

        players[0].load("2/1.wav");
        players[1].load("2/2.wav");
        players[2].load("2/3.wav");
        players[3].load("2/4.wav");
        players[4].load("2/5.wav");
        players[5].load("2/6.wav");
        players[6].load("2/7.wav");
        players[7].load("2/8.wav");
        
		int sampleRate = 44100;
		for (int i = 0; i < 8; i++) {
			((ofSoundBuffer&)players[i].getBuffer()).resample(1.0 * players[i].getBuffer().getSampleRate() / sampleRate);
			((ofSoundBuffer&)players[i].getBuffer()).setSampleRate(sampleRate);
		}
		
		//Mach1 Decode Setup
        //Setup the correct angle convention for orientation Euler input angles
        mach1Decode.setPlatformType(Mach1PlatformOfEasyCam);
        //Setup the expected spatial audio mix format for decoding
        mach1Decode.setDecodeAlgoType(Mach1DecodeAlgoSpatial);
    }
    
    void update() {

		mach1Decode.setFilterSpeed(speed);

        // Handling audio
        if (!perSample) {
			mach1Decode.beginBuffer();
			std::vector<float> tmpVolumes = mach1Decode.decode(angleYaw, anglePitch, angleRoll, 0, 0);
			mach1Decode.endBuffer();
            
            // thread safe copy
            volumes.resize(tmpVolumes.size());
            for(int i = 0; i< volumes.size(); i++) volumes[i] = tmpVolumes[i];
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
    
	float getPlayheadPosition() {
		return  (1.0 * pos / players[0].getRawSamples().size());
	}
	
	void setOverallVolume(float volume) {
        overallVolume = volume;
    }
    
    //
    
    ofPoint spherePoints[8];
    std::vector<float> volumes;
    float overallVolume = 1;
    float coefficients[8];
    ofxAudioDecoder players[8];
    
    //////////////
    int scheduleRestart = 30;
    
    void restart() {
        pos = 0;
    }
    
    void audioOut(float * output, int bufferSize, int nChannels)
    {
        // Handling audio
        if (isPlaying)
        {
            float sample;

			if (perSample) {
				mach1Decode.beginBuffer();
			}

            for (int i = 0; i < bufferSize; i++)
            {
                if (perSample) {
					std::vector<float> tmpVolumes = mach1Decode.decode(angleYaw, anglePitch, angleRoll, bufferSize, i);
                    
                    // thread safe copy
                    volumes.resize(tmpVolumes.size());
                    for (int i = 0; i< volumes.size(); i++) volumes[i] = tmpVolumes[i];
                }
                
                sample = 0;
                for (int j = 0; j < 8; j++) {
                    sample += players[j].getRawSamples()[pos] * volumes[j * 2];
                }
                output[i*nChannels] = sample * overallVolume;
                
                sample = 0;
                for (int j = 0; j < 8; j++) {
                    sample += players[j].getRawSamples()[pos]  *volumes[j * 2 + 1];
                }
                output[i*nChannels + 1] = sample * overallVolume;
                
                pos++;
                
                // loop
                bool isFinish = false;
                for (int j = 0; j < 8; j++) {
                    isFinish |= (pos >= players[j].getRawSamples().size());
                }
                if (isFinish) pos = 0;
            }

			if (perSample) {
				mach1Decode.endBuffer();
			}
        }
    }
    
    void keyPressed(int key) {
        if (key == ' ') {
            isPlaying = !isPlaying;
            restart();
        }
    }
    
};
