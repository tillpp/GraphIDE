#pragma once
#include "Application.h"

#include "OpenGL/Shader.h"
#include "OpenGL/Mesh.h"
#include "OpenGL/Texture.h"

#include "OpenGL/Recorder.h"
#include "Gui/General/Sprite.h"
#include "Gui/General/Text.h"
#include "Gui/General/Scene.h"
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
	Scene scene = Scene(glm::vec2(1280,720));

	//test test stuff
	Sprite sprite,*sprite2;
	Text text;//,text2;

public:
	GuiHandler();
	~GuiHandler();

	void init(Application*)override;
	void draw(Application*)override;
	void update(Application*)override;
};

