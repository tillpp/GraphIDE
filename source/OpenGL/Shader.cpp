#include "Shader.h"
#include "util/Log.h"

void Shader::create(){
	mutex.lock();
	program = glCreateProgram();
	mutex.unlock();
}
void Shader::addVertexShaderFromMemory(sf::String inString)
{
	mutex.lock();
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	auto conString = inString.toAnsiString();
	const GLchar* conStringAddr = conString.data();

	//Shader wird kompiliert
	glShaderSource(vs, 1, &conStringAddr, NULL);
	glCompileShader(vs);

	GLint status;
	GLchar tex[512];
	glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
	if (!status)
	{
		GLsizei size;
		glGetShaderInfoLog(vs, 512, &size, tex);
		std::string msg = "<VertexShader>COMPILATION FAILED! :";
		msg += sf::String(tex).substring(0, size);
		Log::error(pref+msg.data());
	}
	glAttachShader(program, vs);
	shaders.push_back(vs);
	mutex.unlock();
}
void Shader::addFragmentShaderFromMemory(sf::String inString)
{
	mutex.lock();
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	auto conString = inString.toAnsiString();
	const GLchar* conStringAddr = conString.data();

	//Shader wird kompiliert
	glShaderSource(fs, 1, &conStringAddr, NULL);
	glCompileShader(fs);

	GLint status;
	GLchar tex[512];
	glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
	if (!status)
	{
		GLsizei size;
		glGetShaderInfoLog(fs, 512, &size, tex);
		std::string msg = "<FragmentShader>COMPILATION FAILED! :";
		msg += sf::String(tex).substring(0, size);
		Log::error(pref+msg.c_str());
	}
	glAttachShader(program, fs);
	shaders.push_back(fs);
	mutex.unlock();
}
void Shader::generate()
{
	mutex.lock();
	glLinkProgram(program);

	GLint status;
	GLchar tex[512];
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (!status)
	{
		GLsizei size;
		glGetProgramInfoLog(program, 512, &size, tex);
		std::string msg = "<Linking>COMPILATION FAILED! :";
		msg += sf::String(tex).substring(0, size);
		Log::error(pref+ msg.c_str());
	}
	glUseProgram(program);
	for (size_t i = 0; i < shaders.size(); i++)
	{
		glDeleteShader(shaders[i]);
	}
	mutex.unlock();
}
void Shader::use()
{
	mutex.lock();
	glUseProgram(program);
	mutex.unlock();
}
void Shader::setGlobal(sf::String inName, std::vector<GLfloat> inValue)
{
	mutex.lock();
	GLint id = glGetUniformLocation(program, inName.toAnsiString().c_str());
	glUseProgram(program);
	if (inValue.size() == 1)
		glUniform1f(id, inValue[0]);
	else if (inValue.size() == 2)
		glUniform2f(id, inValue[0], inValue[1]);
	else if (inValue.size() == 3)
		glUniform3f(id, inValue[0], inValue[1], inValue[2]);
	else if (inValue.size() == 4)
		glUniform4f(id, inValue[0], inValue[1], inValue[2], inValue[3]);

	glUseProgram(0);
	mutex.unlock();
}
GLint Shader::getOpenGLID()
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return program;
}
Shader::~Shader(){
	if(program != -1)
		glDeleteProgram(program);
	program = -1;
}

#include <fstream>
#include <iostream>
#include <string>
void Shader::addFragmentShaderFromFile(sf::String inPath)
{
	mutex.lock();
	std::fstream file(std::string(inPath), std::ios::in);
	std::string line;
	std::string full;
	while (!file.eof())
	{

		std::getline(file, line);
		full += line;
		full += '\n';
	}
	file.close();
	addFragmentShaderFromMemory(full.c_str());
	mutex.unlock();
}
void Shader::addVertexShaderFromFile(sf::String inPath)
{
	mutex.lock();
	std::fstream file(std::string(inPath), std::ios::in);
	std::string line;
	std::string full;
	while (!file.eof())
	{
		std::getline(file, line);
		full += line;
		full += '\n';
	}
	file.close();
	addVertexShaderFromMemory(full.c_str());
	mutex.unlock();
}