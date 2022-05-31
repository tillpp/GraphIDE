#pragma once
#include "TextSettings.h"

class TextComponent
{

public:
	bool needMeshRegeneration = false;

	TextComponent(/* args */);
	~TextComponent();

	virtual void draw(Shader&shader,TextSettings& TextSettings,const double x,const double y)=0;
	virtual std::string getType()=0;

	virtual int getWidth(const TextSettings &ts)=0;
	
	virtual int getHeight(const TextSettings &ts)=0;
	virtual int getYOffset(const TextSettings &ts)=0;

	/* if component is to big for one line, 
	*/
	virtual std::vector<TextComponent*> split(const int offset,const int width,const TextSettings &ts)=0;
	virtual bool merge(TextComponent* left)=0;

	//selectionBox
	virtual int select_selectableCount()=0;//amount of selectable objects.
	virtual int select_index(glm::vec2 mousePositionRelative2TC,const TextSettings& ts)=0;//index 
};