#include "GuiEquation.h"
#include "../GuiComponent.h"

GuiEqSub::GuiEqSub(const GuiEquation &a, const GuiEquation &b)
{
	this->a = a.clone();
	this->b = b.clone();
}

GuiEqSub::~GuiEqSub()
{
	delete a;
	delete b;
}

double GuiEqSub::evaluate(GuiComponent &guiComponent, bool xAxis){
	return a->evaluate(guiComponent,xAxis)-b->evaluate(guiComponent,xAxis);
}
std::vector<GuiAttribute *> GuiEqSub::getDependencies(GuiComponent &guiComponent, bool xAxis){
	auto rv = a->getDependencies(guiComponent,xAxis);
	auto other = b->getDependencies(guiComponent,xAxis);
	rv.insert(rv.end(),other.begin(),other.end());
	return rv;
}
GuiEquation *GuiEqSub::clone() const{
	return new GuiEqSub(*a,*b);
}