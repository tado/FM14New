#include "FFTData.h"

FFTData::FFTData(int _bufferSize){
    fft.setup(_bufferSize);
    fft.setUseNormalization(false);
    scale = 5.0;
    pow = 0.8;
}

void FFTData::update(){
    fft.update();
    audioBins = fft.getAudio();
    drawBins = fft.getBins();
    for (int i = 0; i < drawBins.size(); i++) {
        drawBins[i] = powf(drawBins[i], pow);
        drawBins[i] *= scale;
    }
}