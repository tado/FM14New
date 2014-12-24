#include "StFftDistortStripe.h"
#include "ofApp.h"

string StFftDistortStripe::getName(){
    return "StFftDistortStripe";
}

void StFftDistortStripe::setup(){
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("FFT Distort");
    gui->addSpacer();
    gui->addSlider("TOP SHIFT", 0, 100.0, 50.0);
    gui->addSlider("NOISE SCALE", 1.0, 30.0, 10.0);
    gui->addSlider("SHIFT SPEED", 0.0, 2.0, 1.0);
    gui->addSlider("ZOOM", 1.0, 5.0, 2.0);
    gui->addSpacer();
    gui->addSlider("HUE", 0, 2.0, 1.0);
    gui->addSlider("SAT", 0, 2.0, 1.0);
    gui->addSlider("BR", 0, 2.0, 1.0);
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("StFftDistortStripe.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    ofAddListener(gui->newGUIEvent,this,&StFftDistortStripe::guiEvent);
    app = ((ofApp*)ofGetAppPtr());

    int width = app->blackmagic->width;
    int height = app->blackmagic->height;
    unsigned char pixels[width * height * 4];

    for (int i = 0; i < width * height * 4; i += 4){
        pixels[i] = pixels[i+1] = pixels[i+2] = 255;
        if (i % 16 == 0) {
            pixels[i + 3] = 255;
        } else {
            pixels[i + 3] = 0;
        }
    }
    tex.loadData(pixels, width, height, GL_RGBA);
    createMesh();
}

void StFftDistortStripe::update(){
    ofxUISlider *gnoisescale = (ofxUISlider *)gui->getWidget("NOISE SCALE"); float noisescale = gnoisescale->getValue();
    ofxUISlider *gshiftspeed = (ofxUISlider *)gui->getWidget("SHIFT SPEED"); float shiftspeed = gshiftspeed->getValue();
    
    float distortionStrength = ofMap(app->oscControl->controlVal[4], 0, 127, 0, 10);
    
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

void StFftDistortStripe::draw(){
    ofxUISlider *gtopshift = (ofxUISlider *)gui->getWidget("TOP SHIFT"); float topshift = gtopshift->getValue();
    ofxUISlider *gshiftspeed = (ofxUISlider *)gui->getWidget("SHIFT SPEED"); float shiftspeed = gshiftspeed->getValue();
    ofxUISlider *gzoom = (ofxUISlider *)gui->getWidget("ZOOM"); float zoom = gzoom->getValue();
    ofxUISlider *ghue = (ofxUISlider *)gui->getWidget("HUE"); float hue = ghue->getValue();
    ofxUISlider *gsat = (ofxUISlider *)gui->getWidget("SAT"); float sat = gsat->getValue();
    ofxUISlider *gbr = (ofxUISlider *)gui->getWidget("BR"); float br = gbr->getValue();
    
    
    app->drawFbo->fbo.begin();
    app->drawFbo->blendMode = 0;
    //ofDisableAlphaBlending();
    ofClear(0,0,0,0);
    ofVec2f scale = ofVec2f(ofGetWidth() / float(app->blackmagic->colorTexture.getWidth()),
                            ofGetHeight() / float(app->blackmagic->colorTexture.getHeight()));
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    ofScale(scale.x * zoom, scale.y * zoom);
    ofRotateZ(70);
    ofSetColor(255);
    ofTranslate(0, -app->drawFbo->top + topshift);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    float controlHue = ofMap(app->oscControl->controlVal[5], 0, 127, 0, 1);

    ofColor col; col.setHsb(controlHue * 255, sat * 255, br * 255);
    ofSetColor(col);
    tex.bind();
    ofPushMatrix();
    ofTranslate(-ofGetWidth()/2, -ofGetWidth()/2);
    mesh.draw();
    ofRotateZ(10);
    col.setHsb(int(controlHue * 255 + 127) % 255, sat * 255, br * 255);
    ofSetColor(col);
    mesh.draw();
    ofPopMatrix();
    tex.unbind();
    
    ofPopMatrix();
    app->drawFbo->fbo.end();
    ofDisableAlphaBlending();
}

void StFftDistortStripe::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("StFftDistortStripe.xml");
    }
}

void StFftDistortStripe::createMesh(){
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    stepSize = 16;
    ySteps = ofGetWidth() * 2 / stepSize;
    xSteps = ofGetWidth() * 2 / stepSize;
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

void StFftDistortStripe::stateExit(){
    gui->setVisible(false);
}
