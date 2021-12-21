#include "GuiFeature.h"
#include "../Attribute/GuiAttribute.h"
#include "../GuiComponent.h"

GuiFeature::GuiFeature(GuiComponent* g)
{	
	component = g;
}

GuiFeature::~GuiFeature()
{
	component->removeFeature(this);
}