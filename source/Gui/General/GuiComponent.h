#pragma once
class Scene;
#include <mutex>
#include "OpenGL/Shader.h"
#include "OpenGL/Camera.h"
#include "OpenGL/Mesh.h"
#include "Gui/Attribute/GuiAttribute.h"
#include "Gui/Attribute/GuiEqPixel.h"
#include "Gui/Attribute/GuiEqPercent.h"
#include "Gui/Attribute/GuiEqMax.h"
#include "Gui/Attribute/GuiEqMin.h"
#include "Gui/Attribute/GuiEqFlex.h"
#include "Gui/Attribute/GuiEqAdd.h"
#include "Gui/Attribute/GuiEqSub.h"
#include "Gui/Attribute/GuiEqMult.h"
#include "Gui/Attribute/GuiEqAttach.h"
#include "Gui/Attribute/GuiEqRatio.h"
#include "Gui/Attribute/GuiEqTexturePercent.h"
#include "Gui/Attribute/GuiEqTextureHoldRatio.h"
#include "Gui/Attribute/GuiEqSize.h"
#include "Gui/Attribute/GuiEqTextPercent.h"

#include "Gui/Feature/GuiFeature.h"
#include "Gui/Feature/GuiFeatureResize.h"
#include "Gui/Feature/GuiFeatureMove.h"
#include "Gui/Feature/GuiFeatureTextEdit.h"
#include "Gui/Event/GuiEvent.h"

#include "Util/BoolTail.h"
#include "Util/Node.h"

#include "Gui/Animation/AnimationHandler.h"

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
	AnimationHandler			animationHandler;

	//cached data
	double cachedTotalPosX,cachedTotalPosY;
public:
	GuiAttribute width,height;
	GuiAttribute xpos, ypos;

	GuiComponent();
	GuiComponent(GuiComponent&)=delete;
	
	~GuiComponent();

	virtual bool contain(const double& mousePositionX,const double& mousePositionY);
	void draw(Shader& shader,Camera& camera,const double& parentx = 0,const double& parenty = 0);
private:
	//only accessed by draw
 	virtual void drawInner(Shader& shader,Camera& camera,const double& parentx = 0,const double& parenty = 0);
public:

	GuiComponent* getParent();
	const std::vector<GuiComponent*>& getChildren();
	virtual std::vector<GuiAttribute*> getGuiAttributes();

	GuiAttribute* getGuiAttribute(std::string name);

	void add(GuiComponent*);
	void disconnect();
	void remove(GuiComponent*);

	GuiComponent* getPrevious();

	virtual std::string getType();
	
	template<class Feature,class... Args>
	Feature* addFeature(Args... args){
		auto feature = new Feature(this,args...);
		features.push_back(feature); 
		animationHandler.updateAttribute();
		return feature;
	}

	void removeFeature(GuiFeature*);
	void triggerEvent(const GuiEvent& event);
	virtual void handleEvent(const GuiEvent& event);
	

	double getTotalPosX();
	double getTotalPosY();
	Scene* getScene();

	std::vector<GuiComponent*> getAncestors();
	GuiComponent* getLastCommonAncestor(GuiComponent*);

	//returns the directHoveredComponent
	virtual GuiComponent* getDirectHover(const double& mousex,const double& mousey);

	// ANIMATION
	Animation* 	createAnimation(std::string name);
	void		   useAnimation(std::string name);

	std::string debugInformation(std::string tabs="");
};

