#pragma once
#include  "Gui/Text/TextGlyphEffect.h"

class RainbowEffect : public TextGlyphEffect
{
	sf::Clock clock;
public:
	RainbowEffect(/* args */);

	virtual void preGlyph(
		const TextSettings& textSettings,
		const sf::String& 	line,
		//changeable
		size_t		 		&index,
		GLfloat 			&offset,
		glm::vec4 			&color,
		bool 				&bold,
		bool				&italic
	)override;

	//before and after each Glyph drawing
	virtual void preDrawGlyph(
		const TextSettings& textSettings,
		const sf::String& 	line,
		const bool 			drawing,
		//changeable
		size_t 				&index,
		glm::vec4 			&drawRect,
		GLfloat  			&advance
	)override;

	virtual void postDrawGlyph(
		const TextSettings& textSettings,
		const sf::String& 	line,
		const bool 			drawing,
		const glm::vec4		&drawRect,
		//changeable
		size_t				&index,
		GLfloat				&advance
	)override;
};