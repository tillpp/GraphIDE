#include "GuiEqMax.h"
#include "../GuiComponent.h"

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
double GuiEqMax::evaluate(GuiComponent& guiComponent,bool xAxis){
	return std::max(
		a->evaluate(guiComponent,xAxis),
		b->evaluate(guiComponent,xAxis)
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