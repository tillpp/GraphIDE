#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "OpenGL/Font.h"

struct TextSettings;
class TextGlyphEffect
{
public:

	// Before Glyph is determined.
	// Can be used to change font & boldness.
	virtual void preGlyph(
		const TextSettings& textSettings,
		const sf::String& 	line,
		//changeable
		size_t		 		&index,
		GLfloat 			&offset,
		glm::vec4 			&color,
		bool 				&bold,
		bool				&italic
	)=0;

	//before and after each Glyph drawing
	virtual void preDrawGlyph(
		const TextSettings& textSettings,
		const sf::String& 	line,
		const bool 			drawing,
		//changeable
		size_t 				&index,
		glm::vec4 			&drawRect,
		GLfloat  			&advance)=0;

	virtual void postDrawGlyph(
		const TextSettings& textSettings,
		const sf::String& 	line,
		const bool 			drawing,
		const glm::vec4		&drawRect,
		//changeable
		size_t				&index,
		GLfloat				&advance)=0;
};

#include "TextSettings.h"