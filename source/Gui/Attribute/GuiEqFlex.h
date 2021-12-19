#pragma once
#include "GuiEquation.h"

class GuiEqFlex
	:public GuiEquation
{
public:
	GuiEqFlex(/* args */);
	~GuiEqFlex();

	virtual double evaluate(GuiComponent& guiComponent,bool xAxis);
	virtual std::vector<GuiAttribute*> getDependencies(GuiComponent& guiComponent,bool xAxis);
	virtual GuiEquation* clone()const; 
};
