#include "Gallery.h"

//--------------------------------------------------------------
vector<Item*> Gallery::setup(int id, bool isUser) {
	// Init xml objects -> items, user_items
	initXmlObjects();

	// if is project
	if (!isUser) {
		int numOfProjects = projectsXml.getNumTags("project");
		for (int i = 0; i < numOfProjects; i++) {
			projectsXml.pushTag("project", i);
			if (projectsXml.getValue("id", 0) == id) {
				projectsXml.pushTag("users");
				int numberOfUsers = projectsXml.getNumTags("user");
				for (int j = 0; j < numberOfUsers; j++) {
					projectsXml.pushTag("user", j);
					int userId = projectsXml.getValue("id", 0);
					// handle user items for the user with id = userId
					handleUserItems(userId);
					projectsXml.popTag(); // user
				}
				projectsXml.popTag(); // users
				projectsXml.popTag(); // project i
				break;
			}
			projectsXml.popTag(); // project i
		}
	}
	else {
		// if is user
		handleUserItems(id);
	}

	// Buttons
	initButtons();

	return auxItems;
}

//--------------------------------------------------------------
void Gallery::update() {
	openWMB1->update();
	openWMB2->update();
	openWMB3->update();

	ex1->update();
	ex2->update();
	ex3->update();

	im1->update();
	im2->update();
	im3->update();

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
	int numItemsDisplay = size - currentItem;
	if (numItemsDisplay > 0) {
		openWMB1->draw();
		ex1->draw();
		im1->draw();
	}
	if (numItemsDisplay > 1) {
		openWMB2->draw();
		ex2->draw();
		im2->draw();
	}
	if (numItemsDisplay > 2) {
		openWMB3->draw();
		ex3->draw();
		im3->draw();
	}
}

