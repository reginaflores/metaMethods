#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ubuntu.loadFont("Ubuntu-LI.ttf", 20, true, true);
    ubuntuSmall.loadFont("Ubuntu-LI.ttf", 10, true, true);
    
    if(searchVersion == false){
        //search 2
        searchTerms.push_back("anger");
        searchTerms.push_back("sad");
        searchTerms.push_back("angst");
        searchTerms.push_back("pain");
        searchTerms.push_back("pride");
        searchTerms.push_back("hurt");
        searchTerms.push_back("tears");
        searchTerms.push_back("war");

        searchTerms.push_back("happy");
        searchTerms.push_back("smile");
        searchTerms.push_back("joy");
        searchTerms.push_back("peace");
        searchTerms.push_back("cheer");
        searchTerms.push_back("blessing");
        searchTerms.push_back("love");
        
        searchTerms.push_back("painting");
        searchTerms.push_back("drawing");
        searchTerms.push_back("sculpture");
        searchTerms.push_back("etching");
        searchTerms.push_back("photograph");
        searchTerms.push_back("textile");
        searchTerms.push_back("ceramic");

        searchTerms.push_back("europe");
        searchTerms.push_back("africa");
        searchTerms.push_back("asia");
        searchTerms.push_back("america");
        searchTerms.push_back("australia");
        
        searchTerms.push_back("black");
        searchTerms.push_back("white");
    }else if (searchVersion == true){
    
    //search 1
    searchTerms.push_back("red");
    searchTerms.push_back("orange");
    searchTerms.push_back("yellow");
    searchTerms.push_back("blue");
    searchTerms.push_back("green");
    searchTerms.push_back("violet");
    searchTerms.push_back("indigo");
    searchTerms.push_back("black");
    searchTerms.push_back("white");
    
    searchTerms.push_back("europe");
    searchTerms.push_back("egypt");
    searchTerms.push_back("africa");
    searchTerms.push_back("asia");
    searchTerms.push_back("mexico");
    searchTerms.push_back("china");
    
    searchTerms.push_back("painting");
    searchTerms.push_back("drawing");
    searchTerms.push_back("sculpture");
    searchTerms.push_back("etching");
    searchTerms.push_back("photograph");
    searchTerms.push_back("textile");
    searchTerms.push_back("ceramic");

    searchTerms.push_back("love");
    searchTerms.push_back("war");
    searchTerms.push_back("human");
    searchTerms.push_back("happy");
    searchTerms.push_back("gold");
    }
     
    for(int i = 0; i < searchTerms.size(); i++){
        colors.push_back(ofVec3f(ofRandom(1), ofRandom(1), ofRandom(1)));
    }
    
    /////////////////////////////////
    //this is for debugging -- limit server requests
    bool runPreset = true;
    if(runPreset == true){
        presets();
    }else{
        nodeSearchCount();
        connectionSearchCount();
    }
    /////////////////////////////////

    
    wt = 1;
    cwt = 1;
    scaleBias =1;
    fancyLine = false;
    margin = 10;
    
    r1 = 0;
    g1 = 0;
    b1 = 0;
    r2 = 255;
    g2 = 255;
    b2 = 255;
    
    gui = new ofxUICanvas();
    
    gui->addSlider("Node Weight", 0, 20, &wt);
    gui->addSlider("Node Scale", 0, 1, &scaleBias);
    gui->addSlider("Connection Weight", 1, 10, &cwt);
    
    gui->addRangeSlider("Red", 0.0, 255.0, &r1, &r2);
    gui->addRangeSlider("Green", 0.0, 255.0, &g1, &g2);
    gui->addRangeSlider("Blue", 0.0, 255.0, &b1, &b2);
    gui->addToggle("Solid Line", &fancyLine);
    gui->addSlider("Margin", 0, 50, &margin);
    gui->autoSizeToFitWidgets();
    
}
//--------------------------------------------------------------
void ofApp::nodeSearchCount(){
    
    int numResults = 1000000;
    
    for(int i = 0 ; i < searchTerms.size(); i++){
        string searchTerm = searchTerms[i];
        nodes.push_back(myAPI.numSearchResults(searchTerm));
        ofSleepMillis(5000);
    }
    
    //this is for debugging -- limit server requests
    for (auto n : nodes) {
        cout << "nodes.push_back("<<n<<");"<<endl;
    }
    
}
//--------------------------------------------------------------

