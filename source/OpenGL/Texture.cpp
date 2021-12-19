#include "Texture.h"
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

Texture::Texture(/* args */)
{
}

Texture::~Texture()
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	if (textureID!=(GLuint)-1)
		glDeleteTextures(1,&textureID);
	textureID = -1;
}
void Texture::LoadFromFile(sf::String inPath, TextureSettings inSettings)
{
	mutex.lock();
	glBindTexture(GL_TEXTURE_2D, textureID);

	//Does the Texture already exist?
	if (textureID!=-1)
		glDeleteTextures(1, &textureID);
	
	//generate texture and select it
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	if (inSettings.linearInterpolation) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	//load it
	sf::Image texture;
	texture.loadFromFile(inPath);
	texture.flipVertically();

	sizex = texture.getSize().x;
	sizey = texture.getSize().y;


	glTexImage2D(GL_TEXTURE_2D, inSettings.Mipmaplevel, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	mutex.unlock();
}
void Texture::use(short inActivate, Shader& inShaderProgram, std::string inTextureNameInShader)
{
	mutex.lock();
	glActiveTexture(GL_TEXTURE0 + inActivate);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(glGetUniformLocation(inShaderProgram.getOpenGLID(), inTextureNameInShader.c_str()), inActivate);
	mutex.unlock();
}
void Texture::LoadFromTexture(
	sf::Texture inTexture,
	TextureSettings inSettings)
{
	mutex.lock();
	glBindTexture(GL_TEXTURE_2D, textureID);
	if (textureID!=-1)
		glDeleteTextures(1, &textureID);
	
	//generate texture and select it 
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	// Set texture filtering
	if (inSettings.linearInterpolation) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	//load it
	sf::Image texture = inTexture.copyToImage();
	if(inSettings.flipVertically)
		texture.flipVertically();

	sizex = texture.getSize().x;
	sizey = texture.getSize().y;
	
	glTexImage2D(GL_TEXTURE_2D, inSettings.Mipmaplevel, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	mutex.unlock();
}

bool Texture::isEmpty(){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return textureID==-1;
}

Texture Texture::theWhiteTexture; 
Texture & Texture::whiteTexture()
{
	if(theWhiteTexture.isEmpty()){
		sf::Image image;
		sf::Texture texture;
		image.create(1,1,sf::Color::White);
		texture.loadFromImage(image);
		theWhiteTexture.LoadFromTexture(texture);
	
	}
	return theWhiteTexture;
}
glm::vec2 Texture::getSize(){
	std::lock_guard<std::recursive_mutex> lock(mutex);

	return glm::vec2(sizex,sizey);
}
double Texture::getRatio(){
	return (double)sizex/sizey;
}