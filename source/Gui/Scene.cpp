#include "Scene.h"

Scene::Scene(glm::vec2 size)
{
	viewProjection = glm::mat4(1.f);
	viewProjection = glm::translate(viewProjection,glm::vec3(-1,1,0));
	viewProjection = glm::scale(viewProjection,glm::vec3(1,-1,1));
	viewProjection = glm::scale(viewProjection,glm::vec3(2.f/size.x,2.f/size.y,1));
}

Scene::~Scene()
{
}

void Scene::setSize(glm::vec2 size){
	viewProjection = glm::mat4(1.f);
	viewProjection = glm::translate(viewProjection,glm::vec3(-1,1,0));
	viewProjection = glm::scale(viewProjection,glm::vec3(1,-1,1));
	viewProjection = glm::scale(viewProjection,glm::vec3(2.f/size.x,2.f/size.y,1));
}