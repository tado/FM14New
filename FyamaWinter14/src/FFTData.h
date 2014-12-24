#pragma once
#include "ofMain.h"
#include "ofxFft.h"
#include "ofxEasyFft.h"

class FFTData {
public:
    FFTData(int bufferSize);
    void update();
    
    ofxEasyFft fft;
    vector<float> drawBins;
    vector<float> audioBins;
    float scale;
    float pow;
};