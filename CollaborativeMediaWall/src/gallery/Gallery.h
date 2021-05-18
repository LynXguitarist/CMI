#pragma once
#include "ofMain.h"
#include <windows.h>
#include "ofxXmlSettings.h"
#include "ofxDatGui.h"
#include <ofxCvHaarFinder.h>

#include "Item.h"

using namespace cv;

class Gallery : public ofBaseApp
{
public:
	void setup(int id);
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	void filterItems(string filter);
	void filterByColor(float hue);
	void toggleMovingIcon(bool isMovingIcon);

private:
	void nextFrame();

	void initXmlObjects();
	void handleUserItems(int userId);
	void generateMetadata(string itemName, ofImage image);
	string filter2DAux(string itemName);

	void extractMetadata(ofxDatGuiButtonEvent e);
	void importMetadata(ofxDatGuiButtonEvent e);
	

	int imageSize = (ofGetViewportWidth() - 200) / 3;

	// Array of items(path)
	ofDirectory dir;
	vector<Item*> items; // constains the filtered items
	vector<Item*> auxItems; // constains all the items
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

	// window explorer
	ofFileDialogResult windowFileSys;

	// GUI
	ofxDatGuiButton* ex1;
	ofxDatGuiButton* ex2;
	ofxDatGuiButton* ex3;
	
	ofxDatGuiButton* im1;
	ofxDatGuiButton* im2;
	ofxDatGuiButton* im3;
};

