#include "BaseAudioTest.h"
#include "ofxAudioDecoder.h"

class AudioOne: public BaseAudioTest {
public:
    AudioOne() {
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
            sounds[0].load("1/1.wav"); 
            sounds[1].load("1/2.wav"); 
            sounds[2].load("1/3.wav"); 
            sounds[3].load("1/4.wav"); 
            sounds[4].load("1/5.wav"); 
            sounds[5].load("1/6.wav"); 
            sounds[6].load("1/7.wav");  
            sounds[7].load("1/8.wav"); 
         }

		pos = 0;
    }
    
    void update() {
         
       
    }

	void audioOut(float * output, int bufferSize, int nChannels)
	{
		// Handling audio

		//angleY = yaw, angleX = pitch
		volumes = audioMixAlgorithm(angleY, angleX, angleZ);

		if (isPlay)
		{
			float sample;

			for (int i = 0; i < bufferSize; i++)
			{
				sample = 0;
				for (int i = 0; i < 8; i++) {
					sample += sounds[i].getRawSamples()[pos] * (volumes[i * 2] * overallVolume);
				}
				output[i*nChannels] = sample / 8;
				
				sample = 0;
				for (int i = 0; i < 8; i++) {
					sample += sounds[i].getRawSamples()[pos] * (volumes[i * 2 + 1] * overallVolume);
				}
				output[i*nChannels+1] = sample / 8;

				pos++;
			}
		}
 
	}

	void setPosition(float percent)
	{
		pos = percent * sounds[0].getNumSamples() / sounds[0].getSampleRate();
	}

	void play()
	{
		isPlay = true;
	}

	void pause()
	{
		isPlay = false;
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
        ofDrawBitmapStringHighlight("Directory: Mach1-CubeDemo/Contents/Resources/1/*.wav", 10, 100);
        ofDrawBitmapStringHighlight("Instructions:", ofGetWidth() - 500, 20);
        ofDrawBitmapStringHighlight("-Press 'spacebar' to play", ofGetWidth() - 500, 40);
        ofDrawBitmapStringHighlight("-Use the Yaw,Pitch,Roll sliders to", ofGetWidth() - 500, 60);
        ofDrawBitmapStringHighlight("simulate different head orientations", ofGetWidth() - 500, 80);
        
      
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
			setPosition(((float)x - 30) / (ofGetWidth() - 200 - 60));
        }
    };
    
    //
    
    ofPoint spherePoints[8];
    std::vector<float> volumes;
    float overallVolume = 0;
    float coefficients[8];
	ofxAudioDecoder sounds[8];
	long int pos;

	bool isPlay;

    //////////////
    
    std::vector<float> audioMixAlgorithm(float X, float Y, float Z) {
        
        return eightChannelsAlgorithm(X, Y, Z);
    }
    
    int scheduleRestart = 30;
    
    void restart() {
		pos = 0;
    }
    
    void keyPressed(int key) {
        if (key == ' ') {
           
        }
    }

};