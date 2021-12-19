#pragma once
#include "GuiEquation.h"

class GuiEqTextureHoldRatio
:public GuiEquation
{
public:
	GuiEqTextureHoldRatio();
	~GuiEqTextureHoldRatio();
	
	double evaluate(GuiComponent& guiComponent,bool xAxis);
	std::vector<GuiAttribute*> getDependencies(GuiComponent& guiComponent,bool xAxis);
	GuiEquation* clone()const; 
};

