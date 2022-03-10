#include "GuiComponent.h"
#include "OpenGL/Shader.h"
#include "OpenGL/Camera.h"
#include "OpenGL/Texture.h"
#include "Text/TextComponent.h"
#include "Text/PolyLineText.h"

class Text
	:public GuiComponent
{
	PolyLineText plt;
public:
	Text();
	~Text();

	virtual void draw(Shader& shader,Camera& camera,const double& x,const double& y)override;
	virtual std::vector<GuiAttribute*> getGuiAttributes()override;

	virtual std::string getType()override;	
};