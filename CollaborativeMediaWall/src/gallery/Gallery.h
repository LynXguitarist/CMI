#pragma once
#include "ofMain.h"
#include <windows.h>
#include "ofxXmlSettings.h"
#include "ofxDatGui.h"

#include "Item.h"

class Gallery : public ofBaseApp
{
public:
	void setup();
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

	void nextFrame();
	void generateMetadata(string itemName, ofImage image);
	void handleUserItems(int userId);
	void filterItems(string filter);
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
	bool isVideoPlaying;
	bool isMovingIcon;
	int videoPlaying;
	float currentFrame;

	//Xml object
	ofxXmlSettings user_itemsXML;
	ofxXmlSettings itemsXML;

	ofFileDialogResult windowFileSys;

	//GUI
	ofxDatGuiButton* ex1;
	ofxDatGuiButton* im1;

};

