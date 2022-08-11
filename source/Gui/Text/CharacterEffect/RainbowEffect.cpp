#include "Gui/Text/CharacterEffect/RainbowEffect.h"


RainbowEffect::RainbowEffect(){
	clock.restart();
}

// Before Glyph is determined.
// Can be used to change font & boldness.
void RainbowEffect::preGlyph(
		const TextSettings& textSettings,
		const sf::String& 	line,
		//changeable
		size_t		 		&index,
		GLfloat 			&offset,
		glm::vec4 			&color,
		bool 				&bold,
		bool				&italic
	){
	auto t = ((float)clock.getElapsedTime().asMilliseconds()+(/*left+*/-offset)*10)/1000.f*2*M_PI;

	color.r = (sin(t)           +1)/2;
	color.g = (sin(t+M_PI*2  /3)+1)/2;
	color.b = (sin(t+M_PI*2*2/3)+1)/2;

	bold = true;
}

//before and after each Glyph drawing
void RainbowEffect::preDrawGlyph(
	const TextSettings& textSettings,
	const sf::String& 	line,
	const bool 			drawing,
	//changeable
	size_t 				&index,
	glm::vec4 			&drawRect,
	GLfloat  			&advance){

	}
void RainbowEffect::postDrawGlyph(
		const TextSettings& textSettings,
		const sf::String& 	line,
		const bool 			drawing,
		const glm::vec4		&drawRect,
		//changeable
		size_t				&index,
		GLfloat				&advance){

	}


