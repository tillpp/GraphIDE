#pragma once
#include "ApplicationFeature.h"

#include "OpenGL/Shader.h"
#include "OpenGL/Mesh.h"
#include "OpenGL/Texture.h"

/*
	A GuiHandler is used to handle the Gui.
*/
class GuiHandler
	:public ApplicationFeature
{
	//thread
	std::recursive_mutex mutex;	

	//test stuff
	Shader shader;
	Mesh mesh;
public:
	GuiHandler();
	~GuiHandler();

	void init(Application*)override;
	void draw(Application*)override;
	void update(Application*)override;
};

