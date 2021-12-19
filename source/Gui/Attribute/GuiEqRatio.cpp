#include "GuiEqRatio.h"
#include "../GuiComponent.h"

GuiEqRatio::GuiEqRatio(double ratio)
{
	this->ratio = ratio;
}

GuiEqRatio::~GuiEqRatio()
{
}

double GuiEqRatio::evaluate(GuiComponent& guiComponent,bool xAxis){
	return xAxis?guiComponent.height*ratio:guiComponent.width/ratio;
}
std::vector<GuiAttribute*> GuiEqRatio::getDependencies(GuiComponent& guiComponent,bool xAxis){
	return std::vector<GuiAttribute*>{xAxis?&guiComponent.height:&guiComponent.width};
}
GuiEquation* GuiEqRatio::clone()const{
	return new GuiEqRatio(ratio);
} 