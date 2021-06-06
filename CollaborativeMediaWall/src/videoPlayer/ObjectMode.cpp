#include "ObjectMode.h"



using cv::Mat;
using cv::Point2f;
using cv::KeyPoint;
using cv::Scalar;
using cv::Ptr;
using cv::BFMatcher;
using cv::FastFeatureDetector;
using cv::SimpleBlobDetector;

void ObjectMode::setup(vector<Item*> thisItems)
{
    initXmlObjects();
    dir.listDir("object_items/");
    dir.sort();
    objectCounter = (int)dir.size();

    camW = 480;
    camH = 360;
    cropSize = camH / 2;
    searchedObject = "";
    items = thisItems;

    camera.setup(camW, camH);
    cameraPosX = ofGetViewportWidth() / 2 - camW / 2;
    cameraPosY = ofGetViewportHeight() / 2 - camH / 2;

    pixelsCropped.allocate(cropSize, cropSize, OF_PIXELS_RGB);
    textureCropped.allocate(pixelsCropped);

    selectedArea = ofRectangle(cameraPosX + (camW - cropSize) / 2, cameraPosY + (camH - cropSize) / 2, cropSize, cropSize);

    searchButton = new ofxDatGuiButton("Pick Object");
    searchButton->setPosition(ofGetViewportWidth() / 2 - 120, cameraPosY - 40);
    searchButton->setIndex(0);
    searchButton->setWidth(100);
    searchButton->onButtonEvent(this, (&ObjectMode::searchFunction));

    submitButton = new ofxDatGuiButton("Submit");
    submitButton->setPosition(ofGetViewportWidth() / 2 + 20, cameraPosY - 40);
    submitButton->setIndex(1);
    submitButton->setWidth(100);
    submitButton->onButtonEvent(this, (&ObjectMode::submitFunction));

    toReturn = false;

}

void ObjectMode::update()
{
    searchButton->update();
    submitButton->update();
    camera.update();
    if (camera.isFrameNew()) {
        ofPixels& pixels = camera.getPixels();
        pixels.cropTo(pixelsCropped, (camW - cropSize) / 2, (camH - cropSize) / 2, cropSize, cropSize);
        textureCropped.loadData(pixelsCropped);
    }
}

void ObjectMode::draw()
{
    searchButton->draw();
    ofSetColor(255);
    camera.draw(cameraPosX + camW, cameraPosY, -camW, camH);

    ofDrawLine(selectedArea.getBottomLeft(), selectedArea.getTopLeft());
    ofDrawLine(selectedArea.getTopLeft(), selectedArea.getTopRight());
    ofDrawLine(selectedArea.getTopRight(), selectedArea.getBottomRight());
    ofDrawLine(selectedArea.getBottomLeft(), selectedArea.getBottomRight());

    textureCropped.draw(cameraPosX, cameraPosY, -cropSize, cropSize);
    if (searchedObject != "") {
        submitButton->draw();
        string path = dir.getAbsolutePath()+"\\"+searchedObject;
        ofImage img = ofImage(path);
        img.draw(cameraPosX - cropSize, cameraPosY + cropSize, cropSize, cropSize);
    }
    

}

void ObjectMode::keyPressed(int key)
{
}

void ObjectMode::keyReleased(int key)
{
}

void ObjectMode::mouseMoved(int x, int y)
{
}

void ObjectMode::mouseDragged(int x, int y, int button)
{
}

void ObjectMode::mousePressed(int x, int y, int button)
{
}

void ObjectMode::mouseReleased(int x, int y, int button)
{
}

void ObjectMode::mouseEntered(int x, int y)
{
}

void ObjectMode::mouseExited(int x, int y)
{
}

void ObjectMode::windowResized(int w, int h)
{
}

void ObjectMode::dragEvent(ofDragInfo dragInfo)
{
}

void ObjectMode::gotMessage(ofMessage msg)
{
}

