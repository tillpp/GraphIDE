#pragma once
#include "GuiEquation.h"

class GuiEqPercent
	:public GuiEquation
{
protected:
	double ratio = 1;
public:
	GuiEqPercent(double percent);
	~GuiEqPercent();

	double evaluate(GuiComponent& guiComponent,const double& currentValue,bool xAxis)override;
	std::vector<GuiAttribute*> getDependencies(GuiComponent& guiComponent,bool xAxis)override;
	GuiEquation* clone()const override; 
};

