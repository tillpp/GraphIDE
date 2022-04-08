#include "GuiEqTexturePercent.h"
#include "../GuiComponent.h"
#include "../../Util/Log.h"
#include "../Sprite.h"
#include "../../OpenGL/Texture.h"

GuiEqTexturePercent::GuiEqTexturePercent(double percent)
{
	ratio = percent/100;
}

GuiEqTexturePercent::~GuiEqTexturePercent()
{
}
double GuiEqTexturePercent::evaluate(GuiComponent& guiComponent,const double& currentValue,bool xAxis){
	if(guiComponent.getType()=="Sprite"){
		auto sprite = ((Sprite*)(&guiComponent));
		return xAxis?sprite->texWidth*ratio:sprite->texHeight*ratio;
	}

	Log::error("GuiEqTexturePercent is not defined for GuiComponent "+guiComponent.getType());
	return 0;
}
std::vector<GuiAttribute*> GuiEqTexturePercent::getDependencies(GuiComponent& guiComponent,bool xAxis){
	if(guiComponent.getType()=="Sprite"){
		auto sprite = (Sprite*)&guiComponent;
		return std::vector<GuiAttribute*>{xAxis?&sprite->texWidth:&sprite->texHeight};
	}
	Log::error("GuiEqTexturePercent is not defined for GuiComponent "+guiComponent.getType());
	return std::vector<GuiAttribute*>{};
}
GuiEquation* GuiEqTexturePercent::clone()const{
	return new GuiEqTexturePercent(ratio*100);
} 
void GuiEqTexturePercent::setPercent(double percent){
	ratio = percent/100;
}
std::string GuiEqTexturePercent::debugInformation(std::string tab){
	return tab+"# GuiEqTexturePercent  ratio:"+std::to_string(ratio)+"\n";
}