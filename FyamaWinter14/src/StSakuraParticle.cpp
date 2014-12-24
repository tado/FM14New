#include "StSakuraParticle.h"

string StSakuraParticle::getName(){
    return "StSakuraParticle";
}

void StSakuraParticle::setup(){
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("SAKURA");
    gui->addSpacer();
    gui->addSlider("RADIUS", 20, 200, 60);
    gui->addIntSlider("NUM", 10, 20000, 10000);
    gui->addIntSlider("DENSITY", 1, 100, 80);
    gui->addSlider("WIND SPEED", 0.0, 10.0, 1.5);
    gui->addSlider("CIRCLE SCALE", 0, 2.0, 1.0);
    gui->addIntSlider("MIX", 0, 255, 127);
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("StSakuraParticle.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    ofAddListener(gui->newGUIEvent,this,&StSakuraParticle::guiEvent);
    
    app = ((ofApp*)ofGetAppPtr());
}

void StSakuraParticle::update(){
    for (int i = 0; i < particles.size(); i++) {
        particles[i]->update();
    }
}

void StSakuraParticle::draw(){
    ofxUISlider *gradius = (ofxUISlider *)gui->getWidget("RADIUS"); float radius = gradius->getValue();
    ofxUIIntSlider *gnum = (ofxUIIntSlider *)gui->getWidget("NUM"); int num = gnum->getValue();
    ofxUISlider *gcircleScale = (ofxUISlider *)gui->getWidget("CIRCLE SCALE"); float circleScale = gcircleScale->getValue();
    ofxUISlider *gwindSpeed = (ofxUISlider *)gui->getWidget("WIND SPEED"); float windSpeed = gwindSpeed->getValue();
    ofxUIIntSlider *gdensity = (ofxUIIntSlider *)gui->getWidget("DENSITY"); int density = gdensity->getValue();
    ofxUIIntSlider *gmix = (ofxUIIntSlider *)gui->getWidget("MIX"); int mix = gmix->getValue();
    
    app->drawFbo->fbo.begin();
    app->drawFbo->blendMode = 1;
    ofDisableAlphaBlending();
    ofClear(0,0,0);
    ofTranslate(0, -app->drawFbo->top);
    ofEnableBlendMode(OF_BLENDMODE_ADD);

    if (ofGetFrameNum() % density == 0){
        for (int i = 0; i < 10; i++) {
            SakuraParticle *p = new SakuraParticle();
            p->setup(ofVec3f(ofRandom(-200, 0), ofRandom(-SCREEN_HEIGHT/2, SCREEN_HEIGHT), ofRandom(400)),
                     ofVec3f(ofRandom(windSpeed * 0.5, windSpeed * 1.5), ofRandom(0.2,1.0), 0),
                     ofColor(255,180,180));
            p->friction = 0.0;
            p->radius = 6;
            particles.push_back(p);
            while (particles.size() > num) {
                delete particles[0];
                particles.pop_front();
            }
        }
    }
    
    for (int i = 0; i < particles.size(); i++) {
        particles[i]->draw();
    }
    ofDisableBlendMode();
    app->drawFbo->fbo.end();
    
    gui->setVisible(getSharedData().guiVisible);
}

void StSakuraParticle::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("StSakuraParticle.xml");
    }
}

void StSakuraParticle::stateExit(){
    gui->setVisible(false);
    particles.clear();
    deque<SakuraParticle*>().swap(particles);
}
