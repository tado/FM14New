#include "StAnimZoom.h"
#include "ofApp.h"

string StAnimZoom::getName(){
    return "StAnimZoom";
}

void StAnimZoom::setup(){
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
    
    cam.setFov(80);
    
    app = ((ofApp*)ofGetAppPtr());
}

void StAnimZoom::update(){
    if (ofGetFrameNum() % 1 == 0) {
        ZoomObject *flow = new ZoomObject(ofVec3f(ofRandom(-ofGetHeight() / 4.0, ofGetHeight() / 4.0),
                                                  ofRandom(-ofGetHeight() / 4.0, ofGetHeight() / 4.0),
                                                  -ofGetWidth() * 4));
        flows.push_back(flow);
    }
    
    for (int i = 0; i < flows.size(); i++) {
        flows[i]->update();
        if (flows.size() > flows[i]->flowMax) {
            flows.pop_front();
        }
    }
}

void StAnimZoom::draw(){
    gui->setVisible(getSharedData().guiVisible);
    app->drawFbo->fbo.begin();
    app->drawFbo->blendMode = 1;
    ofDisableAlphaBlending();
    ofClear(0,0,0);
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    // post.begin(cam);
    cam.begin();
    ofEnableDepthTest();
    for (int i = 0; i < flows.size(); i++) {
        flows[i]->draw();
    }
    ofDisableDepthTest();
    //post.end();
    cam.end();
    
    app->drawFbo->fbo.end();
}

void StAnimZoom::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("StFftBlueBeam.xml");
    }
}

void StAnimZoom::stateExit(){
    gui->setVisible(false);
}
