#include "GuiFeatureMove.h"
#include "../Attribute/GuiAttribute.h"
#include "../../Application/Application.h"
#include "../Event/GuiEvent.h"
#include "../Scene.h"
#include <limits>

GuiFeatureMove::GuiFeatureMove(GuiComponent *g)
	: GuiFeature(g),
	  borderLeft(g, true),
	  borderTop(g, false),
	  borderWidth(g, true),
	  borderHeight(g, false)
{
	//borderTop.setEquation(GuiEqPixel(-std::numeric_limits<double>::max()));
	//borderLeft.setEquation(GuiEqPixel(-std::numeric_limits<double>::max()));
	//borderWidth.setEquation(GuiEqPixel(std::numeric_limits<double>::max()));
	//borderHeight.setEquation(GuiEqPixel(std::numeric_limits<double>::max()));

	borderTop.setEquation(GuiEqPixel(0));
	borderLeft.setEquation(GuiEqPixel(0));
	borderWidth.setEquation(GuiEqPercent(100));
	borderHeight.setEquation(GuiEqPercent(100));
}
GuiFeatureMove::~GuiFeatureMove()
{
}

std::string GuiFeatureMove::getType()
{
	return "GuiFeatureMove";
}
#include "../../Util/Log.h"

//TODO:

bool GuiFeatureMove::handleEvent(const GuiEvent &event)
{
	if (event.getType() == GuiEventType::HOVERING)
	{
		GuiEventHovering *e = (GuiEventHovering *)&event;
		onHover.update(e->direct);
		updateCursor();
		return onHover;
	}
	else if (event.getType() == GuiEventType::UNHOVER)
	{
		onHover.update(false);
		updateCursor();
	}
	else if (event.getType() == GuiEventType::CLICK)
	{
		GuiEventClick *e = (GuiEventClick *)&event;
		if(!e->direct)
			return false;

		isMoving =! isMoving;		
		
		//store old values
		Scene* scene = component->getScene();
		if(isMoving&&scene){
			auto mousepos = scene->getInverseViewProjection()*app().getGLNormalizedMousePosition();
			oldMousePositionX=mousepos.x;
			oldMousePositionY=mousepos.y;

			oldXpos = component->xpos;
			oldYpos = component->ypos;
		}
		return isMoving;
	}
	else if (event.getType() == GuiEventType::UNSELECT){
		isMoving = false;	
		onHover.update(false);
		updateCursor();
	}
	else if (event.getType() == GuiEventType::SELECTING)
	{
		GuiEventSelecting *e = (GuiEventSelecting *)&event;
		Scene* scene = component->getScene();
		
		if(scene&&isMoving){
			auto mousepos = scene->getInverseViewProjection()*app().getGLNormalizedMousePosition();
			auto deltax = mousepos.x-oldMousePositionX;
			auto deltay = mousepos.y-oldMousePositionY;
			setAttribute(component->xpos,borderLeft,borderWidth-component->width,oldXpos+deltax);
			setAttribute(component->ypos,borderTop,borderHeight-component->height,oldYpos+deltay);
		}
		return isMoving;	
	}else if (event.getType() == GuiEventType::ATTRIBUTECHANGE){
		GuiEventAttributeChange* e = (GuiEventAttributeChange*)&event;
		
		if(&e->guiAttribute==&borderLeft||&e->guiAttribute==&borderWidth||&e->guiAttribute==&component->width){
			setAttribute(component->xpos,borderLeft,borderWidth-component->width,component->xpos);
		}else if(&e->guiAttribute==&borderTop||&e->guiAttribute==&borderHeight||&e->guiAttribute==&component->height){
			setAttribute(component->ypos,borderTop,borderHeight-component->height,component->ypos);
		}

	}
	return false;
}

std::vector<GuiAttribute *> GuiFeatureMove::getGuiAttribute()
{
	return std::vector<GuiAttribute *>{&borderLeft, &borderTop, &borderHeight, &borderWidth};
}

void GuiFeatureMove::setAttribute(GuiAttribute& attr,const double& min,const double& max,const double& value){
	if(max<value)
		return attr.overrideCachedValue(max);
	if(min>value)
		return attr.overrideCachedValue(min);
	return attr.overrideCachedValue(value);
}
void GuiFeatureMove::updateCursor(){
	if(onHover==BoolTail::START){
		sf::Cursor cursor;
		if (cursor.loadFromSystem(sf::Cursor::SizeAll))
			app().setCursor(cursor); 
	}else if(onHover==BoolTail::RELEASE){
		sf::Cursor cursor;
		if (cursor.loadFromSystem(sf::Cursor::Arrow))
			app().setCursor(cursor); 	
	}
}