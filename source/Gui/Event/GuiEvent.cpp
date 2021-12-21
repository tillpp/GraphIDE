#include "GuiEvent.h"

GuiEvent::GuiEvent(/* args */)
{
}

GuiEvent::~GuiEvent()
{
}

const std::string GuiEvent::getType()const{return "";}
const std::string GuiEventHover::getType()const{return "GuiEventHover";}
const std::string GuiEventUnhover::getType()const{return "GuiEventUnhover";}

GuiEventHover::GuiEventHover(const double& x,const double& y):mousex(x),mousey(y){}
GuiEventUnhover::GuiEventUnhover(const double& x,const double& y):mousex(x),mousey(y){}
