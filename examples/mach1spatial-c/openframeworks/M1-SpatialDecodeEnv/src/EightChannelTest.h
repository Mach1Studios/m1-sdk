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
        
		players[0].load("8ChannelAudio/1/1.wav");
		players[1].load("8ChannelAudio/1/2.wav");
		players[2].load("8ChannelAudio/1/3.wav");
		players[3].load("8ChannelAudio/1/4.wav");
		players[4].load("8ChannelAudio/1/5.wav");
		players[5].load("8ChannelAudio/1/6.wav");
		players[6].load("8ChannelAudio/1/7.wav");
		players[7].load("8ChannelAudio/1/8.wav");
    }
    
    void update() {
		
		m1DSPAlgorithms.speed = speed;
		
		// Handling audio
		if (!perSample) {
			std::vector<float> tmpVolumes = audioMixAlgorithm(angleX, angleY, angleZ);

			// thread safe copy
			volumes.resize(tmpVolumes.size());
			for (int i = 0; i< volumes.size(); i++) volumes[i] = tmpVolumes[i];
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
	ofxAudioDecoder players[8];

    //////////////
    
    std::vector<float> audioMixAlgorithm(float X, float Y, float Z) {
        return m1DSPAlgorithms.eightChannelsAlgorithm(X, Y, Z);
    }
    
    int scheduleRestart = 30;
    
    void restart() {
		pos = 0;
    }
    
	void audioOut(float * output, int bufferSize, int nChannels)
	{
		// Handling audio
		if (isPlay)
		{
			float sample;

			for (int i = 0; i < bufferSize; i++)
			{
				if (perSample) {
					std::vector<float> tmpVolumes = audioMixAlgorithm(angleX, angleY, angleZ);

					// thread safe copy
					volumes.resize(tmpVolumes.size());
					for (int i = 0; i< volumes.size(); i++) volumes[i] = tmpVolumes[i];
				}

				sample = 0;
				for (int j = 0; j < 8; j++) {
					sample += players[j].getRawSamples()[pos] *  volumes[j * 2];
				}
				output[i*nChannels + 1] = sample / 8 * overallVolume;

				sample = 0;
				for (int j = 0; j < 8; j++) {
					sample += players[j].getRawSamples()[pos] * volumes[j * 2 + 1];
				}
				output[i*nChannels] = sample / 8 * overallVolume;

				pos++;

				// loop
				bool isFinish = false;
				for (int j = 0; j < 8; j++) {
					isFinish |= (pos >= players[j].getRawSamples().size());
				}
				if (isFinish) pos = 0;
			}
		}
	}

    void keyPressed(int key) {
        if (key == ' ') {
			isPlay = !isPlay;
			restart();
        }
    }

};
