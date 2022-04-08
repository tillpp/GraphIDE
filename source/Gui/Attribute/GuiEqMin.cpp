#include "GuiEqMin.h"
#include "../GuiComponent.h"

GuiEqMin::GuiEqMin(const GuiEquation& a,const GuiEquation& b)
{
	this->a = a.clone();
	this->b = b.clone();
}


GuiEqMin::~GuiEqMin()
{
	delete a;
	delete b;
}
double GuiEqMin::evaluate(GuiComponent& guiComponent,const double& currentValue,bool xAxis){
	return std::min(
		a->evaluate(guiComponent,currentValue,xAxis),
		b->evaluate(guiComponent,currentValue,xAxis)
	);
}
std::vector<GuiAttribute*> GuiEqMin::getDependencies(GuiComponent& guiComponent,bool xAxis){
	auto rv = a->getDependencies(guiComponent,xAxis);
	auto other = b->getDependencies(guiComponent,xAxis);
	rv.insert(rv.end(),other.begin(),other.end());
	return rv;
}
GuiEquation* GuiEqMin::clone()const{
	return new GuiEqMin(*a,*b);
}
std::string GuiEqMin::debugInformation(std::string tab){
	return tab+"# GuiEqMin \n"+a->debugInformation(tab+"\t")+b->debugInformation(tab+"\t");
}