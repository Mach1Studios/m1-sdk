#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetDataPathRoot("../Resources/");
    
    logo.load("logo.png");
    
    //hardcode controller input if available
//    serial.setup("/dev/cu.Mach1-04-DevB", 115200);
    
    arduinoWatcher = new ArduinoWatcher();
    arduinoWatcher->start();
    arduinoWatcher->arduinoFound = [&](std::string address) {
        ofLog() << "arduino found at " << address;
        initializedController = true;
        
//        arduinoWatcher->stopThread();

        ArduinoDecoderYP *decoder = new ArduinoDecoderYP();
        
        decoder->setup(address, 115200);
        while(!decoder->isInitialized()) {
            
        }
        
        ofLog() << "serial initialized";
        std::string message = std::string("m1heard");
        for (int i = 0; i < message.size(); i++) {
            decoder->writeByte(message[i]);
        }
        initializedController = true;
        
        decoder->gotNewValues = [&](float Y, float P, float R) {
            float pitchAngle = mmap(P, 0, 360, -90.0, 90.0, true);
            float pitchAngleClampFix = pitchAngle;
//            yAngleRad = ofDegToRad(Y);
//            pAngleRad = ofDegToRad(pitchAngleClampFix);
//            rAngleRad = ofDegToRad(R);
//            ^ this is a proper naming, but we have to refactor it to
            // to work that way
            
            ofLog() << "Y: "<< Y << " ; P: " << pitchAngleClampFix << " ; R: " << R;
            
            angleX = -pitchAngleClampFix;
            angleY = Y;
            angleZ = R;
            
        };
        
        arduinoDecoders.push_back(decoder);

    };
    
//use this for Mach1Spatial
    tests.push_back(new SpatialOne());
    tests.push_back(new SpatialTwo());
//use this for Mach1HorizonPairs
    //tests.push_back(new HorizonPairsOne());
    //tests.push_back(new HorizonPairsTwo());
    
    angleX = 0;
    updateSimulationAngles();
    
    // Visualising setup
    
    sphere.setRadius( ofGetWidth() / 20 );
    
    pointLight = new ofLight();
    ofSetSmoothLighting(true);
    pointLight->setDiffuseColor( ofFloatColor(.85, .85, .55) );
    pointLight->setSpecularColor( ofFloatColor(1.f, 1.f, 1.f));
    
    pointLight2 = new ofLight();
    pointLight2->setDiffuseColor( ofFloatColor( 238.f/255.f, 57.f/255.f, 135.f/255.f ));
    pointLight2->setSpecularColor(ofFloatColor(.8f, .8f, .9f));
    
    pointLight3 = new ofLight();
    pointLight3->setDiffuseColor( ofFloatColor(19.f/255.f,94.f/255.f,77.f/255.f) );
    pointLight3->setSpecularColor( ofFloatColor(18.f/255.f,150.f/255.f,135.f/255.f) );
    
    // shininess is a value between 0 - 128, 128 being the most shiny //
    material.setShininess( 120 );
    // the light highlight of the material //
    material.setSpecularColor(ofColor(255, 255, 255, 255));
    
    setupFinished = true;
}

