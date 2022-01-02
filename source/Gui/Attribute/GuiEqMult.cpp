#include "GuiEquation.h"
#include "../GuiComponent.h"

GuiEqMult::GuiEqMult(const double& value,const GuiEquation& a)
{
	this->a = a.clone();
	this->value = value;
}

GuiEqMult::~GuiEqMult()
{
	delete a;
}

double GuiEqMult::evaluate(GuiComponent &guiComponent,const double& currentValue, bool xAxis){
	return value*a->evaluate(guiComponent,currentValue,xAxis);
}
std::vector<GuiAttribute *> GuiEqMult::getDependencies(GuiComponent &guiComponent, bool xAxis){
	return a->getDependencies(guiComponent,xAxis);
}
GuiEquation *GuiEqMult::clone() const{
	return new GuiEqMult(value,*a);
}
