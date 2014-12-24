#include "SakuraParticle.h"

void SakuraParticle::setup(ofVec3f _position, ofVec3f _velocity, ofColor _color){
    position = _position;
    velocity = _velocity;
    color = _color;
    rot = ofVec3f(ofRandom(360), ofRandom(360), ofRandom(360));
    rotVel = ofVec3f(ofRandom(2.0), ofRandom(2.0), ofRandom(2.0));
    count = 0;
}

void SakuraParticle::resetForce(){
    force.set(0, 0, 0);
}

void SakuraParticle::addForce(ofVec3f _force){
    force = _force;
}

void SakuraParticle::updateForce(){
    force -= velocity * friction;
}

void SakuraParticle::updatePos(){
    velocity += force;
    position += velocity;
}

void SakuraParticle::update(){
    resetForce();
    updateForce();
    updatePos();
    rot += rotVel;
}

void SakuraParticle::draw(){
    ofSetColor(color);
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofPushMatrix();
    ofTranslate(position);
    ofRotateX(rot.x);
    ofRotateY(rot.y);
    ofRotateZ(rot.z);
    ofEllipse(0, 0, radius, radius * 1.4);
    ofPopMatrix();
    ofSetRectMode(OF_RECTMODE_CORNER);
}