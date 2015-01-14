#include "OscControl.h"
#include "ofApp.h"


string stateMatrix[5][8] = {
    {"StSoundWave", "StFftDistortStripe", "StFftSphereStripe", "StFftSphereDistort", "StKaleidoscope", "StFftDistortNotex"},
    {"StFftDrawCircleBlue", "StFftCubeStripe", "StFftCubeStripeDuo", "StFftCubeDistort", "StFftBlueBeam", "StFftDrawBar", "StFftBox", "StFftDrawCircle"},
    {"StRgbDelay", "StKaleidoscopeSimple", "StCvOpNoiseWrap", "StCvOpParticlePath", "StCvOpSparkle", "StCvOpParticle", "StCvOpMesh", "StCvOpDistort"},
    {"StFftDrawRect","StCvOpParticlePath", "StFftPixelate", "StCvOpParticleGJ", "StFftBlueBeam", "StFftDrawBar", "StCvOpMeshColor"},
    {"StAnimRipple", "", "", "", "", "", "StBlank"}
};

void OscControl::setup(){
    receiver.setup(9000);
    for (int i = 0; i < 8; i++) {
        controlVal[i] = 127;
    }
}

void OscControl::update(){
    ofApp *app = (ofApp*)ofGetAppPtr();
    while(receiver.hasWaitingMessages()){
        
        ofxOscMessage m;
        receiver.getNextMessage(&m);
        
        if(m.getAddress() == "/state"){
            
            int row = m.getArgAsInt32(0);
            int col = m.getArgAsInt32(1);

            for (int j = 0; j < 8; j++) {
                for (int i = 0; i < 5; i++) {
                    if ((i == row - 53) && (j == col - 1) && stateMatrix[i][j].length() > 0) {
                        //cout << "Select State : " << stateMatrix[i][j] << endl;
                        app->stateMachine.changeState(stateMatrix[i][j]);
                    }
                }
            }
        }
        
        if(m.getAddress() == "/control"){
            
            int ch = m.getArgAsInt32(0);
            int value = m.getArgAsInt32(1);
            
            for (int i = 0; i < 8; i++) {
                if (ch == i + 1) {
                    controlVal[i] = value;
                }
            }
        }
    }
}