#include "ofApp.h"
#include "ofxEasing.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSoundStreamSetup(0, 1, this, 44100, beat.getBufferSize(), 4);
    
    //ofClear(0,0,0);
    
    ofEnableAntiAliasing();
    ofEnableSmoothing();
    
    
    
    shaderBlurX.load("shadersGL3/shaderBlurX");
    shaderBlurY.load("shadersGL3/shaderBlurY");
    
    fboBlurOnePass.allocate(ofGetWidth(), ofGetHeight());
    fboBlurTwoPass.allocate(ofGetWidth(), ofGetHeight());
    
    //plane.set(1600, 1200, 80, 60);
    //plane.mapTexCoordsFromTexture(img.getTextureReference());
    
    initTime = 0;
    endRadius = 0;
    endRotation = 0;
    radius = 100;
    
    /*sphere.setResolution(25);
    sphere2.setResolution(25);
    sphere3.setResolution(25);
    sphere4.setResolution(25);
    sphere5.setResolution(25);*/
    
    /*sphere.move(ofRandom(-200, 200), ofRandom(-200, 200), ofRandom(-200, 200));
    sphere2.move(ofRandom(-200, 200), ofRandom(-200, 200), ofRandom(-200, 200));
    sphere3.move(ofRandom(-200, 200), ofRandom(-200, 200), ofRandom(-200, 200));
    sphere4.move(ofRandom(-200, 200), ofRandom(-200, 200), ofRandom(-200, 200));
    sphere5.move(ofRandom(-200, 200), ofRandom(-200, 200), ofRandom(-200, 200));*/
    
    
    for( int i=0; i < 25; i++ ) {
        spheres[i].setResolution(25);
        spheres[i].move(ofRandom(-500, 500), ofRandom(-500, 500), ofRandom(-500, 500));
    }
                         
    
    glMatrixMode(GL_PROJECTION);
    cam.setFov(60);
    cam.setNearClip(1);
    cam.setFarClip(20000);
    //cam.move(0, 0, 50000);
    
    //cam.lookAt(sphere);
    
    //setupCamera();
    
    //cam.begin();
    
    
    colors[0] = ofColor::red;
    colors[1] = ofColor::darkRed;
    colors[2] = ofColor::mediumVioletRed;
    colors[3] = ofColor::indianRed;
    colors[4] = ofColor::orangeRed;
    
    point.setDiffuseColor(ofColor(0.0, 255.0, 0.0));
    point.setPointLight();
    
    spot.setDiffuseColor(ofFloatColor(255.0, 0.0, 0.0f));
    spot.setSpecularColor(ofColor(0, 0, 255));
    spot.setSpotlight();
    spot.setSpotConcentration(5);
    spot.setSpotlightCutOff(10);
    spot_rot = ofVec3f(0, 0, 0);
    setLightOri(spot, spot_rot);
    
    spot.setPosition(0, 0, 300);
    
    bOrbit = bRoll = false;
    angleH = roll = 0.0f;
    distance = 500.f;
}

