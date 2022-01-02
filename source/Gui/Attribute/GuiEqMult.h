#pragma once
#include "GuiEquation.h"

class GuiEqMult
	:public GuiEquation
{
	GuiEquation* a;
	double value;
public:
	GuiEqMult(const double& value,const GuiEquation& a);
	~GuiEqMult();

	double evaluate(GuiComponent& guiComponent,const double& currentValue,bool xAxis);
	std::vector<GuiAttribute*> getDependencies(GuiComponent& guiComponent,bool xAxis);
	GuiEquation* clone()const; 
};

