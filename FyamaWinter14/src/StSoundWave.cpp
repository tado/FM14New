#include "StSoundWave.h"

string StSoundWave::getName(){
    return "StSoundWave";
}

void StSoundWave::setup(){
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("SOUND WAVE");
    gui->addSpacer();
    gui->addIntSlider("HUE", 0, 255, 200);
    gui->addIntSlider("SATURATION", 0, 255, 200);
    gui->addIntSlider("BRIGHTNESS", 0, 255, 200);
    gui->addSlider("ZOOM", 1.0, 10.0, 1.0);
    gui->addSlider("FADE", 0.0, 127.0, 31.0);
    gui->addSlider("LINE WIDTH", 1.0, 10.0, 1.0);
    
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("StSoundWave.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    ofAddListener(gui->newGUIEvent,this,&StSoundWave::guiEvent);
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
}

void StSoundWave::update(){
    app = ((ofApp*)ofGetAppPtr());
}

void StSoundWave::draw(){
    ofxUIIntSlider *ghue = (ofxUIIntSlider *)gui->getWidget("HUE"); int hue = ghue->getValue();
    ofxUIIntSlider *gsaturation = (ofxUIIntSlider *)gui->getWidget("SATURATION"); int saturation = gsaturation->getValue();
    ofxUIIntSlider *gbrightness = (ofxUIIntSlider *)gui->getWidget("BRIGHTNESS"); int brightness = gbrightness->getValue();
    ofxUISlider *gzoom = (ofxUISlider *)gui->getWidget("ZOOM"); float zoom = gzoom->getValue();
    ofxUISlider *gfade = (ofxUISlider *)gui->getWidget("FADE"); float fade = gfade->getValue();
    ofxUISlider *glinewidth = (ofxUISlider *)gui->getWidget("LINE WIDTH"); float linewidth = glinewidth->getValue();
    
    gui->setVisible(getSharedData().guiVisible);
    app->drawFbo->fbo.begin();
    app->drawFbo->blendMode = 0;
    cam.begin();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetColor(0, 0, 0, fade);
    ofRect(-app->drawFbo->width, -app->drawFbo->height, app->drawFbo->width * 2.0, app->drawFbo->height * 2.0);
    int num = app->fft->audioBins.size();
    ofNoFill();
    ofSetLineWidth(linewidth);
    ofSetColor(ofColor::fromHsb(hue, saturation, brightness));
    ofScale(zoom, 1.0);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofBeginShape();
    for (int i = 0; i < num; i++) {
        float x = ofMap(i, 0, num, app->drawFbo->width / -2.0, app->drawFbo->width / 2.0);
        ofCurveVertex(x, app->fft->audioBins[i] * app->drawFbo->height / 2.0);
    }
    ofEndShape();
    ofFill();
    ofSetLineWidth(1.0);
    ofDisableAlphaBlending();
    cam.end();
    app->drawFbo->fbo.end();
}

void StSoundWave::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("StSoundWave.xml");
    }
}

void StSoundWave::stateExit(){
    gui->setVisible(false);
}
