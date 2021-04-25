#include "Gallery.h"

void Gallery::setup()
{
	
}

void Gallery::update()
{
}

void Gallery::draw()
{
	
}

void Gallery::keyPressed(int key) {
	if (dir.size() > 0) {
		currentItem++;
		currentItem %= dir.size();
	}
}

void Gallery::keyReleased(int key)
{
}

void Gallery::mouseMoved(int x, int y)
{
}

void Gallery::mouseDragged(int x, int y, int button)
{
}

void Gallery::mousePressed(int x, int y, int button)
{
}

void Gallery::mouseReleased(int x, int y, int button)
{
}

void Gallery::mouseEntered(int x, int y)
{
}

void Gallery::mouseExited(int x, int y)
{
}

void Gallery::windowResized(int w, int h)
{
}

void Gallery::dragEvent(ofDragInfo dragInfo)
{
}

void Gallery::gotMessage(ofMessage msg)
{
}
