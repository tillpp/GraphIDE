#pragma once
#include "TextBlock.h"

 class PolyLineText
 {
	
	std::vector<TextBlock> blocks;

public:
	PolyLineText(sf::String text);
	~PolyLineText();

	void draw(Shader& shader,TextSettings ts,const double x,const double y);

	int getHeight(const TextSettings& ts);
	int getRealWidth(const TextSettings& ts);
	int getInnerWidth(const TextSettings& ts);	

	//goal width, adjusts its blocks
	void adjust(const int width);

	//selectionBox
	int select_selectableCount();//amount of selectable objects.
	int select_index(glm::vec2 mousePositionRelative2Text,TextSettings ts);//index 

 };
 
 