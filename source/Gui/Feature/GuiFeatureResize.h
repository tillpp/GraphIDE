#pragma once
#include "GuiFeature.h"
#include "../GuiComponent.h"
#include "../../Util/BoolTail.h"

class GuiFeatureResize
	:public GuiFeature
{	
	GuiAttribute minWidth,minHeight,maxWidth,maxHeight;
	BoolTail onBorder;
public:
	GuiFeatureResize(GuiComponent*);
	~GuiFeatureResize();	

	virtual std::string getType();

	virtual void setResizableX(bool);
	virtual void setResizableY(bool);
	virtual void setRatio(double ratio);
protected:
	virtual void handleEvent(const GuiEvent& event)override;
	virtual std::vector<GuiAttribute*> getGuiAttribute()override;
};
