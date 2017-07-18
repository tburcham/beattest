#include "ofApp.h"
#include "ofxEasing.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSoundStreamSetup(0, 1, this, 44100, beat.getBufferSize(), 4);
    
    //ofClear(0,0,0);
    
    ofEnableAntiAliasing();
    ofEnableSmoothing();
    
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetVerticalSync(false);
    ofEnableAlphaBlending();
    
    shader.setGeometryInputType(GL_LINES);
    shader.setGeometryOutputType(GL_TRIANGLE_STRIP);
    shader.setGeometryOutputCount(4);
    shader.load("shaders/vert.glsl", "shaders/frag.glsl", "shaders/geom.glsl");
    
    ofLog() << "Maximum number of output vertices support is: " << shader.getGeometryMaxOutputCount();
    
    
    initTime = 0;
    endRadius = 0;
    endRotation = 0;
    radius = 100;
    
    
    
    for( int i=0; i < numSpheres; i++ ) {
        spheres[i].setResolution(25);
        spheres[i].move(ofRandom(-500, 500), ofRandom(-500, 500), ofRandom(-500, 500));
        
        //points.push_back(ofPoint(ofRandomf() * r, ofRandomf() * r, ofRandomf() * r));

        /*tempMesh = spheres[i].getMesh();
        tempVerts = tempMesh.getVertices();*/
        
        /*for ( int j=0; j < tempMesh.getNumVertices(); j++) {
            points.push_back(ofPoint(tempVerts[j].x, tempVerts[j].y, tempVerts[j].z));
        }*/
        
        // create a bunch of random points
        float r = ofGetHeight()/2;
        for(int i=0; i<100; i++) {
            points.push_back(ofPoint(ofRandomf() * r, ofRandomf() * r, ofRandomf() * r));
        }
        
    }
                         
    
    //glMatrixMode(GL_PROJECTION);
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
    
    
    
    bOrbit = bRoll = false;
    angleH = roll = 0.0f;
    distance = 500.f;
    
    doShader = false;
    ofEnableDepthTest();
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
    
    endRadius = ofMap(snare, 0, 1, 100, 250);
    
    
    ofBackground(0,0,0);
    ofSetColor(122,0,255,50);
    
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
    
    
    resolution = ofMap(hihat, 0, 1, 1, 8);
    
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
    
    ofPushMatrix();
    
    
    
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
    

    cam.begin();
    
    //shaderBlurX.begin();
    
    if(doShader) {
        shader.begin();
        
        // set thickness of ribbons
        shader.setUniform1f("thickness", 20);
        
        // make light direction slowly rotate
        shader.setUniform3f("lightDir", sin(ofGetElapsedTimef()/10), cos(ofGetElapsedTimef()/10), 0);
    }
    
    
    
    ofColor(255);
    
    /*glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);*/
    
    
    
    for( int i=0; i < numSpheres; i++ ) {
        
        ofSetColor(colors[i % 5], ofClamp(kick * 100, 100, 100));
        
        spheres[i].setResolution(resolution);
        spheres[i].setRadius(radius * (i + 1) * 0.25);
        
        spheres[i].drawWireframe();
        spheres[i].drawFaces();
        //spheres[i].draw();
        
    }
    
    
    for(unsigned int i=1; i<points.size(); i++) {
        ofDrawLine(points[i-1], points[i]);
    }
    
    
    
    

    
    //shaderBlurX.end();
    
    if(doShader) shader.end();
    
    cam.end();
    
    ofPopMatrix();

    
    
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
        distance = MIN( (distance += 2.5f), 20000);
        cout << distance << endl;
        
    }
    else if (key == OF_KEY_DOWN) {
        distance = MAX( (distance -= 2.5f), 20000);
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
