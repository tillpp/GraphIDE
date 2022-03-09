#pragma once
#include "OpenGL/Font.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class TextSettings
{
public:
	int x=0,y=0;

	Font* font = nullptr;
	glm::vec4 textColor = glm::vec4(0,1,0,1);
	bool bold = true,italic = false;//underline = false;
	// int &fontSize;
	// int &baseline;
	// bool& superScript;
};