#pragma once
#include "ofxState.h"
#include "SharedData.h"
#include "ofxUI.h"
#include "ofApp.h"
#include "ofxPostProcessing.h"

class StKaleidoscopeSimple : public itg::ofxState<SharedData>{
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
    
    float fftSum;
    ofEasyCam cam;
    ofxPostProcessing *post;
    KaleidoscopePass::Ptr kaleido;
    BloomPass::Ptr bloom;
    float fxRatio;
};