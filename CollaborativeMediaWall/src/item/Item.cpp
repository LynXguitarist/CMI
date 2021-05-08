#include "Item.h"

Item::Item(string path, ofImage image, ofVideoPlayer video, bool isVideo, bool isVideoPlaying)
{
	this->path = path;
	this->image = image;
	this->video = video;
	this->isVideo = isVideo;
	this->isVideoPlaying = isVideoPlaying;
}

string Item::getPath() {
	return path;
}

ofImage Item::getImage()
{
	return image;
}

ofVideoPlayer Item::getVideo() {
	return video;
}

bool Item::getIsVideo() {
	return isVideo;
}

bool Item::getIsVideoPlaying() {
	return isVideoPlaying;
}

