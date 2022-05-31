#pragma once
#include "GuiFeature.h"
#include "Gui/General/GuiComponent.h"
#include "../../Util/BoolTail.h"

/*
	Only works,if the component is part of a scene.
*/

class GuiFeatureResize
	:public GuiFeature
{	
	//settings
	static const size_t borderSize = 16;
	GuiAttribute minWidth,minHeight,maxWidth,maxHeight;
private:
	bool resizableLeft = 1,
		resizableTop = 1,
		resizableRight= true,
		resizableBottom= true;

	//cached
	BoolTail onBorderX,onBorderY;
	bool isResizing = false; 
	double oldTotalXpos,oldTotalYpos;
	double oldXpos,oldYpos;
	double oldWidth,oldHeight; 
	enum{NONE,LOW,HIGH} resizingX = NONE,resizingY = NONE;

public:
	GuiFeatureResize(GuiComponent*);
	~GuiFeatureResize();	

	virtual std::string getType();

	virtual void setResizableLeft(bool);
	virtual void setResizableTop(bool);
	virtual void setResizableRight(bool);
	virtual void setResizableBottom(bool);
protected:
	virtual bool handleEvent(const GuiEvent& event)override;
	virtual std::vector<GuiAttribute*> getGuiAttribute()override;

private:
	void updateCursor();
	void setAttribute(GuiAttribute& attr,const double& min,const double& max,const double& value);
};
