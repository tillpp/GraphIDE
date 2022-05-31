#include "GuiComponent.h"
#include "OpenGL/Shader.h"
#include "OpenGL/Camera.h"
#include "OpenGL/Texture.h"
#include "Gui/Text/TextComponent.h"
#include "Gui/Text/PolyLineText.h"

class Text
	:public GuiComponent
{
	PolyLineText plt;
	
public:
	GuiAttribute innerWidth,innerHeight;

	Text();
	~Text();

	virtual void drawInner(Shader& shader,Camera& camera,const double& x,const double& y)override;
	virtual std::vector<GuiAttribute*> getGuiAttributes()override;

	virtual std::string getType()override;	

	int select_index(glm::vec2 mousePositionRelative2Text);
};