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
        playerL.load((std::string)b);
        playerL.setLoop(true);
        playerL.play();
        playerL.setPan(-1);

        playerR.load((std::string)b);
        playerR.setLoop(true);
        playerR.play();
        playerR.setPan(1);
    }
    
    m1Decode.setAngularSettingsType(m1oFEasyCam);
    m1Decode.setAlgorithmType(m1Spatial);
    m1Decode.setFilterSpeed(0.95f);
}

//--------------------------------------------------------------
void ofApp::update() {

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
    
        // Panner DSP point results
    
        m1Encode.rotation = rotation;
        m1Encode.diverge = diverge;
        m1Encode.pitch = pitch;
    
        m1Encode.autoOrbit = autoOrbit;
        m1Encode.sRotate = sRotation;
        m1Encode.sSpread = sSpread;
    
        m1Encode.isotropicEncode = enableIsotropicEncode;

        if (inputKind == 0) { // Input: MONO
            m1Encode.inputMode = M1Encode::INPUT_MONO;
        }
        if (inputKind == 1) { // Input: STERO
            m1Encode.inputMode = M1Encode::INPUT_STEREO;
        }
        if (inputKind == 2) { // Input: Quad
            m1Encode.inputMode = M1Encode::INPUT_QUAD;
        }
        if (inputKind == 3) { // Input: AFORMAT
            m1Encode.inputMode = M1Encode::INPUT_AFORMAT;
        }
        if (inputKind == 4) { // Input: BFORMAT
            m1Encode.inputMode = M1Encode::INPUT_BFORMAT;
        }

        if (outputKind == 0) { // Output: Quad
            m1Encode.outputMode = M1Encode::OUTPUT_4CH;
        }
        if (outputKind == 1) { // Output: 7.1
            m1Encode.outputMode = M1Encode::OUTPUT_8CH;
        }

        auto points = m1Encode.generatePointResults();

        float result[18];
        m1Decode.beginBuffer();
        m1Decode.decode(decoderRotationY, decoderRotationP, decoderRotationR, 1, 0);
        m1Decode.endBuffer();
    
        float volumeL = 0, volumeR = 0;
        for (int i = 0; i < 8; i++) {
            volumeL += (result[i]) * points.gains[0][i];
            volumeR += (result[8 + i]) * points.gains[0][i];
        }

        playerL.setVolume(volumeL * 0);
        playerR.setVolume(volumeR * 0);

        ofPushMatrix();
            ofScale(RENDERING_SCALE, RENDERING_SCALE, RENDERING_SCALE);
            for (int i = 0; i < points.pointsCount; i++) {
                ofSetColor(255, 0 ,0);
                ofDrawSphere(points.ppoints[i][0] * 2 - 1, points.ppoints[i][1] * 2 - 1, points.ppoints[i][2] * 2 - 1, 10. / RENDERING_SCALE);
                ofSetColor(255);
                ofDrawBitmapString(points.pointsNames[i], points.ppoints[i][0] * 2 - 1, points.ppoints[i][1] * 2 - 1, points.ppoints[i][2] * 2 - 1);
                
                ofSetColor(200);
                ofDrawLine(0, 0, 0,
                           points.ppoints[i][0] * 2 - 1, 0, points.ppoints[i][2] * 2 - 1);
                ofDrawLine(points.ppoints[i][0] * 2 - 1, 0, points.ppoints[i][2] * 2 - 1,
                           points.ppoints[i][0] * 2 - 1, points.ppoints[i][1] * 2 - 1, points.ppoints[i][2] * 2 - 1);
            }
    
            ofDrawArrow(ofPoint(0), ofPoint(1, 0, 0));
            ofDrawArrow(ofPoint(0), ofPoint(0, 1, 0));
        ofPopMatrix();
    camera.end();
    
    for (int i = 0; i < points.gains.size(); i++) {
        float widgetWidth = 25 * points.gains[0].size();
        float widgetHeight = 50;
        
        float widgetPartWidth = widgetWidth / (float)points.gains[0].size();
        ofPushMatrix();
            ofSetColor(255);
            ofDrawBitmapString(points.pointsNames[i], ofGetWidth() - widgetWidth - 30, 100 + widgetHeight * i + 25);
        
            ofTranslate(ofGetWidth() - widgetWidth, 100 + widgetHeight * i);
            for (int j = 0; j < points.gains[0].size(); j++) {
                
                float thisPointGain = ofClamp(points.gains[i][j] , 0, 1);
                
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
    
    gui.begin();
        ImGui::LabelText("Encoder settings", "");
        const char* inputOptions[] = {"MONO", "STEREO", "QUAD", "AFORMAT", "BFORMAT"};
        ImGui::Combo("Input type", &inputKind, inputOptions, 5, 5);
        const char* outputOptions[] = {"4CH", "8CH"};
        ImGui::Combo("Output type", &outputKind, outputOptions, 2, 2);
    
        ImGui::SliderFloat("Rotation", &rotation, 0, 1);
        ImGui::SliderFloat("Diverge", &diverge, -1, 1);
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


        if (!enableMouse) {
            camera.disableMouseInput();
        } else {
            camera.enableMouseInput();
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

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
    ofLog() << dragInfo.files[0];
    
    ofFile file;
    if (file.open("lastfile.cfg", ofFile::Mode::WriteOnly, false)) {
        file.writeFromBuffer(ofBuffer(dragInfo.files[0]));
        
        playerL.load(dragInfo.files[0]);
        playerL.setLoop(true);
        playerL.play();

        playerR.load(dragInfo.files[0]);
        playerR.setLoop(true);
        playerR.play();
    }
}
