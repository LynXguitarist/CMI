#pragma once
#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxXmlSettings.h"
#include "ofxCvHaarFinder.h" 
#include "ofxDatGui.h"
#include <ofxCv.h>
#include "Item.h"



using cv::ORB;

class ObjectMode : public ofBaseApp
{
public:
    void setup(vector<Item*> thisItems);
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
    void initXmlObjects();
    void closeCam();

    void searchFunction(ofxDatGuiButtonEvent e);
    void submitFunction(ofxDatGuiButtonEvent e);
    vector<Item*> getFilteredItems();
    bool getToReturn();

    float cameraPosX;
    float cameraPosY;

    float camW;
    float camH;


    float cropSize;

    string searchedObject;

    ofDirectory dir;
    int objectCounter;

    ofVideoGrabber camera;
    
    ofPixels pixelsCropped;
    ofTexture textureCropped;
    ofRectangle selectedArea;
    ofxDatGuiButton* searchButton;
    ofxDatGuiButton* submitButton;

    ofxXmlSettings user_itemsXML;
    ofxXmlSettings itemsXML;

    vector<Item*> items;
    vector<Item*> filteredItems;

    bool toReturn;


};
