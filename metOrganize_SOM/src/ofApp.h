#pragma once

#include "ofMain.h"
#include "ofxSelfOrganizingMap.h"
#include "ofxMetAPI.h"
#include "ofxHistogram.h"
#include "ofxUI.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
    void metAnalysis();
    vector<float> getPixelAve(ofImage &img);
    vector<float> getHistogram(ofImage &img);
    
    bool isInsideCirc(int xpos, int ypos, int rad, int myx, int myy);
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    ofxSelfOrganizingMap som;
    
    vector<ofPoint> points;

    ofxHistogram histogram;
    
    ofxCvColorImage rgb, hsv;
    ofxCvGrayscaleImage r, g, b;
    ofxCvGrayscaleImage h, s, v;
    
    vector<float> histogramR, histogramG, histogramB;
    vector<float> histogramH, histogramS, histogramV;


    ofxMetAPI myAPI;
    vector<ofxMetAPIItem*> myInfo;
    int thumbSize;
    int numResults;
    int numBins;
    int numIterations;
    
    bool displayAll;
    bool displayRawColor;
    
    ///SET TO DETERMINE MODE
    bool useHist = true;

    
    
    vector<ofImage> imageThumbnails;
    vector< vector<float> > imageInfo;
    
    ofxUICanvas* gui;
    float minDist = 3000;


};
