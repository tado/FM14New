#include "StKaleidoscopeGJ.h"

string StKaleidoscopeGJ::getName(){
    return "StKaleidoscopeGJ";
}

void StKaleidoscopeGJ::setup(){
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
    gui->loadSettings("StKaleidoscopeGJ.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    ofAddListener(gui->newGUIEvent,this,&StKaleidoscopeGJ::guiEvent);
    
    app = ((ofApp*)ofGetAppPtr());
}

void StKaleidoscopeGJ::update(){
    ofxUISlider *gsegment = (ofxUISlider *)gui->getWidget("SEGMENT"); float segment = gsegment->getValue();
    ofxUISlider *gnoiseamp = (ofxUISlider *)gui->getWidget("NOISE AMP"); float noiseamp = gnoiseamp->getValue();
    
    fftSum = 0;
    for (int i = 0; i < app->fft->drawBins.size(); i++) {
        fftSum += app->fft->drawBins[i];
    }
    float noiseStrength = ofMap(app->oscControl->controlVal[4], 0, 127, 5, 10);
    noise->setFrequency(1.0 + fftSum/ float(app->fft->drawBins.size()) * noiseStrength);
    //noise->setAmplitude(fftSum/ float(app->fft->drawBins.size()) * noiseStrength * noiseamp);
    
    kaleido->setSegments(segment);
}

void StKaleidoscopeGJ::draw(){
    ofxUISlider *gsat = (ofxUISlider *)gui->getWidget("SAT"); float sat = gsat->getValue();
    ofxUISlider *gbr = (ofxUISlider *)gui->getWidget("BR"); float br = gbr->getValue();
    ofxUISlider *gzoom = (ofxUISlider *)gui->getWidget("ZOOM"); float zoom = gzoom->getValue();
    
    float hue =ofMap(app->oscControl->controlVal[5], 0, 127, 0.0, 1.0);
    app->drawFbo->fbo.begin();
    app->drawFbo->blendMode = 0;
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetColor(0, 0, 0, 20);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    ofTranslate(0, -app->drawFbo->top);
    //ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofTranslate(0, -app->drawFbo->top);

    ofScale(1.0 / fxRatio, 1.0 / fxRatio);
    post->begin(cam);
    ofPushMatrix();
    ofScale(zoom, zoom);
    ofColor col; col.setHsb(hue * 255, sat * 255, br * 255);
    ofSetColor(col);
    ofRotateZ(ofGetElapsedTimef() * 40.0);
    app->blackmagic->colorTexture.draw(-app->blackmagic->width/2, -app->blackmagic->height/2, app->blackmagic->width, app->blackmagic->height);
    ofPopMatrix();
    post->end();
    app->drawFbo->fbo.end();
    ofDisableBlendMode();
    
    gui->setVisible(getSharedData().guiVisible);
}

void StKaleidoscopeGJ::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("StKaleidoscopeGJ.xml");
    }
}

void StKaleidoscopeGJ::stateExit(){
    gui->setVisible(false);
    delete post;
}

void StKaleidoscopeGJ::stateEnter(){
    fxRatio = 0.5;
    
    post = new ofxPostProcessing();
    post->init(ofGetWidth() * fxRatio, ofGetHeight() * fxRatio);
    
    kaleido = post->createPass<KaleidoscopePass>();
    kaleido->setSegments(3.0);
    kaleido->setEnabled(true);
    noise = post->createPass<NoiseWarpPass>();
    noise->setEnabled(true);
    noise->setFrequency(2.0);
    
    bloom = post->createPass<BloomPass>();
    bloom->setEnabled(true);
}
