#include "StFftBlueBeam.h"
#include "ofApp.h"

string StFftBlueBeam::getName(){
    return "StFftBlueBeam";
}

void StFftBlueBeam::setup(){
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("FFT BEAM");
    gui->addSpacer();
    gui->addSlider("SPEED", 10, 2000, 500);
    gui->addIntSlider("LENGTH", 1, 40, 10);
    gui->addSlider("THICKNESS", 0.0, 400.0, 100.0);
    gui->addSpacer();
    //gui->addSlider("HUE", 0, 2.0, 1.0);
    gui->addSlider("SAT", 0, 2.0, 1.0);
    gui->addSlider("BR", 0, 2.0, 1.0);
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("StFftBlueBeam.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    ofAddListener(gui->newGUIEvent,this,&StFftBlueBeam::guiEvent);
    app = ((ofApp*)ofGetAppPtr());
    
    post.init(app->drawFbo->width, app->drawFbo->height);
    post.createPass<BloomPass>()->setEnabled(true);
    
    cam.setFarClip(20000);
    
    for (int i = 0; i < NUM; i++) {
        float angle = ofRandom(PI * 2);
        float length = ofRandom(500, ofGetWidth()*2);
        float x = cos(angle) * length;
        float y = sin(angle) * length;
        ofVec3f pos = ofVec3f(x, y, ofRandom(-20000, 0));
        position.push_back(pos);
        ofxTwistedRibbon *r = new ofxTwistedRibbon();
        r->length = 8;
        r->thickness = 20;
        ofColor col = ofColor(255);
        r->color = col;
        ribbons.push_back(r);
    }   
}

void StFftBlueBeam::update(){
    ofxUISlider *gspeed = (ofxUISlider *)gui->getWidget("SPEED"); float speed = gspeed->getValue();
    ofxUIIntSlider *glength = (ofxUIIntSlider *)gui->getWidget("LENGTH"); int length = glength->getValue();
    ofxUISlider *gthick = (ofxUISlider *)gui->getWidget("THICKNESS"); float thick = gthick->getValue();
    //ofxUISlider *ghue = (ofxUISlider *)gui->getWidget("HUE"); float hue = ghue->getValue();
    ofxUISlider *gsat = (ofxUISlider *)gui->getWidget("SAT"); float sat = gsat->getValue();
    ofxUISlider *gbr = (ofxUISlider *)gui->getWidget("BR"); float br = gbr->getValue();
    
    
    float strength = ofMap(app->oscControl->controlVal[4], 0, 127, 0, 2.0);
    
    //float controlHue;
    //controlHue = ofMap(app->oscControl->controlVal[5], 0, 127, 0, 0.6);
    //ofColor col; col.setHsb(controlHue * 255, sat * 255, br * 255);
    
    float fftSum = 0;
    for (int i = 0; i < app->fft->drawBins.size(); i++) {
        fftSum += app->fft->drawBins[i];
    }
    for (int i = 0; i < NUM; i++) {
        position[i].z += speed;
        if (position[i].z > 1000) {
            float angle = ofRandom(PI * 2);
            float length = ofRandom(500, ofGetWidth()*2);
            position[i].x = cos(angle) * length;
            position[i].y = sin(angle) * length;
            position[i].z = -20000;
            ribbons[i]->points.clear();
        }
        ribbons[i]->thickness =  app->fft->drawBins[i] * thick * strength;
        ribbons[i]->length = length;
        //ribbons[i]->color = col;
        ribbons[i]->update(position[i]);
    }
    gui->setVisible(getSharedData().guiVisible);
}

void StFftBlueBeam::draw(){
    ofxUISlider *gsat = (ofxUISlider *)gui->getWidget("SAT"); float sat = gsat->getValue();
    ofxUISlider *gbr = (ofxUISlider *)gui->getWidget("BR"); float br = gbr->getValue();
    
    app->drawFbo->fbo.begin();
    app->drawFbo->blendMode = 1;
    post.begin(cam);
    
    ofDisableAlphaBlending();
    ofSetColor(0);
    ofRect(-ofGetWidth(), -ofGetHeight(), ofGetWidth() * 2, ofGetHeight() * 2);
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofFill();
    ofRotateZ(45);
   
    float controlHue;
    controlHue = ofMap(app->oscControl->controlVal[5], 0, 127, 0, 0.65);
    ofColor col; col.setHsb(controlHue * 255, sat * 255, br * 255);

    for (int i = 0; i < NUM; i++) {
        for (int j = 0; j < ribbons[i]->colors.size(); j++) {
            ribbons[i]->colors[j] = col;
        }
        ribbons[i]->draw();
    }
    post.end();
    app->drawFbo->fbo.end();
    ofDisableAlphaBlending();
}

void StFftBlueBeam::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("StFftBlueBeam.xml");
    }
}

void StFftBlueBeam::createMesh(){
    
}

void StFftBlueBeam::stateExit(){
    gui->setVisible(false);
}
