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
	vector<Item*> setup(int id, bool isUser, vector<Item*> items_input);
	void update();
	void draw();

	void keyPressed(int key);
	void mousePressed(int x, int y, int button);

	void filterItems(string filter);
	void filterByColor(float hue);
	void toggleMovingIcon(bool isMovingIcon);
	void reset();

private:
	void initButtons();
	void nextFrame();

	void initXmlObjects();
	bool hasItemMetadata(string itemName);
	void handleUserItems(int userId, vector<Item*> items_input);
	void generateMetadata(string itemName, string path, ofImage image, bool isVideo);
	string edgesFilter(string itemName, ofImage image);
	string textureFilter(string itemName);
	string rhythmFilter(string path);

	//----------------------Events---------------//
	void openInWMP(ofxDatGuiButtonEvent e);
	void extractMetadata(ofxDatGuiButtonEvent e);
	void importMetadata(ofxDatGuiButtonEvent e);


	int imageSize = (ofGetViewportWidth() - 200) / 3;

	// Array of items(path)
	ofDirectory dir;
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
};

