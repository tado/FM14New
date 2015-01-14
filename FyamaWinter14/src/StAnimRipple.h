#pragma once
#include "ofxState.h"
#include "SharedData.h"
#include "ofxUI.h"
#include "ofApp.h"
#include "ofxPostProcessing.h"

class Ripple {
public:
    Ripple(ofVec2f _loc, float _zoomSpeed){
        loc = _loc;
        zoomSpeed = _zoomSpeed;
        radius = 0;
        col.setHsb(ofRandom(255), 255, 255, 220);
    }
    
    void update(){
        radius += zoomSpeed;
    }
    
    void draw(){
        ofSetCircleResolution(64);
        ofSetColor(col);
        ofCircle(loc, radius);
    }
    
    ofVec2f loc;
    float radius;
    float zoomSpeed;
    ofColor col;
};


class StAnimRipple : public itg::ofxState<SharedData>{
public:
    string getName();
    void setup();
    void update();
    void draw();
    void stateExit();
    void guiEvent(ofxUIEventArgs &e);
    
    ofxUICanvas *gui;
    ofApp *app;
    ofEasyCam cam;
    ofxPostProcessing post;
    BloomPass::Ptr bloom;
    
    deque<Ripple *> ripples;
};