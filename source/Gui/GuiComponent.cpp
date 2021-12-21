#include "GuiComponent.h"
#include <iostream>

GuiComponent::GuiComponent(/* args */)
:xpos(this,true),ypos(this,false),width(this,true),height(this,false)
{
}

GuiComponent::~GuiComponent()
{
	for (auto &child : children)
	{
		delete child;
	}
	children.clear();
	for (auto &feature : features)
	{
		delete feature;
	}
	features.clear();
}

bool GuiComponent::contain(const double& mousePositionX,const double& mousePositionY,const double& x,const double& y){
	auto relativeMousePositionX = mousePositionX - x;
	auto relativeMousePositionY = mousePositionY - y;

	if(relativeMousePositionX>xpos&&relativeMousePositionX<xpos+width
	&&relativeMousePositionY>ypos&&relativeMousePositionY<ypos+height)
		return true;
	return false;
}
void GuiComponent::draw(Shader& shader,Camera& camera,const double& x ,const double& y){
	for (auto &&child : children)
	{
		child->draw(shader,camera,x+xpos,y+ypos);
	}
	
}
std::vector<GuiAttribute*> GuiComponent::getGuiAttributes(){
	std::vector<GuiAttribute*> rv = {&width,&height,&xpos,&ypos};
	for (auto &fea : features)
	{
		auto attributes = fea->getGuiAttribute();
		rv.insert(rv.end(),attributes.begin(),attributes.end());
	}
	return rv;
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
	for (int i = 0; i < children.size(); i++)
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
void GuiComponent::removeFeature(GuiFeature* guiFeature){	
	for (int i = 0; i < features.size(); i++)
	{
		auto f = features[i];
		if(f==guiFeature){
			features.erase(features.begin()+i);
			i--;
		}
	}
}
void GuiComponent::triggerEvent(const GuiEvent& event){
	for(auto& feat:features){
		feat->handleEvent(event);
	}
}
void GuiComponent::updateHover(const double& mousex,const double& mousey,const double& parentx,const double& parenty){
	hover.update(contain(mousex,mousey,parentx,parenty));
	if(hover)
		triggerEvent(GuiEventHover(mousex-xpos,mousey-ypos));
	else if(hover==BoolTail::RELEASE)
		triggerEvent(GuiEventUnhover{mousex-xpos,mousey-ypos});
	
	for(auto& x:children)
		x->updateHover(mousex,mousey,parentx+xpos,parenty+ypos);
}