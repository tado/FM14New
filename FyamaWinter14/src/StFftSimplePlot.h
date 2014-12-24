#pragma once
#include "ofxState.h"
#include "SharedData.h"
#include "ofxUI.h"
#include "ofApp.h"

class StFftSimplePlot : public itg::ofxState<SharedData>{
public:
    string getName();
    void setup();
    void update();
    void draw();
    void plot(vector<float>& buffer, float scale, float offset);
    void stateExit();
    void guiEvent(ofxUIEventArgs &e);
    
    ofxUICanvas *gui;
    ofApp *app;
};