#pragma once

#include "ofMain.h"
#include "ofxVideoRecorder.h"

class ofApp : public ofBaseApp{
  
public:
  void setup();
  void update();
  void draw();
  void exit();
  void keyReleased(int key);
  void audioIn(float * input, int bufferSize, int nChannels);
  
  ofVideoGrabber      vidGrabber;
  ofxVideoRecorder    vidRecorder;
  ofSoundStream       soundStream;

  ofFbo recordFbo;
  ofPixels recordPixels;

  bool bRecording;
  int sampleRate;
  int channels;
  string fileName;
  string fileExt;
};
