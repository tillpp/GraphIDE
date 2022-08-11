#pragma once
#include <vector>

#include "TextSettings.h"
#include "OpenGL/Shader.h"
#include"Gui/General/GuiComponent.h"

class TextComponent
{
public:
	
	TextComponent();
	~TextComponent();
	virtual TextComponent* copy()=0;
	virtual void draw(Shader&shader,TextSettings& TextSettings,const double GuiComponentx,const double GuiComponenty,bool draw = true)=0;

	// ## Relative to the GuiComponent ##
	int left 	= 0;
	int top 	= 0;
	int width 	= 0;
	int height 	= 0;

	virtual int getYOffset()=0;
	virtual std::string getType()=0;

	// ## Split & Merge ##
	virtual std::vector<TextComponent*> split(const int TextBoxWidth,const TextSettings &ts)=0;
	virtual bool 						merge(TextComponent* left)=0;
	
	// ## CursorSlot ##
	virtual int 		cursorSlotCount()=0;
	virtual int 		cursorSlotIndex(glm::vec2 mousePositionRelative2TextComponent)=0; 
	virtual glm::vec2 	cursorSlotPositition(int cursorSlot)=0;
	/* sf::String::InvalidPos if cursorSlot position is in the next */
	virtual int 		nextCtrlCursorSlot      (int cursorSlot, bool rightDirection )=0;
	virtual int 		nextStructuralCursorSlot(int cursorSlot, bool rightDirection , int layer)=0; 
	virtual int 		nextVerticalCursorSlot  (int cursorSlot, bool downDirection )=0;
	
	// ## Cursor & Selection ##
	virtual void 		addCursor	 (int cursorSlot)=0;
	virtual void 		removeCursor (int cursorSlot)=0;
	virtual void 		select		 (int index,int amount)=0;
	virtual void 		deselect	 (int index,int amount)=0;
	
	// ## Events ##
	virtual void 		triggerEvent (const GuiEvent& event)=0;

	// ## Cursor Events ##
	virtual void 		inputText	 (int 	   cursorSlot, const sf::String& textInput)=0;
	virtual void 		deleteText	 (int startCursorSlot, int endCursorSlot)=0;
	virtual sf::String	copyText	 (int startCursorSlot, int endCursorSlot)=0;

	// ## Styler ##
	/*
		ApplyTextStyler, makes one TextStyle around the parts.
		except:		unsplitable children. instead applied recursivly.
	*/
	virtual std::vector<TextComponent*>	split    		 (int cursorSlot, bool onlyChildren)=0;
	virtual std::vector<TextComponent*>	applyTextStyler  (TextComponent& style, int startCursorSlot, int endCursorSlot)=0;
	virtual std::vector<TextComponent*>	explodeTextStyler(std::string type,		int startCursorSlot, int endCursorSlot)=0;
	virtual TextComponent*				findFirst	   	 (std::string type,		int startCursorSlot, int endCursorSlot)=0;
	
	// ## Children ##
	virtual void 		insert		 (TextComponent* textComponent)=0;
	virtual void 		print		 (std::string="")=0;
};
