#include "GuiComponent.h"
#include "OpenGL/Shader.h"
#include "OpenGL/Camera.h"
#include "OpenGL/Texture.h"

#include "Gui/Text/TextSelectionBoxManager.h"

class Text
	:public GuiComponent
{
	// ## Test
	TextComponent* textComponent;
	double lastX=0,lastY=0;

	TextSelectionBoxManager textSelectionBoxManager;
	sf::Clock cursorKeyboardMovementClock;
	sf::Int32 cursorKeyboardMovementDelay = 1000;
public:
	GuiAttribute innerWidth,innerHeight;

	Text();
	~Text();

	virtual void drawInner(Shader& shader,Camera& camera,const double& x,const double& y)override;
	virtual std::vector<GuiAttribute*> getGuiAttributes()override;

	virtual std::string getType()override;	

	virtual void handleEvent(const GuiEvent& event)override;
	// ## Test
	int select_index(glm::vec2 mousePosition2GuiComponent);
};