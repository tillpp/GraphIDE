#pragma once
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <mutex>
#include "Texture.h"
class Recorder
{
	std::recursive_mutex mutex;
	sf::RenderTexture renderTexture;
	Texture texture;
public:
	void create(int width,int height);
	
	void record();
	void save();

	void use(short inActivate, Shader& inShaderProgram, std::string inTextureNameInShader);
};

