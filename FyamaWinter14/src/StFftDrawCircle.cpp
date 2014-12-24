#include "StFftDrawCircle.h"
#include "ofApp.h"

string StFftDrawCircle::getName(){
    return "StFftDrawCircle";
}

void StFftDrawCircle::setup(){
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("FFT Draw Bar");
    gui->addSpacer();
    gui->addSlider("CIRCLE SIZE", 0, ofGetHeight(), 50.0);
    gui->addIntSlider("SKIP", 1, 12, 4);
    gui->addRangeSlider("HUE", 0, 255, 0, 255);
    gui->addIntSlider("SATURATION", 0, 255, 100);
    gui->addSlider("BRIGHTNESS", 0, 20.0, 5.0);
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("StFftDrawCircle.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    ofAddListener(gui->newGUIEvent,this,&StFftDrawCircle::guiEvent);
    
    app = ((ofApp*)ofGetAppPtr());
}

void StFftDrawCircle::update(){
    gui->setVisible(getSharedData().guiVisible);
}

void StFftDrawCircle::draw() {
    app->drawFbo->fbo.begin();
    app->drawFbo->blendMode = 1;
    cam.begin();
    ofDisableAlphaBlending();
    ofClear(0,0,0);
    ofSetCircleResolution(64);
    ofxUISlider *gcirclesize = (ofxUISlider *)gui->getWidget("CIRCLE SIZE"); float circlesize = gcirclesize->getValue();
    ofxUIIntSlider *gskip = (ofxUIIntSlider *)gui->getWidget("SKIP"); int skip = gskip->getValue();
    ofxUIRangeSlider *ghue = (ofxUIRangeSlider *)gui->getWidget("HUE"); float hueLow = ghue->getValueLow(); float hueHigh = ghue->getValueHigh();
    ofxUIIntSlider *gsaturation = (ofxUIIntSlider *)gui->getWidget("SATURATION"); int saturation = gsaturation->getValue();
    ofxUISlider *gbrightness = (ofxUISlider *)gui->getWidget("BRIGHTNESS"); float brightness = gbrightness->getValue();

    ofEnableBlendMode(OF_BLENDMODE_ADD);
    for (int i = 0; i < app->fft->drawBins.size(); i += skip) {
        float size = ofMap(app->fft->drawBins[i], 0, 1.0, 0, circlesize);
        float x = ofMap(i, 0, app->fft->drawBins.size(), 0, ofGetWidth()/1.2);
        float hue = ofMap(i, 0, app->fft->drawBins.size(), hueLow, hueHigh);
        ofColor col;
        col.setHsb(hue, saturation, brightness);
        ofSetColor(col);

        ofPushMatrix();
        ofTranslate(-ofGetWidth()/5, 0);
        ofCircle(x, 0, size);
        ofCircle(-x, 0, size);
        ofPopMatrix();

        ofPushMatrix();
        ofTranslate(ofGetWidth()/5, 0);
        ofCircle(x, 0, size);
        ofCircle(-x, 0, size);
        ofPopMatrix();
    }
    ofDisableAlphaBlending();
    cam.end();
    app->drawFbo->fbo.end();
}

void StFftDrawCircle::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("StFftDrawCircle.xml");
    }
}

void StFftDrawCircle::stateExit(){
    gui->setVisible(false);
}