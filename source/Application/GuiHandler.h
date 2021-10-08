#pragma once
#include "ApplicationFeature.h"

/*
	A GuiHandler is used to handle the Gui.
*/
class GuiHandler
	:public ApplicationFeature
{
	//thread
	std::recursive_mutex mutex;	
public:
	GuiHandler();
	~GuiHandler();

	void draw(Application*)override;
	void update(Application*)override;
};

