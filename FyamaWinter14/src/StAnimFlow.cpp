#include "StAnimFlow.h"
#include "ofApp.h"

string StAnimFlow::getName(){
    return "StAnimFlow";
}

void StAnimFlow::setup(){
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 180);
    gui->addSpacer();
    gui->addLabel("BLANK");
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("StFftBlueBeam.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    
    direction = false;
    
    for (int i = 0; i < 100; i++) {
        FlowObject *flow = new FlowObject(ofVec3f(ofRandom(-ofGetWidth()/2, ofGetWidth() + ofGetWidth()/2),
                                                  ofRandom(ofGetHeight()),
                                                  ofRandom(ofGetHeight() / 2.0)));
        flows.push_back(flow);
    }
    
    app = ((ofApp*)ofGetAppPtr());
}

void StAnimFlow::update(){
    for (int i = 0; i < flows.size(); i++) {
        flows[i]->update(direction);

        if (flows[i]->loc.x < -ofGetWidth()/2.0) {
            flows[i]->loc.x = ofGetWidth() + ofGetWidth() / 2.0;
        }
        if (flows[i]->loc.x > ofGetWidth() + ofGetWidth()/2.0) {
            flows[i]->loc.x = -ofGetWidth() / 2.0;
        }
    }
    /*
    if (ofGetFrameNum() % 5 == 0) {
        float initPos;
        if (direction) {
            initPos = -ofGetWidth() / 4.0;
        } else {
            initPos = ofGetWidth()  + ofGetWidth() / 4.0;
        }
        
        FlowObject *flow = new FlowObject(ofVec3f(initPos, ofRandom(ofGetHeight()), ofRandom(ofGetHeight()/2.0)));
        flows.push_back(flow);
    }
    
    for (int i = 0; i < flows.size(); i++) {
        flows[i]->update(direction);
        if (flows.size() > flows[i]->flowMax) {
            delete flows[0];
            flows.pop_front();
        }
    }
     */
}

void StAnimFlow::draw(){
    gui->setVisible(getSharedData().guiVisible);
    app->drawFbo->fbo.begin();
    app->drawFbo->blendMode = 1;
    ofDisableAlphaBlending();
    ofClear(0,0,0);
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    post->begin();
    ofEnableDepthTest();
    for (int i = 0; i < flows.size(); i++) {
        flows[i]->draw();
    }
    ofDisableDepthTest();
    post->end();
    
    app->drawFbo->fbo.end();
}

void StAnimFlow::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("StFftBlueBeam.xml");
    }
}

void StAnimFlow::stateExit(){
    gui->setVisible(false);
    if (direction) {
        direction = false;
    } else {
        direction = true;
    }
    delete post;
}

void StAnimFlow::stateEnter(){
    post = new PostProcessing();
    post->init(ofGetWidth(), ofGetHeight());
    bloom = post->createPass<BloomPass>();
    bloom->setEnabled(true);
}
