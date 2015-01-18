#include "StFftDrawRect.h"
#include "ofApp.h"

string StFftDrawRect::getName(){
    return "StFftDrawRect";
}

void StFftDrawRect::setup(){
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("FFT Draw Bar");
    gui->addSpacer();
    gui->addSlider("CIRCLE SIZE", 0, 3000, 200.0);
    gui->addIntSlider("SKIP", 1, 120, 4);
    gui->addIntSlider("STEP", 1, 20, 5);
    gui->addIntSlider("FADE", 0, 127, 12);
    gui->addIntSlider("SATURATION", 0, 255, 100);
    gui->addSlider("BRIGHTNESS", 0, 255.0, 5.0);
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("StFftDrawRect.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    ofAddListener(gui->newGUIEvent,this,&StFftDrawRect::guiEvent);
    
    /*
    post->init(ofGetWidth(), ofGetHeight());
    bloom = post->createPass<BloomPass>();
    bloom->setEnabled(true);
     */

    app = ((ofApp*)ofGetAppPtr());
}

void StFftDrawRect::update(){
    gui->setVisible(getSharedData().guiVisible);
}

void StFftDrawRect::draw() {
    ofSetCircleResolution(64);
    ofxUISlider *gcirclesize = (ofxUISlider *)gui->getWidget("CIRCLE SIZE"); float circlesize = gcirclesize->getValue();
    ofxUIIntSlider *gskip = (ofxUIIntSlider *)gui->getWidget("SKIP"); int skip = gskip->getValue();
    ofxUIIntSlider *gstep = (ofxUIIntSlider *)gui->getWidget("STEP"); int step = gstep->getValue();
    //ofxUIRangeSlider *ghue = (ofxUIRangeSlider *)gui->getWidget("HUE"); float hueLow = ghue->getValueLow(); float hueHigh = ghue->getValueHigh();
    ofxUIIntSlider *gfade = (ofxUIIntSlider *)gui->getWidget("FADE"); int fade = gfade->getValue();
    ofxUIIntSlider *gsaturation = (ofxUIIntSlider *)gui->getWidget("SATURATION"); int saturation = gsaturation->getValue();
    ofxUISlider *gbrightness = (ofxUISlider *)gui->getWidget("BRIGHTNESS"); float brightness = gbrightness->getValue();
    
    app->drawFbo->fbo.begin();
    app->drawFbo->blendMode = 1;
    //post->begin();
    //cam.begin();
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetColor(0, 0, 0, fade);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofPushMatrix();

    ofTranslate(ofGetWidth()/2, -1080 / step * 0.5);

    for (int j = 0; j < step; j ++) {
        ofTranslate(0, 1080 / step);
        for (int i = 0; i < app->fft->drawBins.size(); i += skip) {
            float size = ofMap(app->fft->drawBins[i], 0, 1.0, 0, circlesize);
            float x = ofMap(i, 0, app->fft->drawBins.size(), 0, ofGetWidth()/1.2);
            //float hue = ofMap(i, 0, app->fft->drawBins.size(), hueLow, hueHigh);
            float hue = ofMap(app->oscControl->controlVal[5], 0, 127, 0, 0.6);
            ofColor col;
            col.setHsb(hue * 255, saturation, brightness);
            ofSetColor(col);
            
            ofNoFill();
            ofSetLineWidth(1.0);
            ofSetRectMode(OF_RECTMODE_CENTER);
            
            ofPushMatrix();
            ofTranslate(-ofGetWidth()/5, 0);
            ofRect(x, 0, size * 2.0, size + 1080 / step / 2.0);
            ofRect(-x, 0, size * 2.0, size + 1080/ step/ 2.0);
            ofPopMatrix();
            
            ofPushMatrix();
            ofTranslate(ofGetWidth()/5, 0);
            ofRect(x, 0, size * 2.0, size + 1080 / step / 2.0);
            ofRect(-x, 0, size * 2.0, size + 1080 / step / 2.0);
            ofFill();
            ofSetRectMode(OF_RECTMODE_CORNER);
            ofPopMatrix();
        }
    }
    ofPopMatrix();
    ofDisableBlendMode();
    //post->end();
    app->drawFbo->fbo.end();
}

void StFftDrawRect::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("StFftDrawRect.xml");
    }
}

void StFftDrawRect::stateExit(){
    gui->setVisible(false);
    // delete post;
}

void StFftDrawRect::stateEnter(){
    /*
    post = new ofxPostProcessing();
    post->init(app->drawFbo->width, app->drawFbo->height);
    post->createPass<BloomPass>()->setEnabled(true);
    */
}