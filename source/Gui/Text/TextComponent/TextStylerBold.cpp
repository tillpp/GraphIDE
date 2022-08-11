#include "TextStylerBold.h"
#include "TextUnit.h"

TextStylerBold TextStylerBold::textStylerBold;

TextStylerBold::TextStylerBold(/* args */)
{
}
TextStylerBold::TextStylerBold(sf::String text)
{
	insert(new TextUnit(text));
}
TextStylerBold::~TextStylerBold()
{
}

TextComponent* TextStylerBold::copy(){
	auto rv = new TextStylerBold();
	for(auto& x:children){
		rv->children.push_back(x->copy());
	}
	rv->updateChildData();
	return rv;
}
#include "Gui/Text/CharacterEffect/RainbowEffect.h"
RainbowEffect rbe;
void TextStylerBold::draw(Shader&shader,TextSettings& textSettings,const double guiComponentx,const double guiComponenty,bool draw ){
	bool before = textSettings.bold;
	int beforeSize = textSettings.fontSize;
	textSettings.bold = true;
	textSettings.fontSize *= 1.2;
	//textSettings.effects.push_back(&rbe);
	TextLine::draw(shader,textSettings,guiComponentx,guiComponenty,draw);
	//textSettings.effects.pop_back();
	textSettings.bold = before;
	textSettings.fontSize = beforeSize;

}
std::string TextStylerBold::getType(){
	return "bold";
}