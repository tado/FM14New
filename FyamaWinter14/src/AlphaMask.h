#pragma once
#include "ofMain.h"

class AlphaMask {
public:
    AlphaMask();
    void update();
    void draw();
    
    ofImage mask;
    ofShader maskShader;
    ofTexture srcTex;
    ofFbo fbo;
};