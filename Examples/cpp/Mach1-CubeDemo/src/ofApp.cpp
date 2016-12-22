#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
	ofDisableArbTex();
	
	ofSetDataPathRoot("../Resources/");
 
    logo.load("logo.png");
 
    watermark.init("m1mark.png", "b2318ada53073a1eac0b20560718d58e");

	videoPlayer.load("video.mp4");

    //hardcode controller input if available
//    serial.setup("/dev/cu.Mach1-01-DevB", 115200);
    arduinoWatcher = new ArduinoWatcher();
    arduinoWatcher->arduinoFound = [&](std::string address) {
        ofLog() << "arduino found at " << address;
        ofLog() << "waiting for setup to finish...";
        
        arduinoWatcher->stopThread();

        
//        while (!setupFinished) {
//            
//        };
        
        initializedController = true;
        serial.setup(address, 115200);
        while(!serial.isInitialized()) {
            
        }
        
        ofLog() << "serial initialized at " << ofGetElapsedTimef();
        std::string message = std::string("m1heard");
        for (int i = 0; i < message.size(); i++) {
            serial.writeByte(message[i]);
        }
        initializedController = true;
    };
    
 

    tests.push_back(new AudioOne());
	tests.push_back(new IsotropicEightChannelTest());
	tests.push_back(new AbmisonicTest());

	//    tests.push_back(new AudioTwo());

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
    
	// auto play
	//tests[selectedTest]->play();
	//videoPlayer.play();

	// setup for video
	camera.setPosition(0, 0, 0);

	sphereVideo.setRadius(100);
	sphereVideo.setResolution(32);
	sphereVideo.setPosition(0, 0, 0);
	sphereVideo.rotate(-180, 0, 0, 1);

	//	soundStream.printDeviceList();
	//soundStream.setDeviceID(1); 	//note some devices are input only and some are output only 
	soundStream.setup(this, 2, 0, 44100, 512, 1);

    setupFinished = true;
}

//--------------------------------------------------------------
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
    
    // angle handler//
    if (angleZ)
        
        pointLight->setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.15)*(ofGetWidth()*.3), ofGetHeight()/2, 500);
    pointLight2->setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.015)*(ofGetWidth()*.3),
                            ofGetHeight()*.5 + sin(ofGetElapsedTimef()*.07)*(ofGetHeight()), -300);
    
    pointLight3->setPosition(
                            cos(ofGetElapsedTimef()*0.15) * ofGetWidth()*.5,
                            sin(ofGetElapsedTimef()*0.15f) * ofGetWidth()*.5,
                            cos(ofGetElapsedTimef()*.02) * ofGetWidth()
                            );
    
	for (auto &i : tests) {
		i->setOverallVolume((i == tests[selectedTest]));
		i->update();

		i->angleX = angleX;
		i->angleY = angleY;
		i->angleZ = angleZ;
	}
	//ofLog() << " X: " << angleX << " Y: " << angleY << " Z: " << angleZ << endl;

	float pitchAngle = mmap(lastP, 0, 360, 180.0, -180.0, true);
    float pitchAngleClamp = ofClamp(pitchAngle, -90.0, 90.0);
        
    serialAngleUpdate(lastY, pitchAngleClamp);

	videoPlayer.update();
	// loop
	if (videoPlayer.isPlaying() && videoPlayer.getPosition() >= videoPlayer.getDuration() - 0.2)
	{
		videoPlayer.setPosition(0);
	}

	if (fabs(videoPlayer.getPosition() - tests[selectedTest]->getPosition()) > 0.25)
	{
		ofLog() << "sync video and audio: " << videoPlayer.getPosition() << "         ,         " << tests[selectedTest]->getPosition();
		tests[selectedTest]->setPosition(videoPlayer.getPosition());
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
 
   ofBackground(ofColor::black);
   ofEnableAlphaBlending();

	if (ofGetKeyPressed('s'))
	{
		ofEnableDepthTest();
		watermark.draw(); // watermark also inherits ofImage
 	    //ofDrawBitmapStringHighlight("Y : " + ofToString(lastY), 20, 40);
		//ofDrawBitmapStringHighlight("P : " + ofToString(pitchAngle), 20, 60);
		
		float fogColor[3] = { 0.3, 0.3, 0.3 };
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
			ofLine(-100 * lCount, i * 100, 100 * lCount, i * 100);
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
		ofDisableLighting();
		ofDisableDepthTest();

		tests[selectedTest]->drawOverlay();
	}
	else
	{
		// video player
		ofEnableDepthTest();

		matrix.makeIdentityMatrix();
		matrix.rotate(eulerToQuat(ofVec3f(spectatorCam.y, spectatorCam.x, 0)));

		ofVec3f rot = matrix.getRotate().getEuler();
	 	angleX = rot.x;
		angleY = rot.y;
		angleZ = rot.z;
 
		camera.setTransformMatrix(matrix);
		camera.begin();

		ofPushMatrix();

		if(videoPlayer.isTextureAllocated())
		{
			videoPlayer.getTextureReference().bind();
			sphereVideo.draw();
			videoPlayer.getTextureReference().unbind();
		}

		ofPopMatrix();

		camera.end();

		ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 10, 20);
		ofDisableDepthTest();

		// Player controls
		ofSetColor(255);
		ofLine(30, ofGetHeight() - 30, ofGetWidth() - 170 - 60, ofGetHeight() - 30);
		ofFill();
		ofCircle(ofLerp(30, ofGetWidth() - 170 - 60, videoPlayer.getPosition() / videoPlayer.getDuration()), ofGetWidth() - 170 - 60, 20);
		ofCircle(30 + videoPlayer.getPosition() / videoPlayer.getDuration() * (ofGetWidth() - 200 - 60), ofGetHeight() - 30, 10);
	}
    //    glDisable(GL_FOG);
    
    
    /////////////////////////////////////////////
    
    // UI
    
    ofSetColor(255);
    //logo.draw(0, 0, 75, 80);
    
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
    
    
    gui.begin();
    
    bool aWindow;
    
    ImGui::SetNextWindowPos(ImVec2(ofGetWidth() - SETTINGS_TOOLBAR_WIDTH, 0));
    ImGui::SetNextWindowSize(ImVec2(SETTINGS_TOOLBAR_WIDTH, ofGetHeight()));
    ImGui::Begin("Mach1 Spatial Audio", &aWindow, window_flags);
    
    ImGui::Text("Select source");
    const char* source_options[] = {"M1Spatial-Periphonic", "M1Spatial-Isotropic" , "Ambisonic" };
    
    ImGui::PushItemWidth(-1);
	if (ImGui::ListBox("##", &selectedTest, source_options, 3, 3))
	{
		for (int i = 0; i < tests.size(); i++)
		{
			if (i != selectedTest) tests[i]->pause();
		}
		 
		tests[selectedTest]->play();
		//tests[selectedTest]->setPosition( videoPlayer.getPosition());
	}
    
    ImGui::Text("Angles:");
    bool angleChanged = false;
    angleChanged |= (ImGui::SliderFloat("", &angleY, 0, 360, "Y / Yaw: %.0f deg"));
    angleChanged |= (!ImGui::SliderFloat("X / Pitch", &angleX, -90, 90, "X / Pitch: %.0f deg"));
    angleChanged |= (ImGui::SliderFloat("Z / Roll", &angleZ, -90, 90, "Z / Roll: %.0f deg"));
    if (angleChanged) {
        simulationAngles = ofPoint(angleX, angleY, angleZ);
    }
    
    
    ImGui::End();
    
    gui.end();
}


