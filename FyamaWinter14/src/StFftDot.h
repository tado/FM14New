#pragma once
#include "ofxState.h"
#include "SharedData.h"
#include "ofxUI.h"
#include "ofApp.h"

class StFftDot : public itg::ofxState<SharedData>{
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
};