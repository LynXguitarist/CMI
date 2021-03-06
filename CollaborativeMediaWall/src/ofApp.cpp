#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetWindowTitle("Collaborative MediaWall - Francisco and Frederico");

	// makes the logs log in console
	ofLogToConsole();

	ofBackground(ofColor::grey);
	// voltar a meter a 0
	view = 0;
	currentUserId = -1;
	isUser = true;

	gui = new ofxDatGui(0, 0);
	menu();

	switch (view) {
	case 0:
		videoPlayer.setup();
		break;
	case 1:
		items = gallery.setup(videoPlayer.getSelectedId(), isUser, vector<Item*>(),false);
		galleryUI();
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
			currentUserId = videoPlayer.getSelectedId();
			isUser = !videoPlayer.isNavigateProject();
			items = gallery.setup(currentUserId, isUser, vector<Item*>(),false);
			galleryUI();
			view = 1;
		}
		break;
	case 1:
		search->update();
		isMovingIcon->update();
		colorPicker->update();
		reset->update();
		objectModeB->update();
		colorModeB->update();
		playModeB->update();

		gallery.update();
		break;
	case 2:
		galleryNavigate->update();
		objectMode.update();
		if (objectMode.getToReturn()) {
			gallery.setup(currentUserId, isUser, objectMode.getFilteredItems(), true);
			view = 1;
		}
		break;
	case 3:
		galleryNavigate->update();
		colorMode.update();
		if (colorMode.getToReturn()) {
			gallery.setup(currentUserId, isUser, colorMode.getFilteredItems(),true);
			view = 1;
		}
		break;
	case 4:
		galleryNavigate->update();
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
		reset->draw();
		objectModeB->draw();
		colorModeB->draw();
		playModeB->draw();
		gallery.draw();

		ofFill();
		colorPicker->draw();
		break;
	case 2:
		galleryNavigate->draw();
		objectMode.draw();
		break;
	case 3:
		galleryNavigate->draw();
		colorMode.draw();

		break;
	case 4:
		galleryNavigate->draw();
		playMode.draw();
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (view) {
	case 0:

		break;
	case 1:
		gallery.keyPressed(key);
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
		break;
	case 1:
		gallery.mousePressed(x, y, button);
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

	galleryNavigate = new ofxDatGuiButton("Gallery");
	galleryNavigate->setPosition(0, gui->getHeader()->getHeight());
	galleryNavigate->setIndex(10);
	galleryNavigate->onButtonEvent(this, &ofApp::onButtonEvent);
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
	objectModeB = new ofxDatGuiButton("Object Mode");
	objectModeB->setIndex(0);
	objectModeB->setPosition(0, 50);
	objectModeB->setWidth(100);
	objectModeB->onButtonEvent(this, &ofApp::onButtonEvent);

	colorModeB = new ofxDatGuiButton("Color Mode");
	colorModeB->setIndex(1);
	colorModeB->setPosition(objectModeB->getWidth(), 50);
	colorModeB->setWidth(100);
	colorModeB->onButtonEvent(this, &ofApp::onButtonEvent);

	playModeB = new ofxDatGuiButton("Play Mode");
	playModeB->setIndex(2);
	playModeB->setPosition(objectModeB->getWidth() + colorModeB->getWidth(), 50);
	playModeB->setWidth(100);
	playModeB->onButtonEvent(this, &ofApp::onButtonEvent);

	// reset color to normal, made this cause of fadeOut in PlayMode
	ofSetColor(255, 255, 255, 255);

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
	gallery.setup(id, isUser, vector<Item*>(),false);
	view = 1;
}

//-----------------------Events----------------------------//
void ofApp::onButtonEvent(ofxDatGuiButtonEvent e) {
	int index = e.target->getIndex();
	switch (index) {
	case 0: // objectMode
		view = 2;
		objectMode.setup(items);
		break;
	case 1: // colorMode
		view = 3;
		colorMode.setup(items);
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
	case 10: // go to Gallery
		galleryUI();
		// aqui mudas!!!!!
		items = gallery.setup(videoPlayer.getSelectedId(), isUser, vector<Item*>(),false);
		switch (view) {
		case 2: objectMode.closeCam();
			break;
		case 3: colorMode.closeCam();
			break;
		case 4: playMode.closeCam();
		}
		view = 1;
		break;
	}
}

