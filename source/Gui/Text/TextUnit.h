#pragma once
#include "OpenGL/Shader.h"
#include "OpenGL/Camera.h"
#include "OpenGL/Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "TextSettings.h"
#include "TextComponent.h"

class TextUnit
	:public TextComponent 
{
	sf::String text;
public:
	TextUnit(std::string utf8text);
	~TextUnit();

	virtual void draw(Shader&shader,TextSettings& TextSettings)override;
	virtual std::string getType()override;
	virtual int getWidth(TextSettings &ts)override;
};
