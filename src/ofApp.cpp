#include "ofApp.h"
#include "ofxEasing.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSoundStreamSetup(0, 1, this, 44100, beat.getBufferSize(), 4);
    
    //ofClear(0,0,0);
    //ofBackground(0,0,0);
    
    
    shaderBlurX.load("shadersGL3/shaderBlurX");
    shaderBlurY.load("shadersGL3/shaderBlurY");
    
    fboBlurOnePass.allocate(ofGetWidth(), ofGetHeight());
    fboBlurTwoPass.allocate(ofGetWidth(), ofGetHeight());
    
    plane.set(800, 600, 80, 60);
    //plane.mapTexCoordsFromTexture(img.getTextureReference());
    
    initTime = 0;
    endRadius = 0;
    endRotation = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
    beat.update(ofGetElapsedTimeMillis());
    
    auto duration = 1.f;
    auto endTime = initTime + duration;
    auto now = ofGetElapsedTimef();
    
    float kick = beat.kick();
    float snare = beat.snare();
    float hihat = beat.hihat();
    
    endRadius = ofMap(snare, 0, 1, 100, 500);
    
    
    
    
    //cout << snare;
    //cout << "\n";
    if (snare > 0.75) {
        initTime = ofGetElapsedTimef();
    }
    radius = ofxeasing::map_clamp(now, initTime, endTime, radius, endRadius, &ofxeasing::cubic::easeIn);
    
    if (kick > 0.75) {
        endRotation = ofMap(random(), 0, 1, -60, 60);
    }
    rotation = ofxeasing::map_clamp(now, initTime, endTime, rotation, endRotation, &ofxeasing::cubic::easeIn);
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    //ofSetColor(0, 0, 0);  // Set the drawing color to white
    
    float blur = 20; //ofMap(mouseX, 0, ofGetWidth(), 0, 10, true);
    
    float kick = beat.kick();
    float snare = beat.snare();
    float hihat = beat.hihat();
    
    /*cout << kick;
    cout << '\n';
    cout << snare;
    cout << '\n';
    cout << hihat;
    cout << '\n';*/
    
    //----------------------------------------------------------
    fboBlurOnePass.begin();
    
    ofClear(255, 0, 0);
    //ofBackground(0, 0, 0);  // Clear the screen with a black color
    //ofSetColor(0, 255, 0);
    
    shaderBlurX.begin();
    
    
    
    
    //ofDrawRectangle(50, 50, 100, 100*kick);
    //ofDrawRectangle(200, 50, 100, 100*snare);
    //ofDrawRectangle(350, 50, 100, 100*hihat);
    
    
    // translate plane into center screen.
    float tx = ofGetWidth() / 2;
    float ty = ofGetHeight() / 2;
    ofTranslate(tx, ty);

    
    /*float percentY = mouseY / (float)ofGetHeight();
    float rotation = ofMap(percentY, 0, 1, -60, 60, true) + 60;
    float percentX = mouseX / (float)ofGetWidth();
    float rotationX = ofMap(percentX, 0, 1, -60, 60, true) + 60;*/
    ofRotate(rotation, 1, 0.5, 0);
    //plane.drawWireframe();
    
    
    blur = 30 * snare;
    shaderBlurX.setUniform1f("blurAmnt", blur);
    
    //ofSetColor(255, 0, 0);  // Set the drawing color to white
    
    sphere.setRadius(radius);
    sphere.drawWireframe();
    
    sphere2.setRadius(radius * 2);
    sphere2.move(0.25, 0, 0);
    sphere2.drawWireframe();
    
    sphere3.setRadius(radius * 3);
    sphere3.move(-0.25, 0, 0);
    sphere3.drawWireframe();
    //sphere.drawFaces();

    
    shaderBlurX.end();
    
    fboBlurOnePass.end();
    
    //fboBlurOnePass.draw(0, 0);
    
    
    
    //----------------------------------------------------------
    fboBlurTwoPass.begin();
    //ofClear(255, 255, 255, 255);
    
    shaderBlurY.begin();
    shaderBlurY.setUniform1f("blurAmnt", blur);
    shaderBlurY.setUniform1f("kick", ofMap(kick, 0, 1, 0, 100));
    shaderBlurY.setUniform1f("snare", ofMap(snare, 0, 1, 0, 100));
    shaderBlurY.setUniform1f("hihat", ofMap(hihat, 0, 1, 0, 100));
    
    fboBlurOnePass.draw(0, 0);
    
    shaderBlurY.end();
    
    fboBlurTwoPass.end();
    
    //----------------------------------------------------------
    //ofBackground(255);  // Clear the screen with a black color
    //ofSetColor(ofColor::black);
    
    fboBlurTwoPass.draw(0, 0);
    
}

void ofApp::audioReceived(float* input, int bufferSize, int nChannels) {
    beat.audioReceived(input, bufferSize, nChannels);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

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
