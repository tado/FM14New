#include "StSphereSoundWave.h"

string StSphereSoundWave::getName(){
    return "StSphereSoundWave";
}

void StSphereSoundWave::setup(){
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
    gui->loadSettings("StSphereSoundWave.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    ofAddListener(gui->newGUIEvent,this,&StSphereSoundWave::guiEvent);
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    
    
    img.loadImage("512.jpg");
    sphere.set(400, 24);
    sphere.mapTexCoordsFromTexture(img.getTextureReference());
    
    fbo.allocate(512, 512);
    fbo.begin();
    ofClear(0);
    fbo.end();
}

void StSphereSoundWave::update(){
    app = ((ofApp*)ofGetAppPtr());
}

void StSphereSoundWave::draw(){
    ofxUIIntSlider *ghue = (ofxUIIntSlider *)gui->getWidget("HUE"); int hue = ghue->getValue();
    ofxUIIntSlider *gsaturation = (ofxUIIntSlider *)gui->getWidget("SATURATION"); int saturation = gsaturation->getValue();
    ofxUIIntSlider *gbrightness = (ofxUIIntSlider *)gui->getWidget("BRIGHTNESS"); int brightness = gbrightness->getValue();
    ofxUISlider *gzoom = (ofxUISlider *)gui->getWidget("ZOOM"); float zoom = gzoom->getValue();
    ofxUISlider *gfade = (ofxUISlider *)gui->getWidget("FADE"); float fade = gfade->getValue();
    ofxUISlider *glinewidth = (ofxUISlider *)gui->getWidget("LINE WIDTH"); float linewidth = glinewidth->getValue();
    
    gui->setVisible(getSharedData().guiVisible);
    app->drawFbo->fbo.begin();
    int num = app->fft->audioBins.size();
    fbo.begin();
    ofNoFill();
    ofSetLineWidth(linewidth);
    ofSetColor(ofColor::fromHsb(hue, saturation, brightness));
    ofScale(zoom, 1.0);

    ofBeginShape();
    for (int i = 0; i < num; i++) {
        float x = ofMap(i, 0, num, app->drawFbo->width / -2.0, app->drawFbo->width / 2.0);
        ofCurveVertex(x, app->fft->audioBins[i] * app->drawFbo->height / 2.0);
    }
    ofEndShape();
    fbo.end();
    ofFill();
    ofSetLineWidth(1.0);
    
    cam.begin();
    ofBackground(0);
    ofSetColor(255);

    ofEnableDepthTest();
    fbo.getTextureReference().bind();
    sphere.draw();
    fbo.getTextureReference().unbind();
    ofDisableDepthTest();
    
    ofDisableAlphaBlending();
    cam.end();
    app->drawFbo->fbo.end();
    ofDisableAlphaBlending();
}

void StSphereSoundWave::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("StSphereSoundWave.xml");
    }
}

void StSphereSoundWave::stateEnter(){
    ofDisableArbTex();
}

void StSphereSoundWave::stateExit(){
    ofEnableArbTex();
    gui->setVisible(false);
}
