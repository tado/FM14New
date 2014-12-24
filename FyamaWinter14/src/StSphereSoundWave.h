#pragma once
#include "ofxState.h"
#include "SharedData.h"
#include "ofxUI.h"
#include "ofApp.h"

class StSphereSoundWave : public itg::ofxState<SharedData>{
public:
    string getName();
    void setup();
    void update();
    void draw();
    void stateEnter();
    void stateExit();
    void guiEvent(ofxUIEventArgs &e);
    
    ofxUICanvas *gui;
    ofApp *app;
    ofEasyCam cam;
    ofFbo fbo;
    ofSpherePrimitive sphere;
    ofImage img;
};