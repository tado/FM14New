#include "StAnimRibbon.h"
#include "ofApp.h"

string StAnimRibbon::getName(){
    return "StAnimRibbon";
}

void StAnimRibbon::setup(){
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 180);
    gui->addSpacer();
    gui->addLabel("BLANK");
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("StFftBlueBeam.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    
    post.init(ofGetWidth(), ofGetHeight());
    bloom = post.createPass<BloomPass>();
    bloom->setEnabled(true);
    
    cam.setFov(100);
    maxRibbon = 200;
    
    app = ((ofApp*)ofGetAppPtr());
}

void StAnimRibbon::update(){
    if (ofGetFrameNum() % 1 == 0) {
        ofxTwistedRibbon *r = new ofxTwistedRibbon();

        ofVec3f pos = ofVec3f(
                              ofRandom(-ofGetWidth() / 4.0, ofGetWidth() / 4.0),
                              ofRandom(-ofGetWidth() / 4.0, ofGetWidth() / 4.0),
                              ofGetWidth());
        ofVec3f sp = ofVec3f(0, 0, ofRandom(10, 20));
        speed.push_back(sp);
        
        float ss = ofRandom(-20, 20);
        spinSpeed.push_back(ss);
        
        lastPos.push_back(pos);
        r->update(ofVec3f(pos));
        r->length = 100;
        r->thickness = 20;
        ofColor col;
        col.setHsb(ofRandom(255), 255, 255);
        r->color = col;
        ribbons.push_back(r);

        float ra = ofRandom(ofGetWidth() / 8, ofGetWidth() / 4);
        radius.push_back(ra);
        
        if (ribbons.size() > maxRibbon) {
            ribbons.pop_front();
            lastPos.pop_front();
            speed.pop_front();
            spinSpeed.pop_front();
        }
    }
    for (int i = 0; i < ribbons.size(); i++) {
        lastPos[i] -= speed[i];
        ribbons[i]->update(lastPos[i]);
        ofColor col;
        col.setHsb(int(ribbons[i]->color.getHue() + 10) % 255, 255, 255);
        ribbons[i]->color = col;
    }
}

void StAnimRibbon::draw(){
    gui->setVisible(getSharedData().guiVisible);
    app->drawFbo->fbo.begin();
    app->drawFbo->blendMode = 1;
    ofDisableAlphaBlending();
    ofClear(0,0,0);
    
    post.begin(cam);
    ofEnableDepthTest();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    for (int i = 0; i < ribbons.size(); i++) {
        ofPushMatrix();
        ofRotateZ(ofGetElapsedTimef() * spinSpeed[i]);
        ribbons[i]->draw();
        ofPopMatrix();
    }
    ofDisableDepthTest();
    post.end();
    
    app->drawFbo->fbo.end();
}

void StAnimRibbon::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("StFftBlueBeam.xml");
    }
}

void StAnimRibbon::stateExit(){
    gui->setVisible(false);
}
