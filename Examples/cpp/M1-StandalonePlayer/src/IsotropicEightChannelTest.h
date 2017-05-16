#include "BaseAudioTest.h"
#include "ofxAudioDecoder.h"

class IsotropicEightChannelTest: public BaseAudioTest {
public:
    IsotropicEightChannelTest() {
        spherePoints[0] = ofPoint(200, -200, 0);
        spherePoints[1] = ofPoint(200, 200, 0);
        spherePoints[2] = ofPoint(-200, -200, 0);
        spherePoints[3] = ofPoint(-200, 200, 0);

        spherePoints[4] = ofPoint(200, -200, 200);
        spherePoints[5] = ofPoint(200, 200, 200);
        spherePoints[6] = ofPoint(-200, -200, 200);
        spherePoints[7] = ofPoint(-200, 200, 200);
        
        //
        
        volumes.resize(16);
        
		sounds[0].load("1/1.wav");
		sounds[1].load("1/2.wav");
		sounds[2].load("1/3.wav");
		sounds[3].load("1/4.wav");
		sounds[4].load("1/5.wav");
		sounds[5].load("1/6.wav");
		sounds[6].load("1/7.wav");
		sounds[7].load("1/8.wav");

		pos = 0;
    }
    
    void update() {
    }

	void audioOut(float * output, int bufferSize, int nChannels)
	{
		// Handling audio
		vector<float> volumesPrev = volumes;
		volumes = audioMixAlgorithm(angleX, angleY, angleZ);

		if (isPlay)
		{
			float sample;

			for (int i = 0; i < bufferSize; i++)
			{
				sample = 0;
				for (int j = 0; j < 8; j++) {
					sample += sounds[j].getRawSamples()[pos] * ofLerp(volumesPrev[j * 2], volumes[j * 2], 1.0 *  i  / bufferSize);
				}
				output[i*nChannels + 1] = sample / 8 * overallVolume;
				
				sample = 0;
				for (int j = 0; j < 8; j++) {
					sample += sounds[j].getRawSamples()[pos] * ofLerp(volumesPrev[j * 2 + 1], volumes[j * 2 + 1], 1.0 * i / bufferSize);
				}
				output[i*nChannels] = sample / 8 * overallVolume;

				pos++;
			}
		} 

	}

	void setPosition(float seconds)
	{
		pos = sounds[0].getNumChannels() * (int)(seconds * sounds[0].getSampleRate());
	}

	float getPosition()
	{
		return 1.0 * pos / (sounds[0].getNumChannels() * sounds[0].getSampleRate());
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
    }
    
    //
    ofPoint spherePoints[8];
    std::vector<float> volumes;
    float overallVolume = 0;
    float coefficients[8];

	ofxAudioDecoder sounds[8];
	long int pos;

	bool isPlay;

    //////////////
    
    struct mPoint {
        float x, y, z;
        
        mPoint() {
            x = 0;
            y = 0;
            z = 0;
        }
        
        mPoint(float X, float Y, float Z) {
            x = X;
            y = Y;
            z = Z;
        }
        
        mPoint(float X, float Y) {
            x = X;
            y = Y;
            z = 0;
        }
        
        inline mPoint operator+( const mPoint& pnt ) const {
            return mPoint( x+pnt.x, y+pnt.y, z+pnt.z );
        }

        
        inline mPoint operator*( const float f ) const {
            return mPoint( x*f, y*f, z*f );
        }

        
        inline mPoint operator*( const mPoint& vec ) const {
            return mPoint( x*vec.x, y*vec.y, z*vec.z );
        }

        
        inline mPoint operator-( const mPoint& vec ) const {
            return mPoint( x-vec.x, y-vec.y, z-vec.z );
        }
        
        inline float length() const {
            return (float)sqrt( x*x + y*y + z*z );
        }

        
        float operator[] (int index) {
            float arr[3] = {x, y, z};
            return arr[index];
        }
        
        inline mPoint& rotate( float angle, const mPoint& axis ) {
            mPoint ax = axis.getNormalized();
            float a = (float)(angle*DEG_TO_RAD);
            float sina = sin( a );
            float cosa = cos( a );
            float cosb = 1.0f - cosa;
            
            float nx = x*(ax.x*ax.x*cosb + cosa)
            + y*(ax.x*ax.y*cosb - ax.z*sina)
            + z*(ax.x*ax.z*cosb + ax.y*sina);
            float ny = x*(ax.y*ax.x*cosb + ax.z*sina)
            + y*(ax.y*ax.y*cosb + cosa)
            + z*(ax.y*ax.z*cosb - ax.x*sina);
            float nz = x*(ax.z*ax.x*cosb - ax.y*sina)
            + y*(ax.z*ax.y*cosb + ax.x*sina)
            + z*(ax.z*ax.z*cosb + cosa);
            x = nx; y = ny; z = nz;
            return *this;
        }
        
        inline mPoint& normalize() {
            float length = (float)sqrt(x*x + y*y + z*z);
            if( length > 0 ) {
                x /= length;
                y /= length;
                z /= length;
            }
            return *this;
        }

        
        inline mPoint getNormalized() const {
            float length = (float)sqrt(x*x + y*y + z*z);
            if( length > 0 ) {
                return mPoint( x/length, y/length, z/length );
            } else {
                return mPoint();
            }
        }

        
        inline mPoint getRotated( float angle, const mPoint& axis ) const {
            mPoint ax = axis.getNormalized();
            float a = (float)(angle*DEG_TO_RAD);
            float sina = sin( a );
            float cosa = cos( a );
            float cosb = 1.0f - cosa;
            
            return mPoint( x*(ax.x*ax.x*cosb + cosa)
                           + y*(ax.x*ax.y*cosb - ax.z*sina)
                           + z*(ax.x*ax.z*cosb + ax.y*sina),
                           x*(ax.y*ax.x*cosb + ax.z*sina)
                           + y*(ax.y*ax.y*cosb + cosa)
                           + z*(ax.y*ax.z*cosb - ax.x*sina),
                           x*(ax.z*ax.x*cosb - ax.y*sina)
                           + y*(ax.z*ax.y*cosb + ax.x*sina)
                           + z*(ax.z*ax.z*cosb + cosa) );
        }

    };
    
    M1DSPAlgorithms m1dspAlgorithms;
    
    std::vector<float> audioMixAlgorithm(float X, float Y, float Z) {
        return m1dspAlgorithms.eightChannelsIsotropicAlgorithm(X, Y, Z);
    }
    
    virtual void mousePressed(int x, int y) {
        // Player controls
        if ((x > 30 && (x < (ofGetWidth() - 200 - 60)) ) && (y > (ofGetHeight() - 70))) {
            for (int i = 0; i < 8; i++) {
				setPosition(((float)x - 30) / (ofGetWidth() - 200 - 60));
			}
        }
    };

    
    int scheduleRestart = 30;
    
    void restart() {
		pos = 0;
    }
    
    void keyPressed(int key) {
        if (key == ' ') {
             
        }
    }

};
