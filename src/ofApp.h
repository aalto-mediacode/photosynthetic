#pragma once

#include "ofMain.h"
#include "ofxSyphon.h"  // u need to include this addon
#include "ofxMidi.h"    //u need to include this addon


class ofApp : public ofBaseApp,
public ofxMidiListener { // for the midi
    
public:
    void setup();
    void update();
    void draw();
    
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    
    void exit(); // for midi

    
    void gotMessage(ofMessage msg);  // ?
    
    //_______MIDI STUFF_________
    void newMidiMessage(ofxMidiMessage& eventArgs);
    
    ofxMidiIn midiIn;
    std::vector<ofxMidiMessage> midiMessages;
    std::size_t maxMessages = 10; //< max number of messages to keep track of
    
    // _______webCamStuff_______
    
    ofCamera cam; // add mouse controls for camera movement
    float extrusionAmount;
    ofVboMesh mainMesh;
    
    ofVideoGrabber vidGrabber;
    float time = ofGetElapsedTimef()/2;
    
    
    //______syphon_stuff_________
    
    float     counter;
    bool    bSmooth;
    
    ofTexture tex;
    ofxSyphonServer mainOutputSyphonServer;
    ofxSyphonServer individualTextureSyphonServer;
    ofxSyphonClient mClient;
    
    float fade = 1.0;
    
};
