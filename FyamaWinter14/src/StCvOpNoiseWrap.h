#pragma once
#include "ofxState.h"
#include "SharedData.h"
#include "ofxUI.h"
#include "ofxCv.h"
#include "ofApp.h"
#include "ofxPostProcessing.h"

class StCvOpNoiseWrap : public itg::ofxState<SharedData>{
public:
    string getName();
    void setup();
    void update();
    void draw();
    void stateExit();
    void guiEvent(ofxUIEventArgs &e);
    
    ofxUICanvas *gui;
    ofApp *app;
    
    float fftSum;
    ofEasyCam cam;
    ofxPostProcessing post;
    NoiseWarpPass::Ptr noise;
    BloomPass::Ptr bloom;
    
    ofxCv::FlowFarneback flow;
    
    int cvWidth, cvHeight;
    float pyrScale;
    int levels;
    int winsize;
    int iterations;
    int polyN;
    float polySigma;
    bool OPTFLOW_FARNEBACK_GAUSSIAN;
    
    float currentNoiseFreq;
};