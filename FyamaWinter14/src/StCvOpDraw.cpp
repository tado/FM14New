#include "StCvOpDraw.h"
#include "ofApp.h"

string StCvOpDraw::getName(){
    return "StCvOpDraw";
}

void StCvOpDraw::setup(){
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("DRAW OPTICALFLOW");
    gui->addSpacer();
    gui->addSlider("PYR SCALE", 0, 0.99, 0.5);
    gui->addIntSlider("LEVELS", 1, 10, 5);
    gui->addIntSlider("WIN SIZE", 1, 100, 30);
    gui->addIntSlider("ITERATION", 1, 20, 2);
    gui->addToggle("GAUSS", false);
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("flow.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    ofAddListener(gui->newGUIEvent,this,&StCvOpDraw::guiEvent);
    
    app = ((ofApp*)ofGetAppPtr());
}

void StCvOpDraw::update(){
    //CV params
    ofxUISlider *p = (ofxUISlider *)gui->getWidget("PYR SCALE"); pyrScale = p->getValue();
    ofxUIIntSlider *l = (ofxUIIntSlider *)gui->getWidget("LEVELS"); levels = l->getValue();
    ofxUIIntSlider *w = (ofxUIIntSlider *)gui->getWidget("WIN SIZE"); winsize = w->getValue();
    ofxUIIntSlider *it = (ofxUIIntSlider *)gui->getWidget("ITERATION"); iterations = it->getValue();
    ofxUIToggle *gs = (ofxUIToggle *)gui->getWidget("GAUSS"); OPTFLOW_FARNEBACK_GAUSSIAN = gs->getValue();
    polyN = 7;
    polySigma = 1.5;
    
    ofPixelsRef pix = ((ofApp*)ofGetAppPtr())->blackmagic->colorPixels;
    pix.resize(160, 90);
    flow.setPyramidScale(pyrScale);
    flow.setNumLevels(levels);
    flow.setWindowSize(winsize);
    flow.setNumIterations(iterations);
    flow.setPolyN(polyN);
    flow.setPolySigma(polySigma);
    flow.setUseGaussian(OPTFLOW_FARNEBACK_GAUSSIAN);
    if (pix.size() > 0) {
        flow.calcOpticalFlow(pix);
    }
    
    gui->setVisible(getSharedData().guiVisible);
}

void StCvOpDraw::draw(){
    app->drawFbo->fbo.begin();
    app->drawFbo->blendMode = 1;
    ofDisableAlphaBlending();
    ofClear(0,0,0);
    ofTranslate(0, -app->drawFbo->top);
    ofSetLineWidth(3);
    ofSetColor(255, 200);
    flow.draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    ofSetLineWidth(1);
    app->drawFbo->fbo.end();
}

void StCvOpDraw::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("flow.xml");
    }
}

void StCvOpDraw::stateExit(){
    gui->setVisible(false);
}