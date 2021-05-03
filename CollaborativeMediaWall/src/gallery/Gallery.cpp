#include "Gallery.h"

//--------------------------------------------------------------
void Gallery::setup() {
	handleItems();

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
	itemsSize = (int)dir.size();
}

//--------------------------------------------------------------
void Gallery::update() {
	if (isVideoPlaying)
		video.update();
}

//--------------------------------------------------------------
void Gallery::draw() {
	int x = 0; // postion in the x
	int y = 50;

	int size = currentItem + 3;
	if (size > itemsSize)
		size = itemsSize;

	bool isVideoLoaded = false;
	for (int i = currentItem; i < size; i++) {
		image = ofImage(items[i]); // init image

		if (!image.bAllocated()) { // not image - is video
			video.load(items[i]);
			video.play();
			video.setVolume(0);

			video.setPaused(true);

			video.setPosition(0.5);

			image.setFromPixels(video.getPixels());
			isVideoLoaded = true;
		}

		int imageWidth = 300;
		int position = x * imageWidth + 75;
		if (position + imageWidth >= ofGetViewportWidth()) {
			y += image.getHeight() + 50;
			x = 0;
			position = x * (imageWidth + 75);
		}
		else if (position == 0) {
			position = 75;
		}
		/*
		if (!image.bAllocated())
			video.draw(position, 50 + y, imageSize, imageSize);
		else*/
			image.draw(position, 50 + y, imageSize, imageSize);
		x++;
	}

}

//--------------------------------------------------------------
void Gallery::keyPressed(int key) {
	if (GetKeyState(VK_RIGHT)) {
		if (currentItem < itemsSize - 3) {
			currentItem++;
			currentItem %= itemsSize;
		}
	}
	else if (GetKeyState(VK_LEFT)) {
		if (currentItem >= 1) {
			currentItem--;
			currentItem %= itemsSize;
		}
	}
	else if (GetKeyState(VK_SPACE)) {
		if (video.isPlaying()) {
			video.setPaused(video.isPaused());
		}
	}
}

//--------------------------------------------------------------
void Gallery::keyReleased(int key) {

}

//--------------------------------------------------------------
void Gallery::mouseMoved(int x, int y) {
	// use this to trigger event on video-> moving icon


}

//--------------------------------------------------------------
void Gallery::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void Gallery::mousePressed(int x, int y, int button) {

	cout << "\nVideo Loaded? " + ofToString(video.isLoaded());
	if (video.isLoaded()) {

		int image_x = 0; // postion in the x

		int size = currentItem + 3;
		if (size > itemsSize)
			size = itemsSize;

		for (int i = currentItem; i < size; i++) {
			int imageWidth = 300;

			bool inside_x = (x >= ((image_x * imageWidth) + ((image_x + 1) * 75)) && x <= (((image_x + 1) * 75) + (imageWidth * (image_x + 1))));
			bool inside_y = (y >= 75 && y <= 75 + imageWidth);

			cout << "\nInsideX: " + ofToString(inside_x);

			if (inside_x && inside_y && !isVideoPlaying) {
				isVideoPlaying = true;
				video.firstFrame();
				video.play();
			}
			else {
				//isVideoPlaying = false;
				//video.setPaused(true);
			}

			image_x++;
		}
	}
}

//--------------------------------------------------------------
void Gallery::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void Gallery::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void Gallery::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void Gallery::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void Gallery::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void Gallery::dragEvent(ofDragInfo dragInfo) {

}

void Gallery::handleItems() {
	if (document.loadFile("data_xml/users.xml")) {
		ofLogError("Abriu");
	}
	else {
		ofLogError("Nao abriu");
	}

	document.pushTag("users");
	int numberOfUsers = document.getNumTags("user");

	for (int i = 0; i < numberOfUsers; i++) {
		document.pushTag("user", i);

		int id = document.getValue("id", 0);
		ofDrawBitmapString("id: " + ofToString(id), 20, 20);

		string name = document.getValue("name", "");
		ofDrawBitmapString("name: " + name, 20, 30);

		document.popTag();
	}

	document.popTag(); //pop position

	ofDrawBitmapString("NumberOfUsers: " + ofToString(numberOfUsers), 10, 10);
}
