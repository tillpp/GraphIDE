#include "Camera.h"
#include "Shader.h"

void Camera::use(Shader& shader){
	glUniformMatrix4fv(glGetUniformLocation(shader.getOpenGLID(), "VP"), 1, GL_FALSE, glm::value_ptr(viewProjection));
}