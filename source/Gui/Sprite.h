#include "GuiComponent.h"
#include "OpenGL/Shader.h"
#include "OpenGL/Camera.h"
#include "OpenGL/Texture.h"

class Sprite
	:public GuiComponent
{
	glm::vec4 color = glm::vec4(1,1,1,1);
	Texture* texture = nullptr;
public:
	Sprite();
	~Sprite();

	void setColor(glm::vec4 inColor);
	glm::vec4 getColor();	

	void setTexture(Texture* inColor);
	Texture* getTexture();	

	virtual void draw(Shader& shader,Camera& camera,glm::mat4 matrix = glm::mat4(1.f));
};

