#include "GuiEquation.h"
#include "../GuiComponent.h"

GuiEqAdd::GuiEqAdd(const GuiEquation &a, const GuiEquation &b)
{
	this->a = a.clone();
	this->b = b.clone();
}

GuiEqAdd::~GuiEqAdd()
{
	delete a;
	delete b;
}
double GuiEqAdd::evaluate(GuiComponent &guiComponent,const double& currentValue, bool xAxis){
	return a->evaluate(guiComponent,currentValue,xAxis)+b->evaluate(guiComponent,currentValue,xAxis);
}
std::vector<GuiAttribute *> GuiEqAdd::getDependencies(GuiComponent &guiComponent, bool xAxis){
	auto rv = a->getDependencies(guiComponent,xAxis);
	auto other = b->getDependencies(guiComponent,xAxis);
	rv.insert(rv.end(),other.begin(),other.end());
	return rv;
}
GuiEquation *GuiEqAdd::clone() const{
	return new GuiEqAdd(*a,*b);
}
std::string GuiEqAdd::debugInformation(std::string tab){
	return tab+"# GuiEqAdd \n"+a->debugInformation(tab+"\t")+b->debugInformation(tab+"\t");
}