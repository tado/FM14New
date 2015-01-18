#include "StFftSphereRibbon.h"
#include "ofApp.h"

string StFftSphereRibbon::getName(){
    return "StFftSphereRibbon";
}

void StFftSphereRibbon::setup(){
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("STRIPE SPHERE");
    gui->addSpacer();
    gui->addSlider("NOISE SCALE", 0.0, 5.0, 1.0);
    gui->addSlider("SHIFT SPEED", 0.0, 2.0, 1.0);
    gui->addSlider("INTERPORATION", 0.0, 1.0, 0.2);
    gui->addSlider("RIBBON WIDTH", 0.0, 10.0, 4.0);
    gui->addSlider("ZOOM", 0.0, 3.0, 1.0);
    gui->addSpacer();
    gui->addSlider("HUE", 0, 2.0, 1.0);
    gui->addSlider("SAT", 0, 2.0, 1.0);
    gui->addSlider("BR", 0, 2.0, 1.0);
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("StFftSphereRibbon.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    ofAddListener(gui->newGUIEvent,this,&StFftSphereRibbon::guiEvent);
    app = ((ofApp*)ofGetAppPtr());

    createMesh();
}

void StFftSphereRibbon::update(){
    ofxUISlider *gnoisescale = (ofxUISlider *)gui->getWidget("NOISE SCALE"); float noisescale = gnoisescale->getValue();
    ofxUISlider *gshiftspeed = (ofxUISlider *)gui->getWidget("SHIFT SPEED"); float shiftspeed = gshiftspeed->getValue();
    ofxUISlider *ginterp = (ofxUISlider *)gui->getWidget("INTERPORATION"); float interp = ginterp->getValue();
    ofxUISlider *gthickness = (ofxUISlider *)gui->getWidget("RIBBON WIDTH"); float thickness = gthickness->getValue();
    
    float distortionStrength = ofMap(app->oscControl->controlVal[4], 0, 127, 2, 10);
    
    float fftSum = 0;
    for (int i = 0; i < app->fft->drawBins.size(); i++) {
        fftSum += app->fft->drawBins[i];
    }
    for (int i = 0; i < mesh.getVertices().size(); i++) {
        float noiseX = ofMap(currentVertex[i].x, 0, ofGetWidth(), 0, noisescale);
        float noiseY = ofMap(currentVertex[i].y, 0, ofGetWidth(), 0, noisescale);
        float noiseZ = ofMap(currentVertex[i].y, 0, ofGetWidth(), 0, noisescale);
        float offset = ofNoise(noiseX + ofGetElapsedTimef() * shiftspeed, noiseY + ofGetElapsedTimef() * shiftspeed);
        currentVertex[i] = currentVertex[i].normalize() * (offset * fftSum * distortionStrength);
        float randStrength = distortionStrength * 5.0;
        currentVertex[i] +=  ofVec3f(ofRandom(-randStrength, randStrength), ofRandom(-randStrength, randStrength), ofRandom(-randStrength, randStrength));
        mesh.setVertex(i, currentVertex[i]);
        interplateVertex[i] += (currentVertex[i] - interplateVertex[i]) * interp;
        ribbons[i]->thickness = thickness;
        ribbons[i]->update(interplateVertex[i]);
    }
    
    gui->setVisible(getSharedData().guiVisible);
}

void StFftSphereRibbon::draw(){
    ofxUISlider *gshiftspeed = (ofxUISlider *)gui->getWidget("SHIFT SPEED"); float shiftspeed = gshiftspeed->getValue();
    ofxUISlider *gzoom = (ofxUISlider *)gui->getWidget("ZOOM"); float zoom = gzoom->getValue();
    ofxUISlider *ghue = (ofxUISlider *)gui->getWidget("HUE"); float hue = ghue->getValue();
    ofxUISlider *gsat = (ofxUISlider *)gui->getWidget("SAT"); float sat = gsat->getValue();
    ofxUISlider *gbr = (ofxUISlider *)gui->getWidget("BR"); float br = gbr->getValue();
    
    
    app->drawFbo->fbo.begin();
    app->drawFbo->blendMode = 0;
    post->begin(cam);
    ofScale(zoom, zoom);
    ofRotateX(ofGetElapsedTimef() * shiftspeed);
    ofRotateY(ofGetElapsedTimef() * shiftspeed * 1.1);
    ofRotateZ(ofGetElapsedTimef() * shiftspeed * 1.2);
    ofDisableAlphaBlending();
    ofClear(0,0,0);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    float controlHue;
    controlHue = ofMap(app->oscControl->controlVal[5], 0, 127, 0, 1);
    ofColor col; col.setHsb(controlHue * 255, sat * 255, br * 255);
    ofSetColor(col);
    glPointSize(5.0);

    for (int i = 0; i < mesh.getVertices().size(); i++) {
        ribbons[i]->draw();
    }
    
    ofRotateX(-10);

    ofDisableDepthTest();
    glDisable(GL_CULL_FACE);
    post->end();
    app->drawFbo->fbo.end();
    ofDisableAlphaBlending();
}

void StFftSphereRibbon::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("StFftSphereRibbon.xml");
    }
}

void StFftSphereRibbon::createMesh(){
    mesh = ofSpherePrimitive(5, 8).getMesh();
    for (int i = 0; i < mesh.getVertices().size(); i++) {
        ofVec2f texCoord = mesh.getTexCoord(i);
        currentVertex.push_back(ofVec3f(mesh.getVertices()[i].x, mesh.getVertices()[i].y, mesh.getVertices()[i].z));
        interplateVertex.push_back(currentVertex[currentVertex.size()-1]);
        randomVec.push_back(ofVec3f(ofRandom(-1, 1),ofRandom(-1, 1),ofRandom(-1, 1)));
        int hue = ofMap(i, 0, mesh.getVertices().size()-1, 0, 80);
        ofColor col; col.setHsb(hue, 180, 127);
        ofxTwistedRibbon *r = new ofxTwistedRibbon();
        r->length = 40;
        r->thickness = 4;
        r->color = col;
        ribbons.push_back(r);
    }
    mesh.setMode(OF_PRIMITIVE_POINTS);
}

void StFftSphereRibbon::stateEnter(){
    createMesh();
    post = new ofxPostProcessing();
    post->init(app->drawFbo->width, app->drawFbo->height);
    post->createPass<BloomPass>()->setEnabled(true);
}

void StFftSphereRibbon::stateExit(){
    ribbons.clear();
    currentVertex.clear();
    interplateVertex.clear();
    gui->setVisible(false);
    delete post;
}