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
    }
    
    void update() {
        
        mach1Decode.setFilterSpeed(speed);

        // Handling audio
        if (!perSample) {
            std::vector<float> tmpVolumes = audioMixAlgorithm(angleX, angleY, angleZ);
            
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
    
    std::vector<float> audioMixAlgorithm(float X, float Y, float Z) {
        return mach1Decode.spatialAlgo(X, Y, Z);
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
                    sample += players[j].getRawSamples()[pos] * volumes[j * 2];
                }
                output[i*nChannels + 1] = sample / 8 * overallVolume;
                
                sample = 0;
                for (int j = 0; j < 8; j++) {
                    sample += players[j].getRawSamples()[pos]  *volumes[j * 2 + 1];
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
