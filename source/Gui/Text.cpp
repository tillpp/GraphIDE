#include "Text.h"
#include "Text/TextUnit.h"
#include "OpenGL/FontManager.h"

Text::Text(/* args */):
	innerWidth(this,true,"text.full.width"),innerHeight(this,false,"text.full.height"),plt(
	"hello World x̃ŷz̄\n "
	"\nHow are you?\n"
	"I am fine\n"
	"Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor\n"
	"invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.\n"
	"At vero eos et accusam et justo duo dolores et ea rebum.\nyo")
{
	std::lock_guard<std::recursive_mutex> lock(mutex);

	width.setEquation(GuiEqPixel(500));
	height.setEquation(GuiEqTextPercent(100));

}

Text::~Text()
{
}
#include "Text/CharacterEffect/RainbowEffect.h"
#include <iostream>
void Text::drawInner(Shader& shader,Camera& camera,const double& x,const double& y){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	
	shader.use();
	camera.use(shader);

	//textsettings
	TextSettings ts;
	// ts.x = x + xpos;
	// ts.y = y + ypos;
	ts.font = FontManager::getFont("arial.ttf");//SourceCodePro-Regular.ttf

	//update innerWidth & innerHeigth
	innerWidth.overrideCachedValue(plt.getInnerWidth(ts));
	innerHeight.overrideCachedValue(plt.getHeight(ts));

	plt.adjust(width);

	//draw
	plt.draw(shader,ts,x+xpos,y+ypos);
}
int Text::select_index(glm::vec2 mousePositionRelative2Text){
	//textsettings
	TextSettings ts;
	// ts.x = x + xpos;
	// ts.y = y + ypos;
	ts.font = FontManager::getFont("arial.ttf");//SourceCodePro-Regular.ttf
	
	return plt.select_index(mousePositionRelative2Text,ts);
}
std::vector<GuiAttribute*> Text::getGuiAttributes(){
	return GuiComponent::getGuiAttributes();
}
std::string Text::getType(){
	return "Text";
}