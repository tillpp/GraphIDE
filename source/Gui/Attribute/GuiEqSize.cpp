#include "GuiEqSize.h"
#include "Gui/General/GuiComponent.h"

GuiEqSize::GuiEqSize(double percent)
{
	this->ratio = percent/100;
}

GuiEqSize::~GuiEqSize()
{
}

double GuiEqSize::evaluate(GuiComponent& guiComponent,const double& currentValue,bool xAxis){
	return xAxis?guiComponent.width*ratio:guiComponent.height*ratio;
}
std::vector<GuiAttribute*> GuiEqSize::getDependencies(GuiComponent& guiComponent,bool xAxis){
	return std::vector<GuiAttribute*>{xAxis?&guiComponent.width:&guiComponent.height};
}
GuiEquation* GuiEqSize::clone()const{
	return new GuiEqSize(ratio*100);
} 
std::string GuiEqSize::debugInformation(std::string tab){
	return tab+"# GuiEqSize  ratio:"+std::to_string(ratio)+"\n";
}