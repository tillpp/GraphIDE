#pragma once
#include "GuiEquation.h"

class GuiEqRatio
	:public GuiEquation
{
	double ratio;
public:
	GuiEqRatio(double ratio);
	~GuiEqRatio();

	virtual double evaluate(GuiComponent& guiComponent,bool xAxis)override;
	virtual std::vector<GuiAttribute*> getDependencies(GuiComponent& guiComponent,bool xAxis)override;
	virtual GuiEquation* clone()const override; 
};