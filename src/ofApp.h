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
        /*ofSpherePrimitive sphere;
        ofSpherePrimitive sphere2;
        ofSpherePrimitive sphere3;
        ofSpherePrimitive sphere4;
        ofSpherePrimitive sphere5;*/
    
        ofSpherePrimitive spheres[25];
        int numSpheres = 25;
    
        vector<ofPoint> points;
        ofMesh tempMesh;
        vector<ofVec3f> tempVerts;
    
        ofEasyCam cam;
    
        ofxBeat beat;
        void audioReceived(float*, int, int);
    
        float initTime;
        float endRadius;
        float radius;
    
        float endRotation;
        float rotation;
    
        float resolution;
    
        ofColor colors[5];
    
        ofLight spot;
        ofLight point;
        ofLight dir;
        ofLight amb;
        ofMaterial material;
    
        ofVec3f spot_rot;
		
        void setLightOri(ofLight &light, ofVec3f rot);
    
        void setupCamera();
    
        //ofCamera cam;
        ofBoxPrimitive box;
        float angle;
        ofLight light;
        bool bOrbit, bRoll;
        float angleH, roll, distance;
    
        ofShader shader;
        bool doShader;

    

};
