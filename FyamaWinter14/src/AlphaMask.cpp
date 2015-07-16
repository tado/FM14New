#include "AlphaMask.h"
#include "ofApp.h"

AlphaMask::AlphaMask(){
    ofEnableAlphaBlending();
    
    fbo.allocate(1920, 1080);
    fbo.begin();
    ofClear(0, 0, 0);
    fbo.end();
    
    mask.loadImage("mask.png");
    rectMask[0].loadImage("rectMask0.png");
    rectMask[1].loadImage("rectMask1.png");
    rectMask[2].loadImage("rectMask2.png");
    rectMaskNum = 0;

    maskShader.load("composite");
    maskShader.begin();
    
    ofApp *app = ((ofApp*)ofGetAppPtr());
    srcTex = app->blackmagic->colorTexture;
    
    maskShader.setUniformTexture("Tex1", mask.getTextureReference(), 1);
    maskShader.end();
}

void AlphaMask::update(){
    ofApp *app = ((ofApp*)ofGetAppPtr());
    
    srcTex = app->blackmagic->colorTexture;
    maskShader.setUniformTexture("Tex0", srcTex, 0);
}

void AlphaMask::draw(){
    ofApp *app = ((ofApp*)ofGetAppPtr());
    
    ofPushMatrix();
    ofTranslate(960, 0);
    // Circle Mask (Full HD or 1980 x 630)
    
    if (app->screenMode == 0 || app->screenMode == 1) {
        maskCircle();
    }
    if (app->screenMode == 2) {
        maskRect();
    }
    
    // Rect mask in 1980 x 630
    // Top & Bottom mask
    if (app->screenMode == 1 || app->screenMode == 2) {
        ofSetColor(0);
        ofSetRectMode(OF_RECTMODE_CORNER);
        ofPushMatrix();
        ofRect(0, 0, 1920, 224);
        ofTranslate(0, 630 + 224);
        ofRect(0, 0, 1920, 300);
        ofPopMatrix();
    }
    ofPopMatrix();
}

void AlphaMask::maskRect(){
    int srcMargin = 100;
    ofApp *app = ((ofApp*)ofGetAppPtr());
    srcTex = app->blackmagic->colorTexture;
    
    ofFbo srcFbo;
    srcFbo.allocate(1920, 1080 / 2.0);
    srcFbo.begin();
    ofClear(0, 0, 0);
    ofPushMatrix();
    //ofScale(1.0, 0.5);
    srcTex.draw(0, 0, 1920, 1080 / 2.0);
    ofPopMatrix();
    srcFbo.end();
    
    fbo.begin();
    ofClear(0, 0, 0);
    maskShader.begin();
    
    glActiveTexture(GL_TEXTURE0_ARB);
    srcFbo.getTextureReference().bind();
    
    glActiveTexture(GL_TEXTURE1_ARB);
    rectMask[rectMaskNum].getTextureReference().bind();
    
    glBegin(GL_QUADS);
    
    float maskOffsetX =  ofMap(app->oscControl->controlVal[3], 0, 127, -mask.getWidth(), mask.getWidth() / 4.5);
    //float maskOffsetY =  ofMap(app->oscControl->controlVal[3], 0, 127, -mask.getHeight(), mask.getHeight() / 4.5);
    float maskOffsetY =  ofMap(app->oscControl->controlVal[3], 0, 127, -mask.getWidth(), mask.getWidth() / 4.5);
    
    glMultiTexCoord2d(GL_TEXTURE0_ARB, 0, 0);
    glMultiTexCoord2d(GL_TEXTURE1_ARB, maskOffsetX, maskOffsetY);
    glVertex2f(400, 223);
    
    glMultiTexCoord2d(GL_TEXTURE0_ARB, srcTex.getWidth(), 0);
    glMultiTexCoord2d(GL_TEXTURE1_ARB, mask.getWidth() - maskOffsetX, maskOffsetY);
    glVertex2f(1520, 223);
    
    glMultiTexCoord2d(GL_TEXTURE0_ARB, srcTex.getWidth(), srcTex.getHeight() / 2.0);
    glMultiTexCoord2d(GL_TEXTURE1_ARB, mask.getWidth() - maskOffsetX, mask.getHeight() - maskOffsetY);
    glVertex2f(1520, 856);
    
    glMultiTexCoord2d(GL_TEXTURE0_ARB, 0, srcTex.getHeight() / 2.0);
    glMultiTexCoord2d(GL_TEXTURE1_ARB, maskOffsetX, mask.getHeight() - maskOffsetY);
    glVertex2f(400, 856);
    
    glEnd();
    
    glActiveTexture(GL_TEXTURE1_ARB);
    rectMask[rectMaskNum].getTextureReference().unbind();
    
    glActiveTexture(GL_TEXTURE0_ARB);
    srcFbo.getTextureReference().unbind();
    
    maskShader.end();
    fbo.end();
    
    ofSetColor(255, app->oscControl->controlVal[2] * 2);
    fbo.draw(0, 0);
}

