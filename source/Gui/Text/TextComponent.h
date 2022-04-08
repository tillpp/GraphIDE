#pragma once
#include "TextSettings.h"

class TextComponent
{

public:
	TextComponent(/* args */);
	~TextComponent();

	virtual void draw(Shader&shader,TextSettings& TextSettings)=0;
	virtual std::string getType()=0;

	virtual int getWidth(const TextSettings &ts)=0;
	
	virtual int getHeight(const TextSettings &ts)=0;
	virtual int getYOffset(const TextSettings &ts)=0;

	/* if component is to big for one line, 
	*/
	virtual std::vector<TextComponent*> split(const int offset,const int width,const TextSettings &ts)=0;
	virtual bool merge(TextComponent* left)=0;
};