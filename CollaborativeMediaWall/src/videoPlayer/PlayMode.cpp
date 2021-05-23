#include "PlayMode.h"

void PlayMode::setup(vector<Item*> items)
{
	camera.setVerbose(true);
	camera.setup(320, 240);

	bLearnBakground = true;
	threshold = 80;

	// Gestures positions
	previous = ofRectangle(0, 0, camera.getWidth() / 4, camera.getHeight());
	next = ofRectangle(camera.getWidth() - (camera.getWidth() / 4), 0, camera.getWidth() / 4, camera.getHeight());
	stop = ofRectangle(camera.getWidth() / 3, camera.getHeight()/ 2, camera.getWidth() / 4, camera.getHeight() / 2);


	//itemsSize = items.size();
	//this->items.assign(itemsSize, &Item("", ofImage(), false, false));
	//this->items = items;
	//currentItem = 0;
}

void PlayMode::update()
{
	bool bNewFrame = false;

	camera.update();
	bNewFrame = camera.isFrameNew();

	if (bNewFrame) {

		colorImg.setFromPixels(camera.getPixels());

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
	/*
	if (!video.isPaused())
		video.update();
	*/
}

void PlayMode::draw()
{
	// then draw the contours:
	int x = ofGetViewportWidth() - 320;
	int y = ofGetViewportHeight() - 240;

	ofFill();
	ofSetHexColor(0x333333);
	ofDrawRectangle(x, y, 320, 240);
	ofSetHexColor(0xffffff);

	// or, instead we can draw each blob individually from the blobs vector,
	// this is how to get access to them:
	for (int i = 0; i < contourFinder.nBlobs; i++) {
		contourFinder.blobs[i].draw(x, y);

		// inside for x time
		if (previous.inside(contourFinder.blobs[i].boundingRect)) {
			(void)ofLog(OF_LOG_NOTICE, "Inside previous");
		}
		else if (next.inside(contourFinder.blobs[i].boundingRect)) {
			(void)ofLog(OF_LOG_NOTICE, "Inside next");
		}
		else if (stop.inside(contourFinder.blobs[i].boundingRect)) {
			(void)ofLog(OF_LOG_NOTICE, "Inside stop/play");
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

	/*
	// Items
	if (items[currentItem]->getIsVideo())
		video.draw(ofGetViewportWidth() / 2, 200, 300, 300);
	else
		image.draw(ofGetViewportWidth() / 2, 200, 300, 300);
	*/
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

	if (GetKeyState(VK_RIGHT)) {
		if (currentItem < itemsSize) {
			currentItem++;
			//currentItem %= itemsSize;
			if (items[currentItem]->getIsVideo())
				video.load(items[currentItem]->getPath());
		}
	}
	else if (GetKeyState(VK_LEFT)) {
		if (currentItem >= 1) {
			currentItem--;
			//currentItem %= itemsSize;
			if (items[currentItem]->getIsVideo())
				video.load(items[currentItem]->getPath());
		}
	}
	else if (GetKeyState(VK_SPACE)) {
		if (video.isLoaded())
			video.setPaused(!video.isPaused());
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