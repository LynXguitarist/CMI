#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(ofColor::grey);
	// voltar a meter a 0
	view = 0;

	gui = new ofxDatGui(0, 0);
	menu();

	switch (view) {
	case 0:
		videoPlayer.setup();
		break;
	case 1:
		galleryUI();
		items = gallery.setup(1);
		break;
	case 4:
		playMode.setup(items);
		break;
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	//View id: VideoPlayer=0; Gallery=1;
	switch (view) {
	case 0:
		videoPlayer.update();
		if (videoPlayer.toNavigate() == 1) {
			galleryUI();
			items = gallery.setup(videoPlayer.getSelectedId());
			view = 1;
		}
		
		break;
	case 1:
		search->update();
		isMovingIcon->update();
		colorPicker->update();
		reset->update();
		objectMode->update();
		colorMode->update();
		playModeB->update();
		gallery.update();
		break;
	case 4:
		playMode.update();
		break;
	}

}

//--------------------------------------------------------------
void ofApp::draw() {

	//View id: VideoPlayer=0; Gallery=1;

	switch (view) {
	case 0:
		videoPlayer.draw();
		break;
	case 1:
		search->draw();
		isMovingIcon->draw();
		colorPicker->draw();
		reset->draw();
		objectMode->draw();
		colorMode->draw();
		playModeB->draw();
		gallery.draw();
		break;
	case 4:
		playMode.draw();
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (view) {
	case 0:
		gallery.keyPressed(key);
		break;
	case 1:
		break;
	case 4:
		playMode.keyPressed(key);
		break;
	}
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
	switch (view) {
	case 0:
		gallery.mousePressed(x, y, button);
		break;
	case 1:
		break;
	}
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
	search = new ofxDatGuiTextInput("Pesquisa por:", "");
	search->setPosition(0, gui->getHeader()->getHeight());
	search->setWidth(ofGetViewportWidth() / 2, 100);
	search->onTextInputEvent(this, &ofApp::onTextInputEvent);

	// Toggle_isMovingIcon
	isMovingIcon = new ofxDatGuiToggle("Display moving icon?");
	isMovingIcon->setPosition(search->getWidth(), gui->getHeader()->getHeight());
	isMovingIcon->setWidth(150);
	isMovingIcon->onToggleEvent(this, &ofApp::onToggleEvent);

	// Color_Picker
	colorPicker = new ofxDatGuiColorPicker("Filter by color", ofColor::black);
	colorPicker->setPosition(search->getWidth() + isMovingIcon->getWidth(), gui->getHeader()->getHeight());
	colorPicker->onColorPickerEvent(this, &ofApp::onColorPickerEvent);

	// Reset filters
	reset = new ofxDatGuiButton("Reset");
	reset->setIndex(3);
	reset->setPosition(search->getWidth() + isMovingIcon->getWidth() + colorPicker->getWidth(), gui->getHeader()->getHeight());
	reset->onButtonEvent(this, &ofApp::onButtonEvent);

	// Modes
	objectMode = new ofxDatGuiButton("Object Mode");
	objectMode->setIndex(0);
	objectMode->setPosition(0, 50);
	objectMode->setWidth(100);
	objectMode->onButtonEvent(this, &ofApp::onButtonEvent);

	colorMode = new ofxDatGuiButton("Color Mode");
	colorMode->setIndex(1);
	colorMode->setPosition(objectMode->getWidth(), 50);
	colorMode->setWidth(100);
	colorMode->onButtonEvent(this, &ofApp::onButtonEvent);

	playModeB = new ofxDatGuiButton("Play Mode");
	playModeB->setIndex(2);
	playModeB->setPosition(objectMode->getWidth() + colorMode->getWidth(), 50);
	playModeB->setWidth(100);
	playModeB->onButtonEvent(this, &ofApp::onButtonEvent);

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

//-----------------------Navigate--------------------------//
void ofApp::navigateToGallery(int id) {
	gallery.setup(id);
	view = 1;
}

//-----------------------Events----------------------------//
void ofApp::onButtonEvent(ofxDatGuiButtonEvent e) {
	int index = e.target->getIndex();
	switch (index) {
	case 0: // objectMode
		view = 2;
		break;
	case 1: // colorMode
		view = 3;
		break;
	case 2: // playMode
		view = 4;
		playMode.setup(items);
		break;
	case 3: // reset filters
		gallery.reset();
		colorPicker->setColor(0);
		search->setText("");
		break;
	}
}

