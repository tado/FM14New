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
    
    /*
    post.init(ofGetWidth(), ofGetHeight());
    bloom = post.createPass<BloomPass>();
    bloom->setEnabled(true);
     */
    
    for (int i = 0; i < 50; i++) {
        FlowObject *flow = new FlowObject(ofVec3f(ofRandom(ofGetWidth()),
                                                  ofRandom(ofGetHeight()),
                                                  ofRandom(ofGetHeight() / 2.0)));
        flows.push_back(flow);
    }
    
    app = ((ofApp*)ofGetAppPtr());
}

void StAnimFlow::update(){
    if (ofGetFrameNum() % 5 == 0) {
        FlowObject *flow = new FlowObject(ofVec3f(-ofGetWidth() / 2.0,
                                                  ofRandom(ofGetHeight()),
                                                  ofRandom(ofGetHeight() / 2.0)));
        flows.push_back(flow);
    }
    
    for (int i = 0; i < flows.size(); i++) {
        flows[i]->update();
        if (flows.size() > flows[i]->flowMax) {
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
    
    // post.begin();
    ofEnableDepthTest();
    for (int i = 0; i < flows.size(); i++) {
        flows[i]->draw();
    }
    ofDisableDepthTest();
    /// post.end();
    
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
