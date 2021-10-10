#include "Mesh.h"
#include "Util/Log.h"

#define openGLErrorMessage(type) if(errorCode==type)Log::error(#type);
void openGLError()
{
	auto errorCode = glGetError();
	if (errorCode == GL_NO_ERROR)
		return;
	openGLErrorMessage(GL_INVALID_ENUM)
	openGLErrorMessage(GL_INVALID_VALUE)
	openGLErrorMessage(GL_INVALID_OPERATION)
	openGLErrorMessage(GL_INVALID_FRAMEBUFFER_OPERATION)
	openGLErrorMessage(GL_OUT_OF_MEMORY)
	openGLErrorMessage(GL_STACK_UNDERFLOW)
	openGLErrorMessage(GL_STACK_OVERFLOW)
}


void Mesh::move(glm::mat4 change){
	mutex.lock();
	mMatrix = change*mMatrix;
	mutex.unlock();
}
void Mesh::set(glm::mat4 change){
	mutex.lock();
	mMatrix = change;
	mutex.unlock();
}

void Mesh::LoadFromVertexArray(std::vector<GLfloat> inVertexArray, size_t inVertexCount)
{
	mutex.lock();
	mVertexCount = inVertexCount;

	//selektier VAO
	glBindVertexArray(mVAO);

	//selektiert VBO
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);

	//Daten hochladen
	glBufferData(GL_ARRAY_BUFFER, inVertexArray.size() * sizeof(GLfloat), inVertexArray.data(), GL_STATIC_DRAW);
	//DYNAMIC	STREAM

	//selektiert VBO
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	//selektier VAO
	glBindVertexArray(0);
	mutex.unlock();
}
void Mesh::LoadFromVertexArray(std::vector<GLfloat> inVertexArray, std::vector<GLuint> inFaceArray)
{
	mutex.lock();
	Log::info("Size of the VertexArray:"+std::to_string(inVertexArray.size()));

	nonAutoFaces = inFaceArray.size();

	glGenBuffers(1, &mEBO);

	//selektier VAO
	glBindVertexArray(mVAO);
	//selektiert VBO
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, inVertexArray.size() * sizeof(GLfloat), inVertexArray.data(), GL_STATIC_DRAW);
	//Daten hochladen
	//DYNAMIC	STREAM
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, inFaceArray.size() * sizeof(GLuint), inFaceArray.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	mutex.unlock();
}
void Mesh::create()
{
	mutex.lock();
	alreadyCreated = true;
	//create a Vertex Buffer Object
	glGenBuffers(1, &mVBO);
	//create a Vertex Buffer Object
	glGenVertexArrays(1, &mVAO);
	mutex.unlock();
}


void Mesh::setSettingRead(
	unsigned int inLayer,
	Byte inSizeOfVec,
	bool inNormal,
	unsigned int inStride,
	unsigned int inOffset)
{
	mutex.lock();
	//selektier VAO
	glBindVertexArray(mVAO);

	//selektiert VBO
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);

	//setSettings
	glVertexAttribPointer(inLayer, inSizeOfVec, GL_FLOAT, inNormal, inStride * sizeof(GLfloat), (GLvoid *)(inOffset * sizeof(GLfloat)));
	glEnableVertexAttribArray(inLayer);

	//selektier VAO
	glBindVertexArray(0);
	mutex.unlock();
}

void Mesh::draw(Shader &inShader, glm::mat4 parentMatrix)
{
	mutex.lock();
	//DEBUG COMMENT
	glUniformMatrix4fv(glGetUniformLocation(inShader.getOpenGLID(), "model"), 1, GL_FALSE, glm::value_ptr(parentMatrix * mMatrix));
	glBindVertexArray(mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	if (nonAutoFaces != 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
		glDrawElements(GL_TRIANGLES, nonAutoFaces, GL_UNSIGNED_INT, 0);
	}
	else
		glDrawArrays(GL_TRIANGLES, 0, mVertexCount);
	glBindVertexArray(0);
	mutex.unlock();
}
bool Mesh::isCreated(){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return alreadyCreated;
}
std::recursive_mutex& Mesh::getMutex(){
	return mutex;
}

Mesh Mesh::theRectangle;
Mesh& Mesh::rectangle(){
	std::lock_guard<std::recursive_mutex> lock(theRectangle.getMutex());
	if(theRectangle.isCreated())
		return theRectangle;
	theRectangle.create();
	theRectangle.LoadFromVertexArray({
		0,0,0, 0,0,
		0,1,0, 0,1,
		1,1,0, 1,1,
		0,0,0, 0,0,
		1,0,0, 1,0,
		1,1,0, 1,1,
	},6);
	theRectangle.setSettingRead(0,3,false,5,0);
	theRectangle.setSettingRead(1,2,false,5,2);	

	return theRectangle;
}
