#pragma once

#include "OpenGL/Font.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "TextGlyphEffect.h"

/*
	Stores information how a glyph should be drawn.
*/

struct TextSettings
{
	Font* font 					= nullptr;
	glm::vec4 textColor 		= glm::vec4(0,0,0,1);
	glm::vec4 backgroundColor 	= glm::vec4(1,1,1,0);
	bool bold					= false;
	bool italic					= false;
	bool underline				= false;
	bool strikethrough			= false;
	int fontSize				= 32;
	bool smallScript			= false;

	std::vector<TextGlyphEffect*> effects;

	//## No move constructor
	TextSettings() 					= default;
	TextSettings(TextSettings&& ) 	= delete;

	GLfloat getDisplayTextureCharacterRatio()const;
	
	//Font get Informations
	GLfloat getBaseline();
	GLfloat getLineSpacing();
	GLfloat getKerning(const sf::String& string,unsigned int position);
	
	//Font underline information
	GLfloat getUnderlinePosition();
	GLfloat getUnderlineThickness();

};
