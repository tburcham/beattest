#include "ofApp.h"
#include "ofxEasing.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofLog() << "GL Version" << glGetString( GL_VERSION );
    
    ofSoundStreamSetup(0, 1, this, 44100, beat.getBufferSize(), 4);
    
    //ofClear(0,0,0);
    
    //ofEnableAntiAliasing();
    ofEnableSmoothing();
    
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableDepthTest();
    //ofDisableDepthTest();
    
    
    int camWidth 		= 1280;	// try to grab at this size.
    int camHeight 		= 960;
    
    vidGrabber.setVerbose(true);
    vidGrabber.setup(camWidth,camHeight);

    
    shader.load("shaders/vert.glsl", "shaders/frag.glsl", "shaders/geom.glsl");
    
    
    ofLog() << "Maximum number of output vertices support is: " << shader.getGeometryMaxOutputCount();
    
    
    /* Control Panel */
    
    // we add this listener before setting up so the initial circle resolution is correct
    circleResolution.addListener(this, &ofApp::circleResolutionChanged);
    
    
    texturePatternImg.load("yellowstripes.png");
    
    fbo.allocate(ofGetWidth(),ofGetHeight());
    
    // Let's clear the FBOs
    // otherwise it will bring some junk with it from the memory
    fbo.begin();
    ofClear(0,0,0,255);
    fbo.end();
    
    //shader.setUniformTexture("texture0", texturePatternImg.getTexture(), 1);
    
    
    gui.setup(); // most of the time you don't need a name
    gui.add(circleResolution.setup("resolution", 5, 2, 50));
    gui.add(thickness.setup("thickness", 20, 1, 300));
    gui.add(orbitInc.setup("orbit inc", 0.25, -10, 10));
    gui.add(rollInc.setup("roll inc", 2, -10, 10));
    
    bHide = false;
    
    
    initTime = 0;
    endRadius = 0;
    endRotation = 0;
    radius = 100;
    jitter = 20;
    
    
    for( int i=0; i < numSpheres; i++ ) {
        //spheres[i].setResolution(25);
        spheres[i].move(ofRandom(-500, 500), ofRandom(-500, 500), ofRandom(-500, 500));
        
        //spheres[i].mapTexCoords(0, 0, texturePatternImg.getWidth(), texturePatternImg.getHeight());
        spheres[i].mapTexCoords(0, 0, vidGrabber.getWidth(), vidGrabber.getHeight());
        
        //points.push_back(ofPoint(ofRandomf() * r, ofRandomf() * r, ofRandomf() * r));

        /*tempMesh = spheres[i].getMesh();
        tempVerts = tempMesh.getVertices();*/
        
        /*for ( int j=0; j < tempMesh.getNumVertices(); j++) {
            points.push_back(ofPoint(tempVerts[j].x, tempVerts[j].y, tempVerts[j].z));
        }*/
        
        // create a bunch of random points
        /*float r = ofGetHeight()/2;
        for(int i=0; i<100; i++) {
            points.push_back(ofPoint(ofRandomf() * r, ofRandomf() * r, ofRandomf() * r));
        }*/
        
    }
                         
    
    //glMatrixMode(GL_PROJECTION);
    cam.setFov(60);
    cam.setNearClip(1);
    cam.setFarClip(20000);
    //cam.move(0, 0, 50000);
    
    //cam.lookAt(sphere);
    
    //setupCamera();
    
    //cam.begin();
    
    
    /*colors[0] = ofColor::red;
    colors[1] = ofColor::darkRed;
    colors[2] = ofColor::mediumVioletRed;
    colors[3] = ofColor::indianRed;
    colors[4] = ofColor::orangeRed;*/
    
    /*colors[0] = ofColor::lightBlue;
    colors[1] = ofColor::darkBlue;
    colors[2] = ofColor::skyBlue;
    colors[3] = ofColor::deepSkyBlue;
    colors[4] = ofColor::blueSteel;*/
    
    colors[0] = ofColor::lightGreen;
    colors[1] = ofColor::green;
    colors[2] = ofColor::darkGreen;
    colors[3] = ofColor::greenYellow;
    colors[4] = ofColor::yellow;
    
    
    
    bOrbit = bRoll = false;
    angleH = roll = 0.0f;
    distance = 2000.f;
    
    doShader = true;
    
    
    varyResolution = false;
    resolution = 4;
}

