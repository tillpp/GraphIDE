#pragma once
#include "GuiEquation.h"

class GuiEqTexturePercent
:public GuiEquation
{
	double ratio ;
public:
	GuiEqTexturePercent(double percent);
	~GuiEqTexturePercent();
	
	double evaluate(GuiComponent& guiComponent,bool xAxis);
	std::vector<GuiAttribute*> getDependencies(GuiComponent& guiComponent,bool xAxis);
	GuiEquation* clone()const; 

	void setPercent(double percent);
};

