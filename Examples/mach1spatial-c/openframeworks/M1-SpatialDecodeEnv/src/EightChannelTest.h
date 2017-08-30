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
        /*
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
         */
        
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