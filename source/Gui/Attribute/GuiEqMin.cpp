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
double GuiEqMin::evaluate(GuiComponent& guiComponent,bool xAxis){
	return std::min(
		a->evaluate(guiComponent,xAxis),
		b->evaluate(guiComponent,xAxis)
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