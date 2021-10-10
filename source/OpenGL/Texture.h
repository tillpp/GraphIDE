#pragma once
#include <GL\glew.h>
#include <SFML\OpenGL.hpp>
#include <SFML\Graphics.hpp>
#include "Shader.h"
#include <mutex>


struct TextureSettings
{
	int Mipmaplevel = 0;
	bool linearInterpolation = true;
	bool flipVertically = false;
	TextureSettings() {}
};
class Texture
{
	//thread
	std::recursive_mutex mutex;
	//data
	GLuint textureID = -1;
	int sizex, sizey;

	//statics
	static Texture theWhiteTexture; 
public:
	Texture();
	~Texture();

	void LoadFromFile(sf::String inPath, TextureSettings inSettings = TextureSettings());
	void LoadFromTexture(sf::Texture inTexture, TextureSettings inSettings = TextureSettings());
	virtual void use(short inActivate, Shader& inShaderProgram, std::string inTextureNameInShader);
	bool isEmpty();
	
	//static functions
	static Texture& whiteTexture();
};
