#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    numIterations = 2000;
    
    metAnalysis();

    if (useHist) {
        int n = numBins * 3;
        double minInstance[n];
        double maxInstance[n];
        for (int i =0; i<n; i++) {
            minInstance[i] = 0;
            maxInstance[i] = 1;
        }
        som.setFeaturesRange(n, minInstance, maxInstance);
    }
    else {
        double minInstance[3] = { 0, 0, 0 };
        double maxInstance[3] = { 255, 255, 255 };
        som.setFeaturesRange(3, minInstance, maxInstance);
    }
    
    
    som.setInitialLearningRate(0.1);
    som.setNumIterations(numIterations);
    
    som.setup();

    gui = new ofxUICanvas();
    gui->addSlider("Line Distance", 0, pow(ofGetWidth()*1.5,2), &minDist);
    gui->addIntSlider("Thumb Size", 10, 100, &thumbSize);
    gui->autoSizeToFitWidgets();


}

//--------------------------------------------------------------
void ofApp::metAnalysis()
{
    string searchTerm = "picasso";
    numResults = 100;
    thumbSize = 100;
    numBins = 5;
    
    myAPI.searchByTerm(searchTerm, numResults);
    myInfo = myAPI.getSearchResults();
    //myAPI.downloadThumbs();
    
    for(int i = 0; i < numResults; i++){
        if(myInfo[i]->image_thumb == "http://metmuseum.org/content/img/placeholders/NoImageAvailable_180x180_frame.png"){
            
        }else{
            ofImage temp;
            temp.loadImage(myInfo[i]->image_thumb);
            imageThumbnails.push_back(temp);
            
            if (useHist) {
                imageInfo.push_back(getHistogram(temp));
            }
            else {
                imageInfo.push_back(getPixelAve(temp));
            }
            
        }
    }

}

//--------------------------------------------------------------
vector<float> ofApp::getHistogram(ofImage &img){
    
    int width = img.width;
    int height = img.height;
    
    rgb.allocate(width, height);
    hsv.allocate(width, height);
    r.allocate(width, height);
    g.allocate(width, height);
    b.allocate(width, height);
    h.allocate(width, height);
    s.allocate(width, height);
    v.allocate(width, height);
    
    
    rgb.setFromPixels(img.getPixelsRef());
    
    // get separate red, green, blue channels
    r.setFromPixels(rgb.getPixelsRef().getChannel(0));
    g.setFromPixels(rgb.getPixelsRef().getChannel(1));
    b.setFromPixels(rgb.getPixelsRef().getChannel(2));
    
    // convert rgb to hsv and grab each channel individually
    cvCvtColor(rgb.getCvImage(), hsv.getCvImage(), CV_BGR2HSV);
    h.setFromPixels(hsv.getPixelsRef().getChannel(0));
    s.setFromPixels(hsv.getPixelsRef().getChannel(1));
    v.setFromPixels(hsv.getPixelsRef().getChannel(2));
    
    // get histograms
    histogramR = histogram.getHistogram(r, numBins);
    histogramG = histogram.getHistogram(g, numBins);
    histogramB = histogram.getHistogram(b, numBins);
    histogramH = histogram.getHistogram(h, numBins);
    histogramS = histogram.getHistogram(s, numBins);
    histogramV = histogram.getHistogram(v, numBins);
    
    vector<float> features;
    
    //features vector will be size 15 if bins is 5.
    for(int i=0; i<histogramH.size(); i++ ){
        features.push_back(histogramR[i]);
        features.push_back(histogramG[i]);
        features.push_back(histogramB[i]);
    }
    
    //cout << "histogram features " << ofToString(features) << endl;
    return features;
    
}

