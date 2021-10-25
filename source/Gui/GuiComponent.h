#pragma once
#include <mutex>
#include "OpenGL/Shader.h"
#include "OpenGL/Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class GuiComponent
{
protected:
	//thread
	std::recursive_mutex mutex;
	//data
	glm::mat4 matrix= glm::mat4(1.f);
public:
	GuiComponent();
	~GuiComponent();

	virtual void draw(Shader& shader,Camera& camera,glm::mat4 inMatrix = glm::mat4(1.f));
};

