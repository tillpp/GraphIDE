#include "Scene.h"
#include "Attribute/GuiEqPixel.h"

Scene::Scene(glm::vec2 size)
{
	screenFrame.width.setEquation(GuiEqPixel(size.x));
	screenFrame.height.setEquation(GuiEqPixel(size.y));

	setSize(size);
}

Scene::~Scene()
{
}

void Scene::setSize(glm::vec2 size){
	glm::mat4 viewProjection = glm::mat4(1.f);
	viewProjection = glm::translate(viewProjection,glm::vec3(-1,1,0));
	viewProjection = glm::scale(viewProjection,glm::vec3(1,-1,1));
	viewProjection = glm::scale(viewProjection,glm::vec3(2.f/size.x,2.f/size.y,1));
	this->setViewProjection(viewProjection);

	screenFrame.width.adjustEquation([size](GuiEquation* eq)->void{
		((GuiEqPixel*)eq)->setValue(size.x);
	});
	screenFrame.height.adjustEquation([size](GuiEquation* eq)->void{
		((GuiEqPixel*)eq)->setValue(size.y);
	});
}
void Scene::add(GuiComponent* guiComponent){
	screenFrame.add(guiComponent);
}
void Scene::draw(Shader& shader){
	shader.use();
	use(shader);	
	screenFrame.draw(shader,*this,0,0);
}