#include "StRgbDelay.h"

string StRgbDelay::getName(){
    return "StRgbDelay";
}

void StRgbDelay::setup(){
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("RGB DELAY");
    gui->addSpacer();
    gui->addIntSlider("DELAY", 0, 100, 10);
    gui->addIntSlider("MIX", 0, 255, 127);
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("StRgbDelay.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    ofAddListener(gui->newGUIEvent,this,&StRgbDelay::guiEvent);
    
    app = ((ofApp*)ofGetAppPtr());
}

void StRgbDelay::update(){
    ofxUIIntSlider *gdelay = (ofxUIIntSlider *)gui->getWidget("DELAY"); int delay = gdelay->getValue();
    
    ofPixelsRef pix = app->blackmagic->colorPixels;
    ofTexture t;
    t.loadData(pix);
    texBuffer.push_back(t);
    while (texBuffer.size() > delay * 3) {
        texBuffer[0].clear();
        texBuffer.pop_front();
    }
}

void StRgbDelay::draw(){
    int camWidth = 1920;
    int camHeight = 1080;
    
    ofxUIIntSlider *gdelay = (ofxUIIntSlider *)gui->getWidget("DELAY"); int delay = gdelay->getValue();
    ofxUIIntSlider *gmix = (ofxUIIntSlider *)gui->getWidget("MIX"); int mix = gmix->getValue();
    
    app->drawFbo->fbo.begin();
    app->drawFbo->blendMode = 1;
    ofDisableAlphaBlending();
    ofClear(0,0,0);
    ofTranslate(0, -app->drawFbo->top);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    ofSetColor(255, 0, 0);
    texBuffer[0].draw(0, 0, app->blackmagic->width, app->blackmagic->height);
    if (texBuffer.size() > delay) {
        ofSetColor(0, 255, 0);
        texBuffer[delay].draw(0, 0, app->blackmagic->width, app->blackmagic->height);
    }
    if (texBuffer.size() > delay * 2) {
        ofSetColor(0, 0, 255);
        texBuffer[delay * 2].draw(0, 0, app->blackmagic->width, app->blackmagic->height);
    }
    
    ofDisableBlendMode();
    app->drawFbo->fbo.end();
    
    gui->setVisible(getSharedData().guiVisible);
}

void StRgbDelay::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("StRgbDelay.xml");
    }
}

void StRgbDelay::stateExit(){
    gui->setVisible(false);
}
