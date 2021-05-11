#include "Gallery.h"

//--------------------------------------------------------------
void Gallery::setup() {
	handleItems();

	dir.listDir("images/");
	dir.allowExt("jpg");
	dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order

	//allocate the vector to have as many ofImages as files
	if (dir.size()) {
		items.assign(dir.size(), &Item("", ofImage(), false, false));
	}

	// you can now iterate through the files and load them into the ofImage vector
	for (int i = 0; i < (int)dir.size(); i++) {
		ofImage img = ofImage(dir.getPath(i));
		bool isVideo = false;
		if (!img.bAllocated()) {
			isVideo = true;

			video.load(dir.getPath(i));
			video.play();
			video.setPaused(true);
			video.setPosition(0.5);

			img.setFromPixels(video.getPixels());
		}

		items[i] = new Item(dir.getPath(i), img, isVideo, false);
	}
	currentItem = 0;
	itemsSize = (int)dir.size();

	isMovingIcon = true;
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

	for (int i = currentItem; i < size; i++) {
		string path = items[i]->getPath();
		bool isCurrVideoPlaying = items[i]->getIsVideoPlaying();

		int position = x * imageSize + (x + 1) * 50;

		if (isCurrVideoPlaying && !isMovingIcon) {
			if (video.getCurrentFrame() >= 50)
				video.firstFrame();

			video.draw(position, 50 + y, imageSize, imageSize);
		}
		else {
			if (isMovingIcon && isCurrVideoPlaying) {
				video.setFrame(currentFrame);
				image.setFromPixels(video.getPixels());
				nextFrame();
				// waits 1 sec
				Sleep(100);
			}
			else {
				image = items[i]->getImage(); // init image
			}

			image.draw(position, 50 + y, imageSize, imageSize);
		}

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

	if (video.isLoaded()) {

		int image_x = 0; // postion in the x

		int size = currentItem + 3;
		if (size > itemsSize)
			size = itemsSize;

		for (int i = currentItem; i < size; i++) {

			bool inside_x = (x >= ((image_x * imageSize) + ((image_x + 1) * 75)) && x <= (((image_x + 1) * 75) + (imageSize * (image_x + 1))));
			bool inside_y = (y >= 75 && y <= 75 + imageSize);

			if (inside_x && inside_y && !items[i]->getIsVideoPlaying() && items[i]->getIsVideo()) {
				if (isVideoPlaying) { // check first if another video is playing
					items[videoPlaying]->setVideoPlaying(false);
				}

				isVideoPlaying = true;
				items[i]->setVideoPlaying(true);
				videoPlaying = i;

				currentFrame = 0;

				video.load(items[i]->getPath());
				video.setVolume(0);
				video.firstFrame();
				video.play();

				cout << "\nWill start playing now...";
			}
			else if (inside_x && inside_y && items[i]->getIsVideo()) {
				isVideoPlaying = false;
				items[i]->setVideoPlaying(false);
				video.setPaused(true);
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

void Gallery::nextFrame() {
	if (currentFrame == 0)
		currentFrame = 0.25f;
	else if (currentFrame == 0.25f)
		currentFrame = 0.5f;
	else if (currentFrame == 0.5f)
		currentFrame = 0.75f;
	else if (currentFrame == 0.75f)
		currentFrame = 0;
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
