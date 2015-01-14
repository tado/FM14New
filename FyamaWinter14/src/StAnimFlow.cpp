#include "StAnimFlow.h"
#include "ofApp.h"

string StAnimFlow::getName(){
    return "StAnimFlow";
}

void StAnimFlow::setup(){
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("BLANK");
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("StFftBlueBeam.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    
    post.init(ofGetWidth(), ofGetHeight());
    bloom = post.createPass<BloomPass>();
    bloom->setEnabled(true);
    
    app = ((ofApp*)ofGetAppPtr());
}

void StAnimFlow::update(){
    if (ofGetFrameNum() % 20 == 0) {
        FlowObject *flow = new FlowObject(ofVec2f(ofRandom(ofGetWidth()), ofRandom(ofGetHeight())), 10.0);
        flows.push_back(flow);
    }
    
    for (int i = 0; i < flows.size(); i++) {
        flows[i]->update();
        if (flows[i]->radius > ofGetWidth() * 1.5) {
            flows.pop_front();
        }
    }
}

void StAnimFlow::draw(){
    gui->setVisible(getSharedData().guiVisible);
    app->drawFbo->fbo.begin();
    app->drawFbo->blendMode = 1;
    ofDisableAlphaBlending();
    ofClear(0,0,0);
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    post.begin();
    for (int i = 0; i < flows.size(); i++) {
        flows[i]->draw();
    }
    post.end();
    
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
}
