#pragma once
#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxXmlSettings.h"
#include "ofxCvHaarFinder.h" 
#include "ofxDatGui.h"
#include "Item.h"


class ColorMode : public ofBaseApp
{
public:
	void setup(vector<Item*> thisItems);
	void update();
	void draw();

	// inits the xml files
	void initXmlObjects();
	// closes the cam
	void closeCam();

	// processes the color finder function
	void searchFunction(ofxDatGuiButtonEvent e);
	// filters the items from an user/project based on the previous color finder
	void submitFunction(ofxDatGuiButtonEvent e);
	// gets the items
	vector<Item*> getFilteredItems();
	bool getToReturn();

	float cameraPosX;
	float cameraPosY;

	float camW;
	float camH;


	float cropSize;

	float searchedHue;



	ofVideoGrabber camera;
	ofxCvColorImage color;
	ofxCvGrayscaleImage grayscale;
	ofPixels pixelsCropped;
	ofTexture textureCropped;
	ofRectangle selectedArea;
	ofxDatGuiButton* searchButton;
	ofxDatGuiButton* submitButton;

	ofxXmlSettings user_itemsXML;
	ofxXmlSettings itemsXML;

	vector<Item*> items;
	vector<Item*> filteredItems;

	bool toReturn;


};

