#include "GuiFeatureResize.h"
#include "../Attribute/GuiAttribute.h"
#include "../../Application/Application.h"
#include "../Event/GuiEvent.h"
#include "../Scene.h"
#include <limits>

GuiFeatureResize::GuiFeatureResize(GuiComponent *g)
	: GuiFeature(g),
	  minWidth(g, true,"resize.min.width"),
	  minHeight(g, false,"resize.min.height"),
	  maxWidth(g, true,"resize.max.width"),
	  maxHeight(g, false,"resize.max.height")
{
	maxWidth.setEquation(GuiEqPixel(std::numeric_limits<double>::max()));
	maxHeight.setEquation(GuiEqPixel(std::numeric_limits<double>::max()));
	//maxHeight.setEquation(GuiEqPercent(100));
	//maxWidth.setEquation(GuiEqPercent(100));
	
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
	if (event.getType() == GuiEventType::HOVERING)
	{
		GuiEventHovering *e = (GuiEventHovering *)&event;
		if(!e->direct)
			return false;

		bool sizeInX =  (resizableLeft && e->mousexInGui < borderSize) ||
						(resizableRight && e->mousexInGui > component->width - borderSize);
		bool sizeInY =  (resizableTop && e->mouseyInGui < borderSize) ||
						(resizableBottom && e->mouseyInGui > component->height - borderSize);
		onBorderX.update(sizeInX);
		onBorderY.update(sizeInY);

		updateCursor();
		return onBorderX||onBorderY;
	}
	else if (event.getType() == GuiEventType::UNHOVER)
	{
		onBorderX.update(false);
		onBorderY.update(false);
		updateCursor();
	}
	else if (event.getType() == GuiEventType::CLICK)
	{
		GuiEventClick *e = (GuiEventClick *)&event;
		if(!e->direct)
			return false;

		if (resizableLeft && e->mousexInGui < borderSize)
			resizingX = LOW;
		if (resizableTop &&e->mouseyInGui < borderSize)
			resizingY = LOW;

		if (resizableRight &&e->mousexInGui > component->width - borderSize)
			resizingX = HIGH;
		if (resizableBottom &&e->mouseyInGui > component->height - borderSize)
			resizingY = HIGH;
	
		if(resizingX!=NONE||resizingY!=NONE){
			isResizing =! isResizing;
			//store old values
			oldTotalXpos = component->getTotalPosX();
			oldTotalYpos = component->getTotalPosY();
			
			oldWidth = component->width;
			oldHeight = component->height;
			
			oldXpos = component->xpos;
			oldYpos = component->ypos;
			
		}else 
			isResizing = false;

		return isResizing;
	}
	else if (event.getType() == GuiEventType::UNSELECT){
		resizingX = NONE;
		resizingY = NONE;
		isResizing = false;	
		updateCursor();
	}
	else if (event.getType() == GuiEventType::SELECTING)
	{
		GuiEventSelecting *e = (GuiEventSelecting *)&event;
		Scene* scene = component->getScene();
		if(scene&&isResizing){
			auto mousepos = scene->getInverseViewProjection()*app().getGLNormalizedMousePosition();
			if(resizingX==HIGH)
				setAttribute(component->width,minWidth,maxWidth,mousepos.x-component->getTotalPosX());
			if(resizingY==HIGH)
				setAttribute(component->height,minHeight,maxHeight,mousepos.y-component->getTotalPosY());
			if(resizingX==LOW){
				double newposition = mousepos.x-(component->getParent()?component->getParent()->getTotalPosX():0);
				
				double rightSide = component->xpos+component->width;
				setAttribute(component->xpos,
					rightSide-maxWidth,
					rightSide-minWidth,
					newposition);
				setAttribute(component->width,minWidth,maxWidth,rightSide-newposition);
			}
			if(resizingY==LOW){
				double newposition = mousepos.y-(component->getParent()?component->getParent()->getTotalPosY():0);
				
				double downSide = component->ypos+component->height;
				setAttribute(component->ypos,
					downSide-maxHeight,
					downSide-minHeight,
					newposition);
				setAttribute(component->height,minHeight,maxHeight,downSide-newposition);
			}
		}
		return isResizing;
	}else if (event.getType() == GuiEventType::ATTRIBUTECHANGE){
		GuiEventAttributeChange* e = (GuiEventAttributeChange*)&event;
		
		if(&e->guiAttribute==&minWidth||&e->guiAttribute==&maxWidth){
			setAttribute(component->width,minWidth,maxWidth,component->width);
		}else if(&e->guiAttribute==&minHeight||&e->guiAttribute==&maxHeight){
			setAttribute(component->height,minHeight,maxHeight,component->height);
		}
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