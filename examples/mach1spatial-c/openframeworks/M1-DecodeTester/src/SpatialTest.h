#include "BaseAudioTest.h"

class SpatialTest: public BaseAudioTest {
public:
    SpatialTest() {
        spherePoints[0] = ofPoint(200, -200, 200);
        spherePoints[1] = ofPoint(200, 200, 200);
        spherePoints[2] = ofPoint(-200, -200, 200);
        spherePoints[3] = ofPoint(-200, 200, 200);

        spherePoints[4] = ofPoint(200, -200, 0);
        spherePoints[5] = ofPoint(200, 200, 0);
        spherePoints[6] = ofPoint(-200, -200, 0);
        spherePoints[7] = ofPoint(-200, 200, 0);

		players[0].load("1/1.wav");
		players[1].load("1/2.wav");
		players[2].load("1/3.wav");
		players[3].load("1/4.wav");
		players[4].load("1/5.wav");
		players[5].load("1/6.wav");
		players[6].load("1/7.wav");
		players[7].load("1/8.wav");
    
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
        mach1Decode.setFilterSpeed(1);

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
    
    float coeffDerivativeScore = 0;
    
    void draw() {
        
        // Restart offset fix
        if (scheduleRestart > 0) scheduleRestart--;
        if (scheduleRestart == 0) {
            restart();
            scheduleRestart--;
        }
        
        coeffDerivativeScore = 0;
        int pointsCounted = 0;
        for (int pitch = -90; pitch < 90; pitch += 5) {
            for (int yaw = 0; yaw < 360; yaw += 5) {
                for (int roll = -180; roll < 180; roll += 5) {
                    
                    if ((abs(angleYaw - yaw) < 45) &&
                        (abs(anglePitch - pitch) < 45) &&
                        (abs(angleRoll - roll) < 45)) {

                            mach1Decode.beginBuffer();
                            std::vector<float> tmpVolumes = mach1Decode.decode(yaw, pitch, roll, 0, 0);
                            mach1Decode.endBuffer();
                            
                            float sum = 0;
                            for (auto &x: tmpVolumes) {
                                sum += x;
                            }
                            
                            sum -= 2;
                            
                            float volumeIrregularityTest = (sum / 2.0 - 0.9) * 3;
                            
                            mach1Decode.beginBuffer();
                            std::vector<float> tmpVolumesL = mach1Decode.decode(ofWrap(yaw - 1, 0, 360),
                                                                                pitch, roll, 0, 0);
                            mach1Decode.endBuffer();
                            
                            mach1Decode.beginBuffer();
                            std::vector<float> tmpVolumesR = mach1Decode.decode(ofWrap(yaw + 1, 0, 360),
                                                                                pitch, roll, 0, 0);
                            mach1Decode.endBuffer();

                            mach1Decode.beginBuffer();
                            std::vector<float> tmpVolumesU = mach1Decode.decode(yaw, ofWrap(pitch + 1,                                                      -90, 90), roll, 0, 0);
                            mach1Decode.endBuffer();
                            mach1Decode.beginBuffer();
                            std::vector<float> tmpVolumesD = mach1Decode.decode(yaw, ofWrap(pitch - 1,                                                      -90, 90), roll, 0, 0);
                            mach1Decode.endBuffer();

                            mach1Decode.beginBuffer();
                            std::vector<float> tmpVolumesRL = mach1Decode.decode(yaw, pitch, ofWrap(roll + 1,                                                      -180, 180), 0, 0);
                            mach1Decode.endBuffer();
                            mach1Decode.beginBuffer();
                            std::vector<float> tmpVolumesRR = mach1Decode.decode(yaw, pitch, ofWrap(roll - 1,                                                      -180, 180), 0, 0);
                            mach1Decode.endBuffer();

                            std::function<float(vector<float>, vector<float>)> compare = [&](vector<float> a, vector<float> b)->float {
                                float diff = 0;
                                for (int i = 0; i < a.size(); i++) {
                                    diff += abs(b[i] - a[i]);
                                }
                                return diff;
                            };
                            
                            float diffYaw = compare(tmpVolumesL, tmpVolumes) + compare(tmpVolumesR, tmpVolumes);
                            float diffPitch = compare(tmpVolumesU, tmpVolumes) + compare(tmpVolumesD, tmpVolumes);
                            float diffRoll = compare(tmpVolumesRL, tmpVolumes) + compare(tmpVolumesRR, tmpVolumes);

        //                    ofLog() << "yaw  = " << yaw << " pitch = " << pitch << " ; diffY = " << diffYaw;
        //                    ofLog() << "i  = " << yaw << " pitch = " << pitch << " ; sum = " << sum / 2;
                            
                            pointsCounted += 1;
                            coeffDerivativeScore += diffYaw + diffPitch + diffRoll;
                            
                            float minDrawingThreshold = 1.0;
                            float maxDrawingThreshold = 0.0;
//                            if ((diffYaw < minDrawingThreshold) || (diffYaw > maxDrawingThreshold) ||
//                                (diffPitch < minDrawingThreshold) || (diffPitch > maxDrawingThreshold) ||
//                                (diffRoll < minDrawingThreshold) || (diffRoll > maxDrawingThreshold)) {
                                
                                
                                
                                    ofDisableLighting();
                //                    ofSetColor(200 * volumeIrregularityTest, 0, 0, 200);
                                    ofSetColor(200 * diffYaw, 200 * diffPitch, 200 * diffRoll);
                                    ofDrawSphere(cos(ofDegToRad(yaw)) * (50 + (roll + 180) * 1.2),
                                                 sin(ofDegToRad(yaw)) * (50 + (roll + 180) * 1.2), pitch * 2, 5);
                                    ofEnableLighting();
                                    
//                            }
                                
                    }
                }
            }
        }
        coeffDerivativeScore /= float(pointsCounted);
        /*
        ofSetLineWidth(4);
        ofSetCircleResolution(48);
        for (int i = 0; i < 8; i++) {
            ofSetColor(200, 0, 0); // 1
            ofDrawSphere(spherePoints[i].x, spherePoints[i].y - 20, spherePoints[i].z, volumes[i * 2] * 18 + 2);
            ofSetColor(0, 0, 200);
            ofDrawSphere(spherePoints[i].x, spherePoints[i].y + 20, spherePoints[i].z, volumes[i * 2 + 1] * 18 + 2);
        }
         */
        
        ofDisableLighting();
        for (int i = 0; i < 8; i++) {
            ofSetColor(255);
            ofDrawBitmapString(ofToString(i), spherePoints[i].x, spherePoints[i].y, spherePoints[i].z);
        }
    }
    
    void drawOverlay() {
//        ofDrawBitmapStringHighlight("Eight channel test", 20, ofGetHeight() - 70);
        for (int i = 0; i < volumes.size(); i++) {
            ofDrawBitmapString(ofToString(volumes[i]), 20, ofGetHeight() - 500 + i * 20);
        }
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
					for (int i = 0; i < volumes.size(); i++) volumes[i] = tmpVolumes[i];
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
