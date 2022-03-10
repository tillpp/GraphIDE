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
	glm::vec4 textColor = glm::vec4(1,1,1,1);
	glm::vec4 backgroundColor = glm::vec4(0,0,0,1);
	bool bold = false,italic = false,underline = false;
	int fontSize = 32;
	bool smallScript=false;

	GLfloat getDisplayTextureCharacterRatio()const;
};