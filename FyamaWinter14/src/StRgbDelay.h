#pragma once
#include "ofxState.h"
#include "SharedData.h"
#include "ofxUI.h"
#include "ofApp.h"

class StRgbDelay : public itg::ofxState<SharedData>{
public:
    string getName();
    void setup();
    void update();
    void draw();
    void stateExit();
    void guiEvent(ofxUIEventArgs &e);
    
    ofTexture tex;
    deque<ofTexture> texBuffer;
    ofxUICanvas *gui;
    ofApp *app;
};