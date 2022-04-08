#pragma once
#include "GuiEquation.h"

class GuiEqTextureHoldRatio
:public GuiEquation
{
public:
	GuiEqTextureHoldRatio();
	~GuiEqTextureHoldRatio();
	
	double evaluate(GuiComponent& guiComponent,const double& currentValue,bool xAxis);
	std::vector<GuiAttribute*> getDependencies(GuiComponent& guiComponent,bool xAxis);
	GuiEquation* clone()const; 
	virtual std::string debugInformation(std::string tab)override;

};

