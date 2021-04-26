#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	//gallery::setup();
	dir.listDir("images/");
	dir.allowExt("jpg");
	dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order

	//allocate the vector to have as many ofImages as files
	if (dir.size()) {
		items.assign(dir.size(), string());
	}

	// you can now iterate through the files and load them into the ofImage vector
	for (int i = 0; i < (int)dir.size(); i++) {
		items[i] = dir.getPath(i);
	}
	currentItem = 0;

	ofBackground(ofColor::white);
}

//--------------------------------------------------------------
void ofApp::update() {
	if (isVideoPlaying)
		video.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	if (dir.size() > 0) {
		ofSetColor(ofColor::white);

		image = ofImage(items[currentItem]);
		if (!image.bAllocated()) { // not image
			if (!isVideoPlaying) {
				video.load(items[currentItem]);
				video.play();

				video.setVolume(0);
				video.setLoopState(OF_LOOP_NORMAL);

				isVideoPlaying = true;
			}

			if (video.getCurrentFrame() >= 50)
				video.firstFrame();

			video.draw(300, 50);
		}
		else {
			image.draw(300, 50);
			isVideoPlaying = false;
		}
	}

	// UI
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
		//frames
		ofDrawBitmapString(ofToString(video.getCurrentFrame()), 300, 50);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	//gallery::keyPressed(key, dir, currentItem);
	if (dir.size() > 0) {
		currentItem++;
		currentItem %= dir.size();

		video.stop();
		isVideoPlaying = false;
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
