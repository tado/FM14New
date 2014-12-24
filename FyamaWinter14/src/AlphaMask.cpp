#include "AlphaMask.h"
#include "ofApp.h"

AlphaMask::AlphaMask(){
    ofEnableAlphaBlending();
    
    fbo.allocate(1920, 1080);
    fbo.begin();
    ofClear(0, 0, 0);
    fbo.end();
    
    mask.loadImage("mask.png");
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
    srcTex = app->blackmagic->colorTexture;
    
    fbo.begin();
    ofClear(0, 0, 0);
    maskShader.begin();
    
    glActiveTexture(GL_TEXTURE0_ARB);
    srcTex.bind();
    
    glActiveTexture(GL_TEXTURE1_ARB);
    mask.getTextureReference().bind();
    
    glBegin(GL_QUADS);
    
    float maskOffsetX =  ofMap(app->oscControl->controlVal[3], 0, 127, -mask.getWidth(), mask.getWidth() / 3.0);
    float maskOffsetY =  ofMap(app->oscControl->controlVal[3], 0, 127, -mask.getHeight(), mask.getHeight() / 3.0);
    
    glMultiTexCoord2d(GL_TEXTURE0_ARB, 0, 0);
    glMultiTexCoord2d(GL_TEXTURE1_ARB, maskOffsetX, maskOffsetY);
    glVertex2f(0, 0);
    
    glMultiTexCoord2d(GL_TEXTURE0_ARB, srcTex.getWidth(), 0);
    glMultiTexCoord2d(GL_TEXTURE1_ARB, mask.getWidth() - maskOffsetX, maskOffsetY);
    glVertex2f(1920, 0);
    
    glMultiTexCoord2d(GL_TEXTURE0_ARB, srcTex.getWidth(), srcTex.getHeight());
    glMultiTexCoord2d(GL_TEXTURE1_ARB, mask.getWidth() - maskOffsetX, mask.getHeight() - maskOffsetY);
    glVertex2f( 1920, 1080);
    
    glMultiTexCoord2d(GL_TEXTURE0_ARB, 0, srcTex.getHeight());
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
    
    if (app->screenMode == 0) {
        ofSetColor(0);
        ofSetRectMode(OF_RECTMODE_CORNER);
        ofPushMatrix();
        ofRect(0, 0, 1920, 224);
        ofTranslate(0, 630 + 224);
        ofRect(0, 0, 1920, 300);
        ofPopMatrix();
    }
}