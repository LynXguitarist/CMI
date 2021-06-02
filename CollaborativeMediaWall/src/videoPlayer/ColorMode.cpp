#include "ColorMode.h"

void ColorMode::setup(vector<Item*> thisItems)
{
    initXmlObjects();

    camW = 480;
    camH = 360;
    cropSize = camH/2;
    searchedHue = -1;
    items = thisItems;

    camera.setup(camW, camH);
    cameraPosX = ofGetViewportWidth()/2 -  camW/2;
    cameraPosY = ofGetViewportHeight()/2 - camH/2;
    color.allocate(camW, camH);
    grayscale.allocate(camW, camH);

    pixelsCropped.allocate(cropSize, cropSize , OF_PIXELS_RGB);
    textureCropped.allocate(pixelsCropped);

    selectedArea = ofRectangle(cameraPosX+(camW-cropSize)/2, cameraPosY+(camH - cropSize) / 2,cropSize,cropSize);
    
    searchButton = new ofxDatGuiButton("Pick Color");
    searchButton-> setPosition(ofGetViewportWidth() / 2-120, cameraPosY-40);
    searchButton->setIndex(0);
    searchButton->setWidth(100);
    searchButton->onButtonEvent(this, (&ColorMode::searchFunction));

    submitButton = new ofxDatGuiButton("Submit");
    submitButton->setPosition(ofGetViewportWidth() / 2 +20, cameraPosY - 40);
    submitButton->setIndex(1);
    submitButton->setWidth(100);
    submitButton->onButtonEvent(this, (&ColorMode::submitFunction));

    toReturn = false;

}

void ColorMode::update()
{
    searchButton->update();
    submitButton->update();
    camera.update();
    if (camera.isFrameNew()) {
        ofPixels& pixels = camera.getPixels();
        pixels.cropTo(pixelsCropped,(camW-cropSize)/2, (camH - cropSize) / 2,cropSize,cropSize);
        textureCropped.loadData(pixelsCropped);
    }
}

void ColorMode::draw()
{
    searchButton->draw();
    ofSetColor(255);
    camera.draw(cameraPosX+camW, cameraPosY,-camW,camH);
    
    ofDrawLine(selectedArea.getBottomLeft(), selectedArea.getTopLeft());
    ofDrawLine(selectedArea.getTopLeft(), selectedArea.getTopRight());
    ofDrawLine(selectedArea.getTopRight(), selectedArea.getBottomRight());
    ofDrawLine(selectedArea.getBottomLeft(), selectedArea.getBottomRight());

    textureCropped.draw(cameraPosX,cameraPosY, -cropSize, cropSize);
    if(searchedHue!=-1){
        submitButton->draw();
        ofColor searchColor;
        searchColor.setHsb(searchedHue,255,255);        
        ofSetColor(searchColor);
        ofDrawRectangle(cameraPosX-cropSize, cameraPosY+cropSize, cropSize, cropSize);
    }
    ofSetColor(255);
   
}

void ColorMode::keyPressed(int key)
{
}

void ColorMode::keyReleased(int key)
{
}

void ColorMode::mouseMoved(int x, int y)
{
}

void ColorMode::mouseDragged(int x, int y, int button)
{
}

void ColorMode::mousePressed(int x, int y, int button)
{
}

void ColorMode::mouseReleased(int x, int y, int button)
{
}

void ColorMode::mouseEntered(int x, int y)
{
}

void ColorMode::mouseExited(int x, int y)
{
}

void ColorMode::windowResized(int w, int h)
{
}

void ColorMode::dragEvent(ofDragInfo dragInfo)
{
}

void ColorMode::gotMessage(ofMessage msg)
{
}

void ColorMode::searchFunction(ofxDatGuiButtonEvent e)
{
    float redSum = 0;
    float greenSum = 0;
    float blueSum = 0;

    for (int x = 0; x < cropSize; x++) {
        for(int y=0; y<cropSize;y++){
            ofColor thisColor = pixelsCropped.getColor(x,y);
            redSum += thisColor.r;
            greenSum += thisColor.g;
            blueSum += thisColor.b;
        }   
    }
    redSum /= (cropSize * cropSize);
    greenSum /= (cropSize * cropSize);
    blueSum /= (cropSize * cropSize);
    ofColor newColor;
    newColor.r = redSum;
    newColor.g = greenSum;
    newColor.b = blueSum;
    searchedHue = newColor.getHue();
    if(!filteredItems.empty()) filteredItems.clear();
    int numItems = itemsXML.getNumTags("item");

    

    for (int i = 0; i < numItems; i++) {
        itemsXML.pushTag("item", i);
        float thisHue = itemsXML.getValue("color",0);
        string thisName = itemsXML.getValue("id","");
        itemsXML.popTag();
        int j = 0;
        bool found = false;
        while (j < items.size()&&!found) {
            string fileName = items[j]->getPath();
            int initialChar = fileName.find_last_of('\\')+1;
            string itemName = fileName.substr(initialChar, fileName.find('.')-initialChar);
            if (itemName.compare(thisName) == 0) {
                found = true;
                    if (abs(searchedHue - thisHue) <= 10 || abs(searchedHue - thisHue) >= 245) {
                        filteredItems.push_back(items[j]);
                    }
                
            }
            j++;
        }
        
    }
}

void ColorMode::submitFunction(ofxDatGuiButtonEvent e)
{
    toReturn = true;
    camera.close();
}

vector<Item*> ColorMode::getFilteredItems()
{
    return filteredItems;
}

bool ColorMode::getToReturn()
{
    return toReturn;
}

void ColorMode::initXmlObjects()
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

void ColorMode::closeCam() {
    camera.close();
}

