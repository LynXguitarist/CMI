/* FaceRec, touch, gesture, all video based features*/
#include "VideoPlayer.h"

//--------------------------------------------------------------
void VideoPlayer::setup() {
	ofBackground(255, 255, 255);
	ofSetVerticalSync(true);
	frameByframe = false;

	// Uncomment this to show movies with alpha channels
	// movie.setPixelFormat(OF_PIXELS_RGBA);

	movie.load("movies/fingers.mov");
	movie.setLoopState(OF_LOOP_NORMAL);
	movie.play();
}

//--------------------------------------------------------------
void VideoPlayer::update() {
	movie.update();
}

//--------------------------------------------------------------
void VideoPlayer::draw() {
	ofSetHexColor(0xFFFFFF);

	movie.draw(20, 20);
	ofSetHexColor(0x000000);
	ofPixels& pixels = movie.getPixels();

	int vidWidth = pixels.getWidth();
	int vidHeight = pixels.getHeight();
	int nChannels = pixels.getNumChannels();

	// let's move through the "RGB(A)" char array
	// using the red pixel to control the size of a circle.
	for (int i = 4; i < vidWidth; i += 8) {
		for (int j = 4; j < vidHeight; j += 8) {
			unsigned char r = pixels[(j * 320 + i) * nChannels];
			float val = 1 - ((float)r / 255.0f);
			ofDrawCircle(400 + i, 20 + j, 10 * val);
		}
	}

	ofSetHexColor(0x000000);
	ofDrawBitmapString("press f to change", 20, 320);
	if (frameByframe) ofSetHexColor(0xCCCCCC);
	ofDrawBitmapString("mouse speed position", 20, 340);
	if (!frameByframe) ofSetHexColor(0xCCCCCC); else ofSetHexColor(0x000000);
	ofDrawBitmapString("keys <- -> frame by frame ", 190, 340);
	ofSetHexColor(0x000000);

	ofDrawBitmapString("frame: " + ofToString(movie.getCurrentFrame()) + "/" + ofToString(movie.getTotalNumFrames()), 20, 380);
	ofDrawBitmapString("duration: " + ofToString(movie.getPosition() * movie.getDuration(), 2) + "/" + ofToString(movie.getDuration(), 2), 20, 400);
	ofDrawBitmapString("speed: " + ofToString(movie.getSpeed(), 2), 20, 420);

	if (movie.getIsMovieDone()) {
		ofSetHexColor(0xFF0000);
		ofDrawBitmapString("end of movie", 20, 440);
	}
}

//--------------------------------------------------------------
void VideoPlayer::keyPressed(int key) {
	switch (key) {
	case 'f':
		frameByframe = !frameByframe;
		movie.setPaused(frameByframe);
		break;
	case OF_KEY_LEFT:
		movie.previousFrame();
		break;
	case OF_KEY_RIGHT:
		movie.nextFrame();
		break;
	case '0':
		movie.firstFrame();
		break;
	}
}

//--------------------------------------------------------------
void VideoPlayer::keyReleased(int key) {

}

//--------------------------------------------------------------
void VideoPlayer::mouseMoved(int x, int y) {
	if (!frameByframe) {
		int width = ofGetWidth();
		float pct = (float)x / (float)width;
		float speed = (2 * pct - 1) * 5.0f;
		movie.setSpeed(speed);
	}
}

//--------------------------------------------------------------
void VideoPlayer::mouseDragged(int x, int y, int button) {
	if (!frameByframe) {
		int width = ofGetWidth();
		float pct = (float)x / (float)width;
		movie.setPosition(pct);
	}
}

//--------------------------------------------------------------
void VideoPlayer::mousePressed(int x, int y, int button) {
	if (!frameByframe) {
		movie.setPaused(true);
	}
}


//--------------------------------------------------------------
void VideoPlayer::mouseReleased(int x, int y, int button) {
	if (!frameByframe) {
		movie.setPaused(false);
	}
}

//--------------------------------------------------------------
void VideoPlayer::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void VideoPlayer::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void VideoPlayer::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void VideoPlayer::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void VideoPlayer::dragEvent(ofDragInfo dragInfo) {

}
