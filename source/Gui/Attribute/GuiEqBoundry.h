#pragma once
#include "GuiEquation.h"

class GuiEqBoundry
	:public GuiEquation
{
	GuiEquation* min,*max;
public:
	GuiEqBoundry(const GuiEquation& min,const GuiEquation& max);
	~GuiEqBoundry();

	double evaluate(GuiComponent& guiComponent,const double& currentValue,bool xAxis);
	std::vector<GuiAttribute*> getDependencies(GuiComponent& guiComponent,bool xAxis);
	GuiEquation* clone()const; 
};
