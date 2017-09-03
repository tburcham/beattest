#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    //ofSetupOpenGL(1400,768, OF_FULLSCREEN);
    ofGLWindowSettings settings;
    settings.setGLVersion(3,3);
    settings.windowMode = OF_FULLSCREEN;
    
    ofCreateWindow(settings);
    
	//ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
