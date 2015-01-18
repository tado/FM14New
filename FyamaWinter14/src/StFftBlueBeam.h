#pragma once
#include "ofxState.h"
#include "SharedData.h"
#include "ofxUI.h"
#include "ofxPostProcessing.h"
#include "ofxTwistedRibbon.h"
#include "ofApp.h"

class StFftBlueBeam : public itg::ofxState<SharedData>{
public:
    string getName();
    void setup();
    void update();
    void draw();
    void stateExit();
    void stateEnter();
    void createMesh();
    void guiEvent(ofxUIEventArgs &e);
    
    ofxUICanvas *gui;
    ofApp *app;
    
    ofEasyCam cam;
    vector<ofxTwistedRibbon *> ribbons;
    vector<ofVec3f> position;
    vector<ofVec3f> velocity;
    ofxPostProcessing *post;
    static const int NUM = 128;
};