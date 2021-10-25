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
	recorder.create(1280,720);

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
}
bool once = false;
void GuiHandler::draw(Application* app){
	mutex.lock();

    camera.setSize(app->getSize());

	app->record();
	shader.use();
	camera.use(shader);
	sprite.draw(shader,camera);

	mutex.unlock();
	return;

	mutex.lock();

	if(!once)
		recorder.record();
	else app->record();
	shader.use();
	
	glUniform4f(glGetUniformLocation(shader.getOpenGLID(), "color"),0,1,0,1);
	
	camera.use(shader);
	if(!once)
		Texture::whiteTexture().use(0,shader,"texture1");
	else 
		recorder.use(0,shader,"texture1");

	mesh.draw(shader);
	
	if(!once){
		recorder.save();
		once = true;
	}

	mutex.unlock();
}
void GuiHandler::update(Application* app){

}