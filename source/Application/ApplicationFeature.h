#pragma once
#include <mutex>

class Application;
class ApplicationFeature
{
public:
	virtual	void init(Application* app)=0;
	virtual	void draw(Application* app)=0;
	virtual void update(Application* app)=0;
};
