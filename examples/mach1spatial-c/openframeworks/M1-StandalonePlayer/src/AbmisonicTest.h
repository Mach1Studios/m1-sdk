#include "BaseAudioTest.h"
#include "ofxAudioDecoder.h"
#include "AmbiX.h"

class AbmisonicTest : public BaseAudioTest {
	float overallVolume = 0;

	ofMatrix4x4 imatrix;

	ofxAudioDecoder audio;
	long int pos;

	bool isPlay;

public:
	AbmisonicTest() {
        //load 4 channel ACNSN3D audio file
		audio.load("2/sound.wav");
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
		if (isPlay)
		{
			ofMatrix4x4 matrix;
		 	matrix.rotate(eulerToQuat(ofVec3f(ofDegToRad(angleX), ofDegToRad(angleY), ofDegToRad(angleZ))));

			ofMatrix4x4 imatrixPrev = imatrix;
			imatrix = matrix.getInverse();
			imatrix.rotate(0, 0, 1, 0); // rotate matrix from Stanley to Ambisonic

			const vector<float>& rawSamples = audio.getRawSamples();
			int channels = audio.getNumChannels();

			vector<float> destPrev(bufferSize * nChannels);
			vector<float> dest(bufferSize * nChannels);
			interleavedAmbiXBufferSpatialRender(imatrixPrev, channels, 0, bufferSize * channels * sizeof(float), (float*)&rawSamples[pos], &destPrev[0]);
			interleavedAmbiXBufferSpatialRender(imatrix, channels, 0, bufferSize * channels * sizeof(float), (float*)&rawSamples[pos], &dest[0]);
			pos += bufferSize * channels;
			for (int i = 0; i < bufferSize; i++)
			{
				output[i*nChannels + 1] = ofLerp(destPrev[i*nChannels], dest[i*nChannels], 1.0 * i / bufferSize) * overallVolume * 0.25;
				output[i*nChannels] = ofLerp(destPrev[i*nChannels + 1], dest[i*nChannels + 1], 1.0 * i / bufferSize) * overallVolume * 0.25;
			}

			/*for (int i = 0; i < bufferSize && pos < audio.getNumSamples(); i++, pos += channels)
			{
			output[i*nChannels] = output[i*nChannels + 1] = *((float*)&rawSamples[pos]);
			}*/

			//if (pos >= audio.getNumSamples()) pos = 0;
		}

	}

	void setPosition(float seconds)
	{
		pos = audio.getNumChannels() * (int)(seconds * audio.getSampleRate());
	}

	float getPosition()
	{
		return 1.0 * pos / (audio.getNumChannels() * audio.getSampleRate());
	}


	void draw() {
	}

	void drawOverlay() {

	}

	void play()
	{
		isPlay = true;
	}

	void pause()
	{
		isPlay = false;
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
