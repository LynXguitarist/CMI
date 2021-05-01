#pragma once
#include "ofMain.h"
#include "ofxXmlSettings.h"

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

	void handleItems();

	// Array of items(path)
	ofDirectory dir;
	vector<string> items;

	// Current item
	int currentItem;

	// Image object
	ofImage image;

	// VideoPlayer object
	ofVideoPlayer video;
	bool isVideoPlaying;

	//Xml object
	ofxXmlSettings document;
};

