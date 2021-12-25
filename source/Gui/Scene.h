#pragma once
#include "OpenGL/Camera.h"
#include "GuiComponent.h"

class Scene
	:public Camera 
{
	GuiComponent screenFrame;
public:
	Scene(glm::vec2 size);
	~Scene();

	void setSize(glm::vec2 size);

	void add(GuiComponent* guiComponent);
	void draw(Shader&);

	void update();

	//hover
	GuiComponent* hovered = nullptr;  
	GuiComponent* selected = nullptr; // hover + click  
private:
	void updateHover(const double& mousex,const double& mousey);
	void updateClick(const double& mousex,const double& mousey);
	void updateSelecting();
};