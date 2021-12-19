#include "GuiEqFlex.h"
#include "../GuiComponent.h"

GuiEqFlex::GuiEqFlex(/* args */)
{
}

GuiEqFlex::~GuiEqFlex()
{
}
double GuiEqFlex::evaluate(GuiComponent& guiComponent,bool xAxis){
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
		std::vector<GuiAttribute*> attributes = child->getGuiAttributes();
		rv.insert(rv.end(),attributes.begin(),attributes.end());
	}
	return rv;
}
GuiEquation* GuiEqFlex::clone()const{
	auto rv = new GuiEqFlex;
	return rv;
}