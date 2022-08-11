#pragma once
#include <vector>
#include "TextComponent.h"
#include <functional>

class TextBlock
{
	std::vector<TextComponent*> components;
public:
	double maxLineWidth = 100000;

	TextBlock(sf::String text);
	~TextBlock();

	void draw(Shader& shader,TextSettings ts,const double x,const double y);

	int getHeight(const TextSettings& ts);
	int getRealWidth(const TextSettings& ts);
	int getInnerWidth(const TextSettings& ts);
	
	//selectionBox
	int select_selectableCount();//amount of selectable objects.
	int select_index(glm::vec2 mousePositionRelative2Text,const TextSettings& ts);//index 

private:
	double lastWidth = 0;
	/*
		i is first ,j ist one past end of the components indexes for the line
	*/

	int calculateLineHeight(TextSettings ts,int i,int j);
	/* F get called*/
	void goThroughLine(TextSettings &ts,std::function<void(int& i,int& j,int lineHeight)>);
};

