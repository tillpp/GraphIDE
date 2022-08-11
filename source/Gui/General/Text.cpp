#include "Text.h"
#include "OpenGL/FontManager.h"
#include "Application/Application.h"
#include "Util/Log.h"
#include "Scene.h"


#include "Gui/Text/TextComponent/TextLine.h"
#include "Gui/Text/TextComponent/TextStylerBold.h"

Text::Text(/* args */):
	innerWidth(this,true,"text.full.width"),innerHeight(this,false,"text.full.height"),textSelectionBoxManager(textComponent)
{
	std::lock_guard<std::recursive_mutex> lock(mutex);

	width.setEquation(GuiEqPixel(500));
	height.setEquation(GuiEqTextPercent(100));

	// ## Test
	textComponent = new TextLine("lorem ipsum");
}

Text::~Text()
{
}
void Text::drawInner(Shader& shader,Camera& camera,const double& x,const double& y){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	shader.use();
	camera.use(shader);
	
	//TextSettings
	TextSettings standartSettings;
	standartSettings.font = FontManager::getFont("arial.ttf");//SourceCodePro-Regular.ttf//arial.ttf

	//draw
	textComponent->top = 0;
	textComponent->draw(shader,standartSettings,x+xpos,y+ypos);
	lastX = x;
	lastY = y;

	//update innerWidth & innerHeigth
	innerWidth.overrideCachedValue(textComponent->width);
	innerHeight.overrideCachedValue(textComponent->height);
}
std::vector<GuiAttribute*> Text::getGuiAttributes(){
	return GuiComponent::getGuiAttributes();
}
std::string Text::getType(){
	return "Text";
}
void Text::handleEvent(const GuiEvent& event){
	if(event.getType()==GuiEventType::HOVERING){
		GuiEventHovering *e = (GuiEventHovering*)&event;
		if(!e->direct||!app().getKey(sf::Mouse::Left))return;
		
		//direct click on text
		int index = textComponent->cursorSlotIndex(glm::vec2(e->mousexInGui,e->mouseyInGui));

		if(app().getKey(sf::Mouse::Left)==BoolTail::START){
			
			if(!app().getKey(sf::Keyboard::LShift))
			{	
				if(!app().getKey(sf::Keyboard::LAlt))
					textSelectionBoxManager.removeAll();
				textSelectionBoxManager.start(index);
			}else 
				textSelectionBoxManager.end(index);

			//double,triple,... clicks
			if(app().getConsecutiveLeftMouseClicks() > 1)
			{
				if(!app().getKey(sf::Keyboard::LShift))
					textSelectionBoxManager.removeAll();
				
				int cursorSlotLeft  = textSelectionBoxManager.nextStructuralCursorSlot(index, false, app().getConsecutiveLeftMouseClicks()-1);
				int cursorSlotRight = textSelectionBoxManager.nextStructuralCursorSlot(index, true , app().getConsecutiveLeftMouseClicks()-1);

				textSelectionBoxManager.expand(cursorSlotLeft, cursorSlotRight);
			}
		}
	}
	if(event.getType()==GuiEventType::UNSELECT){
		textSelectionBoxManager.removeAll();
	}
	if(event.getType()==GuiEventType::SELECTING){
		GuiEventSelecting *e = (GuiEventSelecting*)&event;
		// ## Mouse ##
		if(app().getKey(sf::Mouse::Left) && app().getConsecutiveLeftMouseClicks() == 1){
			auto mousepos 	= getScene()->getInverseViewProjection()*app().getGLNormalizedMousePosition();
			int index 		= textComponent->cursorSlotIndex(glm::vec2(mousepos.x-getTotalPosX(),mousepos.y-getTotalPosY()));
			textSelectionBoxManager.end(index);
		}
		// ## Keyboard - Backspace ##
		if(app().getKey(sf::Keyboard::Backspace)) {
			if(app().isKeyboardTextMove())
				textSelectionBoxManager.deleteText(false,app().getKey(sf::Keyboard::LControl));
		}
		// ## Keyboard - Delete ##
		else if(app().getKey(sf::Keyboard::Delete)){ 
			if(app().isKeyboardTextMove())
				textSelectionBoxManager.deleteText(true,app().getKey(sf::Keyboard::LControl));
		}
		// ## Keyboard - Ctrl+A ##
		else if(app().getKey(sf::Keyboard::LControl)&&app().getKey(sf::Keyboard::A)==BoolTail::START) {
			textSelectionBoxManager.removeAll();
			int cursorSlotLeft  = textSelectionBoxManager.nextStructuralCursorSlot(0, false, 5);
			int cursorSlotRight = textSelectionBoxManager.nextStructuralCursorSlot(0, true , 5);

			textSelectionBoxManager.expand(cursorSlotLeft, cursorSlotRight);
		}
		// ## Keyboard - Ctrl+B ##
		else if(app().getKey(sf::Keyboard::LControl)&&app().getKey(sf::Keyboard::B)==BoolTail::START) {
			textSelectionBoxManager.applyTextStyler(TextStylerBold::textStylerBold);
		}
		// ## Keyboard - Ctrl+V ##
		else if(app().getKey(sf::Keyboard::LControl)&&app().getKey(sf::Keyboard::V)==BoolTail::START) {
			textSelectionBoxManager.inputText(sf::Clipboard::getString());
		}
		// ## Keyboard - Ctrl+C ##
		else if(app().getKey(sf::Keyboard::LControl)&&app().getKey(sf::Keyboard::C)==BoolTail::START) {
			sf::Clipboard::setString(textSelectionBoxManager.copyText());
		}
		// ## Keyboard - Ctrl+X ##
		else if(app().getKey(sf::Keyboard::LControl)&&app().getKey(sf::Keyboard::X)==BoolTail::START){
			sf::Clipboard::setString(textSelectionBoxManager.copyText());
			textSelectionBoxManager.inputText("");
		}
		// ## Keyboard - TextInput ##
		else if(app().getText().getSize())
			textSelectionBoxManager.inputText(app().getText());
		
		// ## keyboard - up/down ##
		if(app().getKey(sf::Keyboard::Down)||app().getKey(sf::Keyboard::Up)) {
			if(app().isKeyboardTextMove()){
				
				bool downDirection = app().getKey(sf::Keyboard::Down);
			
				if(!app().getKey(sf::Keyboard::LControl)||app().getKey(sf::Keyboard::LShift))
					textSelectionBoxManager.moveVertical(downDirection,!app().getKey(sf::Keyboard::LShift));
			}
		}
		// ## keyboard - left/right ##
		if(app().getKey(sf::Keyboard::Left)||app().getKey(sf::Keyboard::Right)) {
			if(app().isKeyboardTextMove()){
				
				bool rightDirection = app().getKey(sf::Keyboard::Right);
			
				if(app().getKey(sf::Keyboard::LControl))
					textSelectionBoxManager.moveCtrl(rightDirection,!app().getKey(sf::Keyboard::LShift));
				else textSelectionBoxManager.moveHorizontal(rightDirection,!app().getKey(sf::Keyboard::LShift));
			}
		}

	}
}
int Text::select_index(glm::vec2 mousePosition2GuiComponent){
	int index = textComponent->cursorSlotIndex(mousePosition2GuiComponent);
	return index;
}
