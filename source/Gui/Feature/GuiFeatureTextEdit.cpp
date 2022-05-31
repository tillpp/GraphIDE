#include "GuiFeatureTextEdit.h"
#include "../Text.h"
#include "../../Util/Log.h"

GuiFeatureTextEdit::GuiFeatureTextEdit(GuiComponent* g)
	:GuiFeature(g){

}
GuiFeatureTextEdit::~GuiFeatureTextEdit(){

}

std::string GuiFeatureTextEdit::getType(){
	return "GuiFeatureTextEdit";
}
std::vector<GuiAttribute*> GuiFeatureTextEdit::getGuiAttribute(){
	return {};
}
bool GuiFeatureTextEdit::handleEvent(const GuiEvent& event){
	if(event.getType()==CLICK){
		GuiEventClick* e = (GuiEventClick*)&event;
		if(!e->direct)
			return false;
		
		if(component->getType()!="Text"){
			Log::error("GuiFeatureTextEdit is only allowed on Text components");
			return false;
		}
		Text* text = (Text*)component;
		glm::vec2 mousePositionRelative2Text(e->mousexInGui,e->mouseyInGui);
		Log::info(std::to_string(text->select_index(mousePositionRelative2Text)));
	}
	return false;
}