//--------------------------------------------------------------
void ofApp::updateSimulationAngles() {
    simulationAngles = ofPoint(angleX, angleY, angleZ);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if (initializedController)
    if (serial.isInitialized())
    if (serial.available()) {
        if (queueBuffer.size() > 0) {
            queueBuffer.clear();
        }
        vector<unsigned char> inputs;
        while (serial.available() > 0) {
            unsigned char i = serial.readByte();
            inputs.push_back(i);
        }
        for (int i = 0; i < inputs.size(); i++) {
            queueBuffer.insert(queueBuffer.begin() + i, (int)inputs[i]);
        }
    }
    
    vector<unsigned char> queueBackupDebug = queueBuffer;
    while (queueBuffer.size() > 4) {
        
        int Y, P;
        
        if (getNewDataFromQueue(Y, P)) {
            lastY = Y;
            lastP = P;
        }
    }
    
    if (initializedController) {
        
        for (auto &i:arduinoDecoders)
            i->update();
    }
    
    // angle handler//
//    if (angleZ)
    
        pointLight->setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.15)*(ofGetWidth()*.3), ofGetHeight()/2, 500);
    pointLight2->setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.015)*(ofGetWidth()*.3),
                            ofGetHeight()*.5 + sin(ofGetElapsedTimef()*.07)*(ofGetHeight()), -300);
    
    pointLight3->setPosition(
                            cos(ofGetElapsedTimef()*0.15) * ofGetWidth()*.5,
                            sin(ofGetElapsedTimef()*0.15f) * ofGetWidth()*.5,
                            cos(ofGetElapsedTimef()*.02) * ofGetWidth()
                            );
    
    for (auto &i : tests) {
        i->setOverallVolume( (i == tests[selectedTest]) );
        i->update();
        
        i->angleX = angleX;
        i->angleY = angleY;
        i->angleZ = angleZ;
        
        float pitchAngle = mmap(lastP, 0, 360, 180.0, -180.0, true);
        float pitchAngleClamp = ofClamp(pitchAngle, -90.0, 90.0);
        
        serialAngleUpdate(lastY, pitchAngleClamp);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofEnableDepthTest();
    
    ofBackground(0);
    
    //ofDrawBitmapStringHighlight("Y : " + ofToString(lastY), 20, 40);
    //ofDrawBitmapStringHighlight("P : " + ofToString(pitchAngle), 20, 60);
    
    float arg = (float)ofGetMouseX() / (float)ofGetWidth();
    float time = ofGetElapsedTimef() * 12;
    
    float fogColor[3] = {0.3, 0.3, 0.3};
    glFogfv(GL_FOG_COLOR, fogColor);
    
    //    glEnable(GL_FOG);
    
    ofPushMatrix();
    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
    ofRotate(90 * spectatorCam.y, 1, 0, 0);
    ofRotate(360 * spectatorCam.x, 0, 0, 1);
    
    // Drawing grid
    
    ofSetColor(100);
    ofSetLineWidth(1);
    int lCount = 80;
    for (int i = -lCount; i < lCount; i++) {
        ofLine(i * 100, -100 * lCount, i * 100, 100 * lCount);
        ofLine( -100 * lCount, i * 100, 100 * lCount, i * 100);
    }
    
    // Drawing axis
    
    
    ofSetColor(0, 255, 0, 75);
    
    ofPoint faceVector1 = ofPoint(cos(ofDegToRad(simulationAngles[1])),
                                  sin(ofDegToRad(simulationAngles[1]))).normalize();
    ofPoint faceVector2 = faceVector1.rotate(angleX, ofPoint(cos(ofDegToRad(simulationAngles[1] - 90)),
                                                             sin(ofDegToRad(simulationAngles[1] - 90))).normalize());
    ofPoint faceVectorOffsetted = ofPoint(cos(ofDegToRad(simulationAngles[1])),
                                          sin(ofDegToRad(simulationAngles[1]))).normalize().rotate(angleX + 10, ofPoint(cos(ofDegToRad(simulationAngles[1] - 90)),
                                                                                                                        sin(ofDegToRad(simulationAngles[1] - 90))).normalize()) - faceVector2;

    ofPoint tiltSphereRotated = faceVectorOffsetted.rotate(-angleZ, faceVector2);
    //        ofPoint facePoint = faceVector2 * 120;
    
    ofPushMatrix();
    ofTranslate(0, 0, 100);
    
    drawSphere(faceVector2.x * 150, faceVector2.y * 150, faceVector2.z * 150, 4, true);
    
    ofSetColor(60, 255, 0, 75); // Tilt sphere
    drawSphere((faceVector2.x + tiltSphereRotated.x) * 150,
               (faceVector2.y + tiltSphereRotated.y) * 150,
               (faceVector2.z + tiltSphereRotated.z) * 150, 2, true);
    
    //        ofPoint tiltVector = faceVector2.perpendicular()
    
    ofSetColor(255, 0, 0);
    ofDrawLine(faceVector2.x * 50, faceVector2.y * 50, faceVector2.z * 50,
               faceVector2.x * 120, faceVector2.y * 120, faceVector2.z * 120);
    
    
    // Drawing spectator and arrow
    
    ofEnableLighting();
    pointLight->enable();
    pointLight2->enable();
    pointLight3->enable();
    
    material.begin();
    
    ofSetColor(255);
    drawSphere(0, 0, 0, 32, true);
    
    material.end();
    
    ofPopMatrix();
    
    tests[selectedTest]->draw();
    
    
    ofPopMatrix();
    
    //    glDisable(GL_FOG);
    
    ofDisableDepthTest();
    ofDisableLighting();
    
    /////////////////////////////////////////////
    
    // UI
    
    ofSetColor(255);
    //logo.draw(0, 0, 75, 80);
    
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
    
    
    bool *x;
    
    gui.begin();
    
    bool aWindow;
    
    ImGui::SetNextWindowPos(ImVec2(ofGetWidth() - SETTINGS_TOOLBAR_WIDTH, 0));
    ImGui::SetNextWindowSize(ImVec2(SETTINGS_TOOLBAR_WIDTH, ofGetHeight()));
    ImGui::Begin("Mach1 Spatial Audio", &aWindow, window_flags);
    
    ImGui::Text("Select source");
//Mach1Spatial
    const char* source_options[] = {"M1Spatial-SpatialAlgo", "M1Spatial-SpatialAlgo2"};
//Mach1HorizonPairs
//    const char* source_options[] = {"M1Spatial-HorizonPairsAlgo", "M1Spatial-HorizonPairsAlgo2"};
    
    ImGui::PushItemWidth(-1);
    ImGui::ListBox("##", &selectedTest, source_options, 2, 2);
    
    ImGui::Text("Angles:");
    bool angleChanged = false;
    angleChanged += (ImGui::SliderFloat("", &angleY, 0, 360, "Y / Yaw: %.0f deg"));
    angleChanged += (!ImGui::SliderFloat("X / Pitch", &angleX, -90, 90, "X / Pitch: %.0f deg"));
    angleChanged += (ImGui::SliderFloat("Z / Roll", &angleZ, -90, 90, "Z / Roll: %.0f deg"));
    if (angleChanged) {
        simulationAngles = ofPoint(angleX, angleY, angleZ);
    }
    
    
    ImGui::End();
    
    gui.end();
    
    
    //
    
    tests[selectedTest]->drawOverlay();
    
}

