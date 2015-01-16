#pragma once
#include "ofxState.h"
#include "SharedData.h"
#include "ofxUI.h"
#include "ofApp.h"
#include "ofxPostProcessing.h"

class ZoomObject {
public:
    ZoomObject(ofVec2f _loc){
        loc = _loc;
        speed = ofVec3f(0, 0, ofRandom(5, 10));
        radius = 0;
        col.setHsb(ofRandom(255), 255, 255, 220);
        circleRes = int(ofRandom(3, 8));
        radius = ofRandom(20, 100);
        rotSpeed = ofVec3f(ofRandom(-1, 1),
                           ofRandom(-1, 1),
                           ofRandom(-1, 1)
                           ) * 100;
        flowMax = 1000;
    }
    
    void update(){
        loc.z += speed.z;
    }
    
    void draw(){
        ofSetCircleResolution(circleRes);
        ofSetColor(col);
        ofPushMatrix();
        //ofRotateZ(ofGetElapsedTimef() * -20);
        ofTranslate(loc);
        ofRotateX(-ofGetElapsedTimef() * rotSpeed.x);
        ofRotateY(-ofGetElapsedTimef() * rotSpeed.y);
        ofRotateZ(-ofGetElapsedTimef() * rotSpeed.z);
        ofCircle(0,0, radius);
        ofPopMatrix();
    }
    
    ofVec3f loc;
    ofVec3f speed;
    float radius;
    ofColor col;
    int circleRes;
    ofVec3f rotSpeed;
    int flowMax;
};


class StAnimZoom : public itg::ofxState<SharedData>{
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
    ofxPostProcessing post;
    BloomPass::Ptr bloom;
    
    deque<ZoomObject *> flows;
};