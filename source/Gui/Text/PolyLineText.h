#pragma once
#include "TextBlock.h"

 class PolyLineText
 {
	
	std::vector<TextBlock> blocks;

public:
	PolyLineText(sf::String text);
	~PolyLineText();

	void draw(Shader& shader,TextSettings ts);

	int getHeight(const TextSettings& ts);
	int getRealWidth(const TextSettings& ts);
	int getInnerWidth(const TextSettings& ts);	

	//goal width, adjusts its blocks
	void adjust(const int width);
 };
 
 