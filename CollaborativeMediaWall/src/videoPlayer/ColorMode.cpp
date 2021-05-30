#include "ColorMode.h"

void ColorMode::setup(vector<Item*> thisItems)
{
    initXmlObjects();

    camW = 320;
    camH = 240;
    cropSize = camH/2;
    searchedHue = -1;
    items = thisItems;
    currentItem = 0;
    imageSize = (ofGetViewportWidth() - 200) / 3;
 

    camera.initGrabber(camW, camH);
    cameraPosX = ofGetViewportWidth() -  camW;
    cameraPosY = ofGetViewportHeight() - camH;
    color.allocate(camW, camH);
    grayscale.allocate(camW, camH);

    pixelsCropped.allocate(120, 120 , OF_PIXELS_RGB);
    textureCropped.allocate(pixelsCropped);

    selectedArea = ofRectangle(cameraPosX+(camW-cropSize)/2, cameraPosY+(camH - cropSize) / 2,cropSize,cropSize);
    
    searchButton = new ofxDatGuiButton("Search");
    searchButton-> setPosition(cameraPosX, cameraPosY-40);
    searchButton->setIndex(0);
    searchButton->setWidth(100);
    searchButton->onButtonEvent(this, (&ColorMode::searchFunction));
}

void ColorMode::update()
{
    searchButton->update();
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
    camera.draw(cameraPosX, cameraPosY);
    
    ofDrawLine(selectedArea.getBottomLeft(), selectedArea.getTopLeft());
    ofDrawLine(selectedArea.getTopLeft(), selectedArea.getTopRight());
    ofDrawLine(selectedArea.getTopRight(), selectedArea.getBottomRight());
    ofDrawLine(selectedArea.getBottomLeft(), selectedArea.getBottomRight());

    textureCropped.draw(cameraPosX-cropSize,cameraPosY);


    if (searchedHue != -1) {
    float itemPosX= 0;
    float itemPosY = 50;
    int itemsSize = filteredItems.size();
    int displayRange = currentItem + 3;
    if (displayRange > itemsSize)
        displayRange = itemsSize;

    for (int i = currentItem; i < displayRange; i++) {
        string path = filteredItems[i]->getPath();
        //bool isCurrVideoPlaying = items[i]->getIsVideoPlaying();
        int position = itemPosX * imageSize + (itemPosX + 1) * 50;

        /*      if (isCurrVideoPlaying && !isMovingIcon) {
                  if (video.getCurrentFrame() >= 50)
                      video.firstFrame();

                  video.draw(position, 50 + y, imageSize, imageSize);
              }
              else {
                  if (isMovingIcon && isCurrVideoPlaying) {
                      video.setPosition(currentFrame);
                      image.setFromPixels(video.getPixels());
                      nextFrame();
                      // waits x secs
                      Sleep(800);
                  }
                  */



        ofImage image = items[i]->getImage(); // init image 
        image.draw(position, 50 + itemPosY, imageSize, imageSize);
        itemPosX++;
    }
    }

    if(searchedHue!=-1){
        ofColor searchColor;
        searchColor.setHsb(searchedHue,255,255);        
        ofSetColor(searchColor);
        ofDrawRectangle(cameraPosX-cropSize, cameraPosY+cropSize, cropSize, cropSize);
    }

   
}

void ColorMode::keyPressed(int key)
{
    //short inc = 1;
    if (GetKeyState(VK_RIGHT)) {
        if (currentItem < items.size() - 3) {
            currentItem++;
            //currentItem %= itemsSize;
        }
    }
    else if (GetKeyState(VK_LEFT)) {
        if (currentItem >= 1) {
            currentItem--;
            //currentItem %= itemsSize;
         //   inc = -1;
        }
    }
   /* else if (GetKeyState(VK_SPACE)) {
        if (video.isLoaded()) {
            video.setPaused(!video.isPaused());
        }
    }
    */
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
  // searchedHue = 0;
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

