#pragma once
class GuiComponent;
class GuiAttribute;

#include <vector>
#include <string>
class GuiEquation
{
public:
	GuiEquation();
	GuiEquation(GuiEquation&)=delete;
	~GuiEquation();

	virtual double evaluate(GuiComponent& guiComponent,const double& currentValue,bool xAxis)=0;
	virtual std::vector<GuiAttribute*> getDependencies(GuiComponent& guiComponent,bool xAxis)=0;
	virtual GuiEquation* clone()const=0; 

	virtual std::string debugInformation(std::string tab)=0;
};
