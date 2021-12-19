#pragma once
#include "GuiEquation.h"

class GuiEqSub:
	public GuiEquation
{
	GuiEquation* a,*b;
public:
	GuiEqSub(const GuiEquation& a,const GuiEquation& b);
	~GuiEqSub();

	double evaluate(GuiComponent& guiComponent,bool xAxis);
	std::vector<GuiAttribute*> getDependencies(GuiComponent& guiComponent,bool xAxis);
	GuiEquation* clone()const; 
};