//--------------------------------------------------------------
void ofApp::update(){
    beat.update(ofGetElapsedTimeMillis());
    
    auto duration = 5.f;
    auto endTime = initTime + duration;
    auto now = ofGetElapsedTimef();
    
    float kick = beat.kick();
    float snare = beat.snare();
    float hihat = beat.hihat();
    
    endRadius = ofMap(snare, 0, 1, 50, 2000);
    
    
    ofBackground(0,0,0);
    ofSetColor(0,0,255,50);
    
    cout << snare;
    cout << "\n";
    if (kick > 0.75 && now - initTime > 0.5) {
        initTime = ofGetElapsedTimef();
        cout << "reset";
    }
    radius = ofxeasing::map_clamp(now, initTime, endTime, radius, endRadius, &ofxeasing::quad::easeOut);
    cout << endRadius;
    cout << " ";
    cout << radius;
    cout << " ";
    cout << now;
    cout << " ";
    cout << initTime;
    cout << " ";
    cout << endTime;
    cout << "\n";
    
    //radius = endRadius;
    
    if (snare > 0.75 && now - initTime > 0.5) {
        endRotation = ofMap(random(), 0, 1, -1, 1);
    }
    rotation = ofxeasing::map_clamp(now, initTime, endTime, kick, endRotation, &ofxeasing::cubic::easeIn);
    //rotation = 30;
    
    
    resolution = ofMap(hihat, 0, 1, 10, 50);
    
    //setupCamera();
    
    if (bOrbit) angleH += 1.f;
    if (bRoll) roll += (snare * 2);
    
    // here's where the transformation happens, using the orbit and roll member functions of the ofNode class,
    // since angleH and distance are initialised to 0 and 500, we start up as how we want it
    cam.orbit(angleH, 0, distance);
    cam.roll(roll);
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    //ofSetColor(0, 0, 0);  // Set the drawing color to white
    
    float blur = 20; //ofMap(mouseX, 0, ofGetWidth(), 0, 10, true);
    
    float kick = beat.kick();
    float snare = beat.snare();
    float hihat = beat.hihat();
    
    //float rot = map(snare, 0, 1, -0.5, 0.5) * ofGetFrameNum(); // ofRotate takes degrees
    float rot = 0.5 * ofGetFrameNum();
    
    
    
    ofRotate(rot, 2, 1, 1);
    
    // translate plane into center screen.
    float tx = ofGetWidth() / 2;
    float ty = ofGetHeight() / 2;
    ofTranslate(tx, ty);
    
    

    
    /*float percentY = mouseY / (float)ofGetHeight();
    float rotation = ofMap(percentY, 0, 1, -60, 60, true) + 60;
    float percentX = mouseX / (float)ofGetWidth();
    float rotationX = ofMap(percentX, 0, 1, -60, 60, true) + 60;*/
    //ofRotate(30, 2, 1, 0.5);
    
    //plane.drawWireframe();
    
    
    
    shaderBlurX.begin();
    
    cam.begin();
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    
    spot.enable();


    
    //ofRotate(rotation, 2, 1, 0.5);
    
    //blur = 30 * snare;
    //shaderBlurX.setUniform1f("blurAmnt", blur);
    
    
    /*ofFloatColor sphereColor = ofColor::indigo;
    float sphereColorF[4] = {sphereColor.r, sphereColor.g, sphereColor.b, sphereColor.a};
    shaderBlurX.setUniform4fv("sphereColor", sphereColorF);*/
    
    
    
    /*ofBackground(0,0,0);
    ofSetColor(ofColor::indianRed ,126);*/

    
    //ofSetColor(255, 0, 0);  // Set the drawing color to white
    
    //ofClear(0,0,0);
    //ofBackground(0,0,0);
    
    //ofSetColor(0,0,255);
    
    for( int i=0; i < 25; i++ ) {
        
        ofSetColor(colors[i % 5]);
        
        spheres[i].setResolution(resolution);
        spheres[i].setRadius(radius * (i + 1) * 0.25);
        
        spheres[i].drawWireframe();
        //spheres[i].drawFaces();
        
    }
    
    
    
    
    /*sphere.setRadius(radius * 4);
    
    ofSetColor(colors[0]);
    
    sphere.drawWireframe();
    sphere.drawFaces();
    
    //ofClear(0, 255, 0);
    ofSetColor(colors[1]);
    
    sphere2.setRadius(radius * 2.5);
   
    sphere2.drawWireframe();
    sphere2.drawFaces();
    
    //ofClear(0, 0, 255);
    
    ofSetColor(colors[2]);
    
    sphere3.setRadius(radius * 3.5);
    
    //ofSetColor(255,255,255);
    sphere3.drawWireframe();
    //ofSetColor(0,0,255,126);
    sphere3.drawFaces();
    //sphere3.drawVertices();
    
    
    ofSetColor(colors[3]);
    
    sphere4.setRadius(radius * 3);
    
    sphere4.drawWireframe();
    //sphere2.drawFaces();
    
    ofSetColor(colors[4]);
    
    sphere5.setRadius(radius * 2.5);
    
    sphere5.drawWireframe();*/
    //sphere5.drawAxes(500);
    //sphere5.drawNormals(250);
    //sphere2.drawFaces();
    spot.disable();

    //ofPopMatrix();
    
    cam.end();

    
    shaderBlurX.end();
    
    
    
    
    
    
}

void ofApp::setLightOri(ofLight &light, ofVec3f rot)
{
    ofVec3f xax(1, 0, 0);
    ofVec3f yax(0, 1, 0);
    ofVec3f zax(0, 0, 1);
    ofQuaternion q;
    q.makeRotate(rot.x, xax, rot.y, yax, rot.z, zax);
    light.setOrientation(q);
}

void ofApp::audioReceived(float* input, int bufferSize, int nChannels) {
    beat.audioReceived(input, bufferSize, nChannels);
}

void ofApp::setupCamera() {
    // ofCamera myCam;
    float tweenvalue = (ofGetElapsedTimeMillis() % 2000) /2000.f; // this will slowly change from 0.0f to 1.0f, resetting every 2 seconds
    
    ofQuaternion startQuat;
    ofQuaternion targetQuat;
    ofVec3f startPos;
    ofVec3f targetPos;
    
    // we define the camer's start and end orientation here:
    startQuat.makeRotate(0, 0, 1, 0);			// zero rotation.
    targetQuat.makeRotate(90, 0, 1, 0);			// rotation 90 degrees around y-axis.
    
    // we define the camer's start and end-position here:
    startPos.set(5000,0,0);
    targetPos.set(10000,0,0);
    
    
    ofQuaternion tweenedCameraQuaternion;	// this will be the camera's new rotation.
    
    // calculate the interpolated orientation
    tweenedCameraQuaternion.slerp(tweenvalue, startQuat, targetQuat);
    
    ofVec3f lerpPos;					//this will hold our tweened position.
    
    // calculate the interpolated values.
    lerpPos.x = ofLerp(tweenvalue, startPos.x, targetPos.x);
    lerpPos.y = ofLerp(tweenvalue, startPos.y, targetPos.y);
    lerpPos.z = ofLerp(tweenvalue, startPos.z, targetPos.z);
    
    // alternative way to calculate interpolated values:
    // lerpPos = startPos + ((targetPos-startPos) * tweenvalue);
    
    // now update the camera with the calculated orientation and position.
    cam.setOrientation(tweenedCameraQuaternion);
    cam.setGlobalPosition(lerpPos);
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'h') {
        bOrbit = !bOrbit;
    }
    else if (key == 'r') {
        bRoll = !bRoll;
    }
    else if (key == OF_KEY_UP) {
        distance = MIN( (distance += 2.5f), 1000);
        cout << distance << endl;
        
    }
    else if (key == OF_KEY_DOWN) {
        distance = MAX( (distance -= 2.5f), 150);
        cout << distance << endl;
        
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
