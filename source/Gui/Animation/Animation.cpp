#include "Animation.h"

Animation::Animation(GuiComponent* g,std::string n)
	:gui(g),name(n)
{
}

Animation::~Animation()
{
}
void Animation::start(Animation* lastAnimation){
	clock.restart();
}
void Animation::updateAttribute(){
	if(currentFrame>=0&&currentFrame<keyFrames.size()){
		keyFrames[currentFrame]->updateAttribute(gui);
	}
}
void Animation::update(){
	if(!keyFrames.size())
		return;
		
	int thisFrame_runCounter = clock.getElapsedTime().asMilliseconds() / ((int)(animationDuration*1000));
	double time = clock.getElapsedTime().asMilliseconds() % ((int)(animationDuration*1000));
	time/=1000;
	
	//update it initaly /reset it
	if(currentFrame==-1){
		currentFrame = 0;
		lastFrame_runCounter = thisFrame_runCounter;
		updateAttribute();
	}
	//make at least on step forward if the animation repeats
	if(lastFrame_runCounter!=thisFrame_runCounter){
		auto& currentKeyFrame = *keyFrames[currentFrame];
		currentKeyFrame.apply(1.f);
		currentFrame++;
		if(currentFrame>=keyFrames.size()){
			currentFrame = 0;
		}
		updateAttribute();
	}
	while(currentFrame < keyFrames.size())
	{
		auto& currentKeyFrame = *keyFrames[currentFrame];

		//is it within this Frame?
		if(currentKeyFrame.startTime<=time 
			&& time<currentKeyFrame.startTime+currentKeyFrame.duration){
			//apply by percentage
			currentKeyFrame.apply((time-currentKeyFrame.startTime)/currentKeyFrame.duration);
			break;
		}else
		//not in this frame
		{
			currentKeyFrame.apply(1.f);
			currentFrame++;
			if(currentFrame>=keyFrames.size()){
				currentFrame = 0;
			}
			updateAttribute();
		}
	}
	lastFrame_runCounter = thisFrame_runCounter;
}
void Animation::addKeyFrame(KeyFrame* kf){
	keyFrames.push_back(kf);
	auto& top = keyFrames[keyFrames.size()-1];

	top->startTime 	   = animationDuration;
	animationDuration += top->duration;
}
