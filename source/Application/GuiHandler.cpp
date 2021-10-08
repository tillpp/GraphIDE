#include "GuiHandler.h"
#include <iostream>

GuiHandler::GuiHandler()
{
}

GuiHandler::~GuiHandler()
{
}
void GuiHandler::init(Application*){
	mutex.lock();
	shader.create();
	shader.addVertexShaderFromFile("res/shader/gui.ver");
	shader.addFragmentShaderFromFile("res/shader/gui.fra");
	shader.generate();

	mesh.create();
	mesh.LoadFromVertexArray({
		0,0,0, 0,0,
		0,1,0, 0,1,
		1,0,0, 1,0,
	},3);
	mesh.setSettingRead(0,3,false,5,0);
	mesh.setSettingRead(1,2,false,5,3);
	mutex.unlock();
}
void GuiHandler::draw(Application* app){
	mutex.lock();
	shader.use();
	glm::mat4 viewProjection = glm::mat4(1.f);
	
	glUniformMatrix4fv(glGetUniformLocation(shader.getOpenGLID(), "VP"), 1, GL_FALSE, glm::value_ptr(viewProjection));
	glUniform4f(glGetUniformLocation(shader.getOpenGLID(), "color"),0,1,0,1);
	
	Texture::whiteTexture().use(0,shader,"texture1");
	mesh.draw(shader);
	mutex.unlock();
}
void GuiHandler::update(Application* app){

}