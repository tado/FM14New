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
    
    cam.setFov(80);
    maxRibbon = 100;
    
    app = ((ofApp*)ofGetAppPtr());
}

void StAnimRibbon::update(){
    if (ofGetFrameNum() % 3 == 0) {
        ofxTwistedRibbon *r = new ofxTwistedRibbon();
        r->update(ofVec3f(ofRandom(-ofGetHeight(), ofGetHeight()),
                          ofRandom(-ofGetHeight(), ofGetHeight()),
                          ofGetWidth()/2.0));
        r->length = 200;
        r->thickness = 20;
        ofColor col;
        col.setHsb(ofRandom(255), 255, 255);
        r->color = col;
        ribbons.push_back(r);
        float sp = ofRandom(20, 40);
        speed.push_back(sp);
    }
    for (int i = 0; i < ribbons.size(); i++) {
        ofVec3f loc = ribbons[i]->points[ribbons[i]->points.size() - 1];
        ribbons[i]->update(ofVec3f(loc.x + cos(ofGetElapsedTimef()) * 40,
                                   loc.y + sin(ofGetElapsedTimef()) * 40,
                                   loc.z - speed[i]));
        ofColor col;
        col.setHsb(int(ribbons[i]->color.getHue() + 4) % 255, 255, 255);
        ribbons[i]->color = col;
    }
    if (ribbons.size() > maxRibbon) {
        ribbons.pop_front();
    }
}

void StAnimRibbon::draw(){
    gui->setVisible(getSharedData().guiVisible);
    app->drawFbo->fbo.begin();
    app->drawFbo->blendMode = 1;
    ofDisableAlphaBlending();
    ofClear(0,0,0);
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    post.begin(cam);
    ofEnableDepthTest();
    
    for (int i = 0; i < ribbons.size(); i++) {
        ribbons[i]->draw();
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
