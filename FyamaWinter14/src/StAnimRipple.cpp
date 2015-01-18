#include "StAnimRipple.h"
#include "ofApp.h"

string StAnimRipple::getName(){
    return "StAnimRipple";
}

void StAnimRipple::setup(){
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 100);
    gui->addSpacer();
    gui->addLabel("BLANK");
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("StFftBlueBeam.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    
    baseColor.setHsb(ofRandom(255), 255, 255);
    
    app = ((ofApp*)ofGetAppPtr());
}

void StAnimRipple::update(){
    if (ofGetFrameNum() % 10 == 0) {
        Ripple *rip = new Ripple(ofVec2f(ofRandom(ofGetWidth()), ofRandom(ofGetHeight())), 20.0);
        ripples.push_back(rip);
    }
    
    for (int i = 0; i < ripples.size(); i++) {
        ripples[i]->update();
        if (ripples[i]->radius > ofGetWidth()) {
            delete ripples[0];
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
    
    post->begin();
    
    ofSetColor(baseColor);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    
    for (int i = 0; i < ripples.size(); i++) {
        ripples[i]->draw();
    }
    post->end();
    
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
    delete post;
}

void StAnimRipple::stateEnter(){
    post = new PostProcessing();
    post->init(ofGetWidth(), ofGetHeight());
    bloom = post->createPass<BloomPass>();
    bloom->setEnabled(true);
}

