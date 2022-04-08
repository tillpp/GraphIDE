#pragma once
#include "OpenGL/Shader.h"
#include "OpenGL/Camera.h"
#include "OpenGL/Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "TextSettings.h"
#include "TextComponent.h"

#include "OpenGL/Mesh.h"
#include <functional>

class TextUnit
	:public TextComponent 
{
	sf::String text;

	void drawBackground(Shader&shader,TextSettings& TextSettings,GLfloat displayTextureCharacterRatio);
	void drawUnderline(Shader&shader,TextSettings& TextSettings,GLfloat displayTextureCharacterRatio,GLfloat offset);
	void drawStrikethrough(Shader&shader,TextSettings& TextSettings,GLfloat displayTextureCharacterRatio,GLfloat offset);
	
	Mesh meshGenerate(Shader&shader,TextSettings& TextSettings);
	
	/*
		Goes through all the Glyphs,

		returnvalue of Callback: continue?
	*/
	void goThroughGlyphs(
		GLfloat& offset,
		bool CharacterEffectDrawingMode,
		bool callBackAfterAdvance,
		const TextSettings& ts,
		const GLfloat& displayTextureCharacterRatio,
		std::function<bool(const sf::Glyph& glyph,GLfloat& advance,int& index,glm::vec4& color,glm::vec4& drawRect,bool& italic)> F);
	
	friend TextUnit;
public:
	TextUnit(std::string utf8text);
	~TextUnit();

	virtual void draw(Shader&shader,TextSettings& TextSettings)override;

	

	virtual std::string getType()override;
	virtual int getWidth(const TextSettings &ts)override;

	virtual int getHeight(const TextSettings &ts)override;
	virtual int getYOffset(const TextSettings &ts)override;

	virtual std::vector<TextComponent*> split(int offset,const int width,const TextSettings &ts);
	virtual bool merge(TextComponent* left);
};
