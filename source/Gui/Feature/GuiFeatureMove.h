#pragma once
#include "GuiFeature.h"
#include "../GuiComponent.h"
#include "../../Util/BoolTail.h"

/*
	Only works,if the component is part of a scene.
*/

class GuiFeatureMove
	:public GuiFeature
{	
	//settings
	GuiAttribute borderLeft,borderTop,borderWidth,borderHeight;
private:

	//cached
	BoolTail onHover;
	bool isMoving = false; 
	double oldXpos,oldYpos;

	double oldMousePositionX=0,oldMousePositionY=0;
public:
	GuiFeatureMove(GuiComponent*);
	~GuiFeatureMove();	

	virtual std::string getType();
protected:
	virtual bool handleEvent(const GuiEvent& event)override;
	virtual std::vector<GuiAttribute*> getGuiAttribute()override;

private:
	void setAttribute(GuiAttribute& attr,const double& min,const double& max,const double& value);
};
