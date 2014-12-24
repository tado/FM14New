#include "StFftDot.h"
#include "ofApp.h"

string StFftDot::getName(){
    return "StFftDot";
}

void StFftDot::setup(){
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("FFT Draw Bar");
    gui->addSpacer();
    gui->addSlider("CIRCLE SIZE", 0, 60.0, 10.0);
    gui->addIntSlider("SKIP", 1, 12, 4);
    gui->addRangeSlider("HUE", 0, 255, 0, 255);
    gui->addIntSlider("SATURATION", 0, 255, 100);
    gui->addSlider("BRIGHTNESS", 0, 255.0, 5.0);
    gui->addIntSlider("REPEAT", 1, 120, 40);
    gui->addSlider("SHIFT", 0.0, 50.0, 10.0);
    gui->addSlider("ZOOM", 1.0, 5.0, 1.0);
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("StFftDot.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    ofAddListener(gui->newGUIEvent,this,&StFftDot::guiEvent);
    
    app = ((ofApp*)ofGetAppPtr());
}

void StFftDot::update(){
    gui->setVisible(getSharedData().guiVisible);
}

void StFftDot::draw() {
    app->drawFbo->fbo.begin();
    app->drawFbo->blendMode = 1;
    cam.begin();
    ofDisableAlphaBlending();
    ofClear(0,0,0);
    ofSetCircleResolution(64);
    ofxUISlider *gcirclesize = (ofxUISlider *)gui->getWidget("CIRCLE SIZE"); float circlesize = gcirclesize->getValue();
    ofxUIIntSlider *gskip = (ofxUIIntSlider *)gui->getWidget("SKIP"); int skip = gskip->getValue();
    ofxUIIntSlider *gsaturation = (ofxUIIntSlider *)gui->getWidget("SATURATION"); int saturation = gsaturation->getValue();
    ofxUISlider *gbrightness = (ofxUISlider *)gui->getWidget("BRIGHTNESS"); float brightness = gbrightness->getValue();
    ofxUIIntSlider *grep = (ofxUIIntSlider *)gui->getWidget("REPEAT"); int rep = grep->getValue();
    ofxUISlider *gshift = (ofxUISlider *)gui->getWidget("SHIFT"); float shift = gshift->getValue();
    ofxUISlider *gzoom = (ofxUISlider *)gui->getWidget("ZOOM"); float zoom = gzoom->getValue();
    
    ofScale(zoom, zoom);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    float controlRep = ofMap(app->oscControl->controlVal[4], 0, 127.0, 1.0, 120.0);
    int controlHue = ofMap(app->oscControl->controlVal[5], 0, 127, 0, 63);
    int hueLow = controlHue;
    int hueHigh = (controlHue + 255 - 63);
    
    for (int j = 0; j < controlRep; j++) {
        for (int i = 0; i < app->fft->drawBins.size(); i += skip) {
            float size = ofMap(app->fft->drawBins[i], 0, 1.0, 1, circlesize);
            float x = ofMap(i, 0, app->fft->drawBins.size(), 0, ofGetWidth()/2.0);
            float hue = ofMap(i, 0, app->fft->drawBins.size(), hueLow, hueHigh);
            ofColor col;
            col.setHsb(hue, saturation, brightness);
            ofSetColor(col);
            ofCircle(x, shift, size);
            ofCircle(-x, shift, size);
        }
        ofRotateZ(360.0 / controlRep);
    }
    
    ofDisableAlphaBlending();
    cam.end();
    app->drawFbo->fbo.end();
}

void StFftDot::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("StFftDot.xml");
    }
}

void StFftDot::stateExit(){
    gui->setVisible(false);
}