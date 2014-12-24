#pragma once
#include "ofxState.h"
#include "SharedData.h"
#include "ofxUI.h"
#include "ofxCv.h"
#include "NoteParticle.h"
#include "ofApp.h"
#include "ofxPostProcessing.h"

class StCvOpSparkle : public itg::ofxState<SharedData>{
public:
    string getName();
    void setup();
    void update();
    void draw();
    void guiEvent(ofxUIEventArgs &e);
    void stateExit();
    
    ofPixels pixels;
    ofTexture tex;
    ofxUICanvas *gui;
    
    ofxCv::FlowFarneback flow;
    
    int cvWidth, cvHeight;
    float pyrScale;
    int levels;
    int winsize;
    int iterations;
    int polyN;
    float polySigma;
    bool OPTFLOW_FARNEBACK_GAUSSIAN;
    
    // Particle
    deque<NoteParticle *> particles;
    static const int NUM = 100;
    ofImage img[4];
    
    ofxPostProcessing post;
    BloomPass::Ptr bloom;
    
    ofApp *app;
};