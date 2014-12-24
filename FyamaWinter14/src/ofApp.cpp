#include "ofApp.h"

#include "StBlank.h"
#include "StSimplePixelate.h"
#include "StCvOpDraw.h"
#include "StCvOpParticle.h"
#include "StCvOpParticleGJ.h"
#include "StCvOpParticlePath.h"
#include "StCvOpMesh.h"
#include "StCvOpMeshColor.h"
#include "StCvOpNote.h"
#include "StCvOpSparkle.h"
#include "StCvOpDistort.h"
#include "StFftDistortNotex.h"
#include "StSakuraParticle.h"
#include "StRgbDelay.h"
#include "StKaleidoscope.h"
#include "StKaleidoscopeGJ.h"
#include "StKaleidoscopeSimple.h"
#include "StCvOpNoiseWrap.h"
#include "StTrianglePixelate.h"
//#include "StSphereSoundWave.h"

#include "StSoundWave.h"
#include "StFftDrawCircle.h"
#include "StFftDrawRect.h"
#include "StFftDrawCircleBlue.h"
#include "StFftDot.h"
#include "StFftDrawBar.h"
#include "StFftBox.h"
#include "StFftRibbon.h"
#include "StFftDistort.h"
#include "StFftDistortStripe.h"
#include "StFftSphereStripe.h"
#include "StFftCubeStripe.h"
#include "StFftCubeStripeDuo.h"
#include "StFftCubeDistort.h"
#include "StFftSphereDistort.h"
#include "StFftSphereRibbon.h"
#include "StFftRibbonCool.h"
#include "StFftPixelate.h"
#include "StFftBlueBeam.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    // ofSetWindowPosition(1920, 0);
    
    // FFT
    bufferSize = 1024;
    fft = new FFTData(bufferSize);
    
    // Blackmagic
    blackmagic = new BlackmagicCapture(1920, 1080, 59.94);
    
    // FBO
    drawFbo = new DrawFbo();

    // StateMachine
    stateMachine.addState<StBlank>();
    stateMachine.addState<StSimplePixelate>();
    stateMachine.addState<StCvOpDraw>();
    stateMachine.addState<StCvOpParticle>();
    stateMachine.addState<StCvOpParticleGJ>();
    stateMachine.addState<StCvOpParticlePath>();
    stateMachine.addState<StCvOpMesh>();
    stateMachine.addState<StCvOpMeshColor>();
    stateMachine.addState<StCvOpNote>();
    stateMachine.addState<StCvOpSparkle>();
    stateMachine.addState<StCvOpDistort>();
    stateMachine.addState<StFftDistortNotex>();
    stateMachine.addState<StSakuraParticle>();
    stateMachine.addState<StRgbDelay>();
    stateMachine.addState<StTrianglePixelate>();
    
    stateMachine.addState<StSoundWave>();
    // stateMachine.addState<StSphereSoundWave>();
    stateMachine.addState<StFftDrawCircle>();
    stateMachine.addState<StFftDrawRect>();
    stateMachine.addState<StFftDrawCircleBlue>();
    stateMachine.addState<StFftDot>();
    stateMachine.addState<StFftDrawBar>();
    stateMachine.addState<StFftBox>();
    stateMachine.addState<StFftRibbon>();
    stateMachine.addState<StFftDistort>();
    stateMachine.addState<StFftDistortStripe>();
    stateMachine.addState<StFftSphereStripe>();
    stateMachine.addState<StFftCubeStripe>();
    stateMachine.addState<StFftCubeStripeDuo>();
    stateMachine.addState<StFftSphereDistort>();
    stateMachine.addState<StFftCubeDistort>();
    stateMachine.addState<StKaleidoscope>();
    stateMachine.addState<StKaleidoscopeGJ>();
    stateMachine.addState<StKaleidoscopeSimple>();
    stateMachine.addState<StCvOpNoiseWrap>();
    stateMachine.addState<StFftSphereRibbon>();
    stateMachine.addState<StFftRibbonCool>();
    stateMachine.addState<StFftPixelate>();
    stateMachine.addState<StFftBlueBeam>();
    
    stateMachine.changeState("StBlank");
    guiVisible = false;
    
    // OSC Control
    oscControl = new OscControl();
    oscControl->setup();
    srcMix = 255;
    fxMix = 255;
    
    // Alpha Mask
    alphaMask = new AlphaMask();
    
    // GUI
    gui = new ofxUICanvas();
    gui->init(10, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("MAIN");
    gui->addSpacer();
    gui->addFPS();
    gui->addSpacer();
    gui->addSlider("FFT SCALE", 0.0, 20.0, 5.0);
    gui->addSlider("FFT POW", 0.0, 1.0, 0.5);
    gui->addSpacer();
    gui->addSpacer();
    gui->addIntSlider("MIX", 0, 255, 255);
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->addSpacer();
    vector<string> names;
    names.push_back("BLACKMAGIC"); names.push_back("INTERNAL CAM"); names.push_back("MOVIE");
    ofxUIDropDownList *ddl;
    ddl = gui->addDropDownList("INPUT SOURCE", names);
    ddl->setAllowMultiple(false);
    ddl->setAutoClose(true);
    
    gui->loadSettings("main.xml");
    gui->autoSizeToFitWidgets();
    gui->setVisible(false);
    ofAddListener(gui->newGUIEvent,this,&ofApp::guiEvent);
    
    // 16:9 mode
    screenMode = 1;
}

//--------------------------------------------------------------
void ofApp::update(){
    blackmagic->update();
    oscControl->update();
    alphaMask->update();
    
    // FFT
    ofxUISlider *gfftscale = (ofxUISlider *)gui->getWidget("FFT SCALE"); float fftscale = gfftscale->getValue();
    ofxUISlider *gfftpow = (ofxUISlider *)gui->getWidget("FFT POW"); float fftpow = gfftpow->getValue();
    fft->scale = fftscale;
    fft->pow = fftpow;
    fft->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofxUIIntSlider *m = (ofxUIIntSlider *)gui->getWidget("MIX");

    srcMix = oscControl->controlVal[0] * 2;

    ofSetColor(srcMix);
    blackmagic->drawSub();
    fxMix = oscControl->controlVal[1] * 2;
    ofSetColor(255, fxMix);
    drawFbo->draw();
    
    // Alpha Mask
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    alphaMask->draw();
    
    ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void ofApp::exit(){
    blackmagic->exit();
}

void ofApp::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("main.xml");
    }
    if (name == "INPUT SOURCE") {
        ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
        vector<ofxUIWidget *> &selected = ddlist->getSelected();
        vector<int> &selectID = ddlist->getSelectedIndeces();
        for (int i = 0; i < selectID.size(); i++) {
            blackmagic->changeInput(selectID[i]);
            cout << "Selected Video ID : " << selectID[i] << endl;
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        /*
        case 'f':
            ofToggleFullscreen();
            break;
         */
        case 'g':
            gui->toggleVisible();
            stateMachine.getSharedData().guiVisible?
            ofHideCursor() : ofShowCursor();
            stateMachine.getSharedData().guiVisible?
            stateMachine.getSharedData().guiVisible = false :stateMachine.getSharedData().guiVisible = true;
            break;
        case 'z':
            if (screenMode == 0) {
                screenMode = 1;
            } else {
                screenMode = 0;
            }
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
