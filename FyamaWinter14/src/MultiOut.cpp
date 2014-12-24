//
//  MultiOut.cpp
//  FukuyamaWinter14
//
//  Created by Atsushi Tadokoro on 11/2/14.
//
//

#include "MultiOut.h"

void MultiOut::setup(){
    fbo.allocate(ofGetWidth(), ofGetHeight());
    fbo.begin();
    ofClear(0, 0, 0, 0);
    fbo.end();
    
    ofRectangle s1; s1.set(0, 0, 1920, 1080); screens.push_back(s1);
}

void MultiOut::draw(){
    for (int i = 0; i < screens.size(); i++) {
        ofSetColor(255, 255, 255);
        fbo.draw(screens[i]);
    }
}