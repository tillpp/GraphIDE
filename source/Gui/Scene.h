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
};