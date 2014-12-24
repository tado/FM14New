#include "StFftSimplePlot.h"
#include "ofApp.h"

string StFftSimplePlot::getName(){
    return "StFftSimplePlot";
}

void StFftSimplePlot::setup(){
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("FFT Simple Plot");
    gui->addSpacer();
    gui->addIntSlider("PLOT HEIGHT", 0, ofGetHeight(), 500);
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("StFftSimplePlot.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    ofAddListener(gui->newGUIEvent,this,&StFftSimplePlot::guiEvent);

    app = ((ofApp*)ofGetAppPtr());
}

void StFftSimplePlot::update(){
    gui->setVisible(getSharedData().guiVisible);
}

void StFftSimplePlot::draw() {
    app->drawFbo->fbo.begin();
    ofDisableAlphaBlending();
    ofClear(0,0,0);
    ofxUIIntSlider *gplotsize = (ofxUIIntSlider *)gui->getWidget("PLOT HEIGHT"); int plotsize = gplotsize->getValue();
    ofPushMatrix();
    ofTranslate(0, app->drawFbo->height / 2 + plotsize / 2);
    ofScale((ofGetWidth() - 40) / float(app->fft->drawBins.size()) , 1.0);
    plot(app->fft->drawBins, -plotsize, plotsize / 2);
    ofPopMatrix();
    app->drawFbo->fbo.end();
}

void StFftSimplePlot::plot(vector<float>& buffer, float scale, float offset) {
    ofNoFill();
    int n = buffer.size();
    ofSetColor(255);
    glPushMatrix();
    glTranslatef(0, scale / 2 + offset, 0);
    ofSetLineWidth(2.0);
    ofBeginShape();
    for (int i = 0; i < n; i++) {
        ofVertex(i, sqrt(buffer[i]) * scale);
    }
    ofEndShape();
    ofSetLineWidth(1.0);
    glPopMatrix();
    ofFill();
}

void StFftSimplePlot::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("StFftSimplePlot.xml");
    }
}

void StFftSimplePlot::stateExit(){
    gui->setVisible(false);
}
