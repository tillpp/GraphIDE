#pragma once
#include <mutex>
class GuiComponent
{
	//thread
	std::recursive_mutex mutex;
	//data
	bool needRedraw = false;
public:
	GuiComponent();
	~GuiComponent();

	//does this GuiComponent need a redraw?
	bool getNeedRedraw();
	void setNeedRedraw(bool);

	void draw();
};

