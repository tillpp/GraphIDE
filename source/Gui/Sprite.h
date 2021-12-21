#include "GuiComponent.h"
#include "OpenGL/Shader.h"
#include "OpenGL/Camera.h"
#include "OpenGL/Texture.h"

class Sprite
	:public GuiComponent
{
	glm::vec4 color = glm::vec4(1,1,1,1);
	Texture* texture = nullptr;

	friend GuiEqTexturePercent;
	friend GuiEqTextureHoldRatio;

	bool flipX = false,flipY = false; 
public:
	GuiAttribute texBoundXpos,texBoundYpos,texBoundWidth,texBoundHeight,texWidth,texHeight;

	Sprite();
	~Sprite();

	void setFlipX(bool x);
	bool getFlipX();

	void setFlipY(bool x);
	bool getFlipY();
	

	void setColor(glm::vec4 inColor);
	glm::vec4 getColor();	

	void setTexture(Texture* inColor);
	Texture* getTexture();	

	virtual void draw(Shader& shader,Camera& camera,const double& x,const double& y)override;
	virtual std::vector<GuiAttribute*> getGuiAttributes()override;

	virtual std::string getType()override;

	
};

