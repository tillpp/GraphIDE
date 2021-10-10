#pragma once
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include <mutex>
#define Byte unsigned char

void openGLError();

class Mesh
{
	//thread
	std::recursive_mutex mutex;

	//data
 	unsigned int nonAutoFaces = 0;
 	GLuint mEBO;
 	GLuint mVAO;
 	GLuint mVBO;

 	glm::vec3 mOrigin 	= glm::vec3(0, 0, 0);
 	glm::vec3 mPosition = glm::vec3(0, 0, 0);
 	glm::vec3 mRotation = glm::vec3(0, 0, 0);
 	glm::vec3 mScale 	= glm::vec3(1, 1, 1);

 	glm::mat4 mMatrix = glm::mat4(1.f);
 	size_t mVertexCount = 0;

	bool alreadyCreated = false;
public:
	void move(glm::mat4 change);
	void set(glm::mat4 change);
	
 	void LoadFromVertexArray(std::vector<GLfloat> inVertexArray,size_t inVertexCount);
 	void LoadFromVertexArray(std::vector<GLfloat> inVertexArray, std::vector<GLuint> inFaceArray);
 	void setSettingRead(unsigned int inLayer, Byte inSizeOfVec, bool inNormal, unsigned int inStride, unsigned int inOffset);
 	void draw(Shader& inShader,glm::mat4 =  glm::mat4(1.f));
 	void create();

	bool isCreated();
	std::recursive_mutex& getMutex();

	static Mesh& rectangle();
private:
	static Mesh theRectangle;
};

