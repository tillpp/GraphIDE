#include "GuiHandler.h"
#include <iostream>
#include "OpenGL/TextureManager.h"

GuiHandler::GuiHandler()
{
}

GuiHandler::~GuiHandler()
{
}
void GuiHandler::init(Application* app){
	mutex.lock();

	shader.create();
	shader.addVertexShaderFromFile("res/shader/gui.ver");
	shader.addFragmentShaderFromFile("res/shader/gui.fra");
	shader.generate();

	mesh.create();
	mesh.LoadFromVertexArray({
		1,1,0, 1,1,
		0,1,0, 0,1,
		1,0,0, 1,0,
	},3);
	mesh.setSettingRead(0,3,false,5,0);
	mesh.setSettingRead(1,2,false,5,3);
	mutex.unlock();

	sprite.setTexture(TextureManager::loadFromFile("res/texture/logo_gide.png"));
	text.setUtf8(	  u8"w		ok");
	text2.setUtf8(	  u8"wass	hmm");
}
bool once = false;
void GuiHandler::draw(Application* app){
	mutex.lock();

    camera.setSize(app->getSize());

	app->record();
	shader.use();
	camera.use(shader);
	//sprite.draw(shader,camera);
	text.addString(app->getText());
	text.draw(shader,camera);
	glm::mat4 mat(1.f);
	mat = glm::translate(mat,glm::vec3(0,50,0));
	text2.draw(shader,camera,mat);

	mutex.unlock();
}
void GuiHandler::update(Application* app){

}