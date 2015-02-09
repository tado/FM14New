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
}

void StAnimFlow::draw(){
    gui->setVisible(getSharedData().guiVisible);
    
    app->drawFbo->fbo.begin();
    app->drawFbo->blendMode = 1;
    ofDisableAlphaBlending();
    ofClear(0,0,0);

    ofScale(1.0 / fxRatio, 1.0 / fxRatio);
    post->begin();
    ofScale(1.0 * fxRatio, 1.0 * fxRatio);
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
    fxRatio = 0.5;
    post = new PostProcessing();
    post->init(ofGetWidth() * fxRatio, ofGetHeight() * fxRatio);
    bloom = post->createPass<BloomPass>();
    bloom->setEnabled(true);
}
