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
    
    cam.setFov(80);
    
    for (int i = 0; i < 100; i++) {
        ZoomObject *flow = new ZoomObject(ofVec3f(ofRandom(-ofGetHeight() / 4.0, ofGetHeight() / 4.0),
                                                  ofRandom(-ofGetHeight() / 4.0, ofGetHeight() / 4.0),
                                                  ofRandom(-250, 0)));
        flows.push_back(flow);
    }
    
    app = ((ofApp*)ofGetAppPtr());
}

void StAnimZoom::update(){
    for (int i = 0; i < flows.size(); i++) {
        flows[i]->update();
        if (flows[i]->loc.z > 1000) {
            flows[i]->loc.z = -250;
        }
    }
    
    /*
     if (ofGetFrameNum() % 1 == 0) {
     ZoomObject *flow = new ZoomObject(ofVec3f(ofRandom(-ofGetHeight() / 4.0, ofGetHeight() / 4.0),
     ofRandom(-ofGetHeight() / 4.0, ofGetHeight() / 4.0),
     -ofGetWidth() * 4));
     flows.push_back(flow);
     }
     
     for (int i = 0; i < flows.size(); i++) {
     flows[i]->update();
     if (flows.size() > flows[i]->flowMax) {
     delete flows[0];
     flows.pop_front();
     }
     }
     */
}

void StAnimZoom::draw(){
    gui->setVisible(getSharedData().guiVisible);
    app->drawFbo->fbo.begin();
    app->drawFbo->blendMode = 1;
    ofDisableAlphaBlending();
    ofClear(0,0,0);
    
    ofScale(1.0 / fxRatio, 1.0 / fxRatio);   
    post->begin(cam);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofEnableDepthTest();
    for (int i = 0; i < flows.size(); i++) {
        flows[i]->draw();
    }
    ofDisableDepthTest();
    ofDisableAlphaBlending();
    post->end();
    
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
    delete post;
}

void StAnimZoom::stateEnter(){
    fxRatio = 0.5;
    post = new PostProcessing();
    post->init(ofGetWidth() * fxRatio, ofGetHeight() * fxRatio);
    bloom = post->createPass<BloomPass>();
    bloom->setEnabled(true);
}
