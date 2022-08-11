#pragma once
#include "Gui/Text/TextComponent.h"

// >> Improvement: Buffer some values.

class TextLine : public TextComponent
{
protected:
	std::vector<TextComponent*> children;
	std::vector<size_t> 		childMaxCursorSlot;

	GLfloat maxUp,maxDown;
	
	void updateChildData();

public:
	TextLine(sf::String text);
	TextLine();
	~TextLine();

	virtual TextComponent* copy()override;
	virtual void draw(Shader&shader,TextSettings& TextSettings,const double GuiComponentx,const double GuiComponenty,bool draw = true)override;

	virtual int getYOffset()override;
	virtual std::string getType()override;

	// ## Split & Merge ##
	virtual std::vector<TextComponent*> split(const int TextBoxWidth,const TextSettings &ts)override;
	virtual bool 						merge(TextComponent* left)override;
	
	// ## CursorSlot ##
	virtual int 		cursorSlotCount()override;
	virtual int 		cursorSlotIndex(glm::vec2 mousePositionRelative2TextComponent)override; 
	virtual glm::vec2 	cursorSlotPositition(int cursorSlot)override;
	/* sf::String::InvalidPos if cursorSlot position is in the next */
	virtual int 		nextCtrlCursorSlot      (int cursorSlot, bool rightDirection )override;
	virtual int 		nextStructuralCursorSlot(int cursorSlot, bool rightDirection , int layer)override; 
	virtual int 		nextVerticalCursorSlot  (int cursorSlot, bool downDirection )override;
	
	// ## Cursor & Selection ##
	virtual void 		addCursor	 (int cursorSlot)override;
	virtual void 		removeCursor (int cursorSlot)override;
	virtual void 		select		 (int index,int amount)override;
	virtual void 		deselect	 (int index,int amount)override;
	
	// ## Events ##
	virtual void 		triggerEvent (const GuiEvent& event)override;

	// ## Cursor Events ##
	virtual void 		inputText	 (int 	   cursorSlot, const sf::String& textInput)override;
	virtual void 		deleteText	 (int startCursorSlot, int endCursorSlot)override;
	virtual sf::String	copyText	 (int startCursorSlot, int endCursorSlot)override;

	// ## Styler ##
	virtual std::vector<TextComponent*>	split    		 (int cursorSlot, bool onlyChildren)override;
	virtual std::vector<TextComponent*>	applyTextStyler  (TextComponent& style, int startCursorSlot, int endCursorSlot)override;
	virtual std::vector<TextComponent*>	explodeTextStyler(std::string type,		int startCursorSlot, int endCursorSlot)override; /* TextStyler of children */
	virtual TextComponent*				findFirst	   	 (std::string type,		int startCursorSlot, int endCursorSlot)override;
	
	// ## Children ##
	virtual void 		insert		 (TextComponent* textComponent)override;
	virtual void 		print		 (std::string="")override;

	// ## TextLine specific ##
};