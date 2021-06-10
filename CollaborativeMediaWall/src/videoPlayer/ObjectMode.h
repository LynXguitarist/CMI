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

    // inits xml files
    void initXmlObjects();
    // closes cam
    void closeCam();

    // processes the object finder function
    void searchFunction(ofxDatGuiButtonEvent e);
    // filters the items from an user/project based on the previous object finder
    void submitFunction(ofxDatGuiButtonEvent e);
    // gets the items
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
