#include "BaseAudioTest.h"
#include "ofxAudioDecoder.h"
#include "AmbiX.h"

class AbmisonicTest: public BaseAudioTest {
	ofxAudioDecoder audio;
	float overallVolume = 0;
	long int pos;

public:
    AbmisonicTest() {
		audio.load("ACNSN3D.wav");
		pos = 0;
    }
    
	void update() {
		// Handling audio
	}

	ofQuaternion eulerToQuat(const ofVec3f & rotationEuler) {
		ofQuaternion rotation;
		float c1 = cos(rotationEuler[2] * 0.5);
		float c2 = cos(rotationEuler[1] * 0.5);
		float c3 = cos(rotationEuler[0] * 0.5);
		float s1 = sin(rotationEuler[2] * 0.5);
		float s2 = sin(rotationEuler[1] * 0.5);
		float s3 = sin(rotationEuler[0] * 0.5);

		rotation[0] = c1*c2*s3 - s1*s2*c3;
		rotation[1] = c1*s2*c3 + s1*c2*s3;
		rotation[2] = s1*c2*c3 - c1*s2*s3;
		rotation[3] = c1*c2*c3 + s1*s2*s3;

		return rotation;
	}

	void audioOut(float * output, int bufferSize, int nChannels)
	{
		ofMatrix4x4 matrix;
		matrix.rotate(eulerToQuat(ofVec3f(angleX, angleY, angleZ)));

		const vector<float>& rawSamples = audio.getRawSamples();
		int channels = audio.getNumChannels();

		vector<float> dest(bufferSize * nChannels);

		interleavedAmbiXBufferSpatialRender(matrix, channels, 0, bufferSize * channels * sizeof(float), (float*)&rawSamples[pos], &dest[0]);
		pos += bufferSize * channels;
		for (int i = 0; i < bufferSize; i++)
		{
			output[i*nChannels] = dest[i*nChannels] * overallVolume;
			output[i*nChannels + 1] = dest[i*nChannels + 1] * overallVolume;
		}

		/*for (int i = 0; i < bufferSize && pos < audio.getNumSamples(); i++, pos += channels)
		{
		output[i*nChannels] = output[i*nChannels + 1] = *((float*)&rawSamples[pos]);
		}*/

		//if (pos >= audio.getNumSamples()) pos = 0;

	}

    void draw() {
	}
    
    void drawOverlay() {
        
    }

    void setOverallVolume(float volume) {
        overallVolume = volume;

    }
      
    std::vector<float> audioMixAlgorithm(float X, float Y, float Z) {
        return std::vector<float>();
    }
    
    virtual void mousePressed(int x, int y) {
        
    };

	void restart() {
		pos = 0;
	}
    
    void keyPressed(int key) {
        if (key == ' ') {
             restart();
        }
    }

};