#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	gallery.setup();

	gui = new ofxDatGui(0, 0);
	galleryScroll = new ofxDatGuiScrollView("gallery", 10);

	menu();
}

//--------------------------------------------------------------
void ofApp::update() {
	gallery.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	gallery.draw();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	gallery.keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}


//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

//------------------------GUI_Functions----------------------//
void ofApp::menu() {
	gui->addHeader("Collaborative Media Wall")->setWidth(ofGetViewportWidth());
	gui->getHeader()->setDraggable(false);
	
	/*
	gui->addButton("Items")->setWidth(ofGetViewportWidth() / 2);
	gui->addButton("Project")->setPosition(gui->getButton("Items", "")->getWidth(), gui->getHeader()->getHeight());
	gui->getButton("Project")->setWidth(ofGetViewportWidth() / 2);
	*/
}

void ofApp::galleryUI() {
	
}