void ofApp::connectionSearchCount(){

    connections.clear();
    
    
    for(int i =0; i <searchTerms.size(); i++){
        for(int j = i+1; j < searchTerms.size(); j++){
            string searchTerm = searchTerms[i]+"%20"+searchTerms[j];
            //cout<< "connection string " << searchTerm << endl;
            int n = myAPI.numSearchResults(searchTerm);
            //cout << "number of connections are " << n << endl;
            ofSleepMillis(5000);
            connections.push_back(n);
        }

    }
    
    //this is for debugging -- limit server requests
    for (auto c : connections) {
        cout << "connections.push_back("<<c<<");"<<endl;
    }

}


//--------------------------------------------------------------
void ofApp::update(){

    
}

//--------------------------------------------------------------
void ofApp::draw(){

    
    //float wt = ofMap(mouseX, 0, ofGetWidth(), 0, 0.2);
    ofSetColor(255);
    ofBackground(0);
    int radius = 300;
    ofPoint center(ofGetWidth()/2, ofGetHeight()/2);
    float theta = 0;
    int x = 0;
    int y = 0;

    

    highlighted=-1;
    for(int i = 0; i < searchTerms.size(); i++){
        theta = ofMap(i, 0, searchTerms.size(), 0, TWO_PI);
        x = center.x + radius * cos(theta);
        y = center.y + radius * sin(theta);
        
        ofSetColor(ofMap(colors[i].x, 0, 1, r1, r2),
                   ofMap(colors[i].y, 0, 1, g1, g2),
                   ofMap(colors[i].z, 0, 1, b1, b2),
                   200);
                   
        //float nodeScale = ofMap(nodes[i], 1, 300000, 1*wt, 10*wt);
        float nodeScale = log(1+nodes[i]*scaleBias)*wt;
        //float nodeScale = log(1+nodes[i]*scaleBias)*wt;
        
        ofCircle(x, y, nodeScale);
        
        
        if(isInsideCirc(x,y, nodeScale, myx, myy)){
            highlighted=i;
        }

        
    }
    
    
    float theta1, theta2;
    int x1, x2, y1, y2;
    int idx = 0;
    for(int i =0; i <searchTerms.size(); i++){
        for(int j = i+1; j < searchTerms.size(); j++){
            
            if(highlighted==-1 || (i==highlighted || j==highlighted)){
            
            
                theta1 = ofMap(i, 0, searchTerms.size(), 0, TWO_PI);
                x1 = center.x + radius * cos(theta1);
                y1 = center.y + radius * sin(theta1);

                theta2 = ofMap(j, 0, searchTerms.size(), 0, TWO_PI);
                x2 = center.x + radius * cos(theta2);
                y2 = center.y + radius * sin(theta2);
                
                //weight of line
                //ofSetColor(0, 100);
                ofSetColor(ofMap(colors[i].x, 0, 1, r1, r2),
                           ofMap(colors[i].y, 0, 1, g1, g2),
                           ofMap(colors[i].z, 0, 1, b1, b2),
                           100);

                
                int connectionScale = ofMap(connections[idx], 1, 5000, 1, 10);
                
                if(fancyLine == true){
                    for(int l=0; l < cwt*connectionScale; l++){
                        ofSetLineWidth(1);
                        ofLine(x1+ofSignedNoise(i,l,5)*margin, y1+ofSignedNoise(i,l,10)*margin, x2+ofSignedNoise(i,l,15)*margin, y2+ofSignedNoise(i,l,20)*margin);
                        //ofLine(x1+ofRandom(-5,5), y1+ofRandom(-5,5), x2+ofRandom(-5,5), y2+ofRandom(-5,5));
                    }
                }else{
                    ofSetLineWidth(connectionScale*cwt);
                    ofLine(x1, y1, x2, y2);
                }
            
            }
            idx++;
        }
    }
    
    
    //data labels
    for(int i = 0; i < searchTerms.size(); i++){
        myx = ofGetMouseX();
        myy = ofGetMouseY();
        theta = ofMap(i, 0, searchTerms.size(), 0, TWO_PI);
        x = center.x + radius * cos(theta);
        y = center.y + radius * sin(theta);
        float nodeScale = log(1+nodes[i]*scaleBias)*wt;
        
        if(isInsideCirc(x,y, nodeScale, myx, myy)){
    
            ofSetColor(255);
            ubuntu.drawString(searchTerms[i], x,y);
        }else if(highlighted != i && highlighted !=-1){
            ofSetColor(255);
            ubuntuSmall.drawString(searchTerms[i], x,y);
        }
    
    }

}
//--------------------------------------------------------------

