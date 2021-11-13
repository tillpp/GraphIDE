#pragma once

#include "GuiComponent.h"
#include "OpenGL/Texture.h"
#include "Font.h"
#include <SFML/Graphics.hpp>

struct SelectionBox{
	size_t start;
	size_t end;
};
class Text:public GuiComponent
{

	//font data
	Font* font;
	
	//data
	glm::vec4 color = glm::vec4(1,1,1,1);
	sf::String text;
	std::vector<SelectionBox*> selectionBoxs;
public:
	Text();
	~Text();


 	void addString(sf::String);
 	void addUtf8(std::string);
 	void setString(sf::String);
 	void setUtf8(std::string);
	virtual void draw(Shader& shader,Camera& camera,glm::mat4 inMatrix = glm::mat4(1.f)) override;

	/*
		shift: continues last selectionBox
		ctrl:  initiate selectionsboxes or cursors
	*/
	//SelectionBox& startSelectionBox(); //start
	//void pressClick(bool shift = false,bool ctrl = false);
	//void releaseClick();
	 
public:
	static Font standartFont;
};