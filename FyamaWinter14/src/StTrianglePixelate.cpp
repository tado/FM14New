#include "StTrianglePixelate.h"

string StTrianglePixelate::getName(){
    return "StTrianglePixelate";
}

void StTrianglePixelate::setup(){
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("SIMPLE PIXELATE");
    gui->addSpacer();
    gui->addSlider("RADIUS", 20, 200, 60);
    gui->addSlider("CIRCLE SCALE", 0, 2.0, 1.0);
    gui->addSlider("HUE", 0, 2.0, 1.0);
    gui->addSlider("SAT", 0, 2.0, 1.0);
    gui->addSlider("BR", 0, 2.0, 1.0);
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("StTrianglePixelate.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    ofAddListener(gui->newGUIEvent,this,&StTrianglePixelate::guiEvent);
    
    app = ((ofApp*)ofGetAppPtr());
}

void StTrianglePixelate::update(){
    
}

void StTrianglePixelate::draw(){
    int camWidth = 1920;
    int camHeight = 1080;
    
    ofPixelsRef pixels = ((ofApp*)ofGetAppPtr())->blackmagic->colorPixels;
    pixels.resize(camWidth, camHeight);
    
    ofxUISlider *gradius = (ofxUISlider *)gui->getWidget("RADIUS"); float radius = gradius->getValue();
    ofxUISlider *gcircleScale = (ofxUISlider *)gui->getWidget("CIRCLE SCALE"); float circleScale = gcircleScale->getValue();
    ofxUISlider *ghue = (ofxUISlider *)gui->getWidget("HUE"); float hue = ghue->getValue();
    ofxUISlider *gsat = (ofxUISlider *)gui->getWidget("SAT"); float sat = gsat->getValue();
    ofxUISlider *gbr = (ofxUISlider *)gui->getWidget("BR"); float br = gbr->getValue();
    
    app->drawFbo->fbo.begin();
    app->drawFbo->blendMode = 0;
    ofDisableAlphaBlending();
    ofClear(0,0,0);
    ofTranslate(0, -app->drawFbo->top);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetCircleResolution(32);
    if (pixels.size()>0){
        for (int i = 0; i < camWidth; i+=radius){
            for (int j = 0; j < camHeight - radius/2.0; j+=radius){
                ofColor col = pixels.getColor(i + radius / 2.0, j + radius / 2.0);
                ofPushMatrix();
                ofSetRectMode(OF_RECTMODE_CENTER);
                ofTranslate(i, j);
                ofRotateZ(45);
                
                float controlHue;
                controlHue = ofMap(app->oscControl->controlVal[5], 0, 127, 0, 1);
                col.setHsb(col.getHue() * controlHue, col.getSaturation() * sat, col.getBrightness() * br);
                ofSetColor(col);
                ofRect(0, 0, radius * circleScale, radius* circleScale);
                ofSetRectMode(OF_RECTMODE_CORNER);
                ofPopMatrix();
            }
        }
    }
    ofDisableBlendMode();
    app->drawFbo->fbo.end();
    
    gui->setVisible(getSharedData().guiVisible);
}

void StTrianglePixelate::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("StTrianglePixelate.xml");
    }
}

void StTrianglePixelate::stateExit(){
    gui->setVisible(false);
}
