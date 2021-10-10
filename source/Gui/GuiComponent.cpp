#include "GuiComponent.h"

GuiComponent::GuiComponent(/* args */)
{
}

GuiComponent::~GuiComponent()
{
}
bool GuiComponent::getNeedRedraw(){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return needRedraw;
}
void GuiComponent::setNeedRedraw(bool inNeedRedraw){
	mutex.lock();
	needRedraw = inNeedRedraw;
	mutex.unlock();
}