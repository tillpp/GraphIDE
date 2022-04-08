#pragma once
#include "GuiEquation.h"

class GuiEqAttach
	:public GuiEquation
{

public:
	enum AttachType{BEFORE,AFTER};

	GuiEqAttach(const AttachType& at);
	~GuiEqAttach();

	virtual double evaluate(GuiComponent& guiComponent,const double& currentValue,bool xAxis)override;
	virtual std::vector<GuiAttribute*> getDependencies(GuiComponent& guiComponent,bool xAxis)override;
	virtual GuiEquation* clone()const override; 
	virtual std::string debugInformation(std::string tab)override;

	void setAttachType(const AttachType& at); 
private:
	AttachType attachType;
};
