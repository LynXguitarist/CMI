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

    ofVideoGrabber camera;
    ofxCvColorImage			colorImg;

    ofxCvGrayscaleImage 	grayImage;
    ofxCvGrayscaleImage 	grayBg;
    ofxCvGrayscaleImage 	grayDiff;
    ofxCvContourFinder 	contourFinder;

    int 				threshold;
    bool				bLearnBakground;

    // Modes
    int stopX; // same for playing, when video is stoped, it will play
    int stopY; // when is playing, it will stop
    int nextX; // changes to the next item
    int nextY;
    int previousX; // changes to the previous item
    int previousY;

    // Items
    vector<Item*> items;
    int itemsSize;
    int currentItem;
    ofVideoPlayer video;
    ofImage image;
};

