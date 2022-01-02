#pragma once
#include "GuiEquation.h"

class GuiEqMin
	:public GuiEquation 
{
	GuiEquation* a,*b;
public:
	GuiEqMin(const GuiEquation& a,const GuiEquation& b);
	~GuiEqMin();

	virtual double evaluate(GuiComponent& guiComponent,const double& currentValue,bool xAxis)override;
	virtual std::vector<GuiAttribute*> getDependencies(GuiComponent& guiComponent,bool xAxis)override;
	virtual GuiEquation* clone()const override; 
};