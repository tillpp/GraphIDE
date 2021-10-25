#include "Sprite.h"
#include "OpenGL/Mesh.h"
#include "OpenGL/Texture.h"

Sprite::Sprite(/* args */)
{
	matrix = glm::scale(matrix,glm::vec3(300,300,1));
}

Sprite::~Sprite()
{
}
void Sprite::draw(Shader &shader, Camera &camera, glm::mat4 inMatrix)
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	
	shader.use();

	//uniforms
	glUniform4f(glGetUniformLocation(shader.getOpenGLID(), "color"),1,1,1,1);
	camera.use(shader);
	if(texture)
		texture->use(0, shader, "texture1");
	else
		Texture::whiteTexture().use(0, shader, "texture1");

	Mesh::rectangle().draw(shader,inMatrix*matrix);
	GuiComponent::draw(shader,camera,inMatrix*matrix);
}

void Sprite::setColor(glm::vec4 inColor){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	color = inColor;
}
glm::vec4 Sprite::getColor(){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return color;
}
void Sprite::setTexture(Texture* inTexture){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	this->texture = inTexture;
}
Texture*  Sprite::getTexture(){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return texture;
}