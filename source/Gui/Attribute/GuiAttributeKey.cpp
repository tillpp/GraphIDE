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
void GuiAttributeKey::unregisterGuiAttribute(GuiAttribute* guiAttribute){
	for (int i = 0; i < locked.size(); i++)
	{
		if(locked[i]==guiAttribute){
			locked.erase(locked.begin()+i);
			i--;
		}
	}
	
}
