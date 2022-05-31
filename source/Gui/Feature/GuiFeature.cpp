#include "GuiFeature.h"
#include "../Attribute/GuiAttribute.h"
#include "Gui/General/GuiComponent.h"

GuiFeature::GuiFeature(GuiComponent* g)
{	
	component = g;
}

GuiFeature::~GuiFeature()
{
	component->removeFeature(this);
}