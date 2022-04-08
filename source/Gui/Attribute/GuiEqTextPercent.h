#pragma once
#include "GuiEquation.h"

class GuiEqTextPercent
	:public GuiEquation
{
private:
	double ratio;
public:
	GuiEqTextPercent(double percent);
	~GuiEqTextPercent();

	double evaluate(GuiComponent& guiComponent,const double& currentValue,bool xAxis);
	std::vector<GuiAttribute*> getDependencies(GuiComponent& guiComponent,bool xAxis);
	GuiEquation* clone()const; 
	virtual std::string debugInformation(std::string tab)override;


	void setPercent(double percent);
};

