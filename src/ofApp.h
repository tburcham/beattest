#pragma once

#include "ofMain.h"
#include "ofxBeat.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        ofShader shaderBlurX;
        ofShader shaderBlurY;
    
        ofFbo fboBlurOnePass;
        ofFbo fboBlurTwoPass;
    
        ofPlanePrimitive plane;
        ofSpherePrimitive sphere;
        ofSpherePrimitive sphere2;
        ofSpherePrimitive sphere3;
        ofSpherePrimitive sphere4;
        ofSpherePrimitive sphere5;
    
        ofxBeat beat;
        void audioReceived(float*, int, int);
    
        float initTime;
        float endRadius;
        float radius;
    
        float endRotation;
        float rotation;
    
        float resolution;
		
};
