#pragma once
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

class GuiComponent
{
protected:
	friend GuiAttribute;
	//thread
	std::recursive_mutex mutex;
	//data
	GuiComponent* 				parent = nullptr;
	std::vector<GuiComponent*> 	children;

public:
	GuiAttribute width,height;
	GuiAttribute xpos, ypos;
	
	GuiComponent();
	GuiComponent(GuiComponent&)=delete;
	
	~GuiComponent();

	virtual bool contain(double mousePositionX,double mousePositionY,double x = 0,double y = 0);
	virtual void draw(Shader& shader,Camera& camera,				 double x = 0,double y = 0);

	GuiComponent* getParent();
	const std::vector<GuiComponent*>& getChildren();
	virtual std::vector<GuiAttribute*> getGuiAttributes();

	void add(GuiComponent*);
	void disconnect();
	void remove(GuiComponent*);

	GuiComponent* getPrevious();

	virtual std::string getType();
};

