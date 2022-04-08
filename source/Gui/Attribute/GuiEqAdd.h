#pragma once
#include "GuiEquation.h"

class GuiEqAdd
	:public GuiEquation
{
	GuiEquation* a,*b;
public:
	GuiEqAdd(const GuiEquation& a,const GuiEquation& b);
	~GuiEqAdd();

	double evaluate(GuiComponent& guiComponent,const double& currentValue,bool xAxis);
	std::vector<GuiAttribute*> getDependencies(GuiComponent& guiComponent,bool xAxis);
	GuiEquation* clone()const; 
	std::string debugInformation(std::string tab);
};

