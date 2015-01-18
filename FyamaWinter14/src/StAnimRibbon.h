#pragma once
#include "ofxState.h"
#include "SharedData.h"
#include "ofxUI.h"
#include "ofApp.h"
#include "ofxPostProcessing.h"
#include "ofxTwistedRibbon.h"

class StAnimRibbon : public itg::ofxState<SharedData>{
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
    
    deque<ofxTwistedRibbon *> ribbons;
    //deque<ofVec3f> ribbonPos;
    deque<float> radius;
    deque<float> spinSpeed;
    deque<ofVec3f> lastPos;
    deque<ofVec3f> speed;
    int maxRibbon;
};