#include "VideoPlayer.h"

void VideoPlayer::setup()
{
    camera.setup(320, 240);
    color.allocate(camera.getWidth(), camera.getHeight());
    grayscale.allocate(camera.getWidth(), camera.getHeight());
    haar.setup("data_xml/haarcascade_frontalface_default.xml");
    numberOfFaces = 0;
    navigate = -1;
    selectedId = -1;
    usersXML.loadFile("data_xml/users.xml");
    usersXML.pushTag("users");
    setupButtons();

}

void VideoPlayer::update()
{
    if (numberOfFaces > 0) {
        for (int i = 0; i < names.size(); i++) {
            names[i]->update();
        }
    }
    camera.update();

    if (camera.isFrameNew()) {
        color.setFromPixels(camera.getPixels());
        grayscale = color;
        haar.findHaarObjects(grayscale);
        numberOfFaces = haar.blobs.size();
    }
}

void VideoPlayer::draw()
{
    color.draw(0, 0);

    for (int i = 0; i < haar.blobs.size(); i++) {
        ofSetColor(255);
        ofNoFill();
        ofDrawRectangle(haar.blobs[i].boundingRect);
    }
    if (numberOfFaces > 0) {
        for (int i = 0; i < names.size(); i++) {
            names[i]->draw();
        }
    }

}

void VideoPlayer::keyPressed(int key)
{

}
void VideoPlayer::keyReleased(int key) {

}

//--------------------------------------------------------------
void VideoPlayer::mouseMoved(int x, int y) {



}

//--------------------------------------------------------------
void VideoPlayer::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void VideoPlayer::mousePressed(int x, int y, int button) {


}

//--------------------------------------------------------------
void VideoPlayer::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void VideoPlayer::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void VideoPlayer::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void VideoPlayer::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void VideoPlayer::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void VideoPlayer::dragEvent(ofDragInfo dragInfo) {

}

void VideoPlayer::setupButtons() {
    
    int usersCount = usersXML.getNumTags("user");
    names.assign(usersCount, new ofxDatGuiButton(""));
    for (int i = 0; i < usersCount; i++) {
        usersXML.pushTag("user", i);
        names[i] = new ofxDatGuiButton(usersXML.getValue("name", ""));
        names[i]->setPosition(ofGetWidth() / 2, 30+names[i]->getHeight()*i);
        names[i]->setIndex(usersXML.getValue("id", 0));
        names[i]->setWidth(100);
        names[i]->onButtonEvent(this, (&VideoPlayer::setNavigation));
    }

}

void VideoPlayer::setNavigation(ofxDatGuiButtonEvent e) {

    selectedId = e.target->getIndex();
    camera.close();
    navigate = 1;
}

int VideoPlayer::toNavigate() {
    return navigate;
}

int VideoPlayer::getSelectedId() {
    return selectedId;
}