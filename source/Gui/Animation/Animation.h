#pragma once
class KeyFrame;
class AnimationHandler;
#include <SFML/System.hpp>
#include "KeyFrame.h"
#include "AnimationHandler.h"
#include "../Attribute/GuiAttribute.h"

class Animation
{
	std::string name;
	double animationDuration = 0;
	sf::Clock clock;
	int lastFrame_runCounter; //amount of runs this animation had,(in the last frame)


	//keyframes
	std::vector<KeyFrame*> keyFrames;
	int currentFrame = -1;
	
	//animationhandler
	friend AnimationHandler;
	Animation(GuiComponent*,std::string name);
	GuiComponent* gui;

	//update "GuiAttribute" and "lastKeyFrameValue" in current KeyFrame
	void updateAttribute();
    void start(Animation* lastAnimation = nullptr);
	void update();
public:
	~Animation();

	
	void addKeyFrame(KeyFrame*);
};

