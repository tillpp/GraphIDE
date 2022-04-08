#include "RainbowEffect.h"

RainbowEffect::RainbowEffect(/* args */)
{
	clock.restart();
}

RainbowEffect::~RainbowEffect()
{
}
void RainbowEffect::preGlyph(
	const sf::String &line,
	const size_t &i,
	GLfloat &offset,
	glm::vec4 &color,
	bool &bold,
	bool& italic,
	const TextSettings &ts)
{
	auto t = ((float)clock.getElapsedTime().asMilliseconds()+(ts.x+offset)*10)/1000.f*2*M_PI;
	
	color.r = (sin(t)           +1)/2;
	color.g = (sin(t+M_PI*2  /3)+1)/2;
	color.b = (sin(t+M_PI*2*2/3)+1)/2;
	
}
void RainbowEffect::preDrawGlyph(
	const sf::String &line,
	const size_t &i,
	glm::vec4 &drawRect,
	GLfloat &advance,
	const TextSettings &ts,
	const bool drawing)
{
	//GLfloat dtcr = ts.getDisplayTextureCharacterRatio();
	//drawRect.y += sin(((float)clock.getElapsedTime().asMilliseconds()+drawRect.x*10)/1000.f*2*M_PI)*dtcr*ts.font->getLineSpacing()/2;

}

void RainbowEffect::postDrawGlyph(
	const sf::String &line,
	const size_t &i,
	const glm::vec4 &drawRect,
	const GLfloat &advance,
	const TextSettings &ts,
	const bool drawing)
{
}