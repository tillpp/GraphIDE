#pragma once
#include <mutex>
#include <SFML/graphics.hpp>
#include <OpenGL/Texture.h>
#include <set>

class Font 
{
private:
	std::recursive_mutex mutex;
	Texture texture;
	sf::Font font;
	size_t characterSize = 72;

	//glyphs
	struct GlyphMetaData{
		unsigned int character;
		bool boldness;
		bool operator<(const GlyphMetaData&) const;
	};
	std::set<GlyphMetaData> glyphs; 
public:
	Font(std::string path,size_t characterSize = 72);
	Texture &getTexture();
	const sf::Glyph& getGlyph(unsigned int c,bool bold=false);
	glm::vec4 getRelativTextureRectGlyph(const sf::Glyph&);

	//get Informations
	GLfloat getBaseline();
	GLfloat getLineSpacing();
	GLfloat getKerning(const sf::String& string,unsigned int position);
	
	//underline information
	GLfloat getUnderlinePosition();
	GLfloat getUnderlineThickness();

	//rendering
	void use(Shader& shader);
	GLfloat getDisplayTextureCharacterRatio(const size_t& fontSize);

	//remove Copy constructor
	Font(const Font&) = delete;
	~Font();
};