//--------------------------------------------------------------
void ofApp::circleResolutionChanged(int &circleResolution){
    ofSetCircleResolution(circleResolution);
}

//---------------------------d-----------------------------------
void ofApp::update(){
    
    vidGrabber.update();
    
    beat.update(ofGetElapsedTimeMillis());
    
    auto duration = 200.f;
    auto endTime = initTime + duration;
    auto now = ofGetElapsedTimef();
    
    float kick = beat.kick();
    float snare = beat.snare();
    float hihat = beat.hihat();
    
    endRadius = ofMap(snare, 0, 1, 100, 250);
    
    
    ofBackground(0,0,0);
    ofSetColor(255,255,255,255);
    
    cout << snare;
    cout << "\n";
    if (kick > 0.75 && now - initTime > 0.5) {
        initTime = ofGetElapsedTimef();
        cout << "reset";
    }
    radius = ofxeasing::map_clamp(now, initTime, endTime, radius, endRadius, &ofxeasing::cubic::easeOut);
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
    
    if (varyResolution) {
        resolution = ofMap(hihat, 0, 1, 3, 20);
    } else {
        // resolution = resolution;
    }
    
    //setupCamera();
    
    if (bOrbit) angleH += orbitInc;
    if (bRoll) roll += (snare * rollInc);
    
    // here's where the transformation happens, using the orbit and roll member functions of the ofNode class,
    // since angleH and distance are initialised to 0 and 500, we start up as how we want it
    cam.orbit(angleH, 0, distance);
    cam.roll(roll);
    
    for( int i=0; i < numSpheres; i++ ) {
        
        //ofSetColor(colors[i % 5], ofClamp(kick * 100, 50, 100));
        ofSetColor(ofColor::white, 255);
        
        spheres[i].setResolution(circleResolution);
        spheres[i].setRadius(radius * (i + 1) * 0.25);
        //spheres[i].setRadius(ofMap(random(), 0, 1, 25, 150));

            //spheres[i].get
        
        /*tempMesh = spheres[i].getMeshPtr();
        tempVerts = tempMesh.getVerticesPointer();
        
        for ( int j=0; j < tempMesh.getNumVertices(); j++) {
            tempVerts[j] += ofPoint(ofRandom(-jitter, jitter), ofRandom(-jitter, jitter), ofRandom(-jitter, jitter));
        }*/
        
        vector<ofMeshFace> triangles = spheres[i].getMesh().getUniqueFaces();
        
        /*for ( int j=0; j < triangles.size()   ; j++ ) {
            //triangles[j] += ofPoint(ofRandom(-jitter, jitter), ofRandom(-jitter, jitter), ofRandom(-jitter, jitter));
            triangles[j].setVertex(triangles[j]. += ofPoint(
                                                                       ofRandom(-jitter, jitter),
                                                                       ofRandom(-jitter, jitter),
                                                                       ofRandom(-jitter, jitter)
                                                                       ));
        }*/
        
        
        
    }
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // auto draw?
    // should the gui control hiding?
    /*if(!bHide){
        ofLog() << "GUI! on";
        
        gui.draw();
        
        ofLog() << "GUI! ";
    }*/
    
    
    
    fbo.begin();
    ofClear(0, 0, 0,255);
    
        drawSpheres();
    
    fbo.end();
    fbo.draw(0, 0);
    
    drawGui();

    
    
}

