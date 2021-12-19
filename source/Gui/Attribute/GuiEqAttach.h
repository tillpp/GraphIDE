#pragma once
#include "GuiEquation.h"

class GuiEqAttach
	:public GuiEquation
{

public:
	enum AttachType{BEFORE,AFTER};

	GuiEqAttach(const AttachType& at);
	~GuiEqAttach();

	virtual double evaluate(GuiComponent& guiComponent,bool xAxis)override;
	virtual std::vector<GuiAttribute*> getDependencies(GuiComponent& guiComponent,bool xAxis)override;
	virtual GuiEquation* clone()const override; 

	void setAttachType(const AttachType& at); 
private:
	AttachType attachType;
};
