#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	framerate = 60;

	ofSetFrameRate(framerate);
	ofSetVerticalSync(false);

    logo.load("logo.png");
    
    tests.push_back(new SpatialTest());
    tests.push_back(new SpatialTest2());
    
	anglePitch = 0;
	angleYaw = 0;
	angleRoll = 0;
	updateSimulationAngles();
    
    
    // Visualising setup
    
    sphere.setRadius( ofGetWidth() / 20 );

    ofSetSmoothLighting(true);
    pointLight.setDiffuseColor( ofFloatColor(.85, .85, .55) );
    pointLight.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f));
    
    pointLight2.setDiffuseColor( ofFloatColor( 238.f/255.f, 57.f/255.f, 135.f/255.f ));
    pointLight2.setSpecularColor(ofFloatColor(.8f, .8f, .9f));
    
    pointLight3.setDiffuseColor( ofFloatColor(19.f/255.f,94.f/255.f,77.f/255.f) );
    pointLight3.setSpecularColor( ofFloatColor(18.f/255.f,150.f/255.f,135.f/255.f) );
    
    // shininess is a value between 0 - 128, 128 being the most shiny //
    material.setShininess( 0 );
    // the light highlight of the material //
    material.setSpecularColor(ofColor(255, 255, 255, 255));

	// gui
	gui.setup();

	spectatorCam = ofVec3f(-0.25, 0.5, 0.0);
	  
	//soundStream.printDeviceList();
	//soundStream.setDeviceID(1); 	//note some devices are input only and some are output only
    
    //CHANGE SECOND TO LAST ARG FOR BUFFER SIZE TESTING
	soundStream.setup(this, 2, 0, 44100, 512, 1);
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
	if (tests[selectedTest]->isPlaying)
	{
		tests[selectedTest]->audioOut(output, bufferSize, nChannels);
	}
}

//--------------------------------------------------------------
void ofApp::updateSimulationAngles() {
    simulationAngles = ofPoint(angleYaw, anglePitch, angleRoll);
}

