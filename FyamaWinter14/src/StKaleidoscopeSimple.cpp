#include "StKaleidoscopeSimple.h"

string StKaleidoscopeSimple::getName(){
    return "StKaleidoscopeSimple";
}

void StKaleidoscopeSimple::setup(){
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("KALEIDO SCOPE");
    // gui->addSlider("HUE", 0, 1.0, 0.5);
    gui->addSlider("SEGMENT", 0, 8.0, 3.0);
    gui->addSlider("NOISE AMP", 0.0, 3.0, 1.0);
    gui->addSlider("ZOOM", 1.0, 8.0, 2.0);
    gui->addSlider("SAT", 0, 2.0, 1.0);
    gui->addSlider("BR", 0, 2.0, 1.0);
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("StKaleidoscopeSimple.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    ofAddListener(gui->newGUIEvent,this,&StKaleidoscopeSimple::guiEvent);
    
    post.init(ofGetWidth(), ofGetHeight());
    
    kaleido = post.createPass<KaleidoscopePass>();
    kaleido->setSegments(8.0);
    kaleido->setEnabled(true);
    
    bloom = post.createPass<BloomPass>();
    bloom->setEnabled(true);
    
    app = ((ofApp*)ofGetAppPtr());
}

void StKaleidoscopeSimple::update(){
    ofxUISlider *gsegment = (ofxUISlider *)gui->getWidget("SEGMENT"); float segment = gsegment->getValue();
    ofxUISlider *gnoiseamp = (ofxUISlider *)gui->getWidget("NOISE AMP"); float noiseamp = gnoiseamp->getValue();   
    kaleido->setSegments(segment);
}

void StKaleidoscopeSimple::draw(){
    ofxUISlider *gsat = (ofxUISlider *)gui->getWidget("SAT"); float sat = gsat->getValue();
    ofxUISlider *gbr = (ofxUISlider *)gui->getWidget("BR"); float br = gbr->getValue();
    ofxUISlider *gzoom = (ofxUISlider *)gui->getWidget("ZOOM"); float zoom = gzoom->getValue();
    
    float controlHue = ofMap(app->oscControl->controlVal[5], 0, 127, 0, 0.6);
    app->drawFbo->fbo.begin();
    app->drawFbo->blendMode = 1;
    ofDisableAlphaBlending();
    ofClear(0,0,0);
    //ofTranslate(0, -app->drawFbo->top);
    post.begin(cam);
    ofPushMatrix();
    ofScale(zoom, zoom);
    ofColor col; col.setHsb(controlHue * 255, sat * 255, br * 255);
    ofSetColor(col);
    ofRotateZ(ofGetElapsedTimef() * 20.0);
    app->blackmagic->colorTexture.draw(-app->blackmagic->width/2, -app->blackmagic->height/2, app->blackmagic->width, app->blackmagic->height);
    ofPopMatrix();
    post.end();
    app->drawFbo->fbo.end();
    ofDisableBlendMode();
    
    gui->setVisible(getSharedData().guiVisible);
}

void StKaleidoscopeSimple::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("StKaleidoscopeSimple.xml");
    }
}

void StKaleidoscopeSimple::stateExit(){
    gui->setVisible(false);
}
