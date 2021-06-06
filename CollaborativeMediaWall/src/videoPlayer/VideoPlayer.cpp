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

    passwordInput = new ofxDatGuiTextInput("Password", "");
    passwordInput->setPosition(ofGetWidth() / 2, 100);
    passwordInput->onTextInputEvent(this, &VideoPlayer::onTextInputEvent);
    

    confirmButton = new ofxDatGuiButton("Confirm");
    confirmButton->setWidth(100);
    confirmButton->setPosition(ofGetWidth() / 2 + passwordInput->getWidth(), 100);
    confirmButton->onButtonEvent(this, &VideoPlayer::setNavigation);

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

    if (showPasswordInput)
        passwordInput->update();

    if (showConfirm)
        confirmButton->update();
}

void VideoPlayer::draw()
{
    color.draw(0, 0);

    for (int i = 0; i < haar.blobs.size(); i++) {
        ofSetColor(255);
        ofNoFill();
        ofDrawRectangle(haar.blobs[i].boundingRect);
    }
    if (numberOfFaces > 0 && !showPasswordInput) {
        for (int i = 0; i < names.size(); i++) {
            names[i]->draw();
        }
    }

    if (showPasswordInput)
        passwordInput->draw();

    if (showConfirm)
        confirmButton->draw();
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
        names[i]->onButtonEvent(this, (&VideoPlayer::showPassInput));
        usersXML.popTag();
    }

}

void VideoPlayer::showPassInput(ofxDatGuiButtonEvent e)
{
    showPasswordInput = true;
    selectedId = e.target->getIndex();
    (void)ofLog(OF_LOG_NOTICE, "Selected Id: " + ofToString(selectedId));
    // get the password for the user/project
    int usersCount = usersXML.getNumTags("user");
    for (int i = 0; i < usersCount; i++) {
        usersXML.pushTag("user", i);
        (void)ofLog(OF_LOG_NOTICE, "password for current user: " + usersXML.getValue("password", ""));
        if (usersXML.getValue("id", 0) == selectedId) {
            // saves password to compare
            userProjectPass = usersXML.getValue("password", "");
            
            (void)ofLog(OF_LOG_NOTICE, "Password saved: " + userProjectPass);

            usersXML.popTag(); // user
            break;
        }
        usersXML.popTag(); // user
    }
}

void VideoPlayer::onTextInputEvent(ofxDatGuiTextInputEvent e)
{
    password = e.text;
    if (password != "")
        showConfirm = true;
    else
        showConfirm = false;
}


void VideoPlayer::setNavigation(ofxDatGuiButtonEvent e) {

    (void)ofLog(OF_LOG_NOTICE, "Input password: " + password);
    (void)ofLog(OF_LOG_NOTICE, "Password: " + userProjectPass);

    if (password == userProjectPass) {
        camera.close();
        navigate = 1;
    }
    else {
        // wrong passowrd
        ofSystemAlertDialog("wrong password! Try again");
    }
    
}

int VideoPlayer::toNavigate() {
    return navigate;
}

int VideoPlayer::getSelectedId() {
    return selectedId;
}