#include "GuiEqTextureHoldRatio.h"
#include "../GuiComponent.h"
#include "../../Util/Log.h"
#include "../Sprite.h"
#include "../../OpenGL/Texture.h"

GuiEqTextureHoldRatio::GuiEqTextureHoldRatio()
{
}

GuiEqTextureHoldRatio::~GuiEqTextureHoldRatio()
{
}
double GuiEqTextureHoldRatio::evaluate(GuiComponent& guiComponent,const double& currentValue,bool xAxis){
	if(guiComponent.getType()=="Sprite"){
		auto sprite = ((Sprite*)(&guiComponent));
		auto ratio = sprite->texWidth/sprite->texHeight;
		return xAxis?guiComponent.height*ratio:guiComponent.width/ratio;
	}

	Log::error("GuiEqTextureHoldRatio is not defined for GuiComponent "+guiComponent.getType());
	return 0;
}
std::vector<GuiAttribute*> GuiEqTextureHoldRatio::getDependencies(GuiComponent& guiComponent,bool xAxis){
	if(guiComponent.getType()=="Sprite"){
		Sprite* sprite = (Sprite*)&guiComponent;
		return std::vector<GuiAttribute*>{xAxis?&guiComponent.height:&guiComponent.width,&sprite->texWidth,&sprite->texHeight};		 
	}

	Log::error("GuiEqTextureHoldRatio is not defined for GuiComponent "+guiComponent.getType());
	return std::vector<GuiAttribute*>{};
}
GuiEquation* GuiEqTextureHoldRatio::clone()const{
	return new GuiEqTextureHoldRatio;
} 
std::string GuiEqTextureHoldRatio::debugInformation(std::string tab){
	return tab+"# GuiEqTextureHoldRatio \n";
}