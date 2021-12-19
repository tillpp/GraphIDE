#pragma once
#include "GuiEquation.h"

class GuiEqMax
	:public GuiEquation 
{
	GuiEquation* a,*b;
public:
	GuiEqMax(const GuiEquation& a,const GuiEquation& b);
	~GuiEqMax();

	virtual double evaluate(GuiComponent& guiComponent,bool xAxis)override;
	virtual std::vector<GuiAttribute*> getDependencies(GuiComponent& guiComponent,bool xAxis)override;
	virtual GuiEquation* clone()const override; 
};