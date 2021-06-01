#include "PlayMode.h"

void PlayMode::setup(vector<Item*> items)
{
	camera.setVerbose(true);
	camera.setup(320, 240);

	bLearnBakground = true;
	threshold = 80;

	// then draw the contours:
	x = ofGetViewportWidth() - 320;
	y = ofGetViewportHeight() - 240;

	// buttons setup
	previousButton = new ofxDatGuiButton("<");
	previousButton->setPosition(270, ofGetViewportHeight() / 4 + 50);
	previousButton->setIndex(0);
	previousButton->setWidth(30);
	previousButton->onButtonEvent(this, &PlayMode::onButtonEvent);

	nextButton = new ofxDatGuiButton(">");
	nextButton->setPosition(700, ofGetViewportHeight() / 4 + 50);
	nextButton->setIndex(1);
	nextButton->setWidth(30);
	nextButton->onButtonEvent(this, &PlayMode::onButtonEvent);

	stopButton = new ofxDatGuiButton("||");
	stopButton->setPosition(500, 450);
	stopButton->setIndex(2);
	stopButton->setWidth(30);
	stopButton->onButtonEvent(this, &PlayMode::onButtonEvent);

	isGestureModeButton = new ofxDatGuiToggle("Gesture Mode?");
	isGestureModeButton->setChecked(true);
	isGestureModeButton->setPosition(x, y - isGestureModeButton->getHeight());
	isGestureModeButton->setWidth(150);
	isGestureModeButton->onToggleEvent(this, &PlayMode::onToggleEvent);

	// Gestures positions
	previous = ofRectangle(0, 0, camera.getWidth() / 4, camera.getHeight());
	next = ofRectangle(camera.getWidth() - (camera.getWidth() / 4), 0, camera.getWidth() / 4, camera.getHeight());
	stop = ofRectangle(camera.getWidth() / 3, camera.getWidth() / 4, camera.getWidth() / 3, camera.getHeight() / 2);
	//----------------UI
	previousUI = ofRectangle(x, y, camera.getWidth() / 4, camera.getHeight());
	nextUI = ofRectangle(x + camera.getWidth() - (camera.getWidth() / 4), y, camera.getWidth() / 4, camera.getHeight());
	stopUI = ofRectangle(x + camera.getWidth() / 3, y + camera.getHeight() / 4, camera.getWidth() / 3, camera.getHeight() / 2);

	itemsSize = items.size();
	this->items.assign(itemsSize, &Item("", ofImage(), false, false));
	this->items = items;
	currentItem = 0;
	if (!items.empty()) {
		if (items[currentItem]->getIsVideo()) {
			video.load(items[currentItem]->getPath());
			video.setLoopState(OF_LOOP_NONE); // stops video from looping
		}
		else
			image = items[currentItem]->getImage();
		if (items.size() > 1) {
			if (items[currentItem + 1]->getIsVideo()) {
				video2.load(items[currentItem + 1]->getPath());
				video2.setLoopState(OF_LOOP_NONE); // stops video from looping
			}
			else
				image2 = items[currentItem + 1]->getImage();
		}
	}

	time = ofGetElapsedTimef();
}

void PlayMode::update()
{
	bool bNewFrame = false;

	camera.update();
	bNewFrame = camera.isFrameNew();

	if (bNewFrame) {

		colorImg.setFromPixels(camera.getPixels());
		// mirrors the camera, easier to the user
		colorImg.mirror(false, true);

		grayImage = colorImg;
		if (bLearnBakground == true) {
			grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			bLearnBakground = false;
		}

		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(threshold);

		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, 20, (340 * 240) / 3, 10, true);	// find holes
	}
	if (!isGestureMode) {
		previousButton->update();
		nextButton->update();
		stopButton->update();
	}
	isGestureModeButton->update();

	if (!video.isPaused())
		video.update();

	if (ofGetElapsedTimef() - time > diffTime && !video.isPaused()) {
		if (video.isLoaded() && video.getIsMovieDone())
			changeCurrentItem(true);
	}

}

