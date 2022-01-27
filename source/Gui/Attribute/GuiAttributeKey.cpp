#include "GuiAttributeKey.h"

GuiAttributeKey::~GuiAttributeKey()
{
	
	for (GuiAttribute*& attr : locked)
	{
		attr->unlock(this);
	}	
}
void GuiAttributeKey::registerGuiAttribute(GuiAttribute* guiAttribute){
	locked.push_back(guiAttribute);
}