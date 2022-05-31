#include "GuiEqAttach.h"
#include "Gui/General/GuiComponent.h"

GuiEqAttach::GuiEqAttach(const AttachType &at)
{
	attachType = at;
}

void GuiEqAttach::setAttachType(const AttachType &at)
{
	attachType = at;
}
GuiEqAttach::~GuiEqAttach()
{
}
double GuiEqAttach::evaluate(GuiComponent &guiComponent,const double& currentValue,bool xAxis)
{
	auto prev = guiComponent.getPrevious();
	if (prev)
	{
		if(attachType==BEFORE)
			return xAxis ? prev->xpos-guiComponent.width : prev->ypos-guiComponent.height;
		else if(attachType==AFTER)
			return xAxis ? prev->xpos+prev->width : prev->ypos+prev->height;
	}
	return 0;
}
std::vector<GuiAttribute *> GuiEqAttach::getDependencies(GuiComponent &guiComponent, bool xAxis)
{
	auto prev = guiComponent.getPrevious();
	if(!prev)
		return std::vector<GuiAttribute*>{};
	return xAxis ? 
		std::vector<GuiAttribute*>{&prev->xpos, &prev->width,&guiComponent.width}:
		std::vector<GuiAttribute*>{&prev->ypos, &prev->height,&guiComponent.height};
}
GuiEquation *GuiEqAttach::clone() const
{
	return new GuiEqAttach(attachType);
}
std::string GuiEqAttach::debugInformation(std::string tab){
	return tab+"# GuiEqAttach  AttachType:"+(attachType==AFTER?"AFTER":"BEFORE")+"\n";
}