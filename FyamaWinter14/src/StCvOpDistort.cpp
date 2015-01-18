#include "StCvOpDistort.h"
#include "ofApp.h"

string StCvOpDistort::getName(){
    return "StCvOpDistort";
}

void StCvOpDistort::setup(){
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("OPTICAL PARTICLE");
    gui->addSpacer();
    gui->addSlider("PYR SCALE", 0, 0.99, 0.5);
    gui->addIntSlider("LEVELS", 1, 10, 5);
    gui->addIntSlider("WIN SIZE", 1, 100, 30);
    gui->addIntSlider("ITERATION", 1, 20, 2);
    gui->addToggle("GAUSS", false);
    gui->addSpacer();
    gui->addSlider("THRESH", 0, 2.0, 1.0);
    gui->addIntSlider("SKIP", 1, 10, 1);
    gui->addSlider("TOP SHIFT", 0, 100.0, 50.0);
    gui->addSpacer();
    gui->addSlider("HUE", 0, 2.0, 1.0);
    gui->addSlider("SAT", 0, 2.0, 1.0);
    gui->addSlider("BR", 0, 2.0, 1.0);
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("StCvOpDistort.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    ofAddListener(gui->newGUIEvent,this,&StCvOpDistort::guiEvent);
    
    app = ((ofApp*)ofGetAppPtr());
    createMesh();
}

void StCvOpDistort::update(){
    //CV params
    ofxUISlider *p = (ofxUISlider *)gui->getWidget("PYR SCALE"); pyrScale = p->getValue();
    ofxUIIntSlider *l = (ofxUIIntSlider *)gui->getWidget("LEVELS"); levels = l->getValue();
    ofxUIIntSlider *w = (ofxUIIntSlider *)gui->getWidget("WIN SIZE"); winsize = w->getValue();
    ofxUIIntSlider *it = (ofxUIIntSlider *)gui->getWidget("ITERATION"); iterations = it->getValue();
    ofxUIToggle *gs = (ofxUIToggle *)gui->getWidget("GAUSS"); OPTFLOW_FARNEBACK_GAUSSIAN = gs->getValue();
    ofxUISlider *gthresh = (ofxUISlider *)gui->getWidget("THRESH"); float thresh = gthresh->getValue();
    ofxUIIntSlider *gskip = (ofxUIIntSlider *)gui->getWidget("SKIP"); int skip = gskip->getValue();
    
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
    
    
    flow.calcOpticalFlow(pix);
    
    int i = 0;
    
    float distortionStrength = ofMap(app->oscControl->controlVal[4], 0, 127, 0, 20);
    
    ofVec2f scale = ofVec2f(flow.getWidth() / float(ofGetWidth()), flow.getHeight() / float(ofGetHeight()));
    for(int y = 1; y + 1 < ySteps; y++) {
        for(int x = 1; x + 1 < xSteps; x++) {
            int i = y * xSteps + x;
            ofVec2f position(x * stepSize * scale.x, y * stepSize * scale.y);
            ofRectangle area(position - ofVec2f(stepSize * scale.x, stepSize * scale.y) / 2.0, stepSize * scale.x, stepSize * scale.y);
            ofVec2f offset;
            offset = flow.getAverageFlowInRegion(area);
            if (offset.length() < thresh && ofGetFrameNum() % skip == 0) {
                //currentVertex[i] += ((position + distortionStrength * offset) / scale - currentVertex[i]) * 0.3;
                currentVertex[i] += ((position + distortionStrength * offset) / scale - currentVertex[i]);
            }
            mesh.setVertex(i, currentVertex[i]);
            i++;
        }
    }
    
    gui->setVisible(getSharedData().guiVisible);
}

void StCvOpDistort::draw(){
    ofxUISlider *gtopshift = (ofxUISlider *)gui->getWidget("TOP SHIFT"); float topshift = gtopshift->getValue();
    ofxUISlider *ghue = (ofxUISlider *)gui->getWidget("HUE"); float hue = ghue->getValue();
    ofxUISlider *gsat = (ofxUISlider *)gui->getWidget("SAT"); float sat = gsat->getValue();
    ofxUISlider *gbr = (ofxUISlider *)gui->getWidget("BR"); float br = gbr->getValue();
    
    app->drawFbo->fbo.begin();
    app->drawFbo->blendMode = 0;
    post->begin();
    ofDisableAlphaBlending();
    ofClear(0,0,0);
    ofVec2f scale = ofVec2f(ofGetWidth() / float(app->blackmagic->colorTexture.getWidth()),
                            ofGetHeight() / float(app->blackmagic->colorTexture.getHeight()));
    ofPushMatrix();
    ofScale(scale.x, -scale.y);
    ofTranslate(0, -app->blackmagic->height / scale.y);
    ofSetColor(255);
    //ofTranslate(0, -app->drawFbo->top + topshift);
    //ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    float controlHue = ofMap(app->oscControl->controlVal[5], 0, 127, 0, 0.6);
    ofColor col; col.setHsb(controlHue * 255, sat * 255, br * 255);
    ofSetColor(col);
    
    app->blackmagic->colorTexture.bind();
    mesh.draw();
    app->blackmagic->colorTexture.unbind();
    
    /*
    ofSetLineWidth(1);
    mesh.drawWireframe();
     */
    
    ofPopMatrix();
    post->end();
    app->drawFbo->fbo.end();
    ofDisableAlphaBlending();
}

void StCvOpDistort::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("StCvOpDistort.xml");
    }
}

void StCvOpDistort::createMesh(){
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    stepSize = 20;
    ySteps = app->blackmagic->colorTexture.getHeight() / stepSize;
    xSteps = app->blackmagic->colorTexture.getWidth() / stepSize + 1;
    for(int y = 0; y < ySteps; y++) {
        for(int x = 0; x < xSteps; x++) {
            mesh.addVertex(ofVec2f(x * stepSize, y * stepSize));
            mesh.addTexCoord(ofVec2f(x * stepSize, y * stepSize));
            currentVertex.push_back(ofVec2f(x * stepSize, y * stepSize));
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

void StCvOpDistort::stateExit(){
    gui->setVisible(false);
    delete post;
}

void StCvOpDistort::stateEnter(){
    post = new ofxPostProcessing();
    post->init(ofGetWidth(), ofGetHeight());
    bloom = post->createPass<BloomPass>();
    bloom->setEnabled(true);
}