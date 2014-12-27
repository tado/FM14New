#include "StFftDrawCircleBlue.h"
#include "ofApp.h"

string StFftDrawCircleBlue::getName(){
    return "StFftDrawCircleBlue";
}

void StFftDrawCircleBlue::setup(){
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("FFT Draw Bar");
    gui->addSpacer();
    gui->addSlider("CIRCLE SIZE", 0, ofGetHeight(), 50.0);
    gui->addIntSlider("SKIP", 1, 12, 4);
    //gui->addRangeSlider("HUE", 0, 255, 0, 255);
    gui->addSlider("SATURATION", 0, 2.0, 0.15);
    gui->addSlider("BRIGHTNESS", 0, 1.0, 0.1);
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("StFftDrawCircleBlue.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    ofAddListener(gui->newGUIEvent,this,&StFftDrawCircleBlue::guiEvent);
    
    app = ((ofApp*)ofGetAppPtr());
}

void StFftDrawCircleBlue::update(){
    gui->setVisible(getSharedData().guiVisible);
}

void StFftDrawCircleBlue::draw() {
    app->drawFbo->fbo.begin();
    app->drawFbo->blendMode = 1;
    cam.begin();
    //ofDisableAlphaBlending();
    ofClear(0,0,0,0);
    ofSetCircleResolution(64);
    ofxUISlider *gcirclesize = (ofxUISlider *)gui->getWidget("CIRCLE SIZE"); float circlesize = gcirclesize->getValue();
    ofxUIIntSlider *gskip = (ofxUIIntSlider *)gui->getWidget("SKIP"); int skip = gskip->getValue();
    //ofxUIRangeSlider *ghue = (ofxUIRangeSlider *)gui->getWidget("HUE"); float hueLow = ghue->getValueLow(); float hueHigh = ghue->getValueHigh();
    ofxUISlider *gsaturation = (ofxUISlider *)gui->getWidget("SATURATION"); float saturation = gsaturation->getValue();
    ofxUISlider *gbrightness = (ofxUISlider *)gui->getWidget("BRIGHTNESS"); float brightness = gbrightness->getValue();
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    for (int i = 0; i < app->fft->drawBins.size(); i += skip) {
        float size = ofMap(app->fft->drawBins[i], 0, 1.0, 0, circlesize);
        float x = ofMap(i, 0, app->fft->drawBins.size(), 0, ofGetWidth()/1.2);
        ofColor col;
        
        float controlHue;
        controlHue = ofMap(app->oscControl->controlVal[5], 0, 127, 0, 0.65);
        col.setHsb(controlHue * 255, saturation * 255, brightness * 255);
        ofSetColor(col);
        
        ofPushMatrix();
        ofTranslate(-ofGetWidth()/3.5, 0);
        ofCircle(x, 0, size);
        ofCircle(-x, 0, size);
        ofPopMatrix();
        
        ofPushMatrix();
        ofTranslate(ofGetWidth()/3.5, 0);
        ofCircle(x, 0, size);
        ofCircle(-x, 0, size);
        ofPopMatrix();
    }
    ofDisableAlphaBlending();
    cam.end();
    app->drawFbo->fbo.end();
}

void StFftDrawCircleBlue::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("StFftDrawCircleBlue.xml");
    }
}

void StFftDrawCircleBlue::stateExit(){
    gui->setVisible(false);
}