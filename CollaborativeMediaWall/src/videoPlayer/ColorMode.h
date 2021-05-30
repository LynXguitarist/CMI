#pragma once
#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxXmlSettings.h"
#include "ofxCvHaarFinder.h" 
#include "ofxDatGui.h"
#include "Item.h"


class ColorMode : public ofBaseApp
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

    void searchFunction(ofxDatGuiButtonEvent e);

    float cameraPosX;
    float cameraPosY;

    float camW;
    float camH;


    float cropSize;

    float searchedHue;

    float imageSize;


    ofVideoGrabber camera;
    ofxCvColorImage color;
    ofxCvGrayscaleImage grayscale;
    ofPixels pixelsCropped;
    ofTexture textureCropped;
    ofRectangle selectedArea;
    ofxDatGuiButton* searchButton;

    ofxXmlSettings user_itemsXML;
    ofxXmlSettings itemsXML;

    vector<Item*> items;
    vector<Item*> filteredItems;
    int currentItem;


};

