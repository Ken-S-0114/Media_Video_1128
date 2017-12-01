#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  sampleRate = 44100;
  channels = 2;
  
  ofSetFrameRate(60);
  ofSetLogLevel(OF_LOG_VERBOSE);
  vidGrabber.setDesiredFrameRate(30);
  vidGrabber.initGrabber(640, 480);
  fileName = "MyMovie";
  fileExt = ".mp4";
  vidRecorder.setVideoCodec("mpeg4");
  vidRecorder.setVideoBitrate("800k");
  soundStream.setup(this, 0, channels, sampleRate, 256, 4);
  ofSetWindowShape(vidGrabber.getWidth(), vidGrabber.getHeight());
  bRecording = false;
  ofEnableAlphaBlending();
  
  recordFbo.allocate(640, 480, GL_RGB);
  recordFbo.begin();
  ofClear(0, 0, 0, 255);
  recordFbo.end();
}

void ofApp::exit() {
  vidRecorder.close();
}

//--------------------------------------------------------------
void ofApp::update(){
  vidGrabber.update();
//  if(vidGrabber.isFrameNew() && bRecording){
//    bool success = vidRecorder.addFrame(vidGrabber.getPixelsRef());
//    if (!success) {
//      ofLogWarning("This frame was not added!");
//    }
//  }
  
  if(vidGrabber.isFrameNew() && bRecording){
    recordFbo.readToPixels(recordPixels);
    bool success = vidRecorder.addFrame(recordPixels);
  }
  
  
}

//--------------------------------------------------------------
void ofApp::draw(){
  ofSetColor(255, 255, 255);
  
  recordFbo.begin();
  vidGrabber.draw(0,0);

  ofSetColor(255,0,0);
  ofDrawRectangle(0, 0, 100, 100);
  
  recordFbo.end();
  recordFbo.draw(0,0); // Fboに記録したものを描画
  
  
  stringstream ss;
  ss << "video queue size: " << vidRecorder.getVideoQueueSize() << endl
  << "audio queue size: " << vidRecorder.getAudioQueueSize() << endl
  << "FPS: " << ofGetFrameRate() << endl
  << (bRecording?"pause":"start") << " recording: r" << endl
  << (bRecording?"close current video file: c":"") << endl;
  
  ofSetColor(0,0,0,100);
  ofRect(0, 0, 260, 75);
  ofSetColor(255, 255, 255);
  ofDrawBitmapString(ss.str(),15,15);
  
  if(bRecording){
    ofSetColor(255, 0, 0);
    ofCircle(ofGetWidth() - 20, 20, 5);
  }
  
}

void ofApp::audioIn(float *input, int bufferSize, int nChannels){
  if(bRecording)
    vidRecorder.addAudioSamples(input, bufferSize, nChannels);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
  
  if(key=='r'){
    bRecording = !bRecording;
    if(bRecording && !vidRecorder.isInitialized()) {
      //      vidRecorder.setup(fileName+ofGetTimestampString()+fileExt, vidGrabber.getWidth(), vidGrabber.getHeight(), 30, sampleRate, channels);
      vidRecorder.setup(fileName+ofGetTimestampString()+fileExt, vidGrabber.getWidth(), vidGrabber.getHeight(), 30); // no audio
      //            vidRecorder.setup(fileName+ofGetTimestampString()+fileExt, 0,0,0, sampleRate, channels); // no video
      //          vidRecorder.setupCustomOutput(vidGrabber.getWidth(), vidGrabber.getHeight(), 30, sampleRate, channels, "-vcodec mpeg4 -b 1600k -acodec mp2 -ab 128k -f mpegts udp://localhost:1234"); // for custom ffmpeg output string (streaming, etc)
      
      // Start recording
      vidRecorder.start();
    }
    else if(!bRecording && vidRecorder.isInitialized()) {
      vidRecorder.setPaused(true);
    }
    else if(bRecording && vidRecorder.isInitialized()) {
      vidRecorder.setPaused(false);
    }
  }
  if(key=='c'){
    bRecording = false;
    vidRecorder.close();
  }
}
