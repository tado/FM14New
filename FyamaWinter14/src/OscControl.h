#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

class OscControl {
public:
    void setup();
    void update();
    
    ofxOscReceiver receiver;
    int controlVal[8];
};