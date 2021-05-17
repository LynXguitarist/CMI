#include "Gallery.h"

//--------------------------------------------------------------
void Gallery::setup() {
	// makes the logs log in console
	ofLogToConsole();

	// get xml for item
	if (itemsXML.loadFile("data_xml/items.xml")) {
		(void)ofLog(OF_LOG_NOTICE, "Open!");
	}
	else {
		(void)ofLog(OF_LOG_ERROR, "Didn't open!");
		return;
	}

	// get xml for user_items
	if (user_itemsXML.loadFile("data_xml/users_items.xml")) {
		(void)ofLog(OF_LOG_NOTICE, "Open!");
	}
	else {
		(void)ofLog(OF_LOG_ERROR, "Didn't open!");
		return;
	}

	// passes the id
	handleUserItems(1);

	// Buttons
	//---------Export
	ex1 = new ofxDatGuiButton("Extract Metadata");
	ex1->setPosition(50, 375);
	ex1->setIndex(0);
	ex1->setWidth(100);
	ex1->onButtonEvent(this, &Gallery::extractMetadata);

	//---------Import
	im1 = new ofxDatGuiButton("Import Metadata");
	im1->setPosition(150, 375);
	im1->setIndex(0);
	im1->setWidth(100);
	im1->onButtonEvent(this, &Gallery::importMetadata);

	// Vars
	isMovingIcon = false;
}

//--------------------------------------------------------------
void Gallery::update() {
	ex1->update();
	im1->update();

	if (isVideoPlaying)
		video.update();
}

//--------------------------------------------------------------
void Gallery::draw() {
	ex1->draw();
	im1->draw();

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

void Gallery::generateMetadata(string itemName, ofImage image)
{
	int index = -1;
	itemsXML.pushTag("items");
	int numberOfItems = itemsXML.getNumTags("item");

	for (int i = 0; i < numberOfItems; i++) {
		itemsXML.pushTag("item", i);
		string id = itemsXML.getValue("id", "", i);
		// verify if item exists in xml
		if (id == itemName) {
			index = i;
			break;
		}
		itemsXML.popTag(); // item
	}

	if (index == -1)
		return;

	itemsXML.pushTag("item", index);
	// saves the id - itemName
	itemsXML.setValue("id", itemName);

	// color && luminance
	ofPixels& pixels = image.getPixels();

	double avgColor = 0;
	double avgLuminance = 0;

	int pixelSize = pixels.size();
	for (int i = 0; pixelSize; i++) {
		avgColor += pixels.getColor(pixels[i]).getHue();
		avgLuminance += pixels.getColor(pixels[i]).getBrightness();
	}
	avgColor = avgColor / pixelSize;
	avgLuminance = avgLuminance / pixelSize;

	itemsXML.setValue("luminance", avgLuminance);
	itemsXML.setValue("color", avgColor);

	// faces


	itemsXML.popTag(); // item
	itemsXML.popTag(); // items
}

void Gallery::handleUserItems(int userId) {
	user_itemsXML.pushTag("users_items");
	int numberOfUsers = user_itemsXML.getNumTags("user_items");

	int numberOfItems = 0;
	vector<string> user_items;

	for (int i = 0; i < numberOfUsers; i++) {
		user_itemsXML.pushTag("user_items", i);

		if (user_itemsXML.getValue("user", 0) == userId) {
			// get items
			user_itemsXML.pushTag("items", i);
			numberOfItems = user_itemsXML.getNumTags("item");

			user_items.assign(numberOfItems, string());

			for (int j = 0; j < numberOfItems; j++) {
				user_itemsXML.pushTag("item", j);
				string itemId = user_itemsXML.getValue("id", "");
				// add to vector
				user_items.push_back(itemId);

				user_itemsXML.popTag(); // item
			}
		}
		user_itemsXML.popTag(); // items
		user_itemsXML.popTag(); // user_items
	}

	user_itemsXML.popTag(); //pop position

	//--------------------------------------ofDirectory-------------------------------------------//

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
			// generate metadata if not already generated
			//generateMetadata(itemName, img);
		}
	}
	// saves the items of the user
	auxItems = items;

	currentItem = 0;
	itemsSize = counter;

}

void Gallery::filterItems(string filter)
{
	// if filter is empty, items = auxItems
	if (filter == "") {
		(void)ofLog(OF_LOG_NOTICE, "Items = AuxItems!");
		items = auxItems;
		return;
	}
	// filter items
	int counter = 0;


	if (itemsXML.loadFile("data_xml/items.xml")) {
		(void)ofLog(OF_LOG_NOTICE, "Open!");
	}
	else {
		(void)ofLog(OF_LOG_ERROR, "Didn't open!");
		return;
	}
	itemsXML.pushTag("items");
	int numItems = itemsXML.getNumTags("item");

	for (int i = 0; i < numItems; i++) {
		itemsXML.pushTag("item", i);
		itemsXML.pushTag("tags");

		int numTags = itemsXML.getNumTags("tag");
		for (int j = 0; j < numTags; j++) {
			string tag = itemsXML.getValue("tag", "", j);

			if (tag.find(filter) != std::string::npos) { // add this item
				(void)ofLog(OF_LOG_NOTICE, "found");

			}
		}
		itemsXML.popTag(); // tags
		itemsXML.popTag(); // item
	}

	itemsXML.popTag();
}

