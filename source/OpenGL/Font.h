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
	size_t characterSize = 32;

	//glyphs
	std::set<unsigned int> glyphs; 
public:
	Font(std::string path,size_t characterSize = 32);
	Texture &getTexture();
	const sf::Glyph& getGlyph(unsigned int c);
	glm::vec4 getRelativTextureRectGlyph(const sf::Glyph&);

	//get Informations
	GLfloat getBaseline();
	GLfloat getLineSpacing();
	GLfloat getKerning(sf::String& string,unsigned int position);
	
	//rendering
	void use(Shader& shader);


	//remove Copy constructor
	Font(const Font&) = delete;
	~Font();
};

