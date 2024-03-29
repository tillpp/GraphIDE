#include "Font.h"

Font::Font(std::string path, size_t characterSize)
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	font.loadFromFile(path);
	this->characterSize = characterSize;
}

Texture &Font::getTexture()
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return texture;
}
bool Font::GlyphMetaData::operator<(const GlyphMetaData& other)const{
	if(boldness == other.boldness)
		return character < other.character;
	return boldness;
}
const sf::Glyph &Font::getGlyph(unsigned int c,bool bold)
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	if(glyphs.find({c,bold})==glyphs.end()){
		font.getGlyph(c, characterSize, bold, 0);
		texture.LoadFromTexture(font.getTexture(characterSize));
		glyphs.insert({c,bold});
	}
	return font.getGlyph(c, characterSize, bold, 0);
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
{ //from baseline to the bottom
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return 1 - (font.getUnderlinePosition(characterSize) + font.getUnderlineThickness(characterSize)*2);
}
GLfloat Font::getLineSpacing()
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return font.getLineSpacing(characterSize);
}
GLfloat Font::getKerning(const sf::String &string, unsigned int i)
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	if (i)
		return font.getKerning(string[i - 1], string[i], characterSize);
	else
		return 0;
}
GLfloat Font::getUnderlinePosition(){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return font.getUnderlinePosition(characterSize);
}
GLfloat Font::getUnderlineThickness(){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return font.getUnderlineThickness(characterSize);
}
void Font::use(Shader &shader)
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	texture.use(0, shader, "texture1");
}
Font::~Font(){
}
GLfloat Font::getDisplayTextureCharacterRatio(const size_t& fontSize){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return ((GLfloat)fontSize)/characterSize;
}