void ofApp:: presets(){
    
    if(searchVersion == false){
        //search 2
        nodes.push_back(90);
        nodes.push_back(90);
        nodes.push_back(17);
        nodes.push_back(531);
        nodes.push_back(90);
        nodes.push_back(12);
        nodes.push_back(90);
        nodes.push_back(2880);
        nodes.push_back(462);
        nodes.push_back(90);
        nodes.push_back(90);
        nodes.push_back(528);
        nodes.push_back(74);
        nodes.push_back(617);
        nodes.push_back(1677);
        nodes.push_back(31928);
        nodes.push_back(33667);
        nodes.push_back(27674);
        nodes.push_back(32997);
        nodes.push_back(41138);
        nodes.push_back(34774);
        nodes.push_back(29791);
        nodes.push_back(165660);
        nodes.push_back(35155);
        nodes.push_back(69785);
        nodes.push_back(101225);
        nodes.push_back(90);
        nodes.push_back(18763);
        nodes.push_back(13994);
        
        connections.push_back(1);
        connections.push_back(0);
        connections.push_back(5);
        connections.push_back(6);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(1);
        connections.push_back(3);
        connections.push_back(1);
        connections.push_back(1);
        connections.push_back(2);
        connections.push_back(13);
        connections.push_back(54);
        connections.push_back(43);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(3);
        connections.push_back(90);
        connections.push_back(5);
        connections.push_back(13);
        connections.push_back(23);
        connections.push_back(0);
        connections.push_back(20);
        connections.push_back(14);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(4);
        connections.push_back(15);
        connections.push_back(9);
        connections.push_back(3);
        connections.push_back(6);
        connections.push_back(7);
        connections.push_back(3);
        connections.push_back(4);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(13);
        connections.push_back(25);
        connections.push_back(4);
        connections.push_back(2);
        connections.push_back(56);
        connections.push_back(3);
        connections.push_back(40);
        connections.push_back(33);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(1);
        connections.push_back(1);
        connections.push_back(0);
        connections.push_back(1);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(4);
        connections.push_back(1);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(14);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(2);
        connections.push_back(0);
        connections.push_back(4);
        connections.push_back(16);
        connections.push_back(5);
        connections.push_back(2);
        connections.push_back(8);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(90);
        connections.push_back(35);
        connections.push_back(18);
        connections.push_back(40);
        connections.push_back(36);
        connections.push_back(11);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(32);
        connections.push_back(22);
        connections.push_back(2);
        connections.push_back(2);
        connections.push_back(9);
        connections.push_back(5);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(9);
        connections.push_back(71);
        connections.push_back(22);
        connections.push_back(19);
        connections.push_back(4);
        connections.push_back(11);
        connections.push_back(6);
        connections.push_back(4);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(7);
        connections.push_back(12);
        connections.push_back(0);
        connections.push_back(2);
        connections.push_back(1);
        connections.push_back(1);
        connections.push_back(1);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(6);
        connections.push_back(1);
        connections.push_back(1);
        connections.push_back(1);
        connections.push_back(1);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(5);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(8);
        connections.push_back(6);
        connections.push_back(4);
        connections.push_back(5);
        connections.push_back(3);
        connections.push_back(1);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(13);
        connections.push_back(17);
        connections.push_back(7);
        connections.push_back(6);
        connections.push_back(8);
        connections.push_back(62);
        connections.push_back(5);
        connections.push_back(20);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(12);
        connections.push_back(7);
        connections.push_back(11);
        connections.push_back(78);
        connections.push_back(13);
        connections.push_back(9);
        connections.push_back(81);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(34);
        connections.push_back(1157);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(1039);
        connections.push_back(2);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(46);
        connections.push_back(45);
        connections.push_back(90);
        connections.push_back(28);
        connections.push_back(38);
        connections.push_back(27);
        connections.push_back(25);
        connections.push_back(37);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(80);
        connections.push_back(0);
        connections.push_back(38);
        connections.push_back(48);
        connections.push_back(5);
        connections.push_back(6);
        connections.push_back(0);
        connections.push_back(5);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(5);
        connections.push_back(33);
        connections.push_back(3);
        connections.push_back(8);
        connections.push_back(81);
        connections.push_back(16);
        connections.push_back(40);
        connections.push_back(59);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(1);
        connections.push_back(23);
        connections.push_back(26);
        connections.push_back(71);
        connections.push_back(36);
        connections.push_back(23);
        connections.push_back(14);
        connections.push_back(27);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(56);
        connections.push_back(66);
        connections.push_back(2);
        connections.push_back(22);
        connections.push_back(22);
        connections.push_back(2);
        connections.push_back(28);
        connections.push_back(25);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(77);
        connections.push_back(30);
        connections.push_back(34);
        connections.push_back(47);
        connections.push_back(90);
        connections.push_back(19);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(3);
        connections.push_back(13);
        connections.push_back(20);
        connections.push_back(15);
        connections.push_back(3);
        connections.push_back(3);
        connections.push_back(14);
        connections.push_back(9);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(27);
        connections.push_back(1);
        connections.push_back(8);
        connections.push_back(12);
        connections.push_back(32);
        connections.push_back(90);
        connections.push_back(88);
        connections.push_back(84);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(90);
        connections.push_back(39);
        connections.push_back(90);
        connections.push_back(84);
        connections.push_back(0);
        connections.push_back(71);
        connections.push_back(71);
        connections.push_back(478);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(0);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(42);
        connections.push_back(972);
        connections.push_back(52);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(7);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(5200);
        connections.push_back(3897);
        connections.push_back(1289);
        connections.push_back(1856);
        connections.push_back(1117);
        connections.push_back(5914);
        connections.push_back(10696);
        connections.push_back(4949);
        connections.push_back(9392);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(867);
        connections.push_back(2334);
        connections.push_back(1344);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(16549);
        connections.push_back(90);
        connections.push_back(970);
        connections.push_back(11906);
        connections.push_back(49);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(90);
        connections.push_back(499);
        connections.push_back(90);
        connections.push_back(1266);
        connections.push_back(8528);
        connections.push_back(8704);
        connections.push_back(5742);
        connections.push_back(2838);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(679);
        connections.push_back(64);
        connections.push_back(14);
        connections.push_back(0);
        connections.push_back(55);
        connections.push_back(90);
        connections.push_back(3713);
        connections.push_back(37);
        connections.push_back(1063);
        connections.push_back(90);
        connections.push_back(75);
        connections.push_back(35);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(26483);
        connections.push_back(26);
        connections.push_back(2176);
        connections.push_back(1315);
        connections.push_back(75);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(90);
        connections.push_back(8775);
        connections.push_back(4021);
        connections.push_back(12808);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(553);
        connections.push_back(938);
        connections.push_back(0);
        connections.push_back(48);
        connections.push_back(8980);
        connections.push_back(4703);
        connections.push_back(90);
        connections.push_back(503);
        connections.push_back(9);
        connections.push_back(488);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(2482);
        connections.push_back(90);
        connections.push_back(5723);
        connections.push_back(4670);
        connections.push_back(13);
        connections.push_back(0);
        connections.push_back(3877);
    } else if (searchVersion == true){
    
        //search 1
        nodes.push_back(11271);
        nodes.push_back(1269);
        nodes.push_back(3294);
        nodes.push_back(12982);
        nodes.push_back(7172);
        nodes.push_back(1548);
        nodes.push_back(90);
        nodes.push_back(18763);
        nodes.push_back(13994);
        nodes.push_back(165660);
        nodes.push_back(33987);
        nodes.push_back(35155);
        nodes.push_back(69785);
        nodes.push_back(2752);
        nodes.push_back(18345);
        nodes.push_back(31928);
        nodes.push_back(33667);
        nodes.push_back(27674);
        nodes.push_back(32997);
        nodes.push_back(41138);
        nodes.push_back(34774);
        nodes.push_back(29791);
        nodes.push_back(1677);
        nodes.push_back(2880);
        nodes.push_back(1907);
        nodes.push_back(462);
        nodes.push_back(19045);

        connections.push_back(90);
        connections.push_back(807);
        connections.push_back(2230);
        connections.push_back(1128);
        connections.push_back(76);
        connections.push_back(9);
        connections.push_back(2545);
        connections.push_back(1570);
        connections.push_back(4940);
        connections.push_back(669);
        connections.push_back(667);
        connections.push_back(1760);
        connections.push_back(44);
        connections.push_back(558);
        connections.push_back(1918);
        connections.push_back(3153);
        connections.push_back(635);
        connections.push_back(536);
        connections.push_back(941);
        connections.push_back(90);
        connections.push_back(1141);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(29);
        connections.push_back(928);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(16);
        connections.push_back(0);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(20);
        connections.push_back(26);
        connections.push_back(76);
        connections.push_back(12);
        connections.push_back(22);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(60);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(55);
        connections.push_back(7);
        connections.push_back(16);
        connections.push_back(11);
        connections.push_back(3);
        connections.push_back(90);
        connections.push_back(899);
        connections.push_back(1080);
        connections.push_back(90);
        connections.push_back(2);
        connections.push_back(702);
        connections.push_back(675);
        connections.push_back(877);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(618);
        connections.push_back(15);
        connections.push_back(90);
        connections.push_back(544);
        connections.push_back(614);
        connections.push_back(90);
        connections.push_back(84);
        connections.push_back(88);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(30);
        connections.push_back(20);
        connections.push_back(28);
        connections.push_back(8);
        connections.push_back(90);
        connections.push_back(2150);
        connections.push_back(65);
        connections.push_back(20);
        connections.push_back(2217);
        connections.push_back(2086);
        connections.push_back(4853);
        connections.push_back(1441);
        connections.push_back(1421);
        connections.push_back(2791);
        connections.push_back(45);
        connections.push_back(1430);
        connections.push_back(2633);
        connections.push_back(2515);
        connections.push_back(661);
        connections.push_back(90);
        connections.push_back(668);
        connections.push_back(90);
        connections.push_back(4114);
        connections.push_back(90);
        connections.push_back(81);
        connections.push_back(80);
        connections.push_back(34);
        connections.push_back(0);
        connections.push_back(90);
        connections.push_back(4);
        connections.push_back(932);
        connections.push_back(824);
        connections.push_back(1711);
        connections.push_back(790);
        connections.push_back(735);
        connections.push_back(1157);
        connections.push_back(35);
        connections.push_back(536);
        connections.push_back(870);
        connections.push_back(887);
        connections.push_back(526);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(993);
        connections.push_back(77);
        connections.push_back(59);
        connections.push_back(61);
        connections.push_back(19);
        connections.push_back(452);
        connections.push_back(2);
        connections.push_back(90);
        connections.push_back(601);
        connections.push_back(90);
        connections.push_back(83);
        connections.push_back(84);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(90);
        connections.push_back(706);
        connections.push_back(23);
        connections.push_back(39);
        connections.push_back(68);
        connections.push_back(18);
        connections.push_back(37);
        connections.push_back(11);
        connections.push_back(5);
        connections.push_back(6);
        connections.push_back(5);
        connections.push_back(23);
        connections.push_back(11);
        connections.push_back(26);
        connections.push_back(9);
        connections.push_back(3);
        connections.push_back(34);
        connections.push_back(57);
        connections.push_back(9);
        connections.push_back(8);
        connections.push_back(37);
        connections.push_back(9);
        connections.push_back(27);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(1);
        connections.push_back(1);
        connections.push_back(0);
        connections.push_back(12);
        connections.push_back(3877);
        connections.push_back(8995);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(1770);
        connections.push_back(83);
        connections.push_back(90);
        connections.push_back(2437);
        connections.push_back(10093);
        connections.push_back(870);
        connections.push_back(1063);
        connections.push_back(2078);
        connections.push_back(90);
        connections.push_back(1103);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(686);
        connections.push_back(2608);
        connections.push_back(4660);
        connections.push_back(866);
        connections.push_back(90);
        connections.push_back(1252);
        connections.push_back(90);
        connections.push_back(2261);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(47);
        connections.push_back(656);
        connections.push_back(825);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(10689);
        connections.push_back(0);
        connections.push_back(8528);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(972);
        connections.push_back(1157);
        connections.push_back(687);
        connections.push_back(90);
        connections.push_back(5905);
        connections.push_back(30913);
        connections.push_back(90);
        connections.push_back(2);
        connections.push_back(9);
        connections.push_back(5033);
        connections.push_back(90);
        connections.push_back(7621);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(2054);
        connections.push_back(4787);
        connections.push_back(48);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(15);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(55);
        connections.push_back(89);
        connections.push_back(2408);
        connections.push_back(4021);
        connections.push_back(52);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(21);
        connections.push_back(1367);
        connections.push_back(9);
        connections.push_back(17717);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(90);
        connections.push_back(9511);
        connections.push_back(12808);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(90);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(90);
        connections.push_back(66);
        connections.push_back(777);
        connections.push_back(9);
        connections.push_back(19);
        connections.push_back(45);
        connections.push_back(1);
        connections.push_back(35);
        connections.push_back(4598);
        connections.push_back(90);
        connections.push_back(923);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(77);
        connections.push_back(90);
        connections.push_back(63);
        connections.push_back(74);
        connections.push_back(791);
        connections.push_back(5200);
        connections.push_back(3897);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(478);
        connections.push_back(90);
        connections.push_back(492);
        connections.push_back(90);
        connections.push_back(2640);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(1344);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(28);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(499);
        connections.push_back(90);
        connections.push_back(1266);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(75);
        connections.push_back(35);
        connections.push_back(0);
        connections.push_back(637);
        connections.push_back(90);
        connections.push_back(25);
        connections.push_back(90);
        connections.push_back(75);
        connections.push_back(90);
        connections.push_back(90);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(983);
        connections.push_back(42);
        connections.push_back(34);
        connections.push_back(90);
        connections.push_back(50);
        connections.push_back(894);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(0);
        connections.push_back(90);
        connections.push_back(12);
        connections.push_back(90);
        connections.push_back(16);
        connections.push_back(90);
        connections.push_back(48);
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
    
    if(key == 'x'){
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
