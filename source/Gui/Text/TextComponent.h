#pragma once
#include "TextSettings.h"

class TextComponent
{

public:
	TextComponent(/* args */);
	~TextComponent();

	virtual void draw(Shader&shader,TextSettings& TextSettings)=0;
	virtual std::string getType()=0;

	virtual int getWidth(TextSettings &ts)=0;
	
};