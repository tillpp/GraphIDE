#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class GuiEvent
{
public:
	GuiEvent(/* args */);
	~GuiEvent();

	virtual const std::string getType()const;
};










struct GuiEventHover:
	public GuiEvent
{	
	const double& mousex;
	const double& mousey;

	GuiEventHover(const double& mousex,const double& mousey);
	virtual const std::string getType()const;
};

struct GuiEventUnhover:
	public GuiEvent
{	
	const double& mousex;
	const double& mousey;

	GuiEventUnhover(const double& mousex,const double& mousey);
	virtual const std::string getType()const;
};