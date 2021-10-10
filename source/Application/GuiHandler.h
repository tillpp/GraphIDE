#pragma once
#include "Application.h"

#include "OpenGL/Shader.h"
#include "OpenGL/Mesh.h"
#include "OpenGL/Texture.h"
#include "OpenGL/Camera.h"

#include "OpenGL/Recorder.h"

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
	Camera camera;
	Recorder recorder;
public:
	GuiHandler();
	~GuiHandler();

	void init(Application*)override;
	void draw(Application*)override;
	void update(Application*)override;
};

