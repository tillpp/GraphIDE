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

double GuiEqAdd::evaluate(GuiComponent &guiComponent, bool xAxis){
	return a->evaluate(guiComponent,xAxis)+b->evaluate(guiComponent,xAxis);
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
