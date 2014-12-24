#pragma once
#include "ofxState.h"
#include "SharedData.h"
#include "ofxUI.h"
#include "ofApp.h"
#include "ofxPostProcessing.h"
#include "ofxTwistedRibbon.h"

class StFftSphereRibbon : public itg::ofxState<SharedData>{
public:
    string getName();
    void setup();
    void update();
    void draw();
    void stateEnter();
    void stateExit();
    void createMesh();
    void guiEvent(ofxUIEventArgs &e);
    
    ofxUICanvas *gui;
    ofApp *app;
    ofEasyCam cam;
    
    ofSpherePrimitive sphere;
    ofMesh mesh;
    float stepSize, xSteps, ySteps;
    vector<ofVec3f> currentVertex;
    vector<ofVec3f> interplateVertex;
    vector<ofVec3f> randomVec;
    vector<ofxTwistedRibbon *> ribbons;
    ofxPostProcessing post;
};