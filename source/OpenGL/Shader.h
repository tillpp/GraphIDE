#pragma once
#include <GL\glew.h>
#include <SFML/Graphics.hpp>
#include <vector>
#include <mutex>

class Shader
{
	//thread
	std::recursive_mutex mutex;

	GLint program = -1;
	std::vector<GLuint> shaders;
	std::string pref = "[Shader]"; //prefix for errormessages. 
public:
	void create();
	void addVertexShaderFromMemory(sf::String inString);
	void addVertexShaderFromFile(sf::String inPath);
	void addFragmentShaderFromMemory(sf::String inString);
	void addFragmentShaderFromFile(sf::String inPath);
	void generate();
	~Shader();
	void use();
	void setGlobal(sf::String inName, std::vector<GLfloat> inValue);//uniform ...
	GLint getOpenGLID();
};

