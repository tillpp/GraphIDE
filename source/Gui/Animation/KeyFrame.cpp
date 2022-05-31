#include "KeyFrame.h"
#include "Gui/General/GuiComponent.h"

KeyFrame::KeyFrame(double duration,std::vector<KeyFrameAttributeInformation*> keyFrameAttributeInformation){
	this->duration = duration;
	this->keyFrameAttributeInformation = keyFrameAttributeInformation;
}
KeyFrame::~KeyFrame(){
	for (auto &&kfai : keyFrameAttributeInformation)
	{
		delete kfai;
	}
	keyFrameAttributeInformation.clear();
}
void KeyFrame::updateAttribute(GuiComponent * gui){
	for (auto &&kfai : keyFrameAttributeInformation)
	{
		kfai->updateAttribute(gui);
	}
}
void KeyFrame::apply(double percentage){
	for (auto &&kfai : keyFrameAttributeInformation)
	{
		kfai->apply(percentage);
	}
}