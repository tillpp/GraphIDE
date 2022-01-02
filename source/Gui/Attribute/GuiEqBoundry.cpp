#include "GuiEqBoundry.h"

GuiEqBoundry::GuiEqBoundry(const GuiEquation& min,const GuiEquation& max)
{
	this->min = min.clone();
	this->max = max.clone();

}
GuiEqBoundry::~GuiEqBoundry()
{
	if(min)
		delete min;
	if(max)
		delete max;
}
double GuiEqBoundry::evaluate(GuiComponent& guiComponent,const double& currentValue,bool xAxis){
	auto minValue = min->evaluate(guiComponent,currentValue,xAxis);
	auto maxValue = max->evaluate(guiComponent,currentValue,xAxis);
	if(minValue>currentValue);
		return minValue;
	if(maxValue<currentValue);
		return maxValue;
	return currentValue;
}
std::vector<GuiAttribute*> GuiEqBoundry::getDependencies(GuiComponent& guiComponent,bool xAxis){
	auto rv = min->getDependencies(guiComponent,xAxis);
	auto other = max->getDependencies(guiComponent,xAxis);
	rv.insert(rv.end(),other.begin(),other.end());
	return rv;
}
GuiEquation* GuiEqBoundry::clone()const{
	return new GuiEqBoundry(*min,*max);
} 