// NAO PODE SER PELO INDEX
void Gallery::extractMetadata(ofxDatGuiButtonEvent e) {
	int index = e.target->getIndex();
	ofxXmlSettings saveFile;

	itemsXML.pushTag("items");
	int numberOfItems = itemsXML.getNumTags("item");

	for (int i = 0; i < numberOfItems; i++) {
		itemsXML.pushTag("item", i);

		if (i == index) {
			saveFile.addTag("tags");
			saveFile.pushTag("tags");
			int numTags = itemsXML.getNumTags("tags");
			itemsXML.pushTag("tags");
			for (int j = 0; j < numTags; j++) {
				saveFile.addValue("tag", itemsXML.getValue("tag", "", j));
			}
			itemsXML.popTag(); // tags
			saveFile.popTag(); // tags

			saveFile.addValue("luminance", itemsXML.getValue("luminance", 0));
			saveFile.addValue("color", itemsXML.getValue("color", ""));
			saveFile.addValue("faces", itemsXML.getValue("faces", 0));
			saveFile.addValue("texture", itemsXML.getValue("texture", ""));

			saveFile.addTag("times");
			saveFile.pushTag("times");
			int numTimes = itemsXML.getNumTags("times");
			itemsXML.pushTag("times");
			for (int j = 0; j < numTimes; j++) {
				saveFile.addTag("time");
				saveFile.pushTag("time");
				itemsXML.pushTag("time");

				saveFile.addValue("name", itemsXML.getValue("name", "", j));
				saveFile.addValue("numTime", itemsXML.getValue("numTime", 0, j));

				itemsXML.popTag(); // time
				saveFile.popTag(); // time
			}
			itemsXML.popTag();
			saveFile.popTag(); // times

			if (items[index]->getIsVideo())
				saveFile.addValue("rhythm", itemsXML.getValue("rhythm", 0));

			break;
		}

		itemsXML.popTag(); // item
	}

	itemsXML.popTag(); // items

	// SAVES THE FILE
	windowFileSys = ofSystemSaveDialog("save.txt", "Save");
	if (windowFileSys.bSuccess) {
		string path = windowFileSys.getPath();
		saveFile.save(path);
	}
}

// NAO PODE SER PELO INDEX
// the user types or import file???
void Gallery::importMetadata(ofxDatGuiButtonEvent e)
{
	int index = e.target->getIndex();

	string tags = ofSystemTextBoxDialog("Number of tags", "1");
	int numberOfTags = stoi(tags);

	vector<string> listTags;
	listTags.assign(numberOfTags, string());

	for (int i = 0; i < numberOfTags; i++) {
		string tag = ofSystemTextBoxDialog("Tag " + ofToString(i), "");
		listTags[i] = tag;
	}

	string luminance = ofSystemTextBoxDialog("luminance", "1");
	string color = ofSystemTextBoxDialog("Color", "red");
	string faces = ofSystemTextBoxDialog("Faces", "1");
	string edge = ofSystemTextBoxDialog("Edge", "1");
	string texture = ofSystemTextBoxDialog("Texture", "");
	string rhythm = "";
	if (items[index]->getIsVideo())
		rhythm = ofSystemTextBoxDialog("Rhythm", "0");

	itemsXML.pushTag("items");
	int numberOfItems = itemsXML.getNumTags("item");

	for (int i = 0; i < numberOfItems; i++) {
		itemsXML.pushTag("item", i);

		if (i == index) {
			itemsXML.pushTag("tags");
			for (int j = 0; j < numberOfTags; j++) {
				itemsXML.setValue("tag", listTags[j], j);
			}
			itemsXML.popTag(); // tags

			itemsXML.setValue("luminance", luminance);
			itemsXML.setValue("color", color);
			itemsXML.setValue("faces", faces);
			itemsXML.setValue("edge", edge);
			itemsXML.setValue("texture", texture);

			if (items[index]->getIsVideo())
				itemsXML.setValue("rhythm", rhythm);
			break;
		}
		itemsXML.popTag(); // item
	}
	itemsXML.popTag(); // items
	// Saves file
	if (itemsXML.saveFile())
		(void)ofLog(OF_LOG_NOTICE, "Saved!");
	else
		(void)ofLog(OF_LOG_NOTICE, "Didn't save!");
}
