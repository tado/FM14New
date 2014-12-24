#include "StBlank.h"
#include "ofApp.h"

string StBlank::getName(){
    return "StBlank";
}

void StBlank::setup(){
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("BLANK");
    gui->addSpacer();
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    
    app = ((ofApp*)ofGetAppPtr());
}

void StBlank::update(){

}

void StBlank::draw(){
    gui->setVisible(getSharedData().guiVisible);
    app->drawFbo->fbo.begin();

    app->drawFbo->fbo.end();
}

void StBlank::guiEvent(ofxUIEventArgs &e){

}

void StBlank::stateExit(){
    gui->setVisible(false);
}
