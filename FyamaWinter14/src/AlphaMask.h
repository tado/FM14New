#pragma once
#include "ofMain.h"

class AlphaMask {
public:
    AlphaMask();
    void update();
    void draw();
    void maskCircle();
    void maskRect();
    
    ofImage mask;
    ofImage mask2;
    ofShader maskShader;
    ofTexture srcTex;
    ofFbo fbo;
};