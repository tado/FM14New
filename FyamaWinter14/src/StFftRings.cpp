#include "StFftRings.h"
#include "ofApp.h"

string StFftRings::getName(){
    return "StFftRings";
}

void StFftRings::setup(){
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("FFT Draw Bar");
    gui->addSpacer();
    gui->addSlider("CIRCLE SIZE", 0, ofGetHeight(), 50.0);
    gui->addSlider("LINE WIDTH", 0, 10.0, 5.0);
    gui->addIntSlider("SKIP", 1, 12, 4);
    gui->addRangeSlider("HUE", 0, 255, 0, 255);
    gui->addIntSlider("SATURATION", 0, 255, 100);
    gui->addIntSlider("BRIGHTNESS", 0, 255, 100);
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("StFftRings.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    ofAddListener(gui->newGUIEvent,this,&StFftRings::guiEvent);
    
    app = ((ofApp*)ofGetAppPtr());
}

void StFftRings::update(){
    gui->setVisible(getSharedData().guiVisible);
}

void StFftRings::draw() {
    app->drawFbo->fbo.begin();
    cam.begin();
    ofDisableAlphaBlending();
    ofClear(0,0,0);
    ofSetCircleResolution(64);
    ofxUISlider *gcirclesize = (ofxUISlider *)gui->getWidget("CIRCLE SIZE"); float circlesize = gcirclesize->getValue();
    ofxUISlider *gwidth = (ofxUISlider *)gui->getWidget("LINE WIDTH"); float width = gwidth->getValue();
    ofxUIIntSlider *gskip = (ofxUIIntSlider *)gui->getWidget("SKIP"); int skip = gskip->getValue();
    ofxUIRangeSlider *ghue = (ofxUIRangeSlider *)gui->getWidget("HUE"); float hueLow = ghue->getValueLow(); float hueHigh = ghue->getValueHigh();
    ofxUIIntSlider *gsaturation = (ofxUIIntSlider *)gui->getWidget("SATURATION"); int saturation = gsaturation->getValue();
    ofxUIIntSlider *gbrightness = (ofxUIIntSlider *)gui->getWidget("BRIGHTNESS"); int brightness = gbrightness->getValue();
    
    ofNoFill();
    ofSetLineWidth(width);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofEnableDepthTest();
    
    ofRotateX(ofGetElapsedTimef() * 10);
    ofRotateY(ofGetElapsedTimef() * 11);
    ofRotateZ(ofGetElapsedTimef() * 12);
    
    for (int i = 0; i < app->fft->drawBins.size(); i += skip) {
        ofPushMatrix();
        ofRotateX(1 * i);
        ofRotateY(1.1 * i);
        ofRotateZ(1.2 * i);
        ofRotateX(app->fft->drawBins[i] * 3);
        ofRotateY(app->fft->drawBins[i] * 4);
        ofRotateZ(app->fft->drawBins[i] * 5);
        float baseSize = ofMap(i, 0, app->fft->drawBins.size(), 0, circlesize);
        float size = baseSize * app->fft->drawBins[i] * 3.0;
        //float size = circlesize;
        float hue = ofMap(i, 0, app->fft->drawBins.size(), hueLow, hueHigh);
        ofColor col;
        col.setHsb(hue, saturation, brightness);
        ofSetColor(col);
        ofCircle(0, 0, size);
        ofPopMatrix();
    }
    ofFill();
    ofDisableDepthTest();
    ofDisableAlphaBlending();
    cam.end();
    app->drawFbo->fbo.end();
}

void StFftRings::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("StFftRings.xml");
    }
}

void StFftRings::stateExit(){
    gui->setVisible(false);
}