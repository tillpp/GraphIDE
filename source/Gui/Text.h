#pragma once

#include "GuiComponent.h"
#include "OpenGL/Texture.h"
#include <SFML/Graphics.hpp>

class Text:public GuiComponent
{
	//data
	Texture texture;
	sf::Font* font;
	glm::vec4 color = glm::vec4(1,1,1,1);

	//temporary data
	sf::String lastText,text;
public:
	Text();
	~Text();


 	void setUtf8(std::string);
	virtual void draw(Shader& shader,Camera& camera,glm::mat4 inMatrix = glm::mat4(1.f)) override;

public:
	static sf::Font standartFont;
};