#pragma once
#include "ofxState.h"
#include "SharedData.h"
#include "ofxUI.h"
#include "ofApp.h"
#include "ofxPostProcessing.h"

class FlowObject {
public:
    FlowObject(ofVec2f _loc){
        loc = _loc;
        speed = ofVec3f(ofRandom(6, 12), 0, 0);
        radius = 0;
        col.setHsb(ofRandom(255), 255, 255, 225);
        circleRes = int(ofRandom(3, 8));
        radius = powf(ofRandom(4, 20), 2.0);
        rotSpeed = ofVec3f(ofRandom(-20, 20),
                           ofRandom(-20, 20),
                           ofRandom(-20, 20)
                           );
        flowMax = 150;
    }
    
    void update(bool direction){
        if (direction) {
            loc += speed;
        } else {
            loc -= speed;
        }
    }
    
    void draw(){
        ofSetCircleResolution(circleRes);
        ofSetColor(col);
        ofPushMatrix();
        ofTranslate(loc.x, loc.y, loc.z);
        ofRotateX(-ofGetElapsedTimef() * rotSpeed.x);
        ofRotateY(-ofGetElapsedTimef() * rotSpeed.y);
        ofRotateZ(-ofGetElapsedTimef() * rotSpeed.z);
        ofCircle(0, 0, radius);
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


class StAnimFlow : public itg::ofxState<SharedData>{
public:
    string getName();
    void setup();
    void update();
    void draw();
    void stateExit();
    void stateEnter();
    void guiEvent(ofxUIEventArgs &e);
    
    bool direction;
    ofxUICanvas *gui;
    ofApp *app;
    ofEasyCam cam;
    ofxPostProcessing *post;
    BloomPass::Ptr bloom;
    
    deque<FlowObject *> flows;
};