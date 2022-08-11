#pragma once
#include "TextComponent.h"

class TextSelectionBoxManager
{
	TextComponent*& textComponent;

	struct Box
	{
		size_t start;
		size_t cursor;
	};

	/* - boxes are sorted by `start` - */
	std::vector<Box*> boxes;
	void sortBoxArray();
	Box* lastBox;

public:
	TextSelectionBoxManager(TextComponent*& textComponent);

	// ## SelectionBox ## 
	void start(size_t index);
	void end(size_t index);
	void expand(size_t left, size_t right);					//lastBox expanding at least to ...
	void moveHorizontal(bool rightDirection, bool resetStart);		//moves all cursor
	void moveVertical  (bool downDirection , bool resetStart);		//moves all cursor
	void moveCtrl(bool rightDirection, bool resetStart);	//jumpes all cursor
	void removeAll();

	int  nextStructuralCursorSlot(int cursorSlot, bool rightDirection,unsigned int layer);	
	

	void applyTextStyler(TextComponent& style);
	
	void inputText(const sf::String& textInput);
	void deleteText(bool rightDirection, bool ctrl);
	sf::String copyText();
};