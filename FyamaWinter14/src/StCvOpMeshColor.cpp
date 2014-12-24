#include "StCvOpMeshColor.h"
#include "ofApp.h"

string StCvOpMeshColor::getName(){
    return "StCvOpMeshColor";
}

void StCvOpMeshColor::setup(){
    mesh.setMode(OF_PRIMITIVE_POINTS);
    
    gui = new ofxUICanvas();
    gui->init(212, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("OPTICAL MESH");
    gui->addSpacer();
    gui->addSlider("PYR SCALE", 0, 0.99, 0.5);
    gui->addIntSlider("LEVELS", 1, 10, 5);
    gui->addIntSlider("WIN SIZE", 1, 100, 30);
    gui->addIntSlider("ITERATION", 1, 20, 2);
    gui->addToggle("GAUSS", false);
    gui->addSpacer();
    gui->addSlider("SKIP", 0, 2, 1.0);
    gui->addToggle("CLEAR BACKGROUND", true);
    gui->addIntSlider("MAX", 0, 100000, 20000);
    gui->addSlider("THRESH", 0, 2.0, 1.0);
    gui->addSlider("RADIUS", 0, 40.0, 1.0);
    gui->addSlider("FRICTION", 0, 5.0, 0.5);
    gui->addSlider("ACCEL", 0, 4.0, 1.2);
    gui->addSlider("WIND", 0, 0.05, 1.0);
    gui->addIntSlider("FADE", 0, 63, 12);
    gui->addSpacer();
    gui->addSlider("HUE", 0, 2.0, 1.0);
    gui->addSlider("SAT", 0, 4.0, 1.0);
    gui->addSlider("BR", 0, 2.0, 1.0);
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->loadSettings("StCvOpMeshColor.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    
    ofAddListener(gui->newGUIEvent,this,&StCvOpMeshColor::guiEvent);
    
    ofxUIIntSlider *gmax = (ofxUIIntSlider *)gui->getWidget("MAX"); int max = gmax->getValue();
    for (int i = 0; i < max; i++) {
        Particle *p = new Particle();
        ofColor col; col = ofColor(255);
        p->setup(ofVec3f(ofRandom(SCREEN_WIDTH), ofRandom(SCREEN_HEIGHT), 0), ofVec3f(0, 0, 0), col);
        particles.push_back(p);
    }
    
    app = ((ofApp*)ofGetAppPtr());
}

void StCvOpMeshColor::update(){
    //CV params
    ofxUISlider *p = (ofxUISlider *)gui->getWidget("PYR SCALE"); pyrScale = p->getValue();
    ofxUIIntSlider *l = (ofxUIIntSlider *)gui->getWidget("LEVELS"); levels = l->getValue();
    ofxUIIntSlider *w = (ofxUIIntSlider *)gui->getWidget("WIN SIZE"); winsize = w->getValue();
    ofxUIIntSlider *it = (ofxUIIntSlider *)gui->getWidget("ITERATION"); iterations = it->getValue();
    ofxUIToggle *gs = (ofxUIToggle *)gui->getWidget("GAUSS"); OPTFLOW_FARNEBACK_GAUSSIAN = gs->getValue();
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
    if (pix.size() > 0) {
        flow.calcOpticalFlow(pix);
    }
    
    ofxUISlider *gthresh = (ofxUISlider *)gui->getWidget("THRESH"); float thresh = gthresh->getValue();
    ofxUISlider *gskip = (ofxUISlider *)gui->getWidget("SKIP"); float skip = gskip->getValue();
    ofxUISlider *gaccel = (ofxUISlider *)gui->getWidget("ACCEL"); float accel = gaccel->getValue();
    ofxUISlider *gwind = (ofxUISlider *)gui->getWidget("WIND"); float wind = gwind->getValue();
    ofxUISlider *gfriction = (ofxUISlider *)gui->getWidget("FRICTION"); float friction = gfriction->getValue();
    ofxUISlider *ghue = (ofxUISlider *)gui->getWidget("HUE"); float hue = ghue->getValue();
    ofxUISlider *gsat = (ofxUISlider *)gui->getWidget("SAT"); float sat = gsat->getValue();
    ofxUISlider *gbr = (ofxUISlider *)gui->getWidget("BR"); float br = gbr->getValue();
    
    for (int i = 0; i < particles.size(); i++) {
        float ratio = SCREEN_WIDTH / (pix.getWidth() - skip * 6);
        int x = particles[i]->position.x / ratio + (skip * 2);
        int y = particles[i]->position.y / ratio + (skip * 2);
        particles[i]->resetForce();
        particles[i]->friction = friction / 100.0;
        
        ofVec2f force;
        
        if (x > 0 && y > 0 && x < flow.getWidth() - skip && y < flow.getHeight() - skip) {
            ofRectangle region = ofRectangle(x, y, skip, skip);
            force = flow.getAverageFlowInRegion(region);
        }
        
        // wind
        force += ofVec3f(wind, wind * 0.1, 0.0);
        
        particles[i]->addForce(force * accel);
        particles[i]->updateForce();
        particles[i]->updatePos();
        particles[i]->throughOfWalls();
        
        ofColor col  = pix.getColor(x + skip * 2, y);
        

        float controlHue;
        controlHue = ofMap(app->oscControl->controlVal[5], 0, 127, 1.2, 0.7);
        int h = controlHue;
        col.setHsb(int(controlHue * 255) % 255, 255, 255);
        
        /*
         float controlRB;
         controlRB = ofMap(app->oscControl->controlVal[5], 0, 127, 0, 1.0);
         col.set(255 - controlRB * 255, 0, controlRB * 255);
         */
        particles[i]->col = col;
    }
    
    gui->setVisible(getSharedData().guiVisible);
}

void StCvOpMeshColor::draw(){
    ofxUIIntSlider *gmax = (ofxUIIntSlider *)gui->getWidget("MAX"); int max = gmax->getValue();
    ofxUISlider *gradius = (ofxUISlider *)gui->getWidget("RADIUS"); float radius = gradius->getValue();
    ofxUIIntSlider *gfade = (ofxUIIntSlider *)gui->getWidget("FADE"); int fade = gfade->getValue();
    ofxUIToggle *gclear = (ofxUIToggle *)gui->getWidget("CLEAR BACKGROUND"); bool clear = gclear->getValue();
    ofPixelsRef pix = ((ofApp*)ofGetAppPtr())->blackmagic->colorPixels;
    
    app->drawFbo->fbo.begin();
    app->drawFbo->blendMode = 1;
    ofTranslate(0, -app->drawFbo->top);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetColor(0, 0, 0, fade);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    
    mesh.clear();
    for (int i = 0; i < particles.size(); i++) {
        mesh.addVertex(ofVec3f(particles[i]->position.x,
                               particles[i]->position.y, 0));
        //mesh.addColor(ofFloatColor(particles[i]->col.r/255.0, particles[i]->col.g/255.0, particles[i]->col.b/255.0));
        
        mesh.addColor(ofFloatColor(particles[i]->col.r/255.0, particles[i]->col.g/255.0, particles[i]->col.b/255.0));
    }
    
    //ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetColor(255);
    glPointSize(radius);
    mesh.draw();
    
    ofDisableBlendMode();
    app->drawFbo->fbo.end();
    
    gui->setVisible(getSharedData().guiVisible);
}

void StCvOpMeshColor::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("StCvOpMeshColor.xml");
    }
}

void StCvOpMeshColor::stateExit(){
    gui->setVisible(false);
    for (int i = 0; i < particles.size(); i++) {
        particles[i]->position = ofVec2f(ofRandom(SCREEN_WIDTH), ofRandom(SCREEN_HEIGHT));
    }
}