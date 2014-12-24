#pragma once
#include "ofxState.h"
#include "SharedData.h"
#include "ofxUI.h"
#include "ofApp.h"

class StFftTracer : public itg::ofxState<SharedData>{
public:
    void setup();
    void update();
    void draw();
    string getName();
    
    // ばねのパラメータ
    float stiffness, damping, mass;
    //パーティクルの位置とスピード
    float *magnitude;
    deque<ofVec3f> pos;
    deque<ofVec3f> vel;
    int fft_size;

    ofEasyCam cam;
    ofApp * app;
};
