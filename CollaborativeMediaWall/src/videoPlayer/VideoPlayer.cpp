#include "VideoPlayer.h"
// faceRecog here

void VideoPlayer::setup()
{
    camera.setup(320, 240);
    color.allocate(camera.getWidth(), camera.getHeight());
    grayscale.allocate(camera.getWidth(), camera.getHeight());
    haar.setup("data_xml/haarcascade_frontalface_default.xml");
    numberOfFaces = 0;
}

void VideoPlayer::update()
{
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