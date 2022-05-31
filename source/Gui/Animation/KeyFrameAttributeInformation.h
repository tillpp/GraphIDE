#pragma once
#include "../General/GuiComponent.h"
/*
	This Class is useful for KeyFrame
*/

class KeyFrameAttributeInformation
{	
	//set by constructor
	std::string attributeName; //temporary later gets converts into an attribute
	GuiEquation* equation = nullptr;
	std::function<double(double)> interpolation; // [0,1] -> [0,1]

	//set by "start"
	GuiAttribute *attribute = nullptr;
	double KeyFrameStartValue = 0;
public:
    KeyFrameAttributeInformation(const KeyFrameAttributeInformation&)=delete;
	KeyFrameAttributeInformation(const GuiEquation &equation, std::function<double(double)> interpolation, std::string attributeName);
	~KeyFrameAttributeInformation();
	
	//functions called by KeyFrame
	void updateAttribute(GuiComponent*);
	void apply(double percentage);

	KeyFrameAttributeInformation* clone();
};