#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    gui.setup();
    
    camera.setPosition(0, RENDERING_SCALE * 2, -RENDERING_SCALE * 2);
    
    ofFile file;
    if (file.open("lastfile.cfg", ofFile::Mode::ReadOnly, false)) {
        ofLog() << "file found!";
        auto b = file.readToBuffer();
        ofLog() << (std::string)b;
 
		player.load((std::string)b);

		pos = 0;
	}

	m1Decode.setPlatformType(Mach1PlatformOfEasyCam);
    m1Decode.setDecodeAlgoType(Mach1DecodeAlgoSpatial);
	m1Decode.setFilterSpeed(0.95f);

	decoded.resize(18);
	volumes.resize(2);

	//CHANGE SECOND TO LAST ARG FOR BUFFER SIZE TESTING
	soundStream.setup(this, 2, 0, 44100, 512, 1);
}

//--------------------------------------------------------------
void ofApp::update() {
	// update the sound playing system:
	ofSoundUpdate();

}

//--------------------------------------------------------------
void ofApp::draw() {
    
    ofBackgroundGradient(ofColor(0), ofColor(20));
    
    camera.begin();
    
        // Drawing plane
        ofSetColor(80);
        for (int i = -200; i < 400; i++) {
            ofDrawLine(-4000, 0, i * 20, 4000, 0, i * 20);
            ofDrawLine(i * 20, 0, -4000, i * 20, 0, 4000);
        }
    
        // Monitor axis
    
        // Drawing axis
    
    
        ofSetColor(0, 255, 0, 75);
    
        ofPoint faceVector1 = ofPoint(cos(ofDegToRad(decoderRotationY)),
                                      sin(ofDegToRad(decoderRotationY))).normalize();
    
    
        ofPoint faceVector2 = faceVector1.rotate(decoderRotationP, ofPoint(cos(ofDegToRad(decoderRotationY - 90)),
                        sin(ofDegToRad(decoderRotationY - 90))).normalize());
    
    
        ofPoint faceVectorOffsetted = ofPoint(cos(ofDegToRad(decoderRotationP)),
                        sin(ofDegToRad(decoderRotationP))).normalize().rotate(decoderRotationY + 10, ofPoint(cos(ofDegToRad(decoderRotationP - 90)),
                        sin(ofDegToRad(decoderRotationP - 90))).normalize()) - faceVector2;
    
        ofPoint tiltSphereRotated = faceVectorOffsetted.rotate(-decoderRotationR, faceVector2);
        //        ofPoint facePoint = faceVector2 * 120;
    
        ofPushMatrix();
        ofTranslate(0, 0, 100);
    
//        drawSphere(faceVector2.x * 150, faceVector2.y * 150, faceVector2.z * 150, 4, true);
    
        ofSetColor(60, 255, 0, 75); // Tilt sphere
//        drawSphere((faceVector2.x + tiltSphereRotated.x) * 150,
//                   (faceVector2.y + tiltSphereRotated.y) * 150,
//                   (faceVector2.z + tiltSphereRotated.z) * 150, 2, true);
    
        //        ofPoint tiltVector = faceVector2.perpendicular()
    
        ofSetColor(255, 0, 0);
        ofDrawLine(faceVector2.x * 50, faceVector2.z * 50, faceVector2.y * 50,
                   faceVector2.x * 120, faceVector2.z * 120, faceVector2.y * 120);
    
        // Encode DSP point results
    
		m1Encode.setRotation(rotation);
		m1Encode.setDiverge(diverge);
		m1Encode.setPitch(pitch);
		m1Encode.setStereoRotate(sRotation);
		m1Encode.setStereoSpread(sSpread);
		m1Encode.setAutoOrbit(autoOrbit);
		m1Encode.setIsotropicEncode(enableIsotropicEncode);

		if (inputKind == 0) { // Input: MONO
			m1Encode.setInputMode(Mach1EncodeInputModeType::Mach1EncodeInputModeMono);
		}
		if (inputKind == 1) { // Input: STERO
			m1Encode.setInputMode(Mach1EncodeInputModeType::Mach1EncodeInputModeStereo);
		}
		if (inputKind == 2) { // Input: Quad
			m1Encode.setInputMode(Mach1EncodeInputModeType::Mach1EncodeInputModeQuad);
		}
		if (inputKind == 3) { // Input: AFORMAT
			m1Encode.setInputMode(Mach1EncodeInputModeType::Mach1EncodeInputModeAFormat);
		}
		if (inputKind == 4) { // Input: BFORMAT
			m1Encode.setInputMode(Mach1EncodeInputModeType::Mach1EncodeInputModeBFormat);
		}

		if (outputKind == 0) { // Output: Mach1Horizon / Quad
			m1Encode.setOutputMode(Mach1EncodeOutputModeType::Mach1EncodeOutputMode4Ch);
		}
		if (outputKind == 1) { // Output: Mach1Spatial / Cuboid
			m1Encode.setOutputMode(Mach1EncodeOutputModeType::Mach1EncodeOutputMode8Ch);
		}

        // Resets the Decoding input when changing Encoding output between Mach1Spatial and Mach1Horizon
        if (outputKind == 0) { // Output: Mach1Horizon / Quad
            m1Decode.setDecodeAlgoType(Mach1DecodeAlgoHorizon);
        }
        if (outputKind == 1) { // Output: Mach1Spatial / Cuboid
            m1Decode.setDecodeAlgoType(Mach1DecodeAlgoSpatial);
        }

		mtx.lock();
		m1Encode.generatePointResults();

		m1Decode.beginBuffer();
		decoded = m1Decode.decode(decoderRotationY, decoderRotationP, decoderRotationR, 0, 0);
		m1Decode.endBuffer();

		std::vector<float> volumes = this->volumes;
		mtx.unlock();

		std::vector<Mach1Point3D> points = m1Encode.getPoints();
		std::vector<string> pointsNames = m1Encode.getPointsNames();

        ofPushMatrix();
            ofScale(RENDERING_SCALE, RENDERING_SCALE, RENDERING_SCALE);
            for (int i = 0; i < m1Encode.getPointsCount(); i++) {
                ofSetColor(255, 0 ,0);
                ofDrawSphere(points[i].x * 2 - 1, points[i].y * 2 - 1, points[i].z * 2 - 1, 10. / RENDERING_SCALE);
                ofSetColor(255);
                ofDrawBitmapString(pointsNames[i], points[i].x * 2 - 1, points[i].y * 2 - 1, points[i].z * 2 - 1);
                
                ofSetColor(200);
                ofDrawLine(0, 0, 0,
                           points[i].x * 2 - 1, 0, points[i].z * 2 - 1);
                ofDrawLine(points[i].x * 2 - 1, 0, points[i].z * 2 - 1,
                           points[i].x * 2 - 1, points[i].y * 2 - 1, points[i].z * 2 - 1);
            }
    
            ofDrawArrow(ofPoint(0), ofPoint(1, 0, 0));
            ofDrawArrow(ofPoint(0), ofPoint(0, 1, 0));
        ofPopMatrix();
    camera.end();
    
	std::vector<std::vector<float>> gains = m1Encode.getGains();
	
	for (int i = 0; i < gains.size(); i++) {
        float widgetWidth = 25 * gains[0].size();
        float widgetHeight = 50;
        
        float widgetPartWidth = widgetWidth / (float)gains[0].size();
        ofPushMatrix();
            ofSetColor(255);
            ofDrawBitmapString(pointsNames[i], ofGetWidth() - widgetWidth - 30, 100 + widgetHeight * i + 25);
        
            ofTranslate(ofGetWidth() - widgetWidth, 100 + widgetHeight * i);
            for (int j = 0; j < gains[0].size(); j++) {
                
                float thisPointGain = ofClamp(gains[i][j] , 0, 1);
                
                ofSetColor(220, 150);
                
                ofDrawRectangle(widgetPartWidth * j,
                                0,
                                widgetPartWidth - 5,
                                thisPointGain * (widgetHeight - 5));
                
                ofSetColor(170, 150);
                ofNoFill();
                ofDrawRectangle(widgetPartWidth * j, 0, widgetPartWidth - 5, widgetHeight - 5);
                ofFill();
            }
        ofPopMatrix();
    }
    

	if (player.getRawSamples().size() == 0) {
		ofDrawBitmapStringHighlight("Drop the audio file here!", ofGetWidth() / 2, ofGetHeight() / 2);
	}
	else {
		ofSetLineWidth(2);
		int border = 20;
		ofDrawLine(10, ofGetHeight()- border, ofGetWidth()- border, ofGetHeight()- border);

		float position = (1.0 * pos / player.getRawSamples().size()) * (ofGetWidth() - 2 * border);

		ofDrawCircle(10 + position, ofGetHeight() - border, 10);
		ofSetLineWidth(1);
	}


    gui.begin();
	{
        ImGui::LabelText("Encoder settings", "");
        const char* inputOptions[] = {"MONO", "STEREO", "QUAD", "AFORMAT", "BFORMAT"};
        ImGui::Combo("Input type", &inputKind, inputOptions, 5, 5);
        const char* outputOptions[] = {"Mach1Horizon/4CH", "Mach1Spatial/8CH"};
        ImGui::Combo("Output type", &outputKind, outputOptions, 2, 2);
    
        ImGui::SliderFloat("Rotation", &rotation, 0, 1);
        ImGui::SliderFloat("Diverge", &diverge, -0.707, 0.707);
        ImGui::SliderFloat("Pitch", &pitch, -1, 1);
        ImGui::Checkbox("Isotropic encode", &enableIsotropicEncode);

        if (inputKind == 1) { // Input: STERO
            ImGui::SliderFloat("S Rotation", &sRotation, -180, 180);
            ImGui::SliderFloat("S Spread", &sSpread, 0, 1);
            ImGui::Checkbox("Auto orbit", &autoOrbit);
        }
    
        ImGui::Separator();
        ImGui::LabelText("Decoder settings", "");
        ImGui::SliderFloat("Decoder Yaw", &decoderRotationY, 0, 360);
        ImGui::SliderFloat("Decoder Pitch", &decoderRotationP, -90, 90);
        ImGui::SliderFloat("Decoder Roll", &decoderRotationR, -180, 180);

        ImGui::Separator();
        ImGui::Checkbox("Enable mouse", &enableMouse);

		if (ImGui::IsMouseHoveringAnyWindow() || !enableMouse) {
			camera.disableMouseInput();
		}
		else {
			camera.enableMouseInput();
		}
	}
	gui.end();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

void ofApp::audioOut(float * output, int bufferSize, int nChannels)
{
	mtx.lock();
	std::vector<std::vector<float>> gains = m1Encode.getGains();
	std::vector<float> decoded = this->decoded;
	mtx.unlock();

	std::vector<float> volumes(2);

	float sample;
	for (int i = 0; i < bufferSize; i++)
	{
		sample = 0;
		for (int j = 0; j < 8; j++) {
			if (pos < player.getRawSamples().size()) sample += player.getRawSamples()[pos] * (decoded[2 * j + 0]) * gains[0][j];
		}
		output[i*nChannels] = sample / 8;
		volumes[0] += fabs(output[i*nChannels]);
		
		if(player.getNumChannels()>1) pos++;

		sample = 0;
		for (int j = 0; j < 8; j++) {
			if (pos < player.getRawSamples().size()) sample += player.getRawSamples()[pos] * (decoded[2 * j + 1]) * gains[gains.size() > 1 ? 1 : 0][j];
		}
		output[i*nChannels + 1] = sample / 8;
		volumes[1] += fabs(output[i*nChannels + 1]);
		pos++;

		// loop audio
		if (pos >= player.getRawSamples().size()) pos = 0;
	}

	// show volumes
	volumes[0] /= 1;// bufferSize;
	volumes[1] /= 1;// bufferSize;

	mtx.lock();
	for (int j = 0; j < volumes.size(); j++) {
		this->volumes[j] = ofLerp(this->volumes[j], volumes[j], 0.1);
	}
	mtx.unlock();
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
	ofLog() << dragInfo.files[0];

	ofFile file;
	if (file.open("lastfile.cfg", ofFile::Mode::WriteOnly, false)) {

		file.writeFromBuffer(ofBuffer(dragInfo.files[0].c_str(), dragInfo.files[0].length()));

		player.load(dragInfo.files[0]);
		pos = 0;
	}
}