void PlayMode::draw()
{
	ofFill();
	ofSetHexColor(0x333333);
	ofDrawRectangle(x, y, 320, 240);
	ofSetHexColor(0xffffff);

	// Draws Rectangle Modes
	//---------------previous
	ofSetColor(previousColor);
	ofDrawLine(previousUI.getBottomLeft(), previousUI.getTopLeft());
	ofDrawLine(previousUI.getTopLeft(), previousUI.getTopRight());
	ofDrawLine(previousUI.getTopRight(), previousUI.getBottomRight());
	ofDrawLine(previousUI.getBottomLeft(), previousUI.getBottomRight());
	ofDrawBitmapString("Previous", previousUI.getCenter().x - 30, previousUI.getCenter().y);
	//---------------next
	ofSetColor(nextColor);
	ofDrawLine(nextUI.getBottomLeft(), nextUI.getTopLeft());
	ofDrawLine(nextUI.getTopLeft(), nextUI.getTopRight());
	ofDrawLine(nextUI.getTopRight(), nextUI.getBottomRight());
	ofDrawLine(nextUI.getBottomLeft(), nextUI.getBottomRight());
	ofDrawBitmapString("Next", nextUI.getCenter().x - 10, nextUI.getCenter().y);
	//---------------stop/pause
	ofSetColor(stopColor);
	ofDrawLine(stopUI.getBottomLeft(), stopUI.getTopLeft());
	ofDrawLine(stopUI.getTopLeft(), stopUI.getTopRight());
	ofDrawLine(stopUI.getTopRight(), stopUI.getBottomRight());
	ofDrawLine(stopUI.getBottomLeft(), stopUI.getBottomRight());
	ofDrawBitmapString("Play/Stop", stopUI.getCenter().x - 20, stopUI.getTopLeft().y);

	// or, instead we can draw each blob individually from the blobs vector,
	// this is how to get access to them:
	for (int i = 0; i < contourFinder.nBlobs; i++) {
		contourFinder.blobs[i].draw(x, y);
		//ofSetColor(0, 0, 0);
		// inside for x time
		if (previous.inside(contourFinder.blobs[i].boundingRect) && isGestureMode) {
			// reset other colors
			nextColor = ofColor::white;
			stopColor = ofColor::white;

			if (previousTime > 1.5 * ofGetFrameRate() && previousTime < 3 * ofGetFrameRate()) {// if stayed inside rect more than 1.5 secs
				// change rect color
				previousColor = ofColor::green;
			}
			else if (previousTime > 3 * ofGetFrameRate()) { // if stayed inside rect more than 3 secs
				changeCurrentItem(false);
				previousColor = ofColor::white;
				previousTime = 0;
			}
			else {
				previousColor = ofColor::white;
			}
			previousTime++;
		}
		else if (next.inside(contourFinder.blobs[i].boundingRect) && isGestureMode) {
			// reset other colors
			previousColor = ofColor::white;
			stopColor = ofColor::white;

			if (nextTime > 1.5 * ofGetFrameRate() && nextTime < 3 * ofGetFrameRate()) {// if stayed inside rect more than 1.5 secs
				// change rect color
				nextColor = ofColor::green;
			}
			else if (nextTime > 3 * ofGetFrameRate()) { // if stayed inside rect more than 3 secs
				changeCurrentItem(true);
				nextColor = ofColor::white;
				nextTime = 0;
			}
			else {
				nextColor = ofColor::white;
			}
			nextTime++;
		}
		else if (stop.inside(contourFinder.blobs[i].boundingRect) && video.isLoaded() && isGestureMode) {
			// reset other colors
			previousColor = ofColor::white;
			nextColor = ofColor::white;

			if (stopTime > 1.5 * ofGetFrameRate() && stopTime < 3 * ofGetFrameRate()) {// if stayed inside rect more than 1.5 secs
				// change rect color
				stopColor = ofColor::green;
			}
			else if (stopTime > 3 * fps.getFps()) { // if stayed inside rect more than 3 secs
				if (video.isLoaded())
					video.setPaused(!video.isPaused());

				stopColor = ofColor::white;
				stopTime = 0;
			}
			else {
				stopColor = ofColor::white;
			}
			stopTime++;
		}

		// draw over the centroid if the blob is a hole
		ofSetColor(255);
		if (contourFinder.blobs[i].hole) {
			ofDrawBitmapString("hole",
				contourFinder.blobs[i].boundingRect.getCenter().x + x,
				contourFinder.blobs[i].boundingRect.getCenter().y + y);
		}
	}

	// finally, a report:
	ofSetHexColor(0xffffff);
	stringstream reportStr;
	reportStr << "bg subtraction and blob detection" << endl
		<< "press ' ' to capture bg" << endl
		<< "threshold " << threshold << " (press: +/-)" << endl
		<< "num blobs found " << contourFinder.nBlobs << ", fps: " << ofGetFrameRate();
	ofDrawBitmapString(reportStr.str(), 20, 600);

	if (!isGestureMode) {
		previousButton->draw();
		nextButton->draw();
	}
	isGestureModeButton->draw();

	// Items
	if (items[currentItem]->getIsVideo()) {
		setFadeIn(true);
		video.draw(300, 50, 400, 400);
		if (!isGestureMode)
			stopButton->draw();
	}
	else {
		setFadeIn(false);
		image.draw(300, 50, 400, 400);
	}

}

