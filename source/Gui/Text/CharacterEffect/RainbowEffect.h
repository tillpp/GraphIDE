#pragma once
#include "../CharacterEffect.h"
#include <SFML/System.hpp>

class RainbowEffect
:public CharacterEffect 
{
	sf::Clock clock;
public:
	RainbowEffect(/* args */);
	~RainbowEffect();

	virtual void preGlyph(
		const sf::String& line,
		const size_t& i,
		GLfloat& offset,
		glm::vec4& color,
		bool& bold,
		bool& italic,
		const TextSettings& ts)override;

	virtual void preDrawGlyph(
		const sf::String& line,
		const size_t& i,
		glm::vec4& drawRect,
		GLfloat&  advance,
		const TextSettings& ts,
		const bool drawing)override;

	virtual void postDrawGlyph(
		const sf::String& line,
		const size_t& i,
		const glm::vec4& drawRect,
		const GLfloat& advance,
		const TextSettings& ts,
		const bool drawing)override;
};