//--------------------------------------------------------------
void ofApp::update(){
    pointLight.setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.15)*(ofGetWidth()*.3), ofGetHeight()/2, 500);
    pointLight2.setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.015)*(ofGetWidth()*.3),
                            ofGetHeight()*.5 + sin(ofGetElapsedTimef()*.07)*(ofGetHeight()), -300);
    
    pointLight3.setPosition(
                            cos(ofGetElapsedTimef()*0.15) * ofGetWidth()*.5,
                            sin(ofGetElapsedTimef()*0.15f) * ofGetWidth()*.5,
                            cos(ofGetElapsedTimef()*.02) * ofGetWidth()
                            );

    for (auto &i : tests) {
        i->setOverallVolume( (i == tests[selectedTest]) );
        i->update();
        
        i->angleYaw = angleYaw;
        i->anglePitch = anglePitch;
        i->angleRoll = angleRoll;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    float time2 = ofGetElapsedTimef();
    
    ofEnableDepthTest();
    
    ofBackground(0);
    
    float arg = (float)ofGetMouseX() / (float)ofGetWidth();
    float time = ofGetElapsedTimef() * 12;
    
    float fogColor[3] = {0.3, 0.3, 0.3};
    glFogfv(GL_FOG_COLOR, fogColor);
    
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
    
        ofPoint faceVector1 = ofPoint(cos(ofDegToRad(simulationAngles[0])),
                                      sin(ofDegToRad(simulationAngles[0]))).normalize();
    
    
        ofPoint faceVector2 = faceVector1.rotate(anglePitch, ofPoint(cos(ofDegToRad(simulationAngles[0] - 90)),
                                                             sin(ofDegToRad(simulationAngles[0] - 90))).normalize());
    
    
        ofPoint faceVectorOffsetted = ofPoint(cos(ofDegToRad(simulationAngles[0])),
                                              sin(ofDegToRad(simulationAngles[0]))).normalize().rotate(
                                                  anglePitch + 10,
                                                  ofPoint(cos(ofDegToRad(simulationAngles[0] - 90)),
                                                          sin(ofDegToRad(simulationAngles[0] - 90))).normalize()) - faceVector2;
    
        ofPoint tiltSphereRotated = faceVectorOffsetted.rotate(-angleRoll, faceVector2);
    
        ofPushMatrix();
        ofTranslate(0, 0, 100);
    
        drawSphere(faceVector2.x * 150, faceVector2.y * 150, faceVector2.z * 150, 4, true);
    
        ofSetColor(60, 255, 0, 75); // Tilt sphere
        drawSphere((faceVector2.x + tiltSphereRotated.x) * 150,
                   (faceVector2.y + tiltSphereRotated.y) * 150,
                   (faceVector2.z + tiltSphereRotated.z) * 150, 2, true);
    
			ofSetColor(255, 0, 0);
			ofDrawLine(faceVector2.x * 50, faceVector2.y * 50, faceVector2.z * 50,
				faceVector2.x * 120, faceVector2.y * 120, faceVector2.z * 120);
	
			// current angles vector (blue)
            {
                ofPoint faceVector1_ = ofPoint(cos(ofDegToRad(tests[selectedTest]->mach1Decode.getCurrentAngle().x)),
                    sin(ofDegToRad(tests[selectedTest]->mach1Decode.getCurrentAngle().x))).normalize();

                ofPoint faceVector2_ = faceVector1_.rotate(tests[selectedTest]->mach1Decode.getCurrentAngle().y * -1.0f, ofPoint(cos(ofDegToRad(tests[selectedTest]->mach1Decode.getCurrentAngle().x - 90)),
                    sin(ofDegToRad(tests[selectedTest]->mach1Decode.getCurrentAngle().x - 90))).normalize());

                ofSetColor(0, 0, 255);
                ofDrawLine(faceVector2_.x * 50, faceVector2_.y * 50, faceVector2_.z * 50,
                    faceVector2_.x * 120, faceVector2_.y * 120, faceVector2_.z * 120);
            }

        // Drawing spectator and arrow
        ofEnableLighting();
        pointLight.enable();
        pointLight2.enable();
        pointLight3.enable();

        material.begin();
            ofSetColor(255);
            drawSphere(0, 0, 0, 32, true);
        material.end();
    
        ofPopMatrix();
    
        tests[selectedTest]->draw();
    
    ofPopMatrix();
    
    ofDisableDepthTest();
    ofDisableLighting();
    
    /////////////////////////////////////////////
    
    // UI
    ofSetColor(255);
    if(logo.isAllocated()) logo.draw(0, 0, 75, 80);

	// playhead
	{
		ofSetLineWidth(2);
		int border = 20;
		ofDrawLine(10, ofGetHeight() - border, ofGetWidth() - border, ofGetHeight() - border);
		float position = tests[selectedTest]->getPlayheadPosition() * (ofGetWidth() - 2 * border);
		ofDrawCircle(10 + position, ofGetHeight() - border, 10);
		ofSetLineWidth(1);
	}

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
    
    bool *x;
    
    gui.begin();
    
    bool aWindow;
    
    ImGui::SetNextWindowPos(ImVec2(ofGetWidth() - SETTINGS_TOOLBAR_WIDTH, 0));
    ImGui::SetNextWindowSize(ImVec2(SETTINGS_TOOLBAR_WIDTH, ofGetHeight()));
    ImGui::Begin("M1 SDK Tests", &aWindow, window_flags);
    
		ImGui::SliderFloat("Angle Speed", &tests[selectedTest]->speed, 0.05, 0.95);

		if (ImGui::SliderInt("Framerate", &framerate, 10, 120)) {
			ofSetFrameRate(framerate);
		}
		int bufferSize = soundStream.getBufferSize();
//        if (ImGui::SliderInt("BufferSize", &bufferSize, 32, 1024)) {
        if (ImGui::SliderInt("BufferSize", &bufferSize, 512, 512)) {
			soundStream.setup(this, soundStream.getNumOutputChannels(), 0, soundStream.getSampleRate(), bufferSize, 1);
		}
		ImGui::Checkbox("Per Sample", &tests[selectedTest]->perSample);

		ImGui::NewLine();

        ImGui::Text("Spatial Debug Env");
        const char* source_options[] = { "SpatialTest", "SpatialTest2" };
        
        ImGui::PushItemWidth(-1);
        ImGui::ListBox("##", &selectedTest, source_options, 2, 2);

        ImGui::Text("Angles:");
        bool angleChanged = false;
        angleChanged |= (ImGui::SliderFloat("Y / Yaw", &angleYaw, 0, 360, "Y / Yaw: %.0f deg"));
        angleChanged |= (ImGui::SliderFloat("X / Pitch", &anglePitch, -90, 90, "X / Pitch: %.0f deg"));
        angleChanged |= (ImGui::SliderFloat("Z / Roll", &angleRoll, -90, 90, "Z / Roll: %.0f deg"));

        ImGui::LabelText("currentYaw",("currentYaw: " + ofToString(tests[selectedTest]->mach1Decode.getCurrentAngle().x, 3)).c_str() );
        ImGui::LabelText("currentPitch",("currentPitch: " + ofToString(tests[selectedTest]->mach1Decode.getCurrentAngle().y, 3)).c_str() );
        ImGui::LabelText("currentRoll",("currentRoll: " + ofToString(tests[selectedTest]->mach1Decode.getCurrentAngle().z, 3)).c_str() );

        if (angleChanged) {
                simulationAngles = ofPoint(angleYaw, anglePitch, angleRoll);
        }
    
    ImGui::End();
    
    gui.end();
    
    //
    
    tests[selectedTest]->drawOverlay();

	ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, 20);

	if(!tests[selectedTest]->isPlaying) ofDrawBitmapString("Press space to play", ofGetWidth()/2, ofGetHeight()/2);

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
        spectatorCam.x = ofWrap(spectatorCamStart.x + delta.x, 0., 1.);
        spectatorCam.y = ofClamp(spectatorCamStart.y + delta.y, 0., 1.);
    } else {
        angleYaw = ofClamp(delta.x * 500 + anglesDragStart.x, 0, 360);
        anglePitch = ofClamp(delta.y * 500 + anglesDragStart.y, 0, 180);
        updateSimulationAngles();
    }

	ofLog() << delta << endl;
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
            anglesDragStart = ofVec3f(angleYaw, anglePitch, angleRoll);
        }
    }
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
