#include "GuiFeatureResize.h"
#include "../Attribute/GuiAttribute.h"
#include "../../Application/Application.h"
#include "../Event/GuiEvent.h"

GuiFeatureResize::GuiFeatureResize(GuiComponent* g)
	:GuiFeature(g),
	minWidth(g,true),
	minHeight(g,false),
	maxWidth(g,true),
	maxHeight(g,false)
{
}
GuiFeatureResize::~GuiFeatureResize()
{
}

std::string GuiFeatureResize::getType()
{
	return "GuiFeatureResize";
}
#include "../../Util/Log.h"

void GuiFeatureResize::handleEvent(const GuiEvent &event)
{
	if(event.getType()=="GuiEventHover"){
		GuiEventHover* e = (GuiEventHover*)&event;

		const size_t borderSize = 16; 
		bool sizeInX = e->mousex<borderSize||e->mousex>component->width-borderSize;
		bool sizeInY =e->mousey<borderSize||e->mousey>component->height-borderSize;
		onBorder.update(sizeInX||sizeInY);
			
		if(onBorder==BoolTail::START){
			sf::Cursor cursor;
			if (cursor.loadFromSystem(sizeInX?sf::Cursor::SizeHorizontal:sf::Cursor::SizeVertical))
		    	app().setCursor(cursor);
		}else if(onBorder==BoolTail::RELEASE){
			sf::Cursor cursor;
			if (cursor.loadFromSystem(sf::Cursor::Arrow))
	    		app().setCursor(cursor);
		}
	}else if(event.getType()=="GuiEventUnhover"){
		onBorder.update(0);
		sf::Cursor cursor;
		if (cursor.loadFromSystem(sf::Cursor::Arrow))
	    	app().setCursor(cursor);
	}
}
std::vector<GuiAttribute *> GuiFeatureResize::getGuiAttribute()
{
	return std::vector<GuiAttribute *>{&minWidth, &minHeight,& maxWidth, &maxHeight};
}
void GuiFeatureResize::setResizableX(bool) {}
void GuiFeatureResize::setResizableY(bool) {}
void GuiFeatureResize::setRatio(double ratio) {}