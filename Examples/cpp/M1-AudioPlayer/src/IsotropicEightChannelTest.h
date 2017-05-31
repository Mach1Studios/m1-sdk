#include "BaseAudioTest.h"

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
        
        volumes.resize(8);
        
        for (int i = 0; i < 8; i++){
            playersLeft[0].load("1/1.wav"); playersLeft[0].setLoop(true);
            playersLeft[1].load("1/2.wav"); playersLeft[1].setLoop(true);
            playersLeft[2].load("1/3.wav"); playersLeft[2].setLoop(true);
            playersLeft[3].load("1/4.wav"); playersLeft[3].setLoop(true);
            playersLeft[4].load("1/5.wav"); playersLeft[4].setLoop(true);
            playersLeft[5].load("1/6.wav"); playersLeft[5].setLoop(true);
            playersLeft[6].load("1/7.wav"); playersLeft[6].setLoop(true);
            playersLeft[7].load("1/8.wav"); playersLeft[7].setLoop(true);
            playersLeft[i].setPan(-1);
            playersRight[0].load("1/1.wav"); playersRight[0].setLoop(true);
            playersRight[1].load("1/2.wav"); playersRight[1].setLoop(true);
            playersRight[2].load("1/3.wav"); playersRight[2].setLoop(true);
            playersRight[3].load("1/4.wav"); playersRight[3].setLoop(true);
            playersRight[4].load("1/5.wav"); playersRight[4].setLoop(true);
            playersRight[5].load("1/6.wav"); playersRight[5].setLoop(true);
            playersRight[6].load("1/7.wav"); playersRight[6].setLoop(true);
            playersRight[7].load("1/8.wav"); playersRight[7].setLoop(true);
            playersRight[i].setPan(1);
        }
    }
    
    void update() {
        // Handling audio
        
        volumes = audioMixAlgorithm(angleY, -angleX, angleZ);
        
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
        
        
        /*
         
        // Isotropic math test
        
        ofPoint simulationAngles = ofPoint(angleX, angleY, angleZ);

        ofPoint faceVector1 = ofPoint(cos(ofDegToRad(simulationAngles[1])),
                                      sin(ofDegToRad(simulationAngles[1]))).normalize();
        
        
        ofPoint faceVector2 = faceVector1.getRotated(angleX,
                        ofPoint(cos(ofDegToRad(simulationAngles[1] - 90)),
                                sin(ofDegToRad(simulationAngles[1] - 90))).normalize());
        

        ofPoint faceVector21 = faceVector1.getRotated(angleX + 90,
                                ofPoint(cos(ofDegToRad(simulationAngles[1] - 90)),
                                        sin(ofDegToRad(simulationAngles[1] - 90))).normalize());
        
        ofPoint faceVectorLeft = faceVector21.getRotated(-angleZ - 90, faceVector2);
        ofPoint faceVectorRight = faceVector21.getRotated(-angleZ + 90, faceVector2);
        
        
        float time = ofGetElapsedTimef() * 5;
        
        ofPoint faceVectorOffsetted = ofPoint(cos(ofDegToRad(simulationAngles[1])),
                                              sin(ofDegToRad(simulationAngles[1]))).normalize().rotate(
                            angleX + 10,
                    ofPoint(cos(ofDegToRad(simulationAngles[1] - 90)),
                            sin(ofDegToRad(simulationAngles[1] - 90))).normalize()) - faceVector2;
        
        ofPoint tiltSphereRotated = faceVectorOffsetted.rotate(-angleZ, faceVector2);
        //        ofPoint facePoint = faceVector2 * 120;
        
        
        ofSetColor(255, 0, 255);
        ofDrawSphere(faceVector2 * 100, 10);
        ofSetColor(0, 255, 255);
        ofDrawSphere(faceVectorLeft * 100 + faceVector2 * 100, 5);
        ofDrawSphere(faceVectorRight * 100 + faceVector2 * 100, 5);
        
        ofSetColor(255, 0, 0);

        
        ofSetColor(255, 0, 0);
        ofDrawSphere(0, 0, 5);
        
        // Drawing another 8 dots
        
        ofVec3f points[8] =
                {ofPoint(100, -100, -100),
                 ofPoint(100, 100, -100),
                    ofPoint(-100, -100, -100),
                 ofPoint(-100, 100, -100),
                 
                 ofPoint(100, -100, 100),
                 ofPoint(100, 100, 100),
                    ofPoint(-100, -100, 100),
                 ofPoint(-100, 100, 100)
                 
                 };
        
        float qL[8];
        for (int i = 0; i < 8; i++) {
            qL[i] = (faceVectorLeft * 100 + faceVector2 * 100 - points[i]).length();
        }
        
        float qR[8];
        for (int i = 0; i < 8; i++) {
            qR[i] = (faceVectorRight * 100 + faceVector2 * 100 - points[i]).length();
        }
        
        
        
        for (int i = 0; i < 8; i++) {
            ofSetColor(255);
            ofDrawSphere(points[i], 3);
            ofSetColor(255, 0, 0);
            float v = ofClamp(ofMap(qL[i] + qR[i], 250, 400, 1., 0.), 0, 1);
            ofDrawBitmapString(ofToString(v), points[i].x, points[i].y, points[i].z);
            ofDrawBitmapString(ofToString(i), points[i].x, points[i].y, points[i].z);
        }
        
        
        */

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
        
        // Player controls
        ofSetColor(255);
        ofLine(30, ofGetHeight() - 30, ofGetWidth() - 170 - 60, ofGetHeight() - 30);
        ofFill();
        ofCircle(ofLerp(30, ofGetWidth() - 170 - 60, playersLeft[0].getPosition()), ofGetWidth() - 170 - 60 , 20);
        
        ofCircle(30 + playersLeft[0].getPosition() * (ofGetWidth() - 200 - 60), ofGetHeight() - 30, 10);
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
                playersLeft[i].setPosition((((float)x - 30) / (ofGetWidth() - 200 - 60)));
                playersRight[i].setPosition((((float)x - 30) / (ofGetWidth() - 200 - 60)));
            }
        }
    };

    
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
