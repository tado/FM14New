#include "StAnimPunch.h"
#include "ofApp.h"

string StAnimPunch::getName(){
    return "StAnimPunch";
}

void StAnimPunch::setup(){
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 100);
    gui->addSpacer();
    gui->addLabel("BLANK");
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("StFftBlueBeam.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    
    app = ((ofApp*)ofGetAppPtr());
}

void StAnimPunch::update(){
    
    for (int i = 0; i < punch.size(); i++) {
        punch[i]->update();
        /*
        if (punch[i]->radius > ofGetWidth()) {
            delete punch[0];
            punch.pop_front();
        }
         */
    }
}

void StAnimPunch::draw(){
    gui->setVisible(getSharedData().guiVisible);
    app->drawFbo->fbo.begin();
    app->drawFbo->blendMode = 1;
    ofDisableAlphaBlending();
    ofClear(0,0,0);
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    post->begin();
    for (int i = 0; i < punch.size(); i++) {
        punch[i]->draw();
    }
    post->end();

    app->drawFbo->fbo.end();
}

void StAnimPunch::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("StFftBlueBeam.xml");
    }
}

void StAnimPunch::stateExit(){
    gui->setVisible(false);
    delete post;
}

void StAnimPunch::stateEnter(){
    post = new PostProcessing();
    post->init(ofGetWidth(), ofGetHeight());
    bloom = post->createPass<BloomPass>();
    bloom->setEnabled(true);
    
    for (int i = 0; i < punch.size(); i++) {
        delete punch[i];
    }
    punch.clear();
    int num = 20;
    for (int i = 0; i < num; i++) {
        //Punch *p = new Punch(ofVec2f(ofRandom(ofGetWidth()), ofRandom(ofGetHeight())), 20.0);
        int hue = ofRandom(255);
        Punch *pr = new Punch(ofVec2f(ofGetWidth()/6, ofGetHeight()/2), powf((num - i), 1.4), hue);
        Punch *pl = new Punch(ofVec2f(ofGetWidth()/6 * 5, ofGetHeight()/2), powf((num - i), 1.4), hue);
        punch.push_back(pr);
        punch.push_back(pl);
    }
}
