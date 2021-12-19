#include "Camera.h"
#include "Shader.h"

void Camera::use(Shader& shader){
	glUniformMatrix4fv(glGetUniformLocation(shader.getOpenGLID(), "VP"), 1, GL_FALSE, glm::value_ptr(viewProjection));
}
void Camera::setViewProjection(glm::mat4 viewProjection){
	this->viewProjection  = viewProjection;
	inverseViewProjection = glm::inverse(viewProjection);
}
glm::mat4 Camera::getInverseViewProjection(){
	return inverseViewProjection;
}