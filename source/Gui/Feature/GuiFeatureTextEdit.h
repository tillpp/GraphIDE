#pragma once
#include <vector>
#include <mutex>
#include "GuiFeature.h"
#include "Gui/General/GuiComponent.h"

class GuiFeatureTextEdit: public GuiFeature
{
public:
	GuiFeatureTextEdit(GuiComponent*);
	~GuiFeatureTextEdit();	


	virtual std::string getType();
protected:
	//return true to block further propagation of the event
	virtual bool handleEvent(const GuiEvent& event);
	virtual std::vector<GuiAttribute*> getGuiAttribute();
};
