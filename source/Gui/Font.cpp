#include "Font.h"

Font::Font(std::string path, size_t characterSize)
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	font.loadFromFile(path);
	this->characterSize = characterSize;
}

Font::~Font()
{
}
Texture &Font::getTexture()
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return texture;
}

const sf::Glyph &Font::getGlyph(unsigned int c)
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	if(glyphs.find(c)==glyphs.end()){
		font.getGlyph(c, characterSize, 0, 0);
		texture.LoadFromTexture(font.getTexture(characterSize));
		glyphs.insert(c);
	}
	return font.getGlyph(c, characterSize, 0, 0);
}
glm::vec4 Font::getRelativTextureRectGlyph(const sf::Glyph& glyph){
	glm::vec4 textureRect;
	/*left	*/ textureRect.x = (float)glyph.textureRect.left / texture.getSize().x;
	/*width	*/ textureRect.z = (float)glyph.textureRect.width / texture.getSize().x;
	/*top	*/ textureRect.y = (float)glyph.textureRect.top / texture.getSize().y;
	/*height*/ textureRect.w = (float)glyph.textureRect.height / texture.getSize().y;
	return textureRect;
}
GLfloat Font::getBaseline()
{ //from top to the baseline
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return 1 - (font.getUnderlinePosition(characterSize) + font.getUnderlineThickness(characterSize));
}
GLfloat Font::getLineSpacing()
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return font.getLineSpacing(characterSize);
}
GLfloat Font::getKerning(sf::String &string, unsigned int i)
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	if (i)
		return font.getKerning(string[i - 1], string[i], characterSize);
	else
		return 0;
}
void Font::use(Shader &shader)
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	texture.use(0, shader, "texture1");
}