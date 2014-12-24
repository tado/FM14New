#pragma once
#include "ofxState.h"
#include "SharedData.h"
#include "ofxUI.h"
#include "ofApp.h"
#include "SakuraParticle.h"

class StSakuraParticle : public itg::ofxState<SharedData>{
public:
    string getName();
    void setup();
    void update();
    void draw();
    void stateExit();
    void guiEvent(ofxUIEventArgs &e);
    
    deque<SakuraParticle *> particles;
    
    ofxUICanvas *gui;
    ofApp *app;
};