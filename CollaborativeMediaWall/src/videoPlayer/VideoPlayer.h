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
    void showPassInput(ofxDatGuiButtonEvent e);
    void setNavigation(ofxDatGuiButtonEvent e);
    void onTextInputEvent(ofxDatGuiTextInputEvent e);

    ofVideoGrabber camera;
    ofxCvColorImage color;
    ofxCvGrayscaleImage grayscale;
    ofxCvHaarFinder haar;

    int numberOfFaces;
    int selectedId; // user/project id
    int navigate;

    bool showPasswordInput = false;
    bool showConfirm = false;

    string password; // saves the input text
    string userProjectPass; // aux var to save password

    ofxXmlSettings usersXML;

    vector<ofxDatGuiButton*> names;
    ofxDatGuiTextInput* passwordInput;
    ofxDatGuiButton* confirmButton;

};
