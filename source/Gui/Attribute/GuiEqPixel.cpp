#include "GuiEqPixel.h"

GuiEqPixel::GuiEqPixel(double x)
{
	value = x;
}

GuiEqPixel::~GuiEqPixel()
{
}
double GuiEqPixel::evaluate(GuiComponent& guiComponent,const double& currentValue,bool xAxis){
	return value;
}
std::vector<GuiAttribute*> GuiEqPixel::getDependencies(GuiComponent& guiComponent,bool xAxis){
	return {};
}
GuiEquation* GuiEqPixel::clone()const{
	auto rv = new GuiEqPixel(value);
	return rv;
} 
void GuiEqPixel::setValue(double x){
	this->value = x;
}
#include <iostream>
#include <string>
#include <sstream>
std::string GuiEqPixel::debugInformation(std::string tab){
	std::ostringstream o;
	o<< value;
	return tab+"# GuiEqPixel  value:"+o.str()+"\n";
}