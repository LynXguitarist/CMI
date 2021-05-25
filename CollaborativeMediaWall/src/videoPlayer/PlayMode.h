#pragma once
#include "ofMain.h"
#include "ofxCvHaarFinder.h" 
#include "ofxXmlSettings.h"
#include "ofxDatGui.h"
#include "Item.h"

class PlayMode : public ofBaseApp
{
public:
	void setup(vector<Item*> items);
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

	// Change currentItem
	void changeCurrentItem(bool isNext);
	void onButtonEvent(ofxDatGuiButtonEvent e);
	void onToggleEvent(ofxDatGuiToggleEvent e);

	ofVideoGrabber camera;
	ofxCvColorImage			colorImg;

	ofxCvGrayscaleImage 	grayImage;
	ofxCvGrayscaleImage 	grayBg;
	ofxCvGrayscaleImage 	grayDiff;
	ofxCvContourFinder 	contourFinder;

	int 				threshold;
	bool				bLearnBakground;

	float x;
	float y;

	// Modes - Areas
	ofRectangle stop; // same for playing, when video is stoped, it will play, when is playing, it will stop
	ofRectangle next; // changes to the next item
	ofRectangle previous; // changes to the previous item
	// UI - needed because the pos is relative to the camera not the window
	ofRectangle stopUI;
	ofRectangle nextUI;
	ofRectangle previousUI;

	ofColor previousColor = ofColor::white;
	ofColor nextColor = ofColor::white;
	ofColor stopColor = ofColor::white;

	ofxDatGuiButton* previousButton;
	ofxDatGuiButton* nextButton;
	ofxDatGuiButton* stopButton;

	ofxDatGuiToggle* isGestureModeButton;
	bool isGestureMode = true;

	// Items
	vector<Item*> items;
	int itemsSize;
	int currentItem;
	ofVideoPlayer video;
	ofImage image;

	// Timer - time that each item will preview before changing to the next
	float time;
	short diffTime = 5;
	float previousTime = 0;
	float nextTime = 0;
	float stopTime = 0;
};

