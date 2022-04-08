#pragma once
#include "GuiEquation.h"

class GuiEqTexturePercent
:public GuiEquation
{
	double ratio ;
public:
	GuiEqTexturePercent(double percent);
	~GuiEqTexturePercent();
	
	double evaluate(GuiComponent& guiComponent,const double& currentValue,bool xAxis);
	std::vector<GuiAttribute*> getDependencies(GuiComponent& guiComponent,bool xAxis);
	GuiEquation* clone()const;
	virtual std::string debugInformation(std::string tab)override;
 
	void setPercent(double percent);
};

