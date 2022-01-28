#pragma once
class Animation;
class KeyFrameAttributeInformation;
class GuiComponent;
#include "KeyFrameAttributeInformation.h"

class KeyFrame
{
	//set by constructor
	std::vector<KeyFrameAttributeInformation*> keyFrameAttributeInformation;
	double duration = 0;

	//set by Animatiom
	friend Animation;
	double startTime = 0;

	void updateAttribute(GuiComponent *);
	void apply(double percentage);
public:
	KeyFrame(double duration, std::vector<KeyFrameAttributeInformation*> keyFrameAttributeInformation);
	~KeyFrame();
};