//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
	if(videoPlayer.isPlaying())
	{
		tests[selectedTest]->audioOut(output, bufferSize, nChannels);
	}
}

//--------------------------------------------------------------
void ofApp::serialAngleUpdate(float serialY, float serialP){
    angleY = mmap(serialY, 0., 360., 0., 360., true);
    angleX = serialP;
    updateSimulationAngles();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    //tests[selectedTest]->keyPressed(key);

	if (key == ' ') {
		if (videoPlayer.isPlaying())
		{
			videoPlayer.setPaused(true);
			
			tests[selectedTest]->pause();
		}
		else
		{
			videoPlayer.setPosition(0);
			videoPlayer.setPaused(false);

			videoPlayer.play();
			tests[selectedTest]->play();
		}

	 

	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    ofPoint delta = ofPoint(x - dragStart.x, y - dragStart.y) / 400.;
    
    if (x > (ofGetWidth() - SETTINGS_TOOLBAR_WIDTH)) return;
    
    if (dragginCamera) {
		spectatorCam = spectatorCamStart + delta; 
        //    } else {
        //        angleY = ofClamp(delta.x * 500 + anglesDragStart.y, 0, 360);
        //        angleX = ofClamp(delta.y * 500 + anglesDragStart.x, -90, 90);
        //        updateSimulationAngles();
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    

	// Video Player controls
	if ((x > 30 && (x < (ofGetWidth() - 200 - 60))) && (y >(ofGetHeight() - 70))) {
		float pos = (((float)x - 30) / (ofGetWidth() - 200 - 60));

		videoPlayer.setPosition(videoPlayer.getDuration() * pos);
		//tests[selectedTest]->setPosition(videoPlayer.getDuration() * pos);
	}
	
	
	    if (x < (ofGetWidth() - SETTINGS_TOOLBAR_WIDTH)) {
            dragginCamera = true;
            dragStart = ofPoint(x, y);
            spectatorCamStart = spectatorCam;
        } else dragginCamera = false;
     
	/*else {
        if (x < (ofGetWidth() - SETTINGS_TOOLBAR_WIDTH)) {
            dragginCamera = false;
            dragStart = ofPoint(x, y);
            anglesDragStart = ofVec3f(angleX, angleY, angleZ);
        }
    }
    */
    //
    
    //tests[selectedTest]->mousePressed(x, y);
    
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
