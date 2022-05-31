#include "Scene.h"
#include "Gui/Attribute/GuiEqPixel.h"
#include "Application/Application.h"


Scene::Scene(glm::vec2 size)
{
	screenFrame.width.setEquation(GuiEqPixel(size.x));
	screenFrame.height.setEquation(GuiEqPixel(size.y));
	screenFrame.scene = this;
	
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

	screenFrame.width.overrideCachedValue(size.x);
	screenFrame.height.overrideCachedValue(size.y);
}
void Scene::add(GuiComponent* guiComponent){
	screenFrame.add(guiComponent);
}
void Scene::draw(Shader& shader){
	shader.use();
	use(shader);	
	screenFrame.draw(shader,*this,0,0);
}
void Scene::update(){
	auto mousepos = getInverseViewProjection()*app().getGLNormalizedMousePosition();
	updateHover(mousepos.x,mousepos.y);
	updateClick(mousepos.x,mousepos.y);
	updateSelecting();
}
void Scene::updateHover(const double& mousex,const double& mousey){
	auto lastHovered = hovered;
	hovered = screenFrame.getDirectHover(mousex,mousey);

	//unhover
	if(lastHovered&&lastHovered!=hovered){
		auto LCA = lastHovered->getLastCommonAncestor(hovered);
		
		//trigger unhover old [direct]
		lastHovered->triggerEvent(GuiEventUnhover(mousex-lastHovered->getTotalPosX(),mousey-lastHovered->getTotalPosY(),true));
		
		//trigger unhover old [indirect]
		GuiComponent* iterator = lastHovered->getParent();
		while(iterator!=LCA)
		{
			iterator->triggerEvent(GuiEventUnhover(mousex-iterator->getTotalPosX(),mousey-iterator->getTotalPosY(),false));
			iterator = iterator->getParent();
		}		
	}
	//hover
	if(hovered){
		//direct
		hovered->triggerEvent(GuiEventHovering(mousex-hovered->getTotalPosX(),mousey-hovered->getTotalPosY(),true));

		//indirect
		auto iterator = hovered->getParent();
		while(iterator){
			iterator->triggerEvent(GuiEventHovering(mousex-iterator->getTotalPosX(),mousey-iterator->getTotalPosY(),false));
			iterator = iterator->getParent();
		}
	}
}

void Scene::updateClick(const double& mousex,const double& mousey){
	if(app().getKey(sf::Mouse::Left)==KeyState::clicked){
		
		if(hovered){
			//trigger click [direct]
			hovered->triggerEvent(GuiEventClick(mousex-hovered->getTotalPosX(),mousey-hovered->getTotalPosY(),true));
		
			//trigger click [indirect]
			GuiComponent* iterator = hovered->getParent();
			while(iterator!=nullptr)
			{
				iterator->triggerEvent(GuiEventClick(mousex-iterator->getTotalPosX(),mousey-iterator->getTotalPosY(),false));
				iterator = iterator->getParent();
			}
		}
	}
}
void Scene::updateSelecting(){
	
	if(app().getKey(sf::Mouse::Left)==KeyState::clicked){
		auto lastSelected = selected;
		selected = hovered;

		//trigger unselect	 	
		if(lastSelected&&lastSelected!=selected){
			lastSelected->triggerEvent(GuiEventUnselect());
		}
	}

	//trigger selected
	if(selected){
		selected->triggerEvent(GuiEventSelecting());
	}	
}