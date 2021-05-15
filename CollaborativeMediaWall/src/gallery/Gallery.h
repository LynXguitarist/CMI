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
	void handleUserItems(int userId);
	void filterItems(string filter);
	void extractMetadata(ofxDatGuiButtonEvent e);

	int imageSize = (ofGetViewportWidth() - 200) / 3;

	// Array of items(path)
	ofDirectory dir;
	vector<Item*> items;
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
	ofxXmlSettings document;

	//GUI
	ofxDatGuiButton* ex1;

};

