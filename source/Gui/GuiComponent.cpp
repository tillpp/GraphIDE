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
bool GuiComponent::contain(const double& mousePositionX,const double& mousePositionY){
	if(mousePositionX>cachedTotalPosX&&mousePositionX<cachedTotalPosX+width
	&&mousePositionY>cachedTotalPosY&&mousePositionY<cachedTotalPosY+height)
		return true;
	return false;
}
void GuiComponent::draw(Shader& shader,Camera& camera,const double& x ,const double& y){
	cachedTotalPosX = x+xpos;
	cachedTotalPosY = y+ypos;
	for (auto &&child : children)
	{
		child->draw(shader,camera,cachedTotalPosX,cachedTotalPosY);
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
	g->scene = this->scene;

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
	scene = nullptr;
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
	g->scene = nullptr;

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
		if(feat->handleEvent(event))
			return;
	}
}
double GuiComponent::getTotalPosX(){
	return cachedTotalPosX;
}
double GuiComponent::getTotalPosY(){
	return cachedTotalPosY;
}
Scene* GuiComponent::getScene(){
	return scene;
}
std::vector<GuiComponent*> GuiComponent::getAncestors(){
	std::vector<GuiComponent*> rv;
	if(parent){
		rv = parent->getAncestors();
		rv.push_back(parent);
	}
	return rv;
}
GuiComponent* GuiComponent::getLastCommonAncestor(GuiComponent* bComponent){
	auto listA = getAncestors();
	auto listB = bComponent?bComponent->getAncestors():std::vector<GuiComponent*>();

	GuiComponent* currentLCA = nullptr;
	size_t count = std::min(listA.size(),listB.size());
	for (size_t i = 0; i < count; i++){
		if(listA[i]==listB[i])
			currentLCA = listB[i];
	}

	return currentLCA;
}
GuiComponent* GuiComponent::getDirectHover(const double& mousex,const double& mousey){
	for(auto& x:children){
		auto hoveredComponent = x->getDirectHover(mousex,mousey);
		if(hoveredComponent){
			return hoveredComponent;
		}
	}
	if(contain(mousex,mousey))
		return this;
	return nullptr;
}