//--------------------------------------------------------------
void ofApp::serialAngleUpdate(float serialY, float serialP){
//    angleY = mmap(serialY, 0., 360., 0., 360., true);
//    angleX = serialP;
    updateSimulationAngles();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    tests[selectedTest]->keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    ofPoint delta = ofPoint(x - dragStart.x, y - dragStart.y) / 500.;
    
    if (x > (ofGetWidth() - SETTINGS_TOOLBAR_WIDTH)) return;
    
    if (dragginCamera) {
        spectatorCam.x = -ofWrap(spectatorCamStart.x + delta.x, 0., 1.);
        spectatorCam.y = ofClamp(spectatorCamStart.y + delta.y, 0., 1.);
        //    } else {
        //        angleY = ofClamp(delta.x * 500 + anglesDragStart.y, 0, 360);
        //        angleX = ofClamp(delta.y * 500 + anglesDragStart.x, -90, 90);
        //        updateSimulationAngles();
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    if ((ofVec3f(ofGetWidth() / 2, ofGetHeight() / 2) - ofVec3f(x, y)).length() > (ofGetWidth() / 10)) {
        if (x < (ofGetWidth() - SETTINGS_TOOLBAR_WIDTH)) {
            dragginCamera = true;
            dragStart = ofPoint(x, y);
            spectatorCamStart = spectatorCam;
        } else dragginCamera = false;
    } else {
        if (x < (ofGetWidth() - SETTINGS_TOOLBAR_WIDTH)) {
            dragginCamera = false;
            dragStart = ofPoint(x, y);
            anglesDragStart = ofVec3f(angleX, angleY, angleZ);
        }
    }
    
    //
    
    tests[selectedTest]->mousePressed(x, y);
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
