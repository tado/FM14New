#include "StAnimFade.h"
#include "ofApp.h"

string StAnimFade::getName(){
    return "StAnimFade";
}

void StAnimFade::setup(){
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 180);
    gui->addSpacer();
    gui->addLabel("BLANK");
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("StAnimFade.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    
    post.init(ofGetWidth(), ofGetHeight());
    bloom = post.createPass<BloomPass>();
    bloom->setEnabled(true);
    
    app = ((ofApp*)ofGetAppPtr());
    
    rectNum = 20;
    
    for (int i = 0; i < rectNum; i++) {
        SlideRect *r = new SlideRect(ofRandom(-4, 4));
        rects.push_back(r);
    }
    
    baseColor.setHsb(ofRandom(255), 255, 180);
}

void StAnimFade::update(){
    if (ofGetFrameNum() % 10 == 0) {
    }
    
    for (int i = 0; i < rects.size(); i++) {
        rects[i]->update();
    }
}

void StAnimFade::draw(){
    gui->setVisible(getSharedData().guiVisible);
    app->drawFbo->fbo.begin();
    app->drawFbo->blendMode = 1;
    ofDisableAlphaBlending();
    ofClear(0, 0, 0);
    
    post.begin();
    //ofEnableDepthTest();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);

    ofSetColor(baseColor);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    for (int i = 0; i < rects.size(); i++) {
        rects[i]->draw();
    }
    
    post.end();
    
    app->drawFbo->fbo.end();
}

void StAnimFade::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("StAnimFade.xml");
    }
}

void StAnimFade::stateExit(){
    gui->setVisible(false);
}