//--------------------------------------------------------------
void Gallery::keyPressed(int key) {
	short inc = 1;
	if (GetKeyState(VK_RIGHT)) {
		if (currentItem < itemsSize - 3) {
			currentItem++;
			//currentItem %= itemsSize;
		}
	}
	else if (GetKeyState(VK_LEFT)) {
		if (currentItem >= 1) {
			currentItem--;
			//currentItem %= itemsSize;
			inc = -1;
		}
	}
	else if (GetKeyState(VK_SPACE)) {
		if (video.isLoaded()) {
			video.setPaused(!video.isPaused());
		}
	}

	ex1->setIndex(ex1->getIndex() + inc);
	ex2->setIndex(ex2->getIndex() + inc * 2);
	ex3->setIndex(ex3->getIndex() + inc * 3);

	im1->setIndex(im1->getIndex() + inc);
	im2->setIndex(im2->getIndex() + inc * 2);
	im3->setIndex(im3->getIndex() + inc * 3);
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

		itemsSize = items.size();
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

//------------------------------Aux_Functions----------------------------------//

void Gallery::filterItems(string filter)
{
	// if filter is empty, items = auxItems
	remove(filter.begin(), filter.end(), ' '); // trim
	if (filter == "") {
		(void)ofLog(OF_LOG_NOTICE, "Items = AuxItems!");
		items.clear();

		int size = auxItems.size();
		items.resize(size);
		itemsSize = size;
		items = auxItems;

		return;
	}

	// filter items
	vector<Item*> filteredItems;
	int counter = 0;
	int numItems = itemsXML.getNumTags("item");

	for (int i = 0; i < numItems; i++) {
		itemsXML.pushTag("item", i);
		itemsXML.pushTag("tags");

		int numTags = itemsXML.getNumTags("tag");
		for (int j = 0; j < numTags; j++) {
			string tag = itemsXML.getValue("tag", "", j);

			if (tag.find(filter) != std::string::npos) { // add this item
				filteredItems.push_back(auxItems[i]);
				counter++;
				break;
			}
		}
		itemsXML.popTag(); // tags
		itemsXML.popTag(); // item
	}
	// items = filteredItems
	items.clear();
	items.resize(counter);
	itemsSize = counter;
	items = filteredItems;
}

void Gallery::filterByColor(float hue)
{
	vector<Item*> filteredItems;
	int counter = 0;
	int numberOfItems = itemsXML.getNumTags("item");

	for (int i = 0; i < numberOfItems; i++) {
		itemsXML.pushTag("item", i);
		float color = itemsXML.getValue("color", 0);
		if (color == hue) {
			// this item will apear
			filteredItems.push_back(auxItems[i]);
			counter++;
		}
		itemsXML.popTag(); // item
	}
	// items = filteredItems
	items.clear();
	items.resize(counter);
	itemsSize = counter;
	items = filteredItems;
}

void Gallery::toggleMovingIcon(bool isMovingIcon)
{
	// checks or unchecks the value
	this->isMovingIcon = isMovingIcon;
	(void)ofLog(OF_LOG_NOTICE, "IsMovingIcon: " + ofToString(isMovingIcon));
}

void Gallery::reset()
{
	int counter = auxItems.size();
	items.clear();
	items.resize(counter);
	itemsSize = counter;
	items = auxItems;
}

//------------------------------Private_Fucntions-----------------------------------//

void Gallery::initButtons()
{
	//---------Open Media player
	openWMB1 = new ofxDatGuiButton("MediaPlayer");
	openWMB1->setPosition(250, 375);
	openWMB1->setIndex(0);
	openWMB1->setWidth(75);
	openWMB1->onButtonEvent(this, &Gallery::openInWMP);

	openWMB2 = new ofxDatGuiButton("MediaPlayer");
	openWMB2->setPosition(300 + imageSize, 375);
	openWMB2->setIndex(1);
	openWMB2->setWidth(75);
	openWMB2->onButtonEvent(this, &Gallery::openInWMP);

	openWMB3 = new ofxDatGuiButton("MediaPlayer");
	openWMB3->setPosition(350 + imageSize * 2, 375);
	openWMB3->setIndex(2);
	openWMB3->setWidth(75);
	openWMB3->onButtonEvent(this, &Gallery::openInWMP);

	//---------Export
	ex1 = new ofxDatGuiButton("Extract Metadata");
	ex1->setPosition(50, 375);
	ex1->setIndex(0);
	ex1->setWidth(100);
	ex1->onButtonEvent(this, &Gallery::extractMetadata);

	ex2 = new ofxDatGuiButton("Extract Metadata");
	ex2->setPosition(100 + imageSize, 375);
	ex2->setIndex(1);
	ex2->setWidth(100);
	ex2->onButtonEvent(this, &Gallery::extractMetadata);

	ex3 = new ofxDatGuiButton("Extract Metadata");
	ex3->setPosition(150 + imageSize * 2, 375);
	ex3->setIndex(2);
	ex3->setWidth(100);
	ex3->onButtonEvent(this, &Gallery::extractMetadata);

	//---------Import
	im1 = new ofxDatGuiButton("Import Metadata");
	im1->setPosition(150, 375);
	im1->setIndex(0);
	im1->setWidth(100);
	im1->onButtonEvent(this, &Gallery::importMetadata);

	im2 = new ofxDatGuiButton("Import Metadata");
	im2->setPosition(200 + imageSize, 375);
	im2->setIndex(1);
	im2->setWidth(100);
	im2->onButtonEvent(this, &Gallery::importMetadata);

	im3 = new ofxDatGuiButton("Import Metadata");
	im3->setPosition(250 + imageSize * 2, 375);
	im3->setIndex(2);
	im3->setWidth(100);
	im3->onButtonEvent(this, &Gallery::importMetadata);
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

void Gallery::initXmlObjects()
{
	// get xml for item
	if (itemsXML.loadFile("data_xml/items.xml")) {
		(void)ofLog(OF_LOG_NOTICE, "Open!");
	}
	else {
		(void)ofLog(OF_LOG_ERROR, "Didn't open!");
	}

	// get xml for user_items
	if (user_itemsXML.loadFile("data_xml/users_items.xml")) {
		(void)ofLog(OF_LOG_NOTICE, "Open!");
	}
	else {
		(void)ofLog(OF_LOG_ERROR, "Didn't open!");
	}

	// get xml for projects
	if (projectsXml.loadFile("data_xml/projects.xml")) {
		(void)ofLog(OF_LOG_NOTICE, "Open!");
	}
	else {
		(void)ofLog(OF_LOG_ERROR, "Didn't open!");
	}

	// init objects, in order to not push the headers everytime
	itemsXML.pushTag("items");
	user_itemsXML.pushTag("users_items");
	projectsXml.pushTag("projects");
}

bool Gallery::hasItemMetadata(string itemName)
{
	bool found = false;
	int numberItems = itemsXML.getNumTags("item");
	for (int i = 0; i < numberItems; i++) {
		itemsXML.pushTag("item", i);
		if (itemsXML.getValue("id", "") == itemName) {
			found = true;
			itemsXML.popTag(); // item
			break;
		}
		else
			itemsXML.popTag(); // item
	}
	return found;
}

void Gallery::handleUserItems(int userId) {
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
	//----------ofDirectory
	dir.listDir("items/");
	dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order

	//allocate the vector to have as many ofImages as files
	if (dir.size()) {
		items.assign(numberOfItems, &Item("", ofImage(), false, false));
		auxItems.assign(numberOfItems, &Item("", ofImage(), false, false));
	}

	int counter = 0;

	for (int i = 0; i < (int)dir.size(); i++) {
		// checks if user has the item
		string fileName = dir.getName(i);
		string itemName = fileName.substr(0, fileName.find('.'));

		if (find(user_items.begin(), user_items.end(), itemName) != user_items.end()) {
			string path = dir.getPath(i);
			ofImage img = ofImage(path);
			bool isVideo = false;
			if (!img.bAllocated()) {
				isVideo = true;

				video.load(path);
				video.play();
				video.setPaused(true);
				video.setPosition(0.5);

				img.setFromPixels(video.getPixels());
			}
			Item* item = new Item(path, img, isVideo, false);
			items[counter] = item;
			auxItems[counter++] = item;

			// generate metadata if not already generated
			if (!hasItemMetadata(itemName)) {
				//generateMetadata(itemName, path, img, isVideo);
			}
		}
	}
	currentItem = 0;
	itemsSize = counter;
}

void Gallery::generateMetadata(string itemName, string path, ofImage image, bool isVideo)
{
	int index = -1;
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

	// if metadata already exists
	if (index != -1)
		return;

	itemsXML.pushTag("item", index);
	// saves the id - itemName
	itemsXML.setValue("id", itemName);

	// color && luminance
	ofPixels& pixels = image.getPixels();

	float avgRed = 0;
	float avgGreen = 0;
	float avgBlue = 0;
	float avgLuminance = 0;

	int pixelSize = pixels.size();
	for (int i = 0; pixelSize; i++) {
		avgRed += pixels.getColor(pixels[i]).r;
		avgGreen += pixels.getColor(pixels[i]).r;
		avgBlue += pixels.getColor(pixels[i]).r;
		avgLuminance += pixels.getColor(pixels[i]).getBrightness();
	}
	avgRed /= pixelSize;
	avgGreen /= pixelSize;
	avgBlue /= pixelSize;
	avgLuminance = avgLuminance / pixelSize;
	ofColor newColor;
	newColor.r = avgRed;
	newColor.g = avgGreen;
	newColor.b = avgBlue;


	itemsXML.setValue("luminance", avgLuminance);
	itemsXML.setValue("color", newColor.getHue());

	// faces
	// finder faces
	finder.setup("data_xml/haarcascade_frontalface_default.xml");
	int faces = finder.findHaarObjects(image);

	itemsXML.addValue("faces", faces);
	// edges - filter2D
	string edges = edgesFilter(itemName);
	if (edges != "")
		itemsXML.setValue("edges", edges);
	// texture
	string texture = textureFilter(itemName);
	if (texture != "")
		itemsXML.setValue("texture", texture);
	// rhythm
	if (isVideo) {
		string rhythm = rhythmFilter(path);
		itemsXML.setValue("rhythm", rhythm);
	}
	itemsXML.popTag(); // item
}

string Gallery::edgesFilter(string itemName)
{
	// Declare variables
	Mat src, dst;
	Mat kernel;
	Point anchor;
	double delta;
	int ddepth;
	float kernel_size;
	const char* window_name = "filter2D Demo";
	// Loads an image
	src = imread(samples::findFile(itemName), IMREAD_COLOR); // Load an image
	if (src.empty())
	{
		printf(" Error opening image\n");
		printf(" Program Arguments: [image_name -- default lena.jpg] \n");
		return "";
	}
	// Initialize arguments for the filter
	anchor = Point(-1, -1);
	delta = 0;
	ddepth = -1;
	// Loop - Will filter the image with different kernel sizes each 0.5 seconds
	int ind = 0;
	for (;;)
	{
		// Update kernel size for a normalized box filter
		kernel_size = 3 + 2 * (ind % 5);
		kernel = Mat::ones(kernel_size, kernel_size, CV_32F) / (kernel_size * kernel_size);
		// Apply filter
		filter2D(src, dst, ddepth, kernel, anchor, delta, BORDER_DEFAULT);

		ind++;
	}
	string result = "";
	for (int i = 0; i < dst.rows; i++)
	{
		for (int j = 0; j < dst.cols; j++)
		{
			result += to_string(dst.at<float>(i, j)) + ", ";
		}
	}
	// returns the matrix in string format
	return result;
}

string Gallery::textureFilter(string itemName)
{
	Mat src, dst;
	int kernel_size = 31;

	src = imread(samples::findFile(itemName), IMREAD_COLOR); // Load an image
	if (src.empty())
	{
		printf(" Error opening image\n");
		printf(" Program Arguments: [image_name -- default lena.jpg] \n");
		return "";
	}
	double lambda = imageSize / 5;
	double theta = 45;
	double psi = 180;
	double gamma = 0.5;
	double sigma = 0.56 * lambda;

	Mat kernel = cv::getGaborKernel(cv::Size(kernel_size, kernel_size), sigma, theta, lambda, gamma, psi);
	filter2D(src, dst, CV_32F, kernel);

	string result = "";
	for (int i = 0; i < dst.rows; i++)
	{
		for (int j = 0; j < dst.cols; j++)
		{
			result += to_string(dst.at<float>(i, j)) + ", ";
		}
	}
	// returns the matrix in string format
	return result;
}

string Gallery::rhythmFilter(string path)
{
	CvHistogram* hist;

	ofVideoPlayer video;
	video.load(path);

	for (int i = 0; i < video.getTotalNumFrames(); i++) {
		video.nextFrame();
		ofImage image = ofImage(video.getPixels());

		ofxCvColorImage	colorImg;
		ofxCvGrayscaleImage grayImage;
		ofxCvGrayscaleImage grayBack;

		colorImg.setFromPixels(image.getPixels());

		grayImage = colorImg;

		IplImage* iplImageGray;
		IplImage** plane;

		iplImageGray = grayImage.getCvImage();
		plane = &iplImageGray;

		int hist_size[] = { 30 };
		float range[] = { 0, 180 };
		float* ranges[] = { range };
		hist = cvCreateHist(1, hist_size, CV_HIST_ARRAY, ranges, 1);

		cvCalcHist(plane, hist, 0, 0);
		cvNormalizeHist(hist, 20.0 * 255.0); // Normalize it

		cvCalcBackProject(plane, grayBack.getCvImage(), hist);// Calculate back projection  
		cvNormalizeHist(hist, 1.0); // Normalize it  

		//cvReleaseHist(&hist);
	}
	//rhythm = ofToString( ((float*)(cvPtr1D((hist)->bins, 0))) ); ???

	cvReleaseHist(&hist);

	string rhythm = "";
	return rhythm;
}

void Gallery::openInWMP(ofxDatGuiButtonEvent e)
{
	int index = e.target->getIndex();

	(void)ofLog(OF_LOG_NOTICE, "will play video...");

	string moviePath = ofFilePath::getAbsolutePath(ofToDataPath(items[index]->getPath()));
	string mode = "";
	if (items[index]->getIsVideo())
		mode = "/fullscreen";
	string command = " \"" + moviePath + "\" " + mode;

	string mplayerPath = "C:\\Program Files (x86)\\Windows Media Player\\";
	if (!ofDirectory(mplayerPath).exists())
		mplayerPath = "C:\\Program Files\\Windows Media Player\\";

	string mplayer = " \"" + mplayerPath + "\" ";

	ofSystem("start /d " + mplayer + " wmplayer " + command);
}

void Gallery::extractMetadata(ofxDatGuiButtonEvent e) {
	int index = e.target->getIndex();
	ofxXmlSettings saveFile;

	int numberOfItems = itemsXML.getNumTags("item");
	for (int i = 0; i < numberOfItems; i++) {
		itemsXML.pushTag("item", i);
		if (i == index) {
			saveFile.addValue("id", itemsXML.getValue("id", ""));

			saveFile.addTag("tags");
			saveFile.pushTag("tags");

			itemsXML.pushTag("tags");
			int numTags = itemsXML.getNumTags("tag");
			for (int j = 0; j < numTags; j++) {
				saveFile.addValue("tag", itemsXML.getValue("tag", "", j));
			}
			itemsXML.popTag(); // tags
			saveFile.popTag(); // tags

			saveFile.addValue("luminance", itemsXML.getValue("luminance", ""));
			saveFile.addValue("color", itemsXML.getValue("color", ""));
			saveFile.addValue("faces", itemsXML.getValue("faces", ""));
			saveFile.addValue("edges", itemsXML.getValue("edges", ""));
			saveFile.addValue("texture", itemsXML.getValue("texture", ""));

			saveFile.addTag("times");
			saveFile.pushTag("times");

			itemsXML.pushTag("times");
			int numTimes = itemsXML.getNumTags("time");
			for (int j = 0; j < numTimes; j++) {
				saveFile.addTag("time");
				saveFile.pushTag("time");
				itemsXML.pushTag("time", j);

				saveFile.addValue("name", itemsXML.getValue("name", "", j));
				saveFile.addValue("numTime", itemsXML.getValue("numTime", "", j));

				itemsXML.popTag(); // time
				saveFile.popTag(); // time
			}
			itemsXML.popTag(); // times
			saveFile.popTag(); // times

			if (items[index]->getIsVideo())
				saveFile.addValue("rhythm", itemsXML.getValue("rhythm", ""));

			itemsXML.popTag(); // item
			break;
		}
		itemsXML.popTag(); // item
	}
	// SAVES THE FILE
	windowFileSys = ofSystemSaveDialog("save.txt", "Save");
	if (windowFileSys.bSuccess) {
		string path = windowFileSys.getPath();
		saveFile.save(path);
	}
}

void Gallery::importMetadata(ofxDatGuiButtonEvent e)
{
	int index = e.target->getIndex();

	string tags = ofSystemTextBoxDialog("Number of tags", "1");
	int numberOfTags = stoi(tags);

	vector<string> listTags;
	listTags.assign(numberOfTags, string());

	for (int i = 0; i < numberOfTags; i++) {
		string tag = ofSystemTextBoxDialog("Tag " + ofToString(i + 1), "");
		listTags[i] = tag;
	}

	//string luminance = ofSystemTextBoxDialog("Luminance", "1");
	//string color = ofSystemTextBoxDialog("Color", "0");
	//string faces = ofSystemTextBoxDialog("Faces", "1");
	//string edge = ofSystemTextBoxDialog("Edge", "1");
	//string texture = ofSystemTextBoxDialog("Texture", "");
	string times = ofSystemTextBoxDialog("Number of objects to process (times a specific object (input as an image) appears in the video frame):", "1");
	int numberTimes = stoi(times);
	map<string, int> mapTimes;

	for (int i = 0; i < numberTimes; i++) {
		// process object
	}

	//string rhythm = "";
	//if (items[index]->getIsVideo())
	//	rhythm = ofSystemTextBoxDialog("Rhythm", "0");

	int numberOfItems = itemsXML.getNumTags("item");
	for (int i = 0; i < numberOfItems; i++) {
		itemsXML.pushTag("item", i);
		if (i == index) {
			itemsXML.pushTag("tags");
			int numExTags = itemsXML.getNumTags("tag"); // number of existing tags

			for (int j = 0; j < numberOfTags; j++) {
				itemsXML.setValue("tag", listTags[j], j + numExTags);
			}
			itemsXML.popTag(); // tags

			//itemsXML.setValue("luminance", luminance);
			//itemsXML.setValue("color", color);
			//itemsXML.setValue("faces", faces);
			//itemsXML.setValue("edge", edge);
			//itemsXML.setValue("texture", texture);

			int j = 0;
			itemsXML.pushTag("times");
			int numExTimes = itemsXML.getNumTags("time"); // number of existing times

			for (map<string, int>::iterator itr = mapTimes.begin(); itr != mapTimes.end(); ++itr) {
				itemsXML.addTag("time");
				itemsXML.pushTag("time", j + numExTimes);

				itemsXML.setValue("name", itr->first, j + numExTimes);
				itemsXML.setValue("numTime", itr->second, j + numExTimes);
				itemsXML.popTag();

				j++;
			}
			itemsXML.popTag(); // tags

			//if (items[index]->getIsVideo())
			//	itemsXML.setValue("rhythm", rhythm);

			itemsXML.popTag(); // item
			break;
		}
		itemsXML.popTag(); // item
	}
	// Saves file
	if (itemsXML.saveFile())
		(void)ofLog(OF_LOG_NOTICE, "Saved!");
	else
		(void)ofLog(OF_LOG_NOTICE, "Didn't save!");
}
