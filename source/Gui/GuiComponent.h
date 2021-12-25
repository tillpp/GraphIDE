#pragma once
class Scene;
#include <mutex>
#include "OpenGL/Shader.h"
#include "OpenGL/Camera.h"
#include "Attribute/GuiAttribute.h"
#include "Attribute/GuiEqPixel.h"
#include "Attribute/GuiEqPercent.h"
#include "Attribute/GuiEqMax.h"
#include "Attribute/GuiEqMin.h"
#include "Attribute/GuiEqFlex.h"
#include "Attribute/GuiEqAdd.h"
#include "Attribute/GuiEqSub.h"
#include "Attribute/GuiEqMult.h"
#include "Attribute/GuiEqAttach.h"
#include "Attribute/GuiEqRatio.h"
#include "Attribute/GuiEqTexturePercent.h"
#include "Attribute/GuiEqTextureHoldRatio.h"
#include "Attribute/GuiEqSize.h"

#include "Feature/GuiFeature.h"
#include "Feature/GuiFeatureResize.h"
#include "Feature/GuiFeatureMove.h"
#include "Event/GuiEvent.h"

#include "Util/BoolTail.h"
#include "Util/Node.h"

class GuiComponent
{
protected:
	friend GuiAttribute;
	friend GuiFeature;
	friend Scene;
	
	//thread
	std::recursive_mutex mutex;
	//data
	Scene* 						scene = nullptr;
	GuiComponent* 				parent = nullptr;
	std::vector<GuiComponent*> 	children;
	std::vector<GuiFeature*> 	features;

	//cached data
	double cachedTotalPosX,cachedTotalPosY;
public:
	GuiAttribute width,height;
	GuiAttribute xpos, ypos;

	GuiComponent();
	GuiComponent(GuiComponent&)=delete;
	
	~GuiComponent();

	virtual bool contain(const double& mousePositionX,const double& mousePositionY);
	virtual void draw(Shader& shader,Camera& camera,const double& parentx = 0,const double& parenty = 0);
	
	GuiComponent* getParent();
	const std::vector<GuiComponent*>& getChildren();
	virtual std::vector<GuiAttribute*> getGuiAttributes();

	void add(GuiComponent*);
	void disconnect();
	void remove(GuiComponent*);

	GuiComponent* getPrevious();

	virtual std::string getType();
	
	template<class Feature,class... Args>
	Feature* addFeature(Args... args){
		auto feature = new Feature(this,args...);
		features.push_back(feature); 
		return feature;
	}

	void removeFeature(GuiFeature*);
	void triggerEvent(const GuiEvent& event);

	double getTotalPosX();
	double getTotalPosY();
	Scene* getScene();

	std::vector<GuiComponent*> getAncestors();
	GuiComponent* getLastCommonAncestor(GuiComponent*);

	//returns the directHoveredComponent
	virtual GuiComponent* getDirectHover(const double& mousex,const double& mousey);
};