void ObjectMode::searchFunction(ofxDatGuiButtonEvent e)
{
    //string tempPath = dir.getAbsolutePath() + "/temp.jpg";

    //ofSaveImage(pixelsCropped,tempPath);

        
        ofxCvGrayscaleImage grayscale;
        ofxCvColorImage color;
        grayscale.allocate(cropSize, cropSize);
        color.allocate(cropSize, cropSize);
        color.setFromPixels(pixelsCropped);
        grayscale = color;
        Mat img1 = ofxCv::toCv(grayscale.getPixels());
        string matchName = "";
        int maxMatches = -1;
   
    for (int i = 0; i < (int)dir.size(); i++) {
        string path = dir.getPath(i);
        ofImage img = ofImage(path);
        img.setImageType(OF_IMAGE_GRAYSCALE);
        Mat img2 = ofxCv::toCv(img.getPixels());
        if (!img1.empty() && !img2.empty())
        {
            if (img1.channels() != 1) {
                cvtColor(img1, img1, cv::COLOR_RGB2GRAY);
            }

            if (img2.channels() != 1) {
                cvtColor(img2, img2, cv::COLOR_RGB2GRAY);
            }
            vector<KeyPoint> keyP1;
            vector<KeyPoint> keyP2;
            Mat desc1;
            Mat desc2;
            vector<cv::DMatch> matches;
            Ptr<ORB> detector = ORB::create();
            detector->detectAndCompute(img1, Mat(), keyP1, desc1);
            detector->detectAndCompute(img2, Mat(), keyP2, desc2);
            matches.clear();
            BFMatcher bruteMatcher(cv::NORM_L2, true);
            bruteMatcher.match(desc1, desc2, matches, Mat());
            int k1s = keyP1.size();
            int k2s = keyP2.size();
            int ms = matches.size();
            if (ms >= min(k1s, k2s) * 2 / 5) {
                if (ms > maxMatches) {
                    maxMatches = ms;
                    matchName = dir.getName(i);
                }
            }
        }

    }
    searchedObject = matchName;
    if(matchName.compare("")!=0){
    if (!filteredItems.empty()) filteredItems.clear();

    int numItems = itemsXML.getNumTags("item");

    string parsedName = matchName.substr(matchName.find('.'));
    for (int i = 0; i < numItems; i++) {
        itemsXML.pushTag("item", i);
        //string thisObject = itemsXML.getValue("times", "");
        string thisName = itemsXML.getValue("id", "");
        
        int j = 0;
        bool found = false;
        while (j < items.size() && !found) {
            string fileName = items[j]->getPath();
            int initialChar = fileName.find_last_of('\\') + 1;
            string itemName = fileName.substr(initialChar, fileName.find('.') - initialChar);
            if (itemName.compare(thisName) == 0) {
                found = true;
                int numObjs = itemsXML.getNumTags("time");
                itemsXML.pushTag("times");
                for (int k = 0; k < numObjs; k++) {
                    itemsXML.pushTag("time",k);
                    string objName = itemsXML.getValue("name","");
                    int objTimes = itemsXML.getValue("times", 0);
                    if (parsedName.compare(objName)&& objTimes>0) {
                        filteredItems.push_back(items[j]);
                    }
                }itemsXML.popTag();


            }
            j++;
            
        }itemsXML.popTag();
    }
    }
    
}

void ObjectMode::submitFunction(ofxDatGuiButtonEvent e)
{
    toReturn = true;
    camera.close();
}

vector<Item*> ObjectMode::getFilteredItems()
{
    return filteredItems;
}

bool ObjectMode::getToReturn()
{
    return toReturn;
}

void ObjectMode::initXmlObjects()
{
    // get xml for item
    if (itemsXML.loadFile("data_xml/items.xml")) {
        (void)ofLog(OF_LOG_NOTICE, "Open!");
    }
    else {
        (void)ofLog(OF_LOG_ERROR, "Didn't open!");
    }

    // get xml for user_items
    if (user_itemsXML.loadFile("data_xml/users_items.xml")) {
        (void)ofLog(OF_LOG_NOTICE, "Open!");
    }
    else {
        (void)ofLog(OF_LOG_ERROR, "Didn't open!");
    }
    // init objects, in order to not push the headers everytime
    itemsXML.pushTag("items");
    user_itemsXML.pushTag("users_items");
}

void ObjectMode::closeCam() {
    camera.close();
}

