#include "ofApp.h"
#include <fstream>
//--------------------------------------------------------------
void ofApp::setup(){
	songNum = 6;
	roySux = vector<ofSoundPlayer>(songNum);
	string name;
	ifstream file("songName.txt");
	songName = vector<string>();
	for (int x = 0; x < songNum; x++){
		getline(file, name);
		songName.push_back(name);
		roySux[x].loadSound("song" + to_string(x + 1) + ".mp3");
		roySux[x].setVolume(0.5f);
		roySux[x].setPan(x % 2 == 0 ? -1.0f : 1.0f);
	}
	file.close();
	/*0	Pitbull - Timber ft.Ke$ha
	  1	The Black Eyed Peas - Boom Boom Pow
	  2	Taio Cruz - Dynamite
	  3	Lady Gaga - Poker Face
	  4	Maroon 5 - Animals
	  5	Taylor Swift - Style*/
	song1 = 4;
	song2 = 5;
	roySux[song1].play();
	roySux[song2].play();
	beatNum = 5;
	beats = vector<ofSoundPlayer>(beatNum);
	for (int x = 0; x < beatNum; x++){
		beats[x].loadSound("beat"+to_string(x)+".wav");
		beats[x].setVolume(0.5f);
		beats[x].setMultiPlay(false);
	}

	ofSetCircleResolution(50);
	ofBackground(0, 0, 0);
	ofSetWindowTitle("DJ Leap");
	ofSetVerticalSync(false);
	ofSetWindowShape(1920, 1080);
	ofSetWindowPosition(0, 0);

	

	//ofSetFullscreen(true);
	//ofSetFrameRate(120); // if vertical sync is off, we can go a bit fast... this caps the framerate at 60fps.
	//ofSoundStreamListDevices();


	int bufferSize = 1024;
	soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
	lAudio.assign(bufferSize, 0.0);
	rAudio.assign(bufferSize, 0.0);

	paused = true;
	if (paused){
		roySux[song1].setPaused(true);
		roySux[song2].setPaused(true);
	}


	timer = 0;
	//beatTaps = vector<vector<float>>(beatNum);
	beatTimings = vector<float>(beatNum);
	beatTimings[2] = 2.526315789473684;
	lastTapped = vector<float>(beatNum);
	//for (int x = 0; x < beatNum; x++){
	//	beatTaps[x] = vector<float>();
	//}
	songFont.loadFont("arial.ttf", 15);
	songFont.setSpaceSize(0.5);
	song1x = 0;
	song2x = 0;
	vol1 = 0.5f;
	vol2 = 0.5f;
}

