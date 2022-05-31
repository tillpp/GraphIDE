#include "GuiComponent.h"
#include <iostream>

GuiComponent::GuiComponent(/* args */)
:xpos(this,true,"x"),ypos(this,false,"y"),width(this,true,"width"),height(this,false,"height"),animationHandler(*this)
{
}

GuiComponent::~GuiComponent()
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
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
	animationHandler.updateAttribute();
}
bool GuiComponent::contain(const double& mousePositionX,const double& mousePositionY){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	if(mousePositionX>cachedTotalPosX&&mousePositionX<cachedTotalPosX+width
	&&mousePositionY>cachedTotalPosY&&mousePositionY<cachedTotalPosY+height)
		return true;
	return false;
}
void GuiComponent::drawInner(Shader& shader,Camera& camera,const double& x ,const double& y){
}
void GuiComponent::draw(Shader& shader,Camera& camera,const double& x ,const double& y){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	animationHandler.update();
	
	drawInner(shader,camera,x,y);

	cachedTotalPosX = x+xpos;
	cachedTotalPosY = y+ypos;
	for (auto &&child : children)
	{
		child->draw(shader,camera,cachedTotalPosX,cachedTotalPosY);
	}
	
}
std::vector<GuiAttribute*> GuiComponent::getGuiAttributes(){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	/*
	####################    WARNING    ####################
	WARNING: If you edit this function: 
	What ever changes the attributes i.e. removes/adds them
	must also trigger a Animation.updateAttributes();
	####################################################### 
	*/
	std::vector<GuiAttribute*> rv = {&width,&height,&xpos,&ypos};
	for (auto &fea : features)
	{
		auto attributes = fea->getGuiAttribute();
		rv.insert(rv.end(),attributes.begin(),attributes.end());
	}
	return rv;
}
GuiComponent* GuiComponent::getParent(){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return parent;
}
const std::vector<GuiComponent*>& GuiComponent::getChildren(){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return children;
}
void GuiComponent::add(GuiComponent* g){
	std::lock_guard<std::recursive_mutex> lock(mutex);
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
	std::lock_guard<std::recursive_mutex> lock(mutex);
	if(parent)
		parent->remove(this);
	parent = nullptr;
	scene = nullptr;
}
void GuiComponent::remove(GuiComponent* g){
	std::lock_guard<std::recursive_mutex> lock(mutex);
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
	std::lock_guard<std::recursive_mutex> lock(mutex);
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
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return "";
}
void GuiComponent::removeFeature(GuiFeature* guiFeature){	
	std::lock_guard<std::recursive_mutex> lock(mutex);
	for (int i = 0; i < features.size(); i++)
	{
		auto f = features[i];
		if(f==guiFeature){
			features.erase(features.begin()+i);
			i--;
		}
	}
	animationHandler.updateAttribute();
}
void GuiComponent::triggerEvent(const GuiEvent& event){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	for(auto& feat:features){
		if(feat->handleEvent(event))
			return;
	}
}
double GuiComponent::getTotalPosX(){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return cachedTotalPosX;
}
double GuiComponent::getTotalPosY(){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return cachedTotalPosY;
}
Scene* GuiComponent::getScene(){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return scene;
}
std::vector<GuiComponent*> GuiComponent::getAncestors(){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	std::vector<GuiComponent*> rv;
	if(parent){
		rv = parent->getAncestors();
		rv.push_back(parent);
	}
	return rv;
}
GuiComponent* GuiComponent::getLastCommonAncestor(GuiComponent* bComponent){
	std::lock_guard<std::recursive_mutex> lock(mutex);
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
	std::lock_guard<std::recursive_mutex> lock(mutex);
	GuiComponent* rv = nullptr; 
	for(auto& x:children){
		auto hoveredComponent = x->getDirectHover(mousex,mousey);
		if(hoveredComponent){
			rv = hoveredComponent;
		}
	}
	if(rv)
		return rv;
	if(contain(mousex,mousey))
		return this;
	return nullptr;
}
GuiAttribute* GuiComponent::getGuiAttribute(std::string name){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	auto array = getGuiAttributes();
	for (auto &&i : array)
	{
		if(i->name==name)
			return i;
	}
	return nullptr;
}
Animation* GuiComponent::createAnimation(std::string name){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return animationHandler.createAnimation(name);
}
void GuiComponent::useAnimation(std::string name){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	animationHandler.use(name);
}
std::string GuiComponent::debugInformation(std::string tab){
	std::string rv;
	auto gua = getGuiAttributes();
	
	rv+= tab+"# "+getType()+std::to_string((int)this)+"\n";
	for(auto c:gua){
		rv+=c->debugInformation(tab+"\t");
	}
	return rv;
}