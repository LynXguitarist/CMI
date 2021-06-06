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
    isProject = false;
    currentUserDisplay=0;
    currentProjectDisplay=0;


    usersXML.loadFile("data_xml/users.xml");
    usersXML.pushTag("users");
    projXML.load("data_xml/projects.xml");
    projXML.pushTag("projects");
    setupButtons();

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
        namesHeader->update();
        if(namesArray.size()>10){
            namesUp->update();
            namesDown->update();
        }
    }
    if (numberOfFaces > 1) {
        for (int i = 0; i < projects.size(); i++) {
            projects[i]->update();
           
        }
        projHeader->update();
        if (projectsArray.size() > 10) {
            projUp->update();
            projDown->update();
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
        namesHeader->draw();
        if (namesArray.size() > 10) {
            namesUp->draw();
            namesDown->draw();
        }

    }
    if (numberOfFaces > 1 && !showPasswordInput) {
        for (int i = 0; i < projects.size(); i++) {
            projects[i]->draw();
        }
        projHeader->draw();
        if (projectsArray.size() > 10) {
            projUp->draw();
            projDown->draw();
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
    
    namesHeader = new ofxDatGuiLabel("Users");
    namesHeader->setWidth(100);
    namesHeader->setPosition(ofGetWidth() / 2, 30);

    projHeader = new ofxDatGuiLabel("Projects");
    projHeader->setWidth(100);
    projHeader->setPosition(ofGetWidth() / 2 + 200, 30);

    namesUp= new ofxDatGuiButton("^");
    namesUp->setWidth(30);
    namesUp->setPosition(ofGetWidth() / 2+35, 35+ namesHeader->getHeight());
    namesUp->setIndex(0);
    namesUp->onButtonEvent(this, (&VideoPlayer::onShiftEvent));

    projUp = new ofxDatGuiButton("^");
    projUp->setWidth(30);
    projUp->setPosition(ofGetWidth() / 2 + 235, 35 + projHeader->getHeight());
    projUp->setIndex(1);
    projUp->onButtonEvent(this, (&VideoPlayer::onShiftEvent));

    initialY = namesUp->getY() + namesUp->getHeight() + 5;

    int usersCount = usersXML.getNumTags("user");
    names.assign(min(10,usersCount), new ofxDatGuiButton(""));
    namesArray.assign(usersCount,"");
   
    for (int i = 0; i < usersCount; i++) {
        usersXML.pushTag("user", i);
        string thisUserName = usersXML.getValue("name", "");
        namesArray[i]=thisUserName;
        if(i<10){
            names[i] = new ofxDatGuiButton(thisUserName);
            names[i]->setPosition(ofGetWidth() / 2, initialY+names[i]->getHeight()*i);
            names[i]->setIndex(usersXML.getValue("id", 0));
            names[i]->setWidth(100);
            //MUDAR AQUI
            names[i]->onButtonEvent(this, (&VideoPlayer::showUserPassInput));
            
        }
        usersXML.popTag();
    }
    namesDown = new ofxDatGuiButton("v");
    namesDown->setWidth(30);
    namesDown->setPosition(ofGetWidth() / 2 + 35, initialY + namesUp->getHeight()*10 + 5);
    namesDown->setIndex(2);
    namesDown->onButtonEvent(this, (&VideoPlayer::onShiftEvent));

    int projectsCount = projXML.getNumTags("project");
    projects.assign(min(10, projectsCount), new ofxDatGuiButton(""));
    projectsArray.assign(projectsCount, "");
    projects.assign(projectsCount, new ofxDatGuiButton(""));
    for (int i = 0; i < projectsCount; i++) {
        projXML.pushTag("project", i);
        int projID = projXML.getValue("id", 0);
        projectsArray[i] = to_string(projID);
        if(i<10){
            projects[i] = new ofxDatGuiButton(to_string(projID));
            projects[i]->setPosition(ofGetWidth() / 2+200, initialY + projects[i]->getHeight() * i);
            projects[i]->setIndex(projID);
            projects[i]->setWidth(100);
            //MUDAR AQUI
            projects[i]->onButtonEvent(this, (&VideoPlayer::showProjectPassInput));
        }
        projXML.popTag();
    }
    projDown = new ofxDatGuiButton("v");
    projDown->setWidth(30);
    projDown->setPosition(ofGetWidth() / 2 + 235, initialY + projUp->getHeight() * 10 + 5);
    projDown->setIndex(3);
    projDown->onButtonEvent(this, (&VideoPlayer::onShiftEvent));

}

void VideoPlayer::showUserPassInput(ofxDatGuiButtonEvent e)
{
	showPasswordInput = true;
	selectedId = e.target->getIndex();
	// get the password for the user/project
	int usersCount = usersXML.getNumTags("user");
	for (int i = 0; i < usersCount; i++) {
		usersXML.pushTag("user", i);
		if (usersXML.getValue("id", 0) == selectedId) {
			// saves password to compare
			userProjectPass = usersXML.getValue("password", "");

			usersXML.popTag(); // user
			break;
		}
		usersXML.popTag(); // user
	}
}

void VideoPlayer::showProjectPassInput(ofxDatGuiButtonEvent e)
{
    showPasswordInput = true;
    selectedId = e.target->getIndex();
    // get the password for the project
    int projectsCount = projXML.getNumTags("project");
    for (int i = 0; i < projectsCount; i++) {
        projXML.pushTag("project", i);
        if (projXML.getValue("id", 0) == selectedId) {
            // saves password to compare
            userProjectPass = projXML.getValue("password", "");

            projXML.popTag(); // user
            break;
        }
        projXML.popTag(); // user
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

void VideoPlayer::onShiftEvent(ofxDatGuiButtonEvent e)
{
    int bID = e.target->getIndex();
    switch (bID) {
    case 0:
        shiftButtons(false, currentUserDisplay - 1);
        break;
    case 1:
        shiftButtons(true, currentUserDisplay - 1);
        break;
    case 2:
        shiftButtons(false, currentUserDisplay + 1);
        break;
    case 3:
        shiftButtons(true, currentUserDisplay + 1);
        break;
    }
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

void VideoPlayer::shiftButtons(bool isProj, int shift) {
    if (!isProj) {
        if (shift >= 0 && (shift + 9) < namesArray.size()) {
            for (int i = 0; i < 10;i++) {
                string thisItemName = namesArray[(i+shift)];
                names[i] = new ofxDatGuiButton(thisItemName);
                names[i]->setPosition(ofGetWidth() / 2, initialY + names[i]->getHeight() * i);
                names[i]->setIndex(i+shift+1);
                names[i]->setWidth(100);
                //MUDAR AQUI
                names[i]->onButtonEvent(this, (&VideoPlayer::showUserPassInput));
            }
            currentUserDisplay = shift;
        }
    }
    else {
        if (shift >= 0 && (shift + 9) < projectsArray.size()) {
            for (int i = 0; i < 10; i++) {
                string thisItemName = projectsArray[(i + shift)];
                projects[i] = new ofxDatGuiButton(thisItemName);
                projects[i]->setPosition(ofGetWidth() / 2 + 200, initialY + projects[i]->getHeight() * i);
                projects[i]->setIndex(i + shift+1);
                projects[i]->setWidth(100);
                //MUDAR AQUI
                projects[i]->onButtonEvent(this, (&VideoPlayer::showProjectPassInput));
            }
            currentProjectDisplay = shift;
        }
    }
}

bool VideoPlayer::isNavigateProject()
{
    return isProject;
}
