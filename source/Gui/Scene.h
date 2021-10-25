#pragma once
#include "OpenGL/Camera.h"

class Scene
	:public Camera
{
public:
	Scene(glm::vec2 size);
	~Scene();

	void setSize(glm::vec2 size);
};