void PlayMode::keyPressed(int key)
{
	switch (key) {
	case ' ':
		bLearnBakground = true;
		break;
	case '+':
		threshold++;
		if (threshold > 255) threshold = 255;
		break;
	case '-':
		threshold--;
		if (threshold < 0) threshold = 0;
		break;
	}
}

void PlayMode::keyReleased(int key) {

}

//--------------------------------------------------------------
void PlayMode::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void PlayMode::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void PlayMode::mousePressed(int x, int y, int button) {


}

//--------------------------------------------------------------
void PlayMode::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void PlayMode::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void PlayMode::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void PlayMode::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void PlayMode::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void PlayMode::dragEvent(ofDragInfo dragInfo) {

}

void PlayMode::setFadeIn(bool isVideo)
{
	float fadeTime = 3.0;
	if (isVideo) {
		float videoLength = video.getDuration();
		float videoElapsedTime = video.getPosition() * video.getDuration();
		float videoTimeRemaining = videoLength - videoElapsedTime;

		if (videoTimeRemaining < fadeTime) { //if it is time to fade  
			ofSetColor(255, 255, 255, 255 * videoTimeRemaining / fadeTime); // fade based on time left  
		}
		else {
			ofSetColor(255, 255, 255, 255); //else draw at full opacity  
		}
	}
	else {
		float timePassed = ofGetElapsedTimef() - time;
		if (timePassed > 5 - fadeTime) {
			ofSetColor(255, 255, 255, 255 * (5 - timePassed) / fadeTime); // fade based on time left 
		}
		else {
			ofSetColor(255, 255, 255, 255); //else draw at full opacity
		}
	}
	ofEnableAlphaBlending();
}

// Change CurrentItem
void PlayMode::changeCurrentItem(bool isNext) {
	if (isNext) {
		currentItem++;
		currentItem %= itemsSize;
	}
	else {
		currentItem--;
		if (currentItem < 0)
			currentItem = itemsSize - 1;
	}

	if (items[currentItem]->getIsVideo()) {
		video.load(items[currentItem]->getPath());
		video.play();
		video.setLoopState(OF_LOOP_NONE); // stops video from looping

		time = ofGetElapsedTimef() + video.getDuration();
		diffTime = 0.2f;
	}
	else {
		if (video.isPlaying()) {
			video.stop();
			video.closeMovie();
		}
		image = items[currentItem]->getImage();
		time = ofGetElapsedTimef();
		diffTime = 5;
	}
}

void PlayMode::onButtonEvent(ofxDatGuiButtonEvent e)
{
	if (!isGestureMode) {
		int index = e.target->getIndex();
		if (index == 0) {
			changeCurrentItem(false);
		}
		else if (index == 1) {
			changeCurrentItem(true);
		}
		else if (index == 2) {
			if (video.isLoaded())
				video.setPaused(!video.isPaused());
		}
	}
}

void PlayMode::onToggleEvent(ofxDatGuiToggleEvent e)
{
	isGestureModeButton->toggle();
	isGestureModeButton->setChecked(!isGestureModeButton->getChecked());
	isGestureMode = !isGestureMode;
}
