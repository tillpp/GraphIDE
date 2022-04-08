#include "GuiEqPercent.h"
#include "../GuiComponent.h"
#include <iostream>

GuiEqPercent::GuiEqPercent(double percent)
{
	ratio = percent/100;
}

GuiEqPercent::~GuiEqPercent()
{
}

double GuiEqPercent::evaluate(GuiComponent& guiComponent,const double& currentValue,bool xAxis){
	auto parent = guiComponent.getParent();
	if(parent){
		return ratio*(xAxis?parent->width:parent->height);
	}
	return 0;
}
std::vector<GuiAttribute*> GuiEqPercent::getDependencies(GuiComponent& guiComponent,bool xAxis){
	auto parent = guiComponent.getParent();
	if(parent)
		return {(xAxis?&parent->width:&parent->height)};
	else return {};
}
GuiEquation* GuiEqPercent::clone()const{
	auto rv = new GuiEqPercent(0);
	rv->ratio = ratio;
	return rv;
}
std::string GuiEqPercent::debugInformation(std::string tab){
	return tab+"# GuiEqPercent  ratio:"+std::to_string(ratio)+"\n";
}