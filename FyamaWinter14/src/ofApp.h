#pragma once

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#include "ofMain.h"
#include "ofxStateMachine.h"
#include "SharedData.h"
#include "ofxUI.h"
#include "OscControl.h"
#include "BlackmagicCapture.h"
#include "DrawFbo.h"
#include "FFTData.h"
#include "AlphaMask.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void guiEvent(ofxUIEventArgs &e);
    
    itg::ofxStateMachine<SharedData> stateMachine;
    BlackmagicCapture * blackmagic;
    ofxUICanvas *gui;
    bool guiVisible;
    DrawFbo *drawFbo;
    OscControl *oscControl;
    int srcMix, fxMix;

    int bufferSize;
    FFTData *fft;
    
    AlphaMask *alphaMask;
    
    int screenMode; // 0: 1920 x 630, 1: FullHD
};
