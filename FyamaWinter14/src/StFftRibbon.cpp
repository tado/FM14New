#include "StFftRibbon.h"

string StFftRibbon::getName(){
    return "StFftRibbon";
}

void StFftRibbon::setup() {
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("FFT RIBBON");
    gui->addSpacer();
    gui->addSlider("POWER", 0.01, 1.0, 0.7);
    gui->addSlider("GRAVITY", 0.0, 100.0, 3.0);
    gui->addSlider("STRENGTH", 0, 20.0, 10.0);
    gui->addSlider("STIFFNESS", 0, 0.1, 0.01);
    gui->addSlider("DAMPING", 0.5, 1.0, 0.9);
    gui->addSlider("WIDTH", 0.1, 20.0, 1.0);
    gui->addRangeSlider("HUE", 0, 255, 0, 255);
    gui->addIntSlider("SATURATION", 0, 255, 100);
    gui->addIntSlider("BRIGHTNESS", 0, 255, 100);
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("StFftRibbon.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    ofAddListener(gui->newGUIEvent,this,&StFftRibbon::guiEvent);
    
    app = ((ofApp*)ofGetAppPtr());
    fft_size = app->bufferSize;
    
    for (int i = 0; i < fft_size; i++) {
        ofVec3f p = ofVec3f(ofRandom(-1, 1),ofRandom(-1, 1),ofRandom(-1, 1));
        pos.push_back(p);
        ofVec3f v = ofVec3f(0, 0, 0);
        vel.push_back(v);
        ofVec3f vc = ofVec3f(ofRandom(-1, 1),ofRandom(-1, 1),ofRandom(-1, 1));
        vec.push_back(vc);
        
        ofxTwistedRibbon *r = new ofxTwistedRibbon();
        r->length = 20;
        r->thickness = 4;
        ribbons.push_back(r);
    }
    mass = 1.0;
}

void StFftRibbon::update(){
    ofxUISlider *gpower = (ofxUISlider *)gui->getWidget("POWER"); float power = gpower->getValue();
    ofxUISlider *ggravity = (ofxUISlider *)gui->getWidget("POWER"); float gravity = ggravity->getValue();
    ofxUIRangeSlider *ghue = (ofxUIRangeSlider *)gui->getWidget("HUE"); float hueLow = ghue->getValueLow(); float hueHigh = ghue->getValueHigh();
    ofxUIIntSlider *gsaturation = (ofxUIIntSlider *)gui->getWidget("SATURATION"); int saturation = gsaturation->getValue();
    ofxUIIntSlider *gbrightness = (ofxUIIntSlider *)gui->getWidget("BRIGHTNESS"); int brightness = gbrightness->getValue();
    ofxUISlider *gstrength = (ofxUISlider *)gui->getWidget("STRENGTH"); float strength = gstrength->getValue();
    ofxUISlider *gdamping = (ofxUISlider *)gui->getWidget("DAMPING"); float damping = gdamping->getValue();
    ofxUISlider *gstiffness = (ofxUISlider *)gui->getWidget("STIFFNESS"); float stiffness = gstiffness->getValue();
    ofxUISlider *gwidth = (ofxUISlider *)gui->getWidget("WIDTH"); float width = gwidth->getValue();

    for (int i=0; i < fft_size; i++) {
        float magnitude = powf(app->fft->drawBins[i], power);
        float addFroce;
        addFroce = magnitude * strength;
        float direction = ofRandom(360);
        ofVec3f radd = ofVec3f(cos(direction) * addFroce, sin(direction) * addFroce,  ofRandom(-1,1)*addFroce) * 0.5;
        ofVec3f add = ofVec3f(pos[i]).normalize() * addFroce;
        ofVec3f rev = -ofVec3f(pos[i]).normalize() * gravity;
        ofVec3f force = stiffness * -pos[i] + add + radd + rev;
        ofVec3f acceleration = force / mass;
        vel[i] = damping * (vel[i] + acceleration);
        pos[i] += vel[i];
        
        if(app->fft->drawBins[i] < 1.0){
            float hue = ofMap(i, 0, fft_size, hueLow, hueHigh);
            ofColor col = ofColor::fromHsb(hue, saturation, brightness);
            ribbons[i]->thickness = app->fft->drawBins[i] * width;
            ribbons[i]->update(pos[i], col);
        }
    }
}

void StFftRibbon::draw(){
    app->drawFbo->fbo.begin();
    app->drawFbo->blendMode = 0;
    ofDisableAlphaBlending();
    ofClear(0,0,0);
    cam.begin();
    glEnable(GL_DEPTH_TEST);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofPushMatrix();
    ofScale(1.0, 1.0, 1.0);
    ofRotateX(ofGetElapsedTimef() * 1.1);
    ofRotateY(ofGetElapsedTimef() * 1.3);
    ofRotateZ(ofGetElapsedTimef() * 1.2);
    for (int i=0; i  < fft_size-2; i+=2) {
        ribbons[i]->draw();
    }
    ofPopMatrix();
    glDisable(GL_DEPTH_TEST);
    ofDisableAlphaBlending();
    cam.end();
    app->drawFbo->fbo.end();
    
    gui->setVisible(getSharedData().guiVisible);
}

void StFftRibbon::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("StFftRibbon.xml");
    }
}

void StFftRibbon::stateExit(){
    pos.clear();
    vel.clear();
    vec.clear();
    ribbons.clear();
    for (int i = 0; i < fft_size; i++) {
        ofVec3f p = ofVec3f(ofRandom(-1, 1),ofRandom(-1, 1),ofRandom(-1, 1));
        pos.push_back(p);
        ofVec3f v = ofVec3f(0, 0, 0);
        vel.push_back(v);
        ofVec3f vc = ofVec3f(ofRandom(-1, 1),ofRandom(-1, 1),ofRandom(-1, 1));
        vec.push_back(vc);
        
        ofxTwistedRibbon *r = new ofxTwistedRibbon();
        r->length = 20;
        r->thickness = 4;
        ribbons.push_back(r);
    }
    gui->setVisible(false);
}
