#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

enum GuiEventType{
		HOVERING,
		UNHOVER,
		CLICK,
		SELECTING,
		UNSELECT,
		COUNT
};
class GuiEvent
{
public:
	GuiEvent(/* args */);
	~GuiEvent();

	virtual const GuiEventType getType()const;
};






struct GuiEventHovering:
	public GuiEvent
{	
	const double& mousexInGui;
	const double& mouseyInGui;
	const bool& direct;

	GuiEventHovering(const double& mousexInGui,const double& mouseyInGui,const bool& direct);
	virtual const GuiEventType getType()const;
};

struct GuiEventUnhover:
	public GuiEvent
{	
	const double& mousexInGui;
	const double& mouseyInGui;
	const bool& direct;

	GuiEventUnhover(const double& mousexInGui,const double& mouseyInGui,const bool& direct);
	virtual const GuiEventType getType()const;
};
struct GuiEventClick:
	public GuiEvent
{	
	const double& mousexInGui;
	const double& mouseyInGui;
	const bool& direct;

	GuiEventClick(const double& mousexInGui,const double& mouseyInGui,const bool& direct);
	virtual const GuiEventType getType()const;
};

struct GuiEventSelecting:
	public GuiEvent
{	
	GuiEventSelecting();
	virtual const GuiEventType getType()const;
};
struct GuiEventUnselect:
	public GuiEvent
{	
	GuiEventUnselect();
	virtual const GuiEventType getType()const;
};
