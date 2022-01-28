#include "KeyFrameAttributeInformation.h"
#include "../../Util/Log.h"

KeyFrameAttributeInformation::KeyFrameAttributeInformation(
		const GuiEquation& equation,
		std::function<double(double)> interpolation,
		std::string attributeName){
	this->equation = equation.clone();
	this->interpolation = interpolation;
	this->attributeName = attributeName;
}
KeyFrameAttributeInformation::~KeyFrameAttributeInformation(){
	delete equation;
}

void KeyFrameAttributeInformation::updateAttribute(GuiComponent* gui){
	this->attribute = gui->getGuiAttribute(attributeName);
	this->KeyFrameStartValue = (*this->attribute);
}
void KeyFrameAttributeInformation::apply(double percentage){
	if(!attribute)
		return;
	double rate = interpolation(percentage);
	auto newValue = KeyFrameStartValue+rate*(attribute->evaluate(*equation)-KeyFrameStartValue);
	
	attribute->overrideCachedValue(newValue);
}
KeyFrameAttributeInformation* KeyFrameAttributeInformation::clone(){
	return new KeyFrameAttributeInformation(*equation,interpolation,attributeName);
}