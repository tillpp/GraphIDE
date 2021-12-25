#pragma once
class GuiComponent;
class GuiEvent;
class GuiAttribute;
#include <vector>

#include <mutex>

class GuiFeature
{
protected:
	std::recursive_mutex mutex;

	friend GuiComponent;
	GuiComponent* component = nullptr;
	GuiFeature(GuiComponent*);
public:
	GuiFeature(GuiFeature&)=delete;
	~GuiFeature();	


	virtual std::string getType()=0;
protected:
	//return true to block further propagation of the event
	virtual bool handleEvent(const GuiEvent& event)=0;
	virtual std::vector<GuiAttribute*> getGuiAttribute()=0;
};