//--------------------------------------------------------------
vector<float> ofApp::getPixelAve(ofImage &img){
    
    float rr = 0;
    float gg = 0;
    float bb = 0;
    
    float rAve = 0;
    float gAve = 0;
    float bAve = 0;
    
    //read in all the pixels from the image
    for(int x = 0; x < img.width; x++){
        for(int y = 0; y < img.height; y++){
            
            rr += img.getColor(x, y).r;
            gg += img.getColor(x, y).g;
            bb += img.getColor(x, y).b;
            
        }
    }
    
    rAve = rr / (img.width*img.height);
    gAve = gg / (img.width*img.height);
    bAve = bb / (img.width*img.height);
    
    //cout<< rAve << " " << gAve << " " << bAve << endl;
    
    vector< float> features;
    
    features.push_back(rAve);
    features.push_back(gAve);
    features.push_back(bAve);
    
    
    return features;
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    cout << "frame : " << ofGetFrameNum() << endl;
    if (ofGetFrameNum() == numIterations) {
        cout << "find best matching" << endl;
        
        
        for(int i = 0; i<imageInfo.size(); i++){
            
            double arrayTemp[imageInfo[i].size()];
            
            for(int c = 0; c < imageInfo[i].size(); c++){
                arrayTemp[c] = imageInfo[i][c];
            }
            
            ofPoint best = som.findBestMatchingCell(arrayTemp);
            cout <<  " --> best " << ofToString(best) << endl;
            
            ofPoint idx;
            idx.x = ofMap(best.x, 0, 200, 0, ofGetWidth());
            idx.y = ofMap(best.y, 0, 200, 0, ofGetHeight());
            //idx.z = 0;
            points.push_back(idx);
            
            cout << "i = "<<i<< " "<< points[i] << endl;
        }
        
    }
    else if (ofGetFrameNum() < numIterations) {
    
        int randomFeatureNum = ofRandom(0, imageInfo.size());
        
        double arrayTemp[imageInfo[randomFeatureNum].size()];
        
        for(int c = 0; c < imageInfo[randomFeatureNum].size(); c++){
            arrayTemp[c] = imageInfo[randomFeatureNum][c];
        }
        
        som.updateMap(arrayTemp);
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    ofBackground(0);
    
    if (ofGetFrameNum() > numIterations) {
        
        ofFill();
        ofSetColor(255,100);
        for(int i = 0; i<imageInfo.size(); i++){
            ofEllipse(points[i].x, points[i].y, points[i].z, 5, 5);
            
        }
        
        
        for(int i = 0; i < imageInfo.size(); i++){
            for(int j = i+1; j < imageInfo.size(); j++){
                float  dist = pow(points[i].x - points[j].x, 2) + pow(points[i].y - points[j].y, 2) + pow(points[i].z - points[j].z, 2);
                
                if (dist < minDist) {
                ofSetColor(255, 50);
                ofSetLineWidth(0.5);
                ofLine(points[i].x, points[i].y, points[i].z, points[j].x, points[j].y, points[j].z);
                }
                
            }
        }
        
        //data labels - in this case are the images
        for(int i = 0; i < imageInfo.size(); i++){
            int x = points[i].x;
            int y = points[i].y;
            
            if(isInsideCirc(x,y, 30, ofGetMouseX(), ofGetMouseY()) || displayAll){
                
                if (displayRawColor) {
                    ofSetColor(imageInfo[i][0], imageInfo[i][1], imageInfo[i][2]);
                    ofCircle(points[i].x, points[i].y, 8);
                }
                else {
                    ofSetColor(255);
                    imageThumbnails[i].draw(points[i].x, points[i].y, thumbSize, thumbSize);
                }
            }else{
                
            }
            
        }
        ofSetColor(255);
        
        
    }
    
    else {
        ofFill();
        for (int i=0; i<200; i++) {
            for (int j=0; j<200; j++) {
                int x = ofMap(i, 0, 200, 0, ofGetWidth());
                int y = ofMap(j, 0, 200, 0, ofGetHeight());
                
                double *c = som.getMapAt(i,j);
                ofSetColor(c[0], c[1], c[2]);
                ofCircle(x, y, 4);
                
            }
        }
    }
    
}

//--------------------------------------------------------------
bool ofApp::isInsideCirc(int xpos, int ypos, int rad, int myx, int myy){
    
    if(ofDist(xpos, ypos, myx, myy) < rad){
        return true;
    }else{
        return false;
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key==' ') displayAll=!displayAll;
    if (key=='1') displayRawColor=!displayRawColor;
    if (key == 'x'){
        gui->toggleVisible();
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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){


}
