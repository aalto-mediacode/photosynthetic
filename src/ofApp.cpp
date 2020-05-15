

//  This project includes Wireframe extrucion from VideoGrabber, midi-controlling the mesh and screen capturing the images
//  Base of this code is based on Tim Gfrerer and James George OF cameraMesh -example

// I used for the final version OpenFrameworks and Touchdesigner.
//file for the Touchdesigner is in the bin folder: mlab_GMC_touch_points.32

// to find more of my work, visit: nikotiainen.com



#include "ofApp.h"

const int width = 800;
const int height = 600;  // these are for Syphon

//--------------------------------------------------------------
void ofApp::setup(){
    
    
    //________MIDI STUFF________
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    // print input ports to console
    midiIn.listInPorts();
    
    // open port by number (you may need to change this)
    midiIn.openPort(0);
    
    // add ofApp as a listener
    midiIn.addListener(this);
    
    // print received messages to the console
    midiIn.setVerbose(true);
    
    // _______Syphon stuff________
    mainOutputSyphonServer.setName("Screen Output");
    
    // ______vertex stuff__________
    
    
    mainMesh.setMode(OF_PRIMITIVE_LINE_STRIP_ADJACENCY);
    //for trying different versions
    //mainMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP_ADJACENCY);
    //mainMesh.setMode(OF_PRIMITIVE_POINTS);
    
    ofSetFrameRate(5);
    ofBackground(0);
    
    //initialize the video grabber
    vidGrabber.setVerbose(true);
    vidGrabber.setDeviceID(0);
    //vidGrabber.setDeviceID(1);  // this is for external webcam
    vidGrabber.setup(100, 100);
    
    //store the width and height for convenience
    int width = vidGrabber.getWidth();
    int height = vidGrabber.getHeight();
    
    //add one vertex to the mesh for each pixel
    for (int y = 0; y < height; y++){
        for (int x = 0; x<width; x++){
            mainMesh.addVertex(glm::vec3(x,y,0));	// mesh index = x + y*width
            // this replicates the pixel array within the camera bitmap...
            mainMesh.addColor(ofFloatColor(0,0,0));  // placeholder for colour data, we'll get this from the camera
        }
    }
    
    for (int y = 0; y<height-1; y++){
        for (int x=0; x<width-1; x++){
            mainMesh.addIndex(x+y*width);				// 0
        }
    }
    
    //this is an annoying thing that is used to flip the camera
    cam.setScale(1,-1,1);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    /* //_______MIDI STUFF_________
     
     // this is the midi-setup. You need a midi-controller for using these setups. Messages are mapped for Akai LPO8 -midi-controller
     
     
    for(unsigned int i = 0; i < midiMessages.size(); ++i) {
        
        ofxMidiMessage &message = midiMessages[i];
        int x = 10;
        int y = i*40 + 40;
        
        stringstream text;
        text << ofxMidiMessage::getStatusString(message.status);
        while(text.str().length() < 16) { // pad status width
            text << " ";
        }
        
     
        int knob1 = (message.control==1)? message.value : 0; // its 1
         int knob2 = (message.control==2)? message.value : 0;
         int knob3 = (message.control==3)? message.value : 0;
         int knob4 = (message.control==4)? message.value : 0;
         int knob5 = (message.control==5)? message.value : 0;
         int knob6 = (message.control==6)? message.value : 0;
         int knob7 = (message.control==7)? message.value : 0;
         int knob8 = (message.control==8)? message.value : 0;
         
         
         float pad1 = (message.pitch==40)? message.velocity : 0;
         float pad2 = (message.pitch==41)? message.velocity : 0;
         float pad3 = (message.pitch==42)? message.velocity : 0;
         float pad4 = (message.pitch==43)? message.velocity : 0;
         float pad5 = (message.pitch==44)? message.velocity : 0;
         float pad6 = (message.pitch==45)? message.velocity : 0;
         float pad7 = (message.pitch==46)? message.velocity : 0;
         float pad8 = (message.pitch==47)? message.velocity : 0;
        
        
        //extrusionAmount = ofMap(knob1, 0, 127, -300, -500);
     // this is for controlling the extrucion thru midi
        
        //_________ */
        
       
        extrusionAmount = -500; // this is for mesh extrucion
        
        vidGrabber.update();
        
        //update the mesh if we have a new frame
        if (vidGrabber.isFrameNew()){
            //this determines how far we extrude the mesh
            for (int i=0; i<vidGrabber.getWidth()*vidGrabber.getHeight(); i++){
                
                ofFloatColor sampleColor(vidGrabber.getPixels()[i*3]/255.f,				// r
                                         vidGrabber.getPixels()[i*3+1]/255.f,			// g
                                         vidGrabber.getPixels()[i*3+2]/255.f);			// b
                
                //now we get the vertex aat this position
                //we extrude the mesh based on it's brightness
                glm::vec3 tmpVec = mainMesh.getVertex(i);
                
                
                // these are for changing colors
                 tmpVec.z = sampleColor.getHue() * extrusionAmount;
                // You can try these also:
                //tmpVec.z = sampleColor.getLightness() * extrusionAmount;
               //tmpVec.z = sampleColor.getBrightnes() * extrusionAmount;
                
                mainMesh.setVertex(i, tmpVec);
                
                // this is for coloring the mesh
                mainMesh.setColor(i, sampleColor);
                //B&W -version below
                //mainMesh.setColor(i, 100);
                
            }
        }
        
        //let's move the camera when you move the mouse. Fix the last two values if you want this to work
        float rotateAmount = ofMap(ofGetMouseY(), 0, ofGetHeight(), 0, 0);
        
        
        //move the camera around the mesh
        glm::vec3 camDirection(0,0,1);
        glm::vec3 centre(vidGrabber.getWidth()/2.f,vidGrabber.getHeight()/2.f, 255/2.f);
        glm::vec3 camDirectionRotated = glm::rotate(camDirection, rotateAmount, glm::vec3(1,0,0));
        glm::vec3 camPosition = centre + camDirectionRotated * extrusionAmount;
        
        cam.setPosition(camPosition);
        cam.lookAt(centre);
    }


//--------------------------------------------------------------
void ofApp::draw(){
    
  /* this is for the gradient background
    ofColor colorOne(255, 0, 0);
    ofColor colorTwo(0, 0, 0);
    ofBackgroundGradient(colorOne, colorTwo, OF_GRADIENT_CIRCULAR);
   */
    
    cam.begin();
    mainMesh.draw(); // to draw the wireframe
    cam.end();
    
   /*   this is for the fading
        ofSetColor(0, fade+= 0.2);
     
        if(fade >270){
        fade=1;
        }
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight()) ;*/
    
    
    //____________SYPHON STUFF____________
    //ofSetColor(255, 255, 255);
    //ofEnableAlphaBlending();
    mainOutputSyphonServer.publishScreen();
}




//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    // This is for taking generative still pictures
    if(key == ' '){
        ofSaveFrame();
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}

void ofApp::exit() {
    
    // clean up
    midiIn.closePort();
    midiIn.removeListener(this);
}

void ofApp::newMidiMessage(ofxMidiMessage& msg) {
    
    // add the latest message to the message queue
    midiMessages.push_back(msg);
    
    // remove any old messages if we have too many
    while(midiMessages.size() > maxMessages) {
        midiMessages.erase(midiMessages.begin());
    }
}