//--------------------------------------------------------------
void ofApp::update(){
	ofSoundUpdate();
	//timer=ofGetElapsedTimef();
	//cout << timer << endl;
	/*for (int x = 0; x < beatNum; x++){
		if (beatTimings[x] == 0.0)
			continue;
		if (timer >= lastTapped[x] + beatTimings[x]){
			lastTapped[x] += beatTimings[x];
			beats[x].play();
		}
	}*/

}
void ofApp::changeSong1(){
	roySux[song1].setPaused(false);
	roySux[song1].stop();
	while (true){
		song1++;
		song1 = song1%songNum;
		if (song1 != song2)
			break;
	}
	song1x = 0;
	roySux[song1].play();
	roySux[song1].setVolume(vol1);
	roySux[song1].setPan(-1.0f);
	if (paused)
		roySux[song1].setPaused(true);
}
void ofApp::changeSong2(){
	roySux[song2].setPaused(false);
	roySux[song2].stop();
	while (true){
		song2++;
		song2 = song2%songNum;
		if (song2 != song1)
			break;
	}
	song2x = 0;
	roySux[song2].play();
	roySux[song2].setVolume(vol2);
	roySux[song2].setPan(1.0f);
	if (paused)
		roySux[song2].setPaused(true);
}
void ofApp::tapBeat(int index){
	beats[index].play();
	//beatTaps[index].push_back(ofGetElapsedTimef());
	////cout << beatTaps[index].back() << endl;
	//if (beatTaps[index].size() >= 4){
	//	float difference = beatTaps[index][beatTaps[index].size() - 1] - beatTaps[index][beatTaps[index].size() - 2];
	//	float total = difference;
	//	if (difference > 1.0)
	//		return;
	//	for (int x = beatTaps[index].size() - 2; x >= 1; x--){
	//		float newDiff = beatTaps[index][x] - beatTaps[index][x - 1];
	//		if (abs(newDiff - difference) > 1.0)
	//			return;
	//		total += newDiff;
	//		difference = total / (beatTaps[index].size() - x);
	//		//cout << total << " " << (beatTaps[index].size() - x)<<endl;
	//	}
	//	beatTimings[index] = difference;
	//	lastTapped[index] = ofGetElapsedTimef();
	//}
}
void ofApp::speedUp(float radius){
	
	for (int x = 0; x < roySux.size(); x++){
		float speed = roySux[x].getSpeed() + radius / 5000.000f;
		speed = speed>3.0 ? 3.0 : speed;
		roySux[x].setSpeed(speed);
	}
	std::cout << "speedUp " << "Speed: " << roySux[song1].getSpeed() << " Radius: " << radius << std::endl;
}
void ofApp::speedDown(float radius){
	//std::cout << "speedDown" << std::endl;
	for (int x = 0; x < roySux.size(); x++){
		float speed = roySux[x].getSpeed() - radius / 5000.000f;
		speed = speed < 0 ? 0.0 : speed;
		roySux[x].setSpeed(speed);
	}
	std::cout << "speedDown " << "Speed: " << roySux[song1].getSpeed() << " Radius: " << radius << std::endl;
}
void ofApp::resetSpeed(){
	for (int x = 0; x < roySux.size(); x++){
		roySux[x].setSpeed(1.0f);
	}
	std::cout << "Reset speed to 1.0f" << std::endl;
}
void ofApp::xFade1(){
	vol1 += 0.01;
	if (vol1 > 1.0)
		vol1 = 1.0f;
	roySux[song1].setVolume(vol1);
	vol2 -= 0.01;
	if (vol2 < 0.0)
		vol2 = 0.0f;
	roySux[song2].setVolume(vol2);
}
void ofApp::xFade2(){
	vol1 -= 0.01;
	if (vol1 < 0.0)
		vol1 = 0.0f;
	roySux[song1].setVolume(vol1);
	vol2 += 0.01;
	if (vol2 > 1.0)
		vol2 = 1.0f;
	roySux[song2].setVolume(vol2);
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofFill();
	
	//ofRect(0, 900, 50, -100);
	//ofSetHexColor(0x000000);
	

	ofMesh volume1;
	int volume1Value = roySux[song1].getVolume() * 900;
	volume1.addVertex(ofVec3f(10, 1000, 0));
	volume1.addColor(ofColor(0, 255, 0));
	volume1.addVertex(ofVec3f(25, 1000, 0));
	volume1.addColor(ofColor(0, 255, 0));
	volume1.addVertex(ofVec3f(25, 1000 - volume1Value, 0));
	volume1.addColor(ofColor(0, 255, 0));

	volume1.addVertex(ofVec3f(25, 1000 - volume1Value, 0));
	volume1.addColor(ofColor(0, 255, 0));
	volume1.addVertex(ofVec3f(10, 1000 - volume1Value, 0));
	volume1.addColor(ofColor(0, 255, 0));
	volume1.addVertex(ofVec3f(10, 1000, 0));
	volume1.addColor(ofColor(0, 255, 0));

	volume1.draw();

	ofSetColor(0, 255, 0);
	ofDrawBitmapString("1", 15, 1012);

	ofMesh volume2;
	int volume2Value = roySux[song2].getVolume() * 900;
	volume2.addVertex(ofVec3f(30, 1000, 0));
	volume2.addColor(ofColor(0, 255, 0));
	volume2.addVertex(ofVec3f(45, 1000, 0));
	volume2.addColor(ofColor(0, 255, 0));
	volume2.addVertex(ofVec3f(45, 1000 - volume2Value, 0));
	volume2.addColor(ofColor(0, 255, 0));

	volume2.addVertex(ofVec3f(45, 1000 - volume2Value, 0));
	volume2.addColor(ofColor(0, 255, 0));
	volume2.addVertex(ofVec3f(30, 1000 - volume2Value, 0));
	volume2.addColor(ofColor(0, 255, 0));
	volume2.addVertex(ofVec3f(30, 1000, 0));
	volume2.addColor(ofColor(0, 255, 0));

	volume2.draw();

	ofSetColor(0, 255, 0);
	ofDrawBitmapString("2", 33, 1012);

	ofMesh speed;
	
	int speedValue = roySux[song1].getSpeed() * 300;
	speed.addVertex(ofVec3f(50, 1000, 0));
	speed.addColor(ofColor(255, 180, 0));
	speed.addVertex(ofVec3f(65, 1000, 0));
	speed.addColor(ofColor(255, 180, 0));
	speed.addVertex(ofVec3f(65, 1000 - speedValue, 0));
	speed.addColor(ofColor(255, 180, 0));

	speed.addVertex(ofVec3f(65, 1000 - speedValue, 0));
	speed.addColor(ofColor(255, 180, 0));
	speed.addVertex(ofVec3f(50, 1000 - speedValue, 0));
	speed.addColor(ofColor(255, 180, 0));
	speed.addVertex(ofVec3f(50, 1000, 0));
	speed.addColor(ofColor(255, 180, 0));
	speed.draw();
	ofSetColor(255, 180, 0);
	ofDrawBitmapString("S", 53, 1012);

	if (beats[0].getIsPlaying()){
		ofFill();
	}else
		ofNoFill();
	if (beats[1].getIsPlaying()){
		//ofViewport(50.0f, 50.0f, 500.0f, 500.0f, false);
	}
	// draw the left channel:
	ofPushStyle();
		ofPushMatrix();
		ofTranslate(0, 150, 0);

		ofSetColor(245, 58, 135);
		ofSetLineWidth(3);

			ofBeginShape();
			for (unsigned int i = 0; i < lAudio.size(); i++){
				float x = ofMap(i, 0, lAudio.size(), 0, 1920, true);
				ofVertex(x, 250 - lAudio[i] * 300.0f);
			}
			ofEndShape(false);

		ofPopMatrix();
	ofPopStyle();

	// draw the right channel:
	ofPushStyle();
		ofPushMatrix();
		ofTranslate(0, 350, 0);

		ofSetColor(245, 58, 135);
		ofSetLineWidth(3);

			ofBeginShape();
			for (unsigned int i = 0; i < rAudio.size(); i++){
				float x = ofMap(i, 0, rAudio.size(), 0, 1920, true);
				ofVertex(x, 350 - rAudio[i] * 300.0f);
			}
			ofEndShape(false);

		ofPopMatrix();
	ofPopStyle();

	//SongName
	ofRectangle rect = songFont.getStringBoundingBox(songName[song1], 0, 0);
	if (song1x < rect.getWidth()){
		song1x++;
	}
	songFont.drawString(songName[song1], 1920 - song1x - 10, 30);
	rect = songFont.getStringBoundingBox(songName[song2], 0, 0);
	if (song2x < rect.getWidth()){
		song2x++;
	}
	songFont.drawString(songName[song2], 1920 - song2x - 10, 60);



}
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
	for (int i = 0; i < bufferSize; i++){
		lAudio[i] = input[i * 2];
		rAudio[i] = input[i * 2 + 1];
	}
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
		case 'a':
			xFade1();
			break;
		case 's':
			xFade2();
			break;
		case 'v':
			tapBeat(0);
			break;
		case 'b':
			//beats[0].play();
			tapBeat(1);
			break;
		case 'n':
			//beats[1].play();
			tapBeat(2);
			break;
		case 'm':
			tapBeat(3);
			//beats[2].play();
			break;
		case ',':
			tapBeat(4);
			break;
		case 'z':
			changeSong1();
			break;
		case 'x':
			changeSong2();
			break;
		case ' ':
			if (!paused){
				roySux[song1].setPaused(true);
				roySux[song2].setPaused(true);
				paused = true;
			}
			else{
				roySux[song1].setPaused(false);
				roySux[song2].setPaused(false);
				paused = false;
			}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
