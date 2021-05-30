#pragma once
#include "ofMain.h"
#include "ofxDatGui.h"
#include "Gallery.h"
#include "VideoPlayer.h"
#include "PlayMode.h"
#include "ColorMode.h"
#include "Item.h"

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

	//------------------------GUI_Functions----------------------//
	void menu();
	void galleryUI();
	void navigateToGallery(int id);

	//------------------------Events-----------------------------//
	void ofApp::onButtonEvent(ofxDatGuiButtonEvent e);
	void onTextInputEvent(ofxDatGuiTextInputEvent e);
	void onToggleEvent(ofxDatGuiToggleEvent e);
	void onColorPickerEvent(ofxDatGuiColorPickerEvent e);

	bool isUser = true;
	vector<Item*> items;

	// Addons
	ofxDatGui* gui;
	ofxDatGuiTextInput* search;
	ofxDatGuiToggle* isMovingIcon;
	ofxDatGuiColorPicker* colorPicker;
	ofxDatGuiButton* galleryNavigate;
	ofxDatGuiButton* reset;

	ofxDatGuiButton* objectModeB;
	ofxDatGuiButton* colorModeB;
	ofxDatGuiButton* playModeB;

	//------------------------Auxiliar_classes-------------------//
	Gallery gallery;
	VideoPlayer videoPlayer;
	PlayMode playMode;
	ColorMode colorMode;
	int view;
};