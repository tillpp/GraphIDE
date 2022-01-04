#include "GuiEvent.h"

GuiEvent::GuiEvent(/* args */)
{
}

GuiEvent::~GuiEvent()
{
}

const GuiEventType GuiEvent::getType()const{return COUNT;}
const GuiEventType GuiEventHovering::getType()const{return HOVERING;}
const GuiEventType GuiEventUnhover::getType()const{return UNHOVER;}
const GuiEventType GuiEventClick::getType()const{return CLICK;}
const GuiEventType GuiEventSelecting::getType()const{return SELECTING;}
const GuiEventType GuiEventUnselect::getType() const {return UNSELECT;}
const GuiEventType GuiEventAttributeChange::getType() const {return ATTRIBUTECHANGE;}

GuiEventHovering::GuiEventHovering(const double& x,const double& y,const bool& d):mousexInGui(x),mouseyInGui(y),direct(d){}
GuiEventUnhover::GuiEventUnhover(const double& x,const double& y,const bool& d):mousexInGui(x),mouseyInGui(y),direct(d){}
GuiEventClick::GuiEventClick(const double& x,const double& y,const bool& d):mousexInGui(x),mouseyInGui(y),direct(d){}
GuiEventSelecting::GuiEventSelecting(){}
GuiEventUnselect::GuiEventUnselect(){}
GuiEventAttributeChange::GuiEventAttributeChange(const GuiAttribute& ga):guiAttribute(ga){}
