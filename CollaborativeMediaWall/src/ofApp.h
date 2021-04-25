#pragma once

#include "ofMain.h"
#include "gallery.h"
#include "shlwapi.h";

class ofApp : public ofBaseApp {

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

	// we will have a dynamic number of images, based on the content of a directory:
	ofDirectory dir;
	vector<ofImage> items;

	vector<ofVideoPlayer> video;
	int videoInt;
	int currentVideo;

	int currentItem;
};

