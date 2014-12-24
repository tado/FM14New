#include "StFftDistortNotex.h"
#include "ofApp.h"

string StFftDistortNotex::getName(){
    return "StFftDistortNotex";
}

void StFftDistortNotex::setup(){
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("FFT Distort");
    gui->addSpacer();
    gui->addSlider("TOP SHIFT", 0, 100.0, 50.0);
    gui->addSlider("NOISE SCALE", 1.0, 30.0, 10.0);
    gui->addSlider("SHIFT SPEED", 0.0, 2.0, 1.0);
    gui->addSlider("LINE WIDTH", 0, 3.0, 1.0);
    gui->addSpacer();
    gui->addSlider("HUE", 0, 2.0, 1.0);
    gui->addSlider("SAT", 0, 2.0, 1.0);
    gui->addSlider("BR", 0, 2.0, 1.0);
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("StFftDistortNotex.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    ofAddListener(gui->newGUIEvent,this,&StFftDistortNotex::guiEvent);
    app = ((ofApp*)ofGetAppPtr());
    
    createMesh();
}

void StFftDistortNotex::update(){
    ofxUISlider *gnoisescale = (ofxUISlider *)gui->getWidget("NOISE SCALE"); float noisescale = gnoisescale->getValue();
    ofxUISlider *gshiftspeed = (ofxUISlider *)gui->getWidget("SHIFT SPEED"); float shiftspeed = gshiftspeed->getValue();
    
    float distortionStrength = ofMap(app->oscControl->controlVal[4], 0, 127, 1, 8);
    
    float fftSum = 0;
    for (int i = 0; i < app->fft->drawBins.size(); i++) {
        fftSum += app->fft->drawBins[i];
    }
    
    int i = 0;
    for(int y = 1; y + 1 < ySteps; y++) {
        for(int x = 1; x + 1 < xSteps; x++) {
            int i = y * xSteps + x;
            float noiseX = ofMap(currentVertex[i].x, 0, ofGetWidth(), 0, noisescale);
            float noiseY = ofMap(currentVertex[i].y, 0, ofGetWidth(), 0, noisescale);
            float offset = ofNoise(noiseX + ofGetElapsedTimef() * shiftspeed, noiseY + ofGetElapsedTimef() * shiftspeed);
            currentVertex[i].z = offset * distortionStrength * fftSum;
            mesh.setVertex(i, currentVertex[i]);
            i++;
        }
    }
    
    gui->setVisible(getSharedData().guiVisible);
}

void StFftDistortNotex::draw(){
    ofxUISlider *gtopshift = (ofxUISlider *)gui->getWidget("TOP SHIFT"); float topshift = gtopshift->getValue();
    ofxUISlider *glinewidth = (ofxUISlider *)gui->getWidget("LINE WIDTH"); float linewidth = glinewidth->getValue();
    ofxUISlider *gsat = (ofxUISlider *)gui->getWidget("SAT"); float sat = gsat->getValue();
    ofxUISlider *gbr = (ofxUISlider *)gui->getWidget("BR"); float br = gbr->getValue();
    
    app->drawFbo->fbo.begin();
    app->drawFbo->blendMode = 0;
    ofDisableAlphaBlending();
    ofClear(0,0,0);
    ofVec2f scale = ofVec2f(ofGetWidth() / float(app->blackmagic->colorTexture.getWidth()),
                            ofGetHeight() / float(app->blackmagic->colorTexture.getHeight()));
    ofPushMatrix();
    //ofScale(scale.x, scale.y);
    ofSetColor(255);
    //ofTranslate(0, -app->drawFbo->top + topshift);
    ofDisableBlendMode();
    float hue =ofMap(app->oscControl->controlVal[5], 0, 127, 0.7, 1.0);
    ofColor col; col.setHsb(hue * 255, sat * 255, br * 255);
    ofSetColor(col);
    ofSetLineWidth(linewidth);
    if (linewidth > 0) {
        mesh.drawWireframe();
    }
    ofPopMatrix();
    app->drawFbo->fbo.end();
    ofDisableAlphaBlending();
}

void StFftDistortNotex::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("StFftDistortNotex.xml");
    }
}

void StFftDistortNotex::createMesh(){
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    stepSize = 10;
    ySteps = ofGetHeight() / stepSize;
    xSteps = ofGetWidth() / stepSize + 1;
    for(int y = 0; y < ySteps; y++) {
        for(int x = 0; x < xSteps; x++) {
            mesh.addVertex(ofVec3f(x * stepSize, y * stepSize, 0));
            mesh.addTexCoord(ofVec3f(x * stepSize, y * stepSize, 0));
            currentVertex.push_back(ofVec3f(x * stepSize, y * stepSize, 0));
        }
    }
    for(int y = 0; y + 1 < ySteps; y++) {
        for(int x = 0; x + 1 < xSteps; x++) {
            int nw = y * xSteps + x;
            int ne = nw + 1;
            int sw = nw + xSteps;
            int se = sw + 1;
            mesh.addIndex(nw);
            mesh.addIndex(ne);
            mesh.addIndex(se);
            mesh.addIndex(nw);
            mesh.addIndex(se);
            mesh.addIndex(sw);
        }
    }
}

void StFftDistortNotex::stateExit(){
    gui->setVisible(false);
}
