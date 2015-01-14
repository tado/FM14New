#include "StAnimRipple.h"
#include "ofApp.h"

string StAnimRipple::getName(){
    return "StAnimRipple";
}

void StAnimRipple::setup(){
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

void StAnimRipple::update(){
    if (ofGetFrameNum() % 20 == 0) {
        Ripple *rip = new Ripple(ofVec2f(ofRandom(ofGetWidth()), ofRandom(ofGetHeight())), 10.0);
        ripples.push_back(rip);
    }
    
    for (int i = 0; i < ripples.size(); i++) {
        ripples[i]->update();
        if (ripples[i]->radius > ofGetWidth() * 1.5) {
            ripples.pop_front();
        }
    }
}

void StAnimRipple::draw(){
    gui->setVisible(getSharedData().guiVisible);
    app->drawFbo->fbo.begin();
    app->drawFbo->blendMode = 1;
    ofDisableAlphaBlending();
    ofClear(0,0,0);
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    post.begin();
    for (int i = 0; i < ripples.size(); i++) {
        ripples[i]->draw();
    }
    post.end();
    
    app->drawFbo->fbo.end();
}

void StAnimRipple::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("StFftBlueBeam.xml");
    }
}

void StAnimRipple::stateExit(){
    gui->setVisible(false);
}
