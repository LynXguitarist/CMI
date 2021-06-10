#pragma once
#include "ofMain.h"
#include "ofxCvHaarFinder.h" 
#include "ofxXmlSettings.h"
#include "ofxDatGui.h"

class VideoPlayer : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();

    //---- VIDEO PLAYER---
    // init buttons
    void setupButtons();
    // navigate to gallery
    int toNavigate();
    // gets the user/project selected
    int getSelectedId();
    // displays password input
    void showUserPassInput(ofxDatGuiButtonEvent e);
    // displays password input
    void showProjectPassInput(ofxDatGuiButtonEvent e);
    void setNavigation(ofxDatGuiButtonEvent e);
    void onTextInputEvent(ofxDatGuiTextInputEvent e);
    void onShiftEvent(ofxDatGuiButtonEvent e);
    void shiftButtons(bool isProj, int shift);
    bool isNavigateProject();

    ofVideoGrabber camera;
    ofxCvColorImage color;
    ofxCvGrayscaleImage grayscale;
    ofxCvHaarFinder haar;

    int numberOfFaces;
    int selectedId; // user/project id
    int navigate;

    float initialY;

    bool showPasswordInput = false;
    bool showConfirm = false;
    bool isProject;
    int currentUserDisplay;
    int currentProjectDisplay;

    string password; // saves the input text
    string userProjectPass; // aux var to save password

    ofxXmlSettings usersXML;
    ofxXmlSettings projXML;

    ofxDatGuiLabel* namesHeader;
    ofxDatGuiLabel* projHeader;

    ofxDatGuiButton* namesUp;
    ofxDatGuiButton* projUp;

    ofxDatGuiButton* namesDown;
    ofxDatGuiButton* projDown;

    vector<ofxDatGuiButton*> names;
    vector<ofxDatGuiButton*> projects;
    vector<string> namesArray;
    vector<string> projectsArray;
    ofxDatGuiTextInput* passwordInput;
    ofxDatGuiButton* confirmButton;

};
