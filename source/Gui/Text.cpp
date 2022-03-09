#include "Text.h"
#include "Text/TextUnit.h"
#include "OpenGL/FontManager.h"

Text::Text(/* args */)
{
	std::lock_guard<std::recursive_mutex> lock(mutex);

	mlt = new MonoLineText();
	
	width.setEquation(GuiEqPixel(100));
	height.setEquation(GuiEqPixel(100));

}

Text::~Text()
{
}
void Text::draw(Shader& shader,Camera& camera,const double& x,const double& y){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	
	shader.use();
	camera.use(shader);

	TextSettings ts;
	ts.font = FontManager::getFont("arial.ttf");//SourceCodePro-Regular.ttf
	mlt->draw(shader,ts);

	GuiComponent::draw(shader, camera, x, y);
}
std::vector<GuiAttribute*> Text::getGuiAttributes(){
	return GuiComponent::getGuiAttributes();
}
std::string Text::getType(){
	return "Text";
}