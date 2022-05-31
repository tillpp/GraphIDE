#include "GuiEqRatio.h"
#include "Gui/General/GuiComponent.h"

GuiEqRatio::GuiEqRatio(double ratio)
{
	this->ratio = ratio;
}

GuiEqRatio::~GuiEqRatio()
{
}

double GuiEqRatio::evaluate(GuiComponent& guiComponent,const double& currentValue,bool xAxis){
	return xAxis?guiComponent.height*ratio:guiComponent.width/ratio;
}
std::vector<GuiAttribute*> GuiEqRatio::getDependencies(GuiComponent& guiComponent,bool xAxis){
	return std::vector<GuiAttribute*>{xAxis?&guiComponent.height:&guiComponent.width};
}
GuiEquation* GuiEqRatio::clone()const{
	return new GuiEqRatio(ratio);
} 
std::string GuiEqRatio::debugInformation(std::string tab){
	return tab+"# GuiEqRatio  ratio:"+std::to_string(ratio)+"\n";
}