void ofApp::drawSpheres() {
    
    
    
    
    //ofPushMatrix();
    
    //texturePatternImg.getTexture().bind();
    
    
    
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
        shader.setUniform1f("thickness", thickness);
        
        // make light direction slowly rotate
        shader.setUniform3f("lightDir", sin(ofGetElapsedTimef()/20), cos(ofGetElapsedTimef()/20), 0);
        
        shader.setUniform2f("resolution", ofVec2f(texturePatternImg.getWidth(), texturePatternImg.getHeight()));
        shader.setUniform2f("mouse", ofVec2f(mouseX, mouseY));
        shader.setUniform1f("time", ofGetElapsedTimef());
        //shader.setUniformTexture("tex0", texturePatternImg.getTexture(), 1);
        shader.setUniformTexture("tex0", vidGrabber.getTexture(), 1);
    }
    
    
    
    //ofColor(255);
    
    /*glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);*/
    
    points.clear();
    
    for( int i=0; i < numSpheres; i++ ) {
        
        ofSetColor(colors[i % 5], ofClamp(kick * 100, 50, 100));
        
        //spheres[i].setResolution(resolution);
        //spheres[i].setRadius(radius * (i + 1) * 0.25);
        
        //spheres[i].drawWireframe();
        //spheres[i].drawFaces();
        spheres[i].draw();
        
        //ofDrawSphere(ofRandom(-500, 500), ofRandom(-500, 500), ofRandom(-500, 500), radius * (i + 1) * 0.25);
        
        //for( int i=0; i < numSpheres; i++ ) {
            /*spheres[i].setResolution(25);
             spheres[i].move(ofRandom(-500, 500), ofRandom(-500, 500), ofRandom(-500, 500));*/
            
            //points.push_back(ofPoint(ofRandomf() * r, ofRandomf() * r, ofRandomf() * r));
            
        
        
        /*spheres[i].drawWireframe();
        spheres[i].drawFaces();*/
        
        tempMesh = spheres[i].getMesh();
        tempVerts = tempMesh.getVertices();
        
        for ( int j=0; j < tempMesh.getNumVertices(); j++) {
            tempVerts[j] += ofPoint(ofRandom(-jitter, jitter), ofRandom(-jitter, jitter), ofRandom(-jitter, jitter));
        }
        
            
            for ( int j=0; j < tempMesh.getNumVertices(); j++) {
                points.push_back(ofPoint(tempVerts[j].x + spheres[i].getX(), tempVerts[j].y + spheres[i].getY(), tempVerts[j].z + spheres[i].getZ()));
            }
        //}
        
    }
    
    
    /*for(unsigned int i=1; i<points.size(); i++) {
        ofSetColor(colors[(i / 20) % 5], ofClamp(kick * 100, 50, 100));
        ofDrawLine(points[i-1], points[i]);
    }*/
    
    
    
    

    
    //shaderBlurX.end();
    
    if(doShader) shader.end();
    

    
    cam.end();
    
    //texturePatternImg.getTexture().bind();
    
    //ofPopMatrix();
    
    
    
    
    
}

void ofApp::drawGui() {
    
    /*guiFPS = (int)(ofGetFrameRate() + 0.5);
    
    // calculate minimum fps
    deltaTimeDeque.push_back(deltaTime);
    
    while (deltaTimeDeque.size() > guiFPS.get())
        deltaTimeDeque.pop_front();
    
    float longestTime = 0;
    for (int i=0; i<deltaTimeDeque.size(); i++){
        if (deltaTimeDeque[i] > longestTime)
            longestTime = deltaTimeDeque[i];
    }
    
    guiMinFPS.set(1.0 / longestTime);*/
    
    
    //ofPushStyle();
    //ofBackgroundGradient(ofColor::white, ofColor::gray);
    
    //ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    //ofNoFill();

    if (!bHide) {
        gui.draw();
    }
    
    //ofPopStyle();
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
    if (key == 't') {
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
    if(key == 'h'){
        bHide = !bHide;
    }
    else if(key == 's'){
        gui.saveToFile("settings.xml");
    }
    if(key == 'l'){
        gui.loadFromFile("settings.xml");
    }
    if( key == 'd' ){
        doShader = !doShader;
    }
    if (key == 'v'){
        varyResolution = !varyResolution;
    }
    
    if (key == '2') {
        resolution = 2;
    }
    if (key == '3') {
        resolution = 3;
    }
    if (key == '4') {
        resolution = 4;
    }
    if (key == '5') {
        resolution = 5;
    }
    if (key == '6') {
        resolution = 6;
    }
    if (key == '7') {
        resolution = 7;
    }
    if (key == '8') {
        resolution = 15;
    }
    if (key == '9') {
        resolution = 30;
    }
    if (key == '0') {
        resolution = 45;
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
