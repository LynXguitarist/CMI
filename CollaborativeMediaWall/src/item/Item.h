#pragma once
#include "ofMain.h"
#include <string>

class Item
{
private:
	string path;
	ofImage image; // if video loads one frame
	ofVideoPlayer video;
	bool isVideo;
	bool isVideoPlaying;

public:
	Item(string path, ofImage image, ofVideoPlayer video, bool isVideo, bool isVideoPlaying);
	string getPath();
	ofImage getImage();
	ofVideoPlayer getVideo();
	bool getIsVideo();
	bool getIsVideoPlaying();
	void setVideoPlaying();
};

