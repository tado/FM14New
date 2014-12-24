#pragma once
#include "ofMain.h"

class MultiOut {
public:
    void setup();
    void draw();
    
    ofFbo fbo;
    vector<ofRectangle> screens;
};