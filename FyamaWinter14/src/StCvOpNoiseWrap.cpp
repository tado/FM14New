#include "StCvOpNoiseWrap.h"

string StCvOpNoiseWrap::getName(){
    return "StCvOpNoiseWrap";
}

void StCvOpNoiseWrap::setup(){
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("NOISE WRAP");
    gui->addSlider("PYR SCALE", 0, 0.99, 0.5);
    gui->addIntSlider("LEVELS", 1, 10, 5);
    gui->addIntSlider("WIN SIZE", 1, 100, 30);
    gui->addIntSlider("ITERATION", 1, 20, 2);
    gui->addToggle("GAUSS", false);
    gui->addSpacer();
    gui->addSlider("NOISE AMP", 0.0, 3.0, 1.0);
    gui->addSlider("ZOOM", 1.0, 8.0, 2.0);
    gui->addSlider("SAT", 0, 2.0, 1.0);
    gui->addSlider("BR", 0, 2.0, 1.0);
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("StCvOpNoiseWrap.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    ofAddListener(gui->newGUIEvent,this,&StCvOpNoiseWrap::guiEvent);
    
    post.init(ofGetWidth(), ofGetHeight());
    
    noise = post.createPass<NoiseWarpPass>();
    noise->setEnabled(true);
    
    bloom = post.createPass<BloomPass>();
    bloom->setEnabled(true);
    
    currentNoiseFreq = 2.0;
    
    app = ((ofApp*)ofGetAppPtr());
}

void StCvOpNoiseWrap::update(){
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
}

void StCvOpNoiseWrap::draw(){
    ofxUISlider *gsat = (ofxUISlider *)gui->getWidget("SAT"); float sat = gsat->getValue();
    ofxUISlider *gbr = (ofxUISlider *)gui->getWidget("BR"); float br = gbr->getValue();
    ofxUISlider *gzoom = (ofxUISlider *)gui->getWidget("ZOOM"); float zoom = gzoom->getValue();
    
    float controlHue = ofMap(app->oscControl->controlVal[5], 0, 127, 0, 0.6);
    app->drawFbo->fbo.begin();
    app->drawFbo->blendMode = 0;
    ofDisableAlphaBlending();
    ofClear(0,0,0);
    post.begin(cam);
    ofPushMatrix();
    ofScale(zoom, zoom);
    ofColor col; col.setHsb(controlHue * 255, sat * 255, br * 255);
    ofSetColor(col);
    ofRotateZ(ofGetElapsedTimef() * 40.0);
    //ofRotateZ(90);

    /*
    currentNoiseFreq = ofMap(flow.getTotalFlow().length(), 0, 1, 2, 2.1);
    if (flow.getTotalFlow().length() < 10.0) {
        noise->setFrequency(currentNoiseFreq);
    }
     */
    currentNoiseFreq = ofMap(sin(ofGetElapsedTimef() * 1.0), -1, 1, 2.0, 3.0);
    noise->setFrequency(currentNoiseFreq);
    noise->setAmplitude(0.5);

    app->blackmagic->colorTexture.draw(-app->blackmagic->width/2, -app->blackmagic->height/2, app->blackmagic->width, app->blackmagic->height);
    ofPopMatrix();
    post.end();
    app->drawFbo->fbo.end();
    ofDisableBlendMode();
    
    gui->setVisible(getSharedData().guiVisible);
}

void StCvOpNoiseWrap::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("StCvOpNoiseWrap.xml");
    }
}

void StCvOpNoiseWrap::stateExit(){
    gui->setVisible(false);
}
