#include "GuiFeatureResize.h"
#include "../Attribute/GuiAttribute.h"
#include "../../Application/Application.h"
#include "../Event/GuiEvent.h"
#include "../Scene.h"
#include <limits>

GuiFeatureResize::GuiFeatureResize(GuiComponent *g)
	: GuiFeature(g),
	  minWidth(g, true),
	  minHeight(g, false),
	  maxWidth(g, true),
	  maxHeight(g, false)
{
	maxWidth.setEquation(GuiEqPixel(std::numeric_limits<double>::max()));
	maxHeight.setEquation(GuiEqPixel(std::numeric_limits<double>::max()));
}
GuiFeatureResize::~GuiFeatureResize()
{
}

std::string GuiFeatureResize::getType()
{
	return "GuiFeatureResize";
}
#include "../../Util/Log.h"



bool GuiFeatureResize::handleEvent(const GuiEvent &event)
{
	if (event.getType() == GuiEventType::HOVERING )
	{
		GuiEventHovering *e = (GuiEventHovering *)&event;

		bool sizeInX =  (resizableLeft && e->mousexInGui < borderSize) ||
						(resizableRight && e->mousexInGui > component->width - borderSize);
		bool sizeInY =  (resizableTop && e->mouseyInGui < borderSize) ||
						(resizableBottom && e->mouseyInGui > component->height - borderSize);
		onBorderX.update(sizeInX && e->direct);
		onBorderY.update(sizeInY && e->direct);

		updateCursor();
		return onBorderX||onBorderY;
	}
	else if (event.getType() == GuiEventType::UNHOVER)
	{
		onBorderX.update(false);
		onBorderY.update(false);
		sf::Cursor cursor;
		if(!isResizing)
			if (cursor.loadFromSystem(sf::Cursor::Arrow))
				app().setCursor(cursor);
	}
	else if (event.getType() == GuiEventType::CLICK)
	{
		GuiEventClick *e = (GuiEventClick *)&event;
		if (resizableLeft && e->mousexInGui < borderSize)
			resizingX = LOW;
		if (resizableTop &&e->mouseyInGui < borderSize)
			resizingY = LOW;

		if (resizableRight &&e->mousexInGui > component->width - borderSize)
			resizingX = HIGH;
		if (resizableBottom &&e->mouseyInGui > component->height - borderSize)
			resizingY = HIGH;
	
		if(resizingX!=NONE||resizingY!=NONE){
			isResizing = true;
			//store old values
			oldTotalXpos = component->getTotalPosX();
			oldTotalYpos = component->getTotalPosY();
			
			oldWidth = component->width;
			oldHeight = component->height;
			
			oldXpos = component->xpos;
			oldYpos = component->ypos;
			
		}
		return isResizing;
	}
	else if (event.getType() == GuiEventType::UNSELECT){
		resizingX = NONE;
		resizingY = NONE;
		isResizing = false;	
		sf::Cursor cursor;
		if (cursor.loadFromSystem(sf::Cursor::Arrow))
			app().setCursor(cursor);
	}
	else if (event.getType() == GuiEventType::SELECTING)
	{
		GuiEventSelecting *e = (GuiEventSelecting *)&event;
		Scene* scene = component->getScene();
		if(scene&&isResizing){
			auto mousepos = scene->getInverseViewProjection()*app().getGLNormalizedMousePosition();
			if(resizingX==HIGH)
				setAttribute(component->width,minWidth,maxWidth,mousepos.x-oldTotalXpos);
			if(resizingY==HIGH)
				setAttribute(component->height,minHeight,maxHeight,mousepos.y-oldTotalYpos);
			if(resizingX==LOW){
				double delta = mousepos.x-oldTotalXpos;
				setAttribute(component->width,minWidth,maxWidth,oldWidth-delta);
				setAttribute(component->xpos,
					oldXpos+oldWidth-maxWidth,
					oldXpos+oldWidth-minWidth,
					oldXpos+delta);
			}
			if(resizingY==LOW){
				double delta = mousepos.y-oldTotalYpos;
				setAttribute(component->height,minHeight,maxHeight,oldHeight-delta);
				setAttribute(component->ypos,
					oldYpos+oldHeight-maxHeight,
					oldYpos+oldHeight-minHeight,
					oldYpos+delta);
			}
		}
		return isResizing;
	}
	return false;
}
void GuiFeatureResize::updateCursor()
{
	if (onBorderX == BoolTail::START || onBorderX == BoolTail::RELEASE || onBorderY == BoolTail::START || onBorderY == BoolTail::RELEASE)
	{
		sf::Cursor cursor;
		if (onBorderX)
		{
			if (cursor.loadFromSystem(sf::Cursor::SizeHorizontal))
				app().setCursor(cursor);
		}
		else if (onBorderY)
		{
			if (cursor.loadFromSystem(sf::Cursor::SizeVertical))
				app().setCursor(cursor);
		}
		else if(!isResizing)
			if (cursor.loadFromSystem(sf::Cursor::Arrow))
				app().setCursor(cursor);
	}
}

std::vector<GuiAttribute *> GuiFeatureResize::getGuiAttribute()
{
	return std::vector<GuiAttribute *>{&minWidth, &minHeight, &maxWidth, &maxHeight};
}
void GuiFeatureResize::setResizableLeft(bool b){
	resizableLeft = b;
}
void GuiFeatureResize::setResizableTop(bool b){
	resizableTop = b;
}
void GuiFeatureResize::setResizableRight(bool b){
	resizableRight = b;
}
void GuiFeatureResize::setResizableBottom(bool b){
	resizableBottom = b;
}
void GuiFeatureResize::setAttribute(GuiAttribute& attr,const double& min,const double& max,const double& value){
	if(max<value)
		return attr.overrideCachedValue(max);
	if(min>value)
		return attr.overrideCachedValue(min);
	return attr.overrideCachedValue(value);
}