#include "BlackmagicCapture.h"
#include "ofApp.h"

BlackmagicCapture::BlackmagicCapture(int _width, int _height, float _framerate){
    inputMode = 0;
    
    width = _width;
    height = _height;
    framerate = _framerate;

    switch (inputMode) {
        case 0:
            blackmagic.setup(width, height, framerate);
            break;
        case 1:
            grabber.initGrabber(width, height);
            break;
        case 2:
            video.loadMovie("source.mp4");
            video.play();
            break;
    }
    
    colorTexture.allocate(width, height, GL_RGB);
    fbo.allocate(ofGetWidth(), ofGetHeight());
    fbo.begin();
    ofClear(0, 0, 0, 0);
    fbo.end();
}

void BlackmagicCapture::exit() {
    switch (inputMode) {
        case 0:
            blackmagic.close();
            break;
        case 1:
            grabber.close();
            break;
        case 2:
            video.close();
            break;
    }
}

void BlackmagicCapture::update() {
    switch (inputMode) {
        case 0:
            if(blackmagic.update()) {
                colorTexture = blackmagic.getColorTexture();
                colorPixels = blackmagic.getColorPixels();
            }
            break;
            
        case 1:
            grabber.update();
            if (grabber.isFrameNew()) {
                colorTexture = grabber.getTextureReference();
                colorPixels = grabber.getPixelsRef();
            }
            break;
            
        case 2:
            video.update();
            if (video.isFrameNew()) {
                colorTexture = video.getTextureReference();
                colorPixels = video.getPixelsRef();
            }
            break;
    }
    
    fbo.begin();
    ofSetColor(255);
    colorTexture.draw(0, 0, width, height/2.0);
    fbo.end();
}

void BlackmagicCapture::draw() {
    ofPushMatrix();
    ofScale(1.0, 2.0);
    fbo.draw(0, 0);
    ofPopMatrix();
}

void BlackmagicCapture::drawSub(){
    ofApp *app = ((ofApp*)ofGetAppPtr());
    if (app->screenMode == 0) {
        drawTop = sourceTop = 223 / 2.0;
        drawHeight = sourceHeight = 630 / 2.0;
    } else {
        drawTop = sourceTop = 0;
        drawHeight = sourceHeight = 1080 / 2.0;
    }
    
    drawWidth = sourceWidth = ofGetWidth();
    
    ofPushMatrix();
    ofScale(1.0, 2.0);
    fbo.getTextureReference().drawSubsection(0, drawTop, drawWidth, drawHeight, 0, sourceTop, sourceWidth, sourceHeight);
    ofPopMatrix();
}

void BlackmagicCapture::changeInput(int _inputMode){
    switch (_inputMode) {
        case 0:
            if(inputMode == 1) grabber.close();
            if(inputMode == 2) video.close();
            blackmagic.setup(width, height, framerate);
            inputMode = 0;
            break;
            
        case 1:
            if(inputMode == 0) blackmagic.close();
            if(inputMode == 2) video.close();
            grabber.initGrabber(width, height);
            inputMode = 1;
            break;
            
        case 2:
            if(inputMode == 0) blackmagic.close();
            if(inputMode == 1) grabber.close();
            video.loadMovie("source.mp4");
            video.play();
            inputMode = 2;
            break;
    }
}