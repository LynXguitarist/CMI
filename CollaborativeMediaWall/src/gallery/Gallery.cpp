#include "Gallery.h"

//--------------------------------------------------------------
void Gallery::setup() {
	// makes the logs log in console
	ofLogToConsole();

	// passes the id
	handleUserItems(1);

	// Vars
	isMovingIcon = false;
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
				video.setPosition(currentFrame);
				image.setFromPixels(video.getPixels());
				nextFrame();
				// waits x secs
				Sleep(800);
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
	// BUG AO ANDAR PARA TRAS
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

void Gallery::handleUserItems(int userId) {
	string msg = "Abriu";
	if (document.loadFile("data_xml/users_items.xml")) {
		(void)ofLog(OF_LOG_NOTICE, msg);
	}
	else {
		msg = "Não abriu";
		(void)ofLog(OF_LOG_ERROR, msg);
		return;
	}

	document.pushTag("users_items");
	int numberOfUsers = document.getNumTags("user_items");

	int numberOfItems = 0;
	vector<string> user_items;

	for (int i = 0; i < numberOfUsers; i++) {
		document.pushTag("user_items", i);

		if (document.getValue("user", 0) == userId) {
			// get items
			document.pushTag("items", i);
			numberOfItems = document.getNumTags("item");

			user_items.assign(numberOfItems, string());

			for (int j = 0; j < numberOfItems; j++) {
				document.pushTag("item", j);
				string itemId = document.getValue("id", "");
				// add to vector
				user_items.push_back(itemId);

				document.popTag(); // item
			}
		}
		document.popTag(); // items
		document.popTag(); // user_items
	}

	document.popTag(); //pop position

	(void)ofLog(OF_LOG_NOTICE, "NumberOfUsers: " + ofToString(numberOfUsers));

	//------------DIR-------------//

	dir.listDir("items/");
	dir.allowExt("jpg");
	dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order

	//allocate the vector to have as many ofImages as files
	if (dir.size()) {
		items.assign(numberOfItems, &Item("", ofImage(), false, false));
	}

	int counter = 0;

	for (int i = 0; i < (int)dir.size(); i++) {
		// checks if user has the item
		string fileName = dir.getName(i);
		string itemName = fileName.substr(0, fileName.find('.'));

		if (find(user_items.begin(), user_items.end(), itemName) != user_items.end()) {
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

			items[counter++] = new Item(dir.getPath(i), img, isVideo, false);
		}
	}
	currentItem = 0;
	itemsSize = counter;

}
