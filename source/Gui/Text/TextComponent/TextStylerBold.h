#pragma once
#include "Gui/Text/TextComponent.h"
#include "Gui/Text/TextSettings.h"
#include "TextLine.h"

class TextStylerBold: public TextLine
{
public:
	TextStylerBold();
	TextStylerBold(sf::String text);
	~TextStylerBold();

	virtual TextComponent* copy()override;
	virtual void draw(Shader&shader,TextSettings& TextSettings,const double GuiComponentx,const double GuiComponenty,bool draw = true)override;

	virtual std::string getType()override;

	static TextStylerBold textStylerBold;
};