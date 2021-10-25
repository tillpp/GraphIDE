#pragma once
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

class Camera
{
protected:
	glm::mat4 viewProjection = glm::mat4(1.f);
public:
	void use(Shader& shader);
	 
};

