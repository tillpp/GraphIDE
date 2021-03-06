#pragma once
#include "GuiEquation.h"

class GuiEqPixel:
	public GuiEquation
{
	double value;
public:
	GuiEqPixel(double x);
	~GuiEqPixel();

	double evaluate(GuiComponent& guiComponent,const double& currentValue,bool xAxis)override;
	std::vector<GuiAttribute*> getDependencies(GuiComponent& guiComponent,bool xAxis)override;
	GuiEquation* clone()const override; 
	virtual std::string debugInformation(std::string tab)override;

	void setValue(double x);
};
