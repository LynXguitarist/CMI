#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	//gallery::setup();
	dir.listDir("images/");
	dir.allowExt("jpg");
	dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order

	//allocate the vector to have as many ofImages as files
	if (dir.size()) {
		items.assign(dir.size(), ofImage());
		video.assign(dir.size(), ofVideoPlayer());
	}

	//items[i].bAllocated() -> verifica se imagem loaded, se nao, porque video

	videoInt = 0;

	// you can now iterate through the files and load them into the ofImage vector
	for (int i = 0; i < (int)dir.size(); i++) {
		items[i].load(dir.getPath(i));

		if (!items[i].bAllocated()) {
			video[videoInt].load(dir.getPath(i));
			video[videoInt].setLoopState(OF_LOOP_NORMAL);
			video[videoInt].setVolume(0);
			videoInt++;
		}
	}
	currentItem = 0;

	ofBackground(ofColor::white);
}

//--------------------------------------------------------------
void ofApp::update() {
	video[currentVideo].update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	//gallery::draw();

	if (dir.size() > 0) {
		ofSetColor(ofColor::white);

		if (!items[currentItem].bAllocated()) {
			if (!video[currentVideo].isPlaying()) {
				video[currentVideo].play();
			}
			video[currentVideo].draw(300, 50);

			if (video[currentVideo].getCurrentFrame() >= 40) {
				video[currentVideo].firstFrame();
			}
		}
		else {
			video[currentVideo].stop();
			items[currentItem].draw(300, 50);
		}

		ofSetColor(ofColor::grey);
	}


	ofSetColor(ofColor::gray);
	for (int i = 0; i < (int)dir.size(); i++) {
		if (i == currentItem) {
			ofSetColor(ofColor::red);
		}
		else {
			ofSetColor(ofColor::black);
		}

		string fileInfo = "file " + ofToString(i + 1) + " = " + dir.getName(i);
		ofDrawBitmapString(fileInfo, 50, i * 20 + 50);
		ofDrawBitmapString(ofToString(currentVideo) + " - " + ofToString(video[currentVideo].getCurrentFrame()), 300,50);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	//gallery::keyPressed(key, dir, currentItem);
	if (dir.size() > 0) {
		if (!items[currentItem].bAllocated()) {
			video[currentVideo].stop();
			currentVideo++;
			currentVideo %= videoInt;
		}

		currentItem++;
		currentItem %= dir.size();
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}


//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
