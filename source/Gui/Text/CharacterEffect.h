#pragma once
#include "TextSettings.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class CharacterEffect
{
public:

	// //befor and after the full Text has been drawn
	// virtual void preDrawText(TextSettings& ts)=0;
	// virtual void postDrawText(TextSettings& ts)=0;

	/*
		Before Glyph is determined.
		Can be used to change font & boldness.
	*/
	virtual void preGlyph(
		const sf::String& line,
		const size_t& i,
		GLfloat& offset,
		glm::vec4& color,
		bool& bold,
		bool& italic,
		const TextSettings& ts)=0;

	//before and after each Glyph drawing
	virtual void preDrawGlyph(
		const sf::String& line,
		const size_t& i,
		glm::vec4& drawRect,
		GLfloat&  advance,
		const TextSettings& ts,
		const bool drawing)=0;

	virtual void postDrawGlyph(
		const sf::String& line,
		const size_t& i,
		const glm::vec4& drawRect,
		const GLfloat& advance,
		const TextSettings& ts,
		const bool drawing)=0;
		
};

