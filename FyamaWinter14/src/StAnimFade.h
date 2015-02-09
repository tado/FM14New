#pragma once
#include "ofxState.h"
#include "SharedData.h"
#include "ofxUI.h"
#include "ofApp.h"
#include "ofxPostProcessing.h"
#include "ofxTwistedRibbon.h"

class SlideRect {
public:
    SlideRect(float _speed) {
        loc = ofVec2f(ofRandom(ofGetWidth()), 0);
        speed = _speed;
        rectWidth = ofRandom(ofGetWidth()/12, ofGetWidth()/8);
        alpha = 200;
        col.setHsb(ofRandom(255), 255, 180, alpha);
    }
    
    void update(){
        loc.x += speed;
        if (loc.x < -rectWidth || loc.x > ofGetWidth() + rectWidth) {
            speed *= -1;
        }
    }
    
    void draw(){
        ofSetColor(col);
        ofRect(loc, rectWidth, ofGetHeight());
    }

    ofVec2f loc;
    float speed;
    float rectWidth;
    int alpha;
    ofColor col;
};

class StAnimFade : public itg::ofxState<SharedData>{
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
    ofColor baseColor;
    float fxRatio;
    
    deque<SlideRect * > rects;
    int rectNum;
};