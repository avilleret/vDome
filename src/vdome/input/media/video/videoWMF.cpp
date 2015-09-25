#include "videoWMF.h"
using namespace vd;

VideoWMF::VideoWMF() {
	bLoop = false;
	markEnd = false;
	positionRequestFrameCnt = 0;
	positionRequest = -1;
	storePositionFix =-1;
	ofAddListener(player.videoLoadEvent, this, &VideoWMF::videoLoaded);
}

bool VideoWMF::open(string filepath){
	bLoaded = false;
	bSupported = false;
	bEnded = false;
	markEnd = false;
	positionRequestFrameCnt = 0;
	storePositionFrameCnt = 0;
	positionRequest = -1;
	storePositionFix = -1;
	player.loadMovie(filepath);
    play();
	player.setLoop(bLoop);
    return true;
}

void VideoWMF::update(){

	player.update();

	if (markEnd && !player.getPosition()){
		bEnded = true;
		markEnd = false;
	}	
	
	if (player.getPosition() > 0.99){
		markEnd = true;
	}else {
		markEnd = false;
	}

	if (positionRequest >= 0){
		positionRequestFrameCnt++;
	}
	if (positionRequestFrameCnt > 20){
		positionRequestFrameCnt = -1;
		player.setPosition(positionRequest);
		if (bPaused) stop();
		positionRequest = -1;
		storePositionFrameCnt = 0;
	}
}

void VideoWMF::play(){
	bPaused = false;
	player.play();
}

void VideoWMF::stop(){
	bPaused = true;
    player.pause();
}

void VideoWMF::close(){
    //player.stop();
    //player.close();
	//player.forceExit();
}

void VideoWMF::seek(float f){
	positionRequest = f;
	storePositionFix = positionRequest;
}

bool VideoWMF::isPlaying(){
    bool bPlaying;
    bPlaying = player.isPlaying();
    return bPlaying;
}

void VideoWMF::setLoop(bool lp){
    bLoop = lp;
    player.setLoop(lp);
}

float VideoWMF::getPosition(){
	if (storePositionFix != -1){
		if (player.getPosition() != storePositionFix || !player.getPosition()){
			storePositionFrameCnt++;
			return storePositionFix;
		}
		if (floorf(player.getPosition()) == floorf(storePositionFix) || storePositionFrameCnt > 20) {
			storePositionFix = -1;
			storePositionFrameCnt = 0;
			return player.getPosition();
		}
	}
	return player.getPosition();
}

float VideoWMF::getDuration(){
    return player.getDuration();
}

bool VideoWMF::getIsMovieDone(){
	return bEnded;
}

void VideoWMF::videoLoaded(bool &success){
	bLoaded = true;
	bSupported = success;
}

bool VideoWMF::isLoaded(){
	return bLoaded;
}

bool VideoWMF::isSupported(){
	return bSupported;
}

void VideoWMF::setVolume(float v){
    player.setVolume(v);
}

float VideoWMF::getWidth(){
    return player.getWidth();
}

float VideoWMF::getHeight(){
    return player.getHeight();
}

void VideoWMF::bind() {
	return player.bind();
}

void VideoWMF::unbind() {
	return player.unbind();
}