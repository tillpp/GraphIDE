#include "GuiComponent.h"
#include <iostream>

GuiComponent::GuiComponent(/* args */)
:xpos(this,true),ypos(this,false),width(this,true),height(this,false)
{
}

GuiComponent::~GuiComponent()
{
}

bool GuiComponent::contain(double mousePositionX,double mousePositionY,double x,double y){
	auto relativeMousePositionX = mousePositionX - x;
	auto relativeMousePositionY = mousePositionY - y;

	if(relativeMousePositionX>xpos&&relativeMousePositionX<xpos+width
	&&relativeMousePositionY>ypos&&relativeMousePositionY<ypos+height)
		return true;
	return false;
}
void GuiComponent::draw(Shader& shader,Camera& camera,double x ,double y){
	for (auto &&child : children)
	{
		child->draw(shader,camera,x+xpos,y+ypos);
	}
	
}
std::vector<GuiAttribute*> GuiComponent::getGuiAttributes(){
	return {&width,&height,&xpos,&ypos};
}
GuiComponent* GuiComponent::getParent(){
	return parent;
}
const std::vector<GuiComponent*>& GuiComponent::getChildren(){
	return children;
}
void GuiComponent::add(GuiComponent* g){
	g->disconnect();
	//parent connect
	children.push_back(g);
	g->parent = this;

	//attribute connect
	auto attributes = g->getGuiAttributes();
	for (auto & attr : attributes)
	{
		attr->connect();
	}
	
}
void GuiComponent::disconnect(){
	if(parent)
		parent->remove(this);
	parent = nullptr;
}
void GuiComponent::remove(GuiComponent* g){
	//disconnect attributes
	auto attributes = g->getGuiAttributes();
	for (auto & attr : attributes)
	{
		attr->deconnect();
	}
	//disconnect from parent
	for (size_t i = 0; i < children.size(); i++)
	{
		if(children[i]==g){
			children.erase(children.begin()+i);
			i--;
		}
	}
	g->parent = nullptr;

}
GuiComponent* GuiComponent::getPrevious(){
	if(!parent)
		return nullptr;
	auto& children = parent->getChildren();
	for (size_t i = 1; i < children.size(); i++){
		if(children[i]==this)
			return children[i-1];
	}
	return nullptr;
}
std::string GuiComponent::getType(){
	return "";
}