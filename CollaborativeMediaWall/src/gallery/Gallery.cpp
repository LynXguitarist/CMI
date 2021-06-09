#include "Gallery.h"

//--------------------------------------------------------------
vector<Item*> Gallery::setup(int id, bool isUser, vector<Item*> items_input, bool useItemsInput) {
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
					handleUserItems(userId, items_input, useItemsInput);
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
		handleUserItems(id, items_input, useItemsInput);
	}

	// Buttons
	initButtons();

	if (auxItems.empty())
		ofSystemAlertDialog("No items to preview!");

	return auxItems;
}

//--------------------------------------------------------------
void Gallery::update() {
	int size = currentItem + 3;
	if (size > itemsSize)
		size = itemsSize;

	int numItemsDisplay = size - currentItem;

	if (numItemsDisplay > 0) {
		openWMB1->update();
		ex1->update();
		im1->update();
	}

	if (numItemsDisplay > 1) {
		openWMB2->update();
		ex2->update();
		im2->update();
	}

	if (numItemsDisplay > 2) {
		openWMB3->update();
		ex3->update();
		im3->update();
	}

	if (currentItem > 0)
		previousButton->update();

	if (currentItem != items.size() - 1)
		nextButton->update();

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
			if (image.isAllocated())
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

	if (currentItem > 0)
		previousButton->draw();

	if (currentItem + 2 != items.size() - 1)
		nextButton->draw();
}

//--------------------------------------------------------------
void Gallery::keyPressed(int key) {
	short inc = 1;
	if (key == 57358) {
		if (currentItem < itemsSize - 3) {
			currentItem++;
			//currentItem %= itemsSize;
		}
	}
	else if (key == 57356) {
		if (currentItem >= 1) {
			currentItem--;
			//currentItem %= itemsSize;
			inc = -1;
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
		bool wasAdded = false;
		// tags
		itemsXML.pushTag("item", i);
		itemsXML.pushTag("tags");

		int numTags = itemsXML.getNumTags("tag");
		for (int j = 0; j < numTags; j++) {
			string tag = itemsXML.getValue("tag", "", j);

			if (tag.find(filter) != std::string::npos) { // add this item
				filteredItems.push_back(auxItems[i]);
				counter++;
				wasAdded = true;
				break;
			}
		}
		itemsXML.popTag(); // tags
		// so the same item isnt added twice
		if (!wasAdded) {
			// times
			itemsXML.pushTag("times");

			int numTimes = itemsXML.getNumTags("time");
			for (int j = 0; j < numTimes; j++) {
				itemsXML.pushTag("time", j);
				string name = itemsXML.getValue("name", "");

				if (name.find(filter) != std::string::npos) { // add this item
					filteredItems.push_back(auxItems[i]);
					counter++;
					break;
				}
			}
			itemsXML.popTag(); // times
		}

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
		if (abs(color - hue) <= 10 || abs(color - hue) >= 245) {
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

	// change items buttons 
	nextButton = new ofxDatGuiButton(">");
	nextButton->setPosition(ofGetViewportWidth() - 25, 100 + imageSize / 2);
	nextButton->setIndex(0);
	nextButton->setWidth(25);
	nextButton->onButtonEvent(this, &Gallery::changeItems);

	previousButton = new ofxDatGuiButton("<");
	previousButton->setPosition(0, 100 + imageSize / 2);
	previousButton->setIndex(1);
	previousButton->setWidth(25);
	previousButton->onButtonEvent(this, &Gallery::changeItems);
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

void Gallery::handleUserItems(int userId, vector<Item*> items_input, bool useItemsInput) {
	if (!useItemsInput) {
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
		dir.sort();

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
				generateMetadata(itemName, path, img, isVideo);
			}
		}
		itemsSize = counter;
	}
	else {
		int counter = items_input.size();
		items.clear();
		itemsSize = counter;
		items = items_input;
	}
	currentItem = 0;
}

void Gallery::generateMetadata(string itemName, string path, ofImage image, bool isVideo)
{
	int numberOfItems = itemsXML.getNumTags("item");

	for (int i = 0; i < numberOfItems; i++) {
		itemsXML.pushTag("item", i);
		string id = itemsXML.getValue("id", "", i);
		// verify if item exists in xml
		if (id == itemName) {
			itemsXML.popTag(); // item
			return;
		}
		itemsXML.popTag(); // item
	}

	itemsXML.addTag("item");
	itemsXML.pushTag("item", numberOfItems); // mudar para numberOfItems + 1 depois de testar
	// saves the id - itemName
	itemsXML.setValue("id", itemName, numberOfItems);

	// color && luminance
	ofPixels& pixels = image.getPixels();

	float avgRed = 0;
	float avgGreen = 0;
	float avgBlue = 0;
	float avgLuminance = 0;

	int pixelSize = pixels.size();
	for (int i = 0; i < pixelSize; i++) {
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

	itemsXML.setValue("luminance", avgLuminance, numberOfItems);
	itemsXML.setValue("color", newColor.getHue(), numberOfItems);

	// faces
	// finder faces
	finder.setup("data_xml/haarcascade_frontalface_default.xml");
	int faces = 0;
	if (isVideo) {
		ofVideoPlayer auxVideo;
		auxVideo.load(path);

		for (int i = 0; i <= 5; i++) {
			ofImage auxImg;

			auxImg.setFromPixels(auxVideo.getPixels());
			faces += finder.findHaarObjects(auxImg);

			auxVideo.setPosition(i * 0.25);
		}
		faces /= 5;
	}
	else {
		faces = finder.findHaarObjects(image);
	}

	itemsXML.setValue("faces", faces, numberOfItems);
	// edges - filter2D
	string edges = edgesFilter(image);
	(void)ofLog(OF_LOG_NOTICE, "edges: " + edges);
	if (edges != "")
		itemsXML.setValue("edges", edges, numberOfItems);
	// texture
	string texture = textureFilter(image);
	(void)ofLog(OF_LOG_NOTICE, "texture: " + texture);
	if (texture != "")
		itemsXML.setValue("texture", texture, numberOfItems);
	// rhythm
	if (isVideo) {
		double rhythm = rhythmFilter(path);
		(void)ofLog(OF_LOG_NOTICE, "rhythm: " + ofToString(rhythm));
		itemsXML.setValue("rhythm", rhythm, numberOfItems);
	}

	itemsXML.popTag(); // item

	itemsXML.saveFile();
}

string Gallery::edgesFilter(ofImage image)
{
	double kernel_size;
	Mat kernel;
	kernel_size = 31;
	kernel = Mat::ones(kernel_size, kernel_size, CV_32F) / (kernel_size * kernel_size);

	Mat src = toCv(image);
	Mat dst;
	Mat gray_image;

	vector<Mat> channels;
	Mat hsv;
	cvtColor(src, hsv, CV_RGB2HSV);
	split(hsv, channels);
	gray_image = channels[0];

	Canny(image, dst, 10, 350);

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

string Gallery::textureFilter(ofImage image)
{
	Mat src, dst;
	int kernel_size = 31;

	src = toCv(image.getPixels()); // Load an image

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

int Gallery::objectTimesFilter(ofImage image, ofImage objImage) {
	int numberOfMatches = 0;
	ofImage  tempImg = image;
	tempImg.setImageType(OF_IMAGE_GRAYSCALE);
	Mat img1 = ofxCv::toCv(tempImg.getPixels());
	objImage.setImageType(OF_IMAGE_GRAYSCALE);
	Mat img2 = ofxCv::toCv(objImage.getPixels());
	if (!img1.empty() && !img2.empty())
	{
		if (img1.channels() != 1) {
			cvtColor(img1, img1, cv::COLOR_RGB2GRAY);
		}

		if (img2.channels() != 1) {
			cvtColor(img2, img2, cv::COLOR_RGB2GRAY);
		}
		vector<KeyPoint> keyP1;
		vector<KeyPoint> keyP2;
		Mat desc1;
		Mat desc2;
		vector<cv::DMatch> matches;
		Ptr<ORB> detector = ORB::create();
		detector->detectAndCompute(img1, Mat(), keyP1, desc1);
		detector->detectAndCompute(img2, Mat(), keyP2, desc2);
		matches.clear();
		BFMatcher bruteMatcher(cv::NORM_L2, true);
		bruteMatcher.match(desc1, desc2, matches, Mat());
		int k1s = keyP1.size();
		int k2s = keyP2.size();
		int ms = matches.size();
		numberOfMatches = ms / (k2s * 3 / 5);
	}
	return numberOfMatches;
}

double Gallery::rhythmFilter(string path)
{
	ofVideoPlayer videoAux;
	videoAux.load(path);

	Mat src_0 = toCv(video.getPixels());
	video.setPosition(0.25);
	Mat src_1 = toCv(video.getPixels());
	video.setPosition(0.5);
	Mat src_2 = toCv(video.getPixels());
	video.setPosition(0.75);
	Mat src_3 = toCv(video.getPixels());
	video.setPosition(1);
	Mat src_4 = toCv(video.getPixels());

	if (src_0.empty() || src_1.empty() || src_2.empty() || src_3.empty() || src_4.empty())
	{
		// something went wrong
		return -1;
	}

	Mat hsv_0, hsv_1, hsv_2, hsv_3, hsv_4;
	cvtColor(src_0, hsv_0, COLOR_BGR2HSV);
	cvtColor(src_1, hsv_1, COLOR_BGR2HSV);
	cvtColor(src_2, hsv_2, COLOR_BGR2HSV);
	cvtColor(src_3, hsv_3, COLOR_BGR2HSV);
	cvtColor(src_4, hsv_4, COLOR_BGR2HSV);

	Mat hsv_half_down = src_0(Range(hsv_0.rows / 2, hsv_0.rows), Range(0, hsv_0.cols));
	int h_bins = 50, s_bins = 60;
	int histSize[] = { h_bins, s_bins };

	// hue varies from 0 to 179, saturation from 0 to 255
	float h_ranges[] = { 0, 180 };
	float s_ranges[] = { 0, 256 };
	const float* ranges[] = { h_ranges, s_ranges };

	// Use the 0-th and 1-st channels
	int channels[] = { 0, 1 };

	Mat hist_0, hist_half_down, hist_1, hist_2, hist_3, hist_4;
	calcHist(&hsv_0, 1, channels, Mat(), hist_0, 2, histSize, ranges, true, false);
	normalize(hist_0, hist_0, 0, 1, NORM_MINMAX, -1, Mat());

	calcHist(&hsv_1, 1, channels, Mat(), hist_1, 2, histSize, ranges, true, false);
	normalize(hist_1, hist_1, 0, 1, NORM_MINMAX, -1, Mat());

	calcHist(&hsv_2, 1, channels, Mat(), hist_2, 2, histSize, ranges, true, false);
	normalize(hist_2, hist_2, 0, 1, NORM_MINMAX, -1, Mat());

	calcHist(&hsv_3, 1, channels, Mat(), hist_3, 2, histSize, ranges, true, false);
	normalize(hist_3, hist_3, 0, 1, NORM_MINMAX, -1, Mat());

	calcHist(&hsv_4, 1, channels, Mat(), hist_4, 2, histSize, ranges, true, false);
	normalize(hist_4, hist_4, 0, 1, NORM_MINMAX, -1, Mat());

	vector<Mat> histVector = { hist_0, hist_1, hist_2, hist_3, hist_4 };
	double rhythm = 0;
	for (int i = 1; i < histVector.size(); i++)
	{
		rhythm += compareHist(hist_0, histVector[i], i);
	}
	rhythm /= histVector.size();
	return rhythm;
}

void Gallery::changeItems(ofxDatGuiButtonEvent e)
{
	int index = e.target->getIndex();
	short inc = 1;
	if (index == 0) {
		if (currentItem < itemsSize - 3) {
			currentItem++;
			//currentItem %= itemsSize;
		}
	}
	else if (e.target->getIndex() == 1) {
		if (currentItem >= 1) {
			currentItem--;
			//currentItem %= itemsSize;
			inc = -1;
		}
	}

	ex1->setIndex(ex1->getIndex() + inc);
	ex2->setIndex(ex2->getIndex() + inc * 2);
	ex3->setIndex(ex3->getIndex() + inc * 3);

	im1->setIndex(im1->getIndex() + inc);
	im2->setIndex(im2->getIndex() + inc * 2);
	im3->setIndex(im3->getIndex() + inc * 3);
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
	ofImage auxImg = items[index]->getImage();
	ofImage object = ofImage();

	string tags = ofSystemTextBoxDialog("Number of tags", "1");
	int numberOfTags = stoi(tags);

	vector<string> listTags;
	listTags.assign(numberOfTags, string());

	for (int i = 0; i < numberOfTags; i++) {
		string tag = ofSystemTextBoxDialog("Tag " + ofToString(i + 1), "");
		listTags[i] = tag;
	}

	string times = ofSystemTextBoxDialog("Number of objects to process (times a specific object (input as an image) appears in the item):", "1");
	int numberTimes = stoi(times);
	map<string, int> mapTimes;

	for (int i = 0; i < numberTimes; i++) {
		ofFileDialogResult result = ofSystemLoadDialog("Load file", false, "/data/object_items/");

		if (result.bSuccess) {
			string path = result.getPath();
			object.load(path);
			// Number of times the object appears
			int numberOfTimes = objectTimesFilter(auxImg, object);
			mapTimes.insert({ result.fileName, numberOfTimes });
		}
		else {
			ofSystemTextBoxDialog("Error loading file...");
		}
	}

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

			int j = 0;
			itemsXML.pushTag("times");
			int numExTimes = itemsXML.getNumTags("time"); // number of existing times

			for (map<string, int>::iterator itr = mapTimes.begin(); itr != mapTimes.end(); ++itr) {
				itemsXML.addTag("time");
				itemsXML.pushTag("time", j + numExTimes);

				itemsXML.addValue("name", itr->first);
				itemsXML.addValue("numTime", itr->second);
				itemsXML.popTag(); // time

				j++;
			}
			itemsXML.popTag(); // times

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

