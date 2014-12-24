#pragma once
#include "ofxState.h"
#include "SharedData.h"
#include "ofxUI.h"
#include "ofApp.h"
#include "ofxTwistedRibbon.h"

class StFftRibbon : public itg::ofxState<SharedData>{
public:
    void setup();
    void update();
    void draw();
    void stateExit();
    string getName();
    void guiEvent(ofxUIEventArgs &e);
    
    // ばねのパラメータ
    float stiffness, damping, mass;
    //パーティクルの位置とスピード
    float *magnitude;
    deque<ofVec3f> pos;
    deque<ofVec3f> vel;
    deque<ofVec3f> vec;
    int fft_size;
    
    ofxUICanvas *gui;

    ofEasyCam cam;
    ofApp * app;
    vector<ofxTwistedRibbon *> ribbons;
};
