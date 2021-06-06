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

    //---- VIDEO PLAYER---

    void setupButtons();
    int toNavigate();
    int getSelectedId();
    void showUserPassInput(ofxDatGuiButtonEvent e);
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
