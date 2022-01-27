#pragma once
class GuiAttribute;
#include "GuiAttribute.h"
#include <mutex>

class GuiAttributeKey
{
	std::recursive_mutex mutex;

	friend GuiAttribute;
	std::vector<GuiAttribute*> locked;

	void registerGuiAttribute(GuiAttribute*);
public:
	~GuiAttributeKey();
};


