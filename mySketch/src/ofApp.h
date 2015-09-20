#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

public:

	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	void audioIn(float * output, int bufferSize, int nChannels);
	void speedUp(float radius);
	void speedDown(float radius);
	void tapBeat(int index);
	void changeSong1();
	void changeSong2();
	void resetSpeed();
	void xFade1();
	void xFade2();

	vector<ofSoundPlayer> roySux;
	vector<ofSoundPlayer> beats;
	ofSoundStream soundStream;

	//float 	pan;
	//int		sampleRate;
	//bool 	bNoise;
	//float 	volume;
	bool paused;
	vector <float> lAudio;
	vector <float> rAudio;

	float timer;
	int beatNum;
	int songNum;
	//vector <vector<float>>beatTaps;
	vector<float> beatTimings;
	vector<float> lastTapped;
	vector<string>	songName;
	ofTrueTypeFont songFont;
	float song1x;
	float song2x;

	int song1;
	int song2;
	float vol1;
	float vol2;
	//float * vals;
};
