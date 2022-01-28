#include "AnimationHandler.h"

AnimationHandler::AnimationHandler(GuiComponent& g)
	:gui(g)
{
}

AnimationHandler::~AnimationHandler()
{
}

Animation* AnimationHandler::createAnimation(std::string name){
	if(animations.find(name)!=animations.end()){
		return nullptr;
	}
	Animation* rv = new Animation(&gui,name);
	animations[name] = rv;
	return rv;
}
void AnimationHandler::use(std::string name){
	if(animations.find(name)==animations.end())
		return;
	currentAnimation = animations[name];
}
void AnimationHandler::update(){
	if(currentAnimation)
		currentAnimation->update();
}

void AnimationHandler::updateAttribute(){
	if(currentAnimation)
		currentAnimation->updateAttribute();
}