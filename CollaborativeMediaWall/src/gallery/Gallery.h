#pragma once
#include "ofMain.h"
#include <windows.h>
#include "ofxXmlSettings.h"
#include "ofxDatGui.h"
#include <ofxCvHaarFinder.h>
#include "ofxCv.h"

#include "Item.h"

using namespace cv;
using namespace ofxCv;

class Gallery : public ofBaseApp
{
public:
	vector<Item*> setup(int id, bool isUser, vector<Item*> items_input, bool useItemsInput);
	void update();
	void draw();

	void keyPressed(int key);
	void mousePressed(int x, int y, int button);

	void filterItems(string filter);
	void filterByColor(float hue);
	void toggleMovingIcon(bool isMovingIcon);
	void reset();

private:
	// init the buttons
	void initButtons();
	// returns the nextFrame for movieIcon
	void nextFrame();

	// init xml files
	void initXmlObjects();
	// handles items for the users
	void handleUserItems(int userId, vector<Item*> items_input, bool useItemsInput);
	// generates the metadata
	void generateMetadata(string itemName, string path, ofImage image, bool isVideo);
	// gets the path where the edge image was saved
	string edgesFilter(string itemName, ofImage image);
	// gets the path where the texture image was saved
	string textureFilter(string itemName, ofImage image);
	// gets the number of times an object appears in an item
	int objectTimesFilter(ofImage image, ofImage objImage);
	// gets the rhythm
	double rhythmFilter(string path);

	//----------------------Events---------------//
	// change the items being previewed
	void changeItems(ofxDatGuiButtonEvent e);
	// opens item in windows media player
	void openInWMP(ofxDatGuiButtonEvent e);
	// exports metadata
	void extractMetadata(ofxDatGuiButtonEvent e);
	// imports tags and number of times an object appears in an item
	void importMetadata(ofxDatGuiButtonEvent e);


	int imageSize = (ofGetViewportWidth() - 200) / 3;

	// Array of items(path)
	ofDirectory dir;
	ofDirectory objDir;
	vector<Item*> items; // contains the filtered items
	vector<Item*> auxItems; // contains all the items
	int itemsSize;

	// Current item
	int currentItem;

	// Image object
	ofImage image;

	// VideoPlayer object
	ofVideoPlayer video;
	ofxCvHaarFinder finder;
	bool isMovingIcon;
	bool isVideoPlaying;
	int videoPlaying;
	float currentFrame;

	// Xml object
	ofxXmlSettings user_itemsXML;
	ofxXmlSettings itemsXML;
	ofxXmlSettings projectsXml;

	// window explorer
	ofFileDialogResult windowFileSys;

	// GUI
	ofxDatGuiButton* openWMB1;
	ofxDatGuiButton* openWMB2;
	ofxDatGuiButton* openWMB3;

	ofxDatGuiButton* ex1;
	ofxDatGuiButton* ex2;
	ofxDatGuiButton* ex3;

	ofxDatGuiButton* im1;
	ofxDatGuiButton* im2;
	ofxDatGuiButton* im3;

	ofxDatGuiButton* nextButton;
	ofxDatGuiButton* previousButton;
};

