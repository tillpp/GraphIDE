#pragma once
#include "GuiEquation.h"

class GuiEqSize
	:public GuiEquation
{
	double ratio;
public:
	GuiEqSize(double percent);
	~GuiEqSize();

	virtual double evaluate(GuiComponent& guiComponent,const double& currentValue,bool xAxis)override;
	virtual std::vector<GuiAttribute*> getDependencies(GuiComponent& guiComponent,bool xAxis)override;
	virtual GuiEquation* clone()const override; 
	virtual std::string debugInformation(std::string tab)override;

};