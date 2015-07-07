#pragma once

#include "ofMain.h"
#include "ofxMetAPI.h"
#include "ofxUI.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void nodeSearchCount();
        void connectionSearchCount();
    void presets();
    
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

    ofxMetAPI myAPI;
    
    vector<string> searchTerms;
    vector<int> connections;
    vector<int> nodes;
    vector<ofVec3f> colors;
    
    float wt;
    float cwt;
    float scaleBias;
    float margin;
    float r1, r2, g1, g2, b1, b2;
    
    ofxUICanvas* gui;
    
    bool fancyLine;
    
    //data labels
    ofTrueTypeFont	ubuntu;
    ofTrueTypeFont ubuntuSmall;
    bool isInsideCirc(int xpos, int ypos, int rad, int myx, int myy);
    
    int myx, myy;
    int highlighted;
    bool searchVersion = true;
};