void AlphaMask::maskCircle(){
    ofApp *app = ((ofApp*)ofGetAppPtr());
    srcTex = app->blackmagic->colorTexture;
    
    ofFbo srcFbo;
    srcFbo.allocate(1920, 1080 / 2.0);
    srcFbo.begin();
    ofClear(0, 0, 0);
    ofPushMatrix();
    //ofScale(1.0, 0.5);
    srcTex.draw(0, 0, 1920, 1080 / 2.0);
    ofPopMatrix();
    srcFbo.end();
    
    fbo.begin();
    ofClear(0, 0, 0);
    maskShader.begin();
    
    glActiveTexture(GL_TEXTURE0_ARB);
    //srcTex.bind();
    srcFbo.getTextureReference().bind();
    
    glActiveTexture(GL_TEXTURE1_ARB);
    //mask.getTextureReference().bind();
    rectMask[rectMaskNum].getTextureReference().bind();
    
    glBegin(GL_QUADS);
    
    float maskOffsetX =  ofMap(app->oscControl->controlVal[3], 0, 127, -mask.getWidth(), mask.getWidth() / 2.0);
    float maskOffsetY =  ofMap(app->oscControl->controlVal[3], 0, 127, -mask.getHeight(), mask.getHeight() / 2.0);
    
    glMultiTexCoord2d(GL_TEXTURE0_ARB, 0, 0);
    glMultiTexCoord2d(GL_TEXTURE1_ARB, maskOffsetX, maskOffsetY);
    glVertex2f(0, 0);
    
    glMultiTexCoord2d(GL_TEXTURE0_ARB, srcTex.getWidth(), 0);
    glMultiTexCoord2d(GL_TEXTURE1_ARB, mask.getWidth() - maskOffsetX, maskOffsetY);
    glVertex2f(1920, 0);
    
    glMultiTexCoord2d(GL_TEXTURE0_ARB, srcTex.getWidth(), srcTex.getHeight() / 2.0);
    glMultiTexCoord2d(GL_TEXTURE1_ARB, mask.getWidth() - maskOffsetX, mask.getHeight() - maskOffsetY);
    glVertex2f( 1920, 1080);
    
    glMultiTexCoord2d(GL_TEXTURE0_ARB, 0, srcTex.getHeight() / 2.0);
    glMultiTexCoord2d(GL_TEXTURE1_ARB, maskOffsetX, mask.getHeight() - maskOffsetY);
    glVertex2f(0, 1080);
    
    glEnd();
    
    glActiveTexture(GL_TEXTURE1_ARB);
    mask.getTextureReference().unbind();
    
    glActiveTexture(GL_TEXTURE0_ARB);
    srcTex.unbind();
    
    maskShader.end();
    fbo.end();
    
    ofSetColor(255, app->oscControl->controlVal[2] * 2);
    fbo.draw(0, 0);
}