#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(ofColor::grey);
	view = 0;

	gui = new ofxDatGui(0, 0);
	menu();
	
	/* tudo gallery
	galleryUI();
	gallery.setup(1);
	*/
	videoPlayer.setup();
}

//--------------------------------------------------------------
void ofApp::update() {

	//View id: VideoPlayer=0; Gallery=1;
	
	switch (view) {
	case 0:videoPlayer.update();
		break;
	case 1:
		isMovingIcon->update();
		colorPicker->update();
		gallery.update();
		break;
	}
	
}

//--------------------------------------------------------------
void ofApp::draw() {
	
	//View id: VideoPlayer=0; Gallery=1;

	switch (view) {
	case 0:videoPlayer.draw();
		break;
	case 1:
		isMovingIcon->draw();
		colorPicker->draw();
		gallery.draw();
		break;
	}
	videoPlayer.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	//gallery.keyPressed(key);
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
	//gallery.mousePressed(x, y, button);
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
}

void ofApp::galleryUI() {
	// Search_input
	ofxDatGuiTextInput* search = gui->addTextInput("Pesquisa por:", "");
	search->onTextInputEvent(this, &ofApp::onTextInputEvent);
	search->setWidth(ofGetViewportWidth() / 2, 100);

	// Toggle_isMovingIcon
	isMovingIcon = new ofxDatGuiToggle("Display moving icon?");
	isMovingIcon->setPosition(search->getWidth(), gui->getHeader()->getHeight());
	isMovingIcon->setWidth(150);
	isMovingIcon->onToggleEvent(this, &ofApp::onToggleEvent);

	// Color_Picker
	colorPicker = new ofxDatGuiColorPicker("Filter by color", ofColor::black);
	colorPicker->setPosition(search->getWidth() + isMovingIcon->getWidth(), gui->getHeader()->getHeight());
	colorPicker->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
}

//------------------------Events-----------------------------//
void ofApp::onTextInputEvent(ofxDatGuiTextInputEvent e)
{
	cout << "the input field changed to: " << e.text << endl;
	gallery.filterItems(e.text);
}

void ofApp::onToggleEvent(ofxDatGuiToggleEvent e)
{
	// checks or unckecks the button
	isMovingIcon->toggle();
	isMovingIcon->setChecked(!isMovingIcon->getChecked());
	// change the bool in gallery
	gallery.toggleMovingIcon(isMovingIcon->getChecked());
}

void ofApp::onColorPickerEvent(ofxDatGuiColorPickerEvent e) {
	float hue = e.color.getHue();
	gallery.filterByColor(hue);

}

void ofApp::navigateToGallery(int id) {
	
	gallery.setup(id);
	view = 1;
}


