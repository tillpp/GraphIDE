#include "GuiEqTextPercent.h"
#include "Gui/Text.h"
#include "Util/Log.h"

GuiEqTextPercent::GuiEqTextPercent(double percent)
{
	ratio = percent/100;
}

GuiEqTextPercent::~GuiEqTextPercent()
{
}
void GuiEqTextPercent::setPercent(double percent){
	ratio = percent/100;
}

GuiEquation* GuiEqTextPercent::clone()const{
	return new GuiEqTextPercent(ratio*100);
} 


double GuiEqTextPercent::evaluate(GuiComponent& guiComponent,const double& currentValue,bool xAxis){
	if(guiComponent.getType()=="Text"){
		auto text = ((Text*)(&guiComponent));
		return xAxis?text->innerWidth*ratio:text->innerHeight*ratio;
	}

	Log::error("GuiEqTextPercent is not defined for GuiComponent "+guiComponent.getType());
	return 0;
}
std::vector<GuiAttribute*> GuiEqTextPercent::getDependencies(GuiComponent& guiComponent,bool xAxis){
	if(guiComponent.getType()=="Text"){
		auto text = (Text*)&guiComponent;
		return std::vector<GuiAttribute*>{xAxis?&text->innerWidth:&text->innerHeight};
	}
	Log::error("GuiEqTextPercent is not defined for GuiComponent "+guiComponent.getType());
	return std::vector<GuiAttribute*>{};
}
std::string GuiEqTextPercent::debugInformation(std::string tab){
	return tab+"# GuiEqTextPercent  ratio:"+std::to_string(ratio)+"\n";
}