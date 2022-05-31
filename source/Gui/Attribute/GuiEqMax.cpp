#include "GuiEqMax.h"
#include "Gui/General/GuiComponent.h"

GuiEqMax::GuiEqMax(const GuiEquation& a,const GuiEquation& b)
{
	this->a = a.clone();
	this->b = b.clone();
}


GuiEqMax::~GuiEqMax()
{
	delete a;
	delete b;
}
double GuiEqMax::evaluate(GuiComponent& guiComponent,const double& currentValue,bool xAxis){
	return std::max(
		a->evaluate(guiComponent,currentValue,xAxis),
		b->evaluate(guiComponent,currentValue,xAxis)
	);
}
std::vector<GuiAttribute*> GuiEqMax::getDependencies(GuiComponent& guiComponent,bool xAxis){
	auto rv = a->getDependencies(guiComponent,xAxis);
	auto other = b->getDependencies(guiComponent,xAxis);
	rv.insert(rv.end(),other.begin(),other.end());
	return rv;
}
GuiEquation* GuiEqMax::clone()const{
	return new GuiEqMax(*a,*b);
}
std::string GuiEqMax::debugInformation(std::string tab){
	return tab+"# GuiEqMax \n"+a->debugInformation(tab+"\t")+b->debugInformation(tab+"\t");
}