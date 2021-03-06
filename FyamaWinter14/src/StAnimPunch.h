#pragma once
#include "ofxState.h"
#include "SharedData.h"
#include "ofxUI.h"
#include "ofApp.h"
#include "ofxPostProcessing.h"

class Punch {
public:
    Punch(ofVec2f _loc, float _zoomSpeed, int _hue){
        loc = _loc;
        zoomSpeed = _zoomSpeed;
        radius = 0;
        col.setHsb(_hue, 255, 255);
    }
    
    void update(){
        radius += zoomSpeed;
    }
    
    void draw(){
        ofSetCircleResolution(64);
        col.setHsb(col.getHue(), col.getSaturation(), col.getBrightness(), 200);
        ofSetColor(col);
        ofCircle(loc, radius);
    }
    
    ofVec2f loc;
    float radius;
    float zoomSpeed;
    ofColor col;
    int alpha;
};


class StAnimPunch : public itg::ofxState<SharedData>{
public:
    string getName();
    void setup();
    void update();
    void draw();
    void stateExit();
    void stateEnter();
    void guiEvent(ofxUIEventArgs &e);
    
    ofxUICanvas *gui;
    ofApp *app;
    ofEasyCam cam;
    ofxPostProcessing *post;
    BloomPass::Ptr bloom;
    float fxRatio;
    
    deque<Punch *> punch;
};