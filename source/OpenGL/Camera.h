#pragma once
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

class Camera
{
private:
	glm::mat4 viewProjection = glm::mat4(1.f);
	glm::mat4 inverseViewProjection = glm::mat4(1.f);
public:
	void setViewProjection(glm::mat4 viewProjection = glm::mat4(1.0f));
	void use(Shader& shader);
	glm::mat4 getInverseViewProjection();
};

