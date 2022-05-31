#pragma once
class GuiComponent;
class Animation;
#include <map>
#include <string>

class AnimationHandler
{
	friend GuiComponent;
	
	std::map<std::string,Animation*> animations;
	GuiComponent& gui;

	Animation* currentAnimation = nullptr;
	AnimationHandler(GuiComponent&);
public:
	~AnimationHandler();

	/*
		@return nullptr if name is taken. 
	*/
	Animation* createAnimation(std::string name);

	void use(std::string name);
	void update();

	// this function is called when a attribute is added or removed from the gui.
	void updateAttribute();
};

#include "../General/GuiComponent.h"
#include "Animation.h"