#include "GuiEqFlex.h"
#include "Gui/General/GuiComponent.h"

GuiEqFlex::GuiEqFlex(/* args */)
{
}

GuiEqFlex::~GuiEqFlex()
{
}
double GuiEqFlex::evaluate(GuiComponent& guiComponent,const double& currentValue,bool xAxis){
	auto children = guiComponent.getChildren();
	double maxSize = 0;
	for (auto &child : children)
	{
		double childSize = xAxis?child->width+child->xpos:child->height+child->ypos;
		if(maxSize<childSize)
			maxSize = childSize;
	}
	return maxSize;
}
std::vector<GuiAttribute*> GuiEqFlex::getDependencies(GuiComponent& guiComponent,bool xAxis){
	auto children = guiComponent.getChildren();
	std::vector<GuiAttribute*> rv;
	for (auto &child : children)
	{
		std::vector<GuiAttribute*> attributes = xAxis
		?std::vector<GuiAttribute*>{&child->width,&child->xpos}
		:std::vector<GuiAttribute*>{&child->height,&child->ypos};
		rv.insert(rv.end(),attributes.begin(),attributes.end());
	}
	return rv;
}
GuiEquation* GuiEqFlex::clone()const{
	auto rv = new GuiEqFlex;
	return rv;
}
std::string GuiEqFlex::debugInformation(std::string tab){
	return tab+"# GuiEqFlex \n";
}