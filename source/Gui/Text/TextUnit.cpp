#include "TextUnit.h"
#include "OpenGL/Mesh.h"
#include "Util/Log.h"

TextUnit::TextUnit(std::string utf8text)
	: text(sf::String::fromUtf8(utf8text.begin(),utf8text.end()))
{
}

TextUnit::~TextUnit()
{
}
void TextUnit::draw(Shader &shader, TextSettings &ts)
{
	if (!ts.font)
		Log::info("No font used.");

	//displayTextureCharacterRatio
	GLfloat dtcr = ts.getDisplayTextureCharacterRatio();

	//draw background
	{
		Texture::whiteTexture().use(0,shader,"texture1");
		glUniform4f(glGetUniformLocation(shader.getOpenGLID(), "textureRect"), 0,0,1,1);
		glUniform4f(glGetUniformLocation(shader.getOpenGLID(), "color"), ts.backgroundColor.r,ts.backgroundColor.g,ts.backgroundColor.b,ts.backgroundColor.a);
		

		GLfloat ypos =  ts.y;
		GLfloat xpos = ts.x;
		GLfloat height = dtcr*ts.font->getLineSpacing();
		GLfloat width  = getWidth(ts);

		glm::mat4 letterMatrix = glm::mat4(1.f);
		letterMatrix = glm::translate(letterMatrix, glm::vec3(xpos, ypos+height, 0.f));
		letterMatrix = glm::scale(letterMatrix, glm::vec3(width, -height, 1.f));
		
		Mesh::rectangle().draw(shader, /*inMatrix * matrix */ letterMatrix);
	}
	//draw Text
	glUniform4f(glGetUniformLocation(shader.getOpenGLID(), "color"), ts.textColor.r,ts.textColor.g,ts.textColor.b,ts.textColor.a);
	ts.font->use(shader);

	auto &line = text;
	GLfloat offset = 0;
	for (size_t i = 0; i < text.getSize(); i++)
	{
		//tab
		
		if (line[i] == '\t')
		{
			auto &glyph = ts.font->getGlyph(' ',ts.bold);

			float tabsize = glyph.advance * 4;
			int tabs = (int)offset / tabsize;

			if (tabsize * tabs == offset)
				offset += tabsize;
			else
				offset = (tabs + 1) * tabsize;
			continue;
		}
		
		auto &glyph = ts.font->getGlyph(line[i],ts.bold);
		//kerning
		offset += dtcr*ts.font->getKerning(line, i);

		//draw letter
		glUniform4fv(glGetUniformLocation(shader.getOpenGLID(), "textureRect"), 1, glm::value_ptr(ts.font->getRelativTextureRectGlyph(glyph)));

		GLfloat ypos 	= dtcr*(glyph.bounds.top + ts.font->getBaseline() + ts.font->getLineSpacing()) +ts.y;
		GLfloat xpos 	= dtcr*glyph.bounds.left + offset +ts.x;
		GLfloat height	= dtcr*glyph.bounds.height;
		GLfloat width  	= dtcr*glyph.bounds.width;

		glm::mat4 letterMatrix = glm::mat4(1.f);
		letterMatrix = glm::translate(letterMatrix, glm::vec3(xpos, ypos+height, 0.f));
		if(ts.italic)
			letterMatrix[1] += glm::vec4(-0.15,0,0,0);//change for italic
		letterMatrix = glm::scale(letterMatrix, glm::vec3(width, -height, 1.f));
		
		Mesh::rectangle().draw(shader, /*inMatrix * matrix */ letterMatrix);
	
		//offset advancing
		offset += dtcr*glyph.advance;
	}

	//Draw Underline
	if(ts.underline)
	{
		Texture::whiteTexture().use(0,shader,"texture1");
		glUniform4f(glGetUniformLocation(shader.getOpenGLID(), "textureRect"), 0,0,1,1);

		GLfloat xpos 	= ts.x;
		GLfloat width  	= offset;

		GLfloat ypos 	= dtcr*(ts.font->getUnderlinePosition()+ts.font->getBaseline() + ts.font->getLineSpacing()) +ts.y;
		GLfloat height	= dtcr*ts.font->getUnderlineThickness();
		
		glm::mat4 letterMatrix = glm::mat4(1.f);
		letterMatrix = glm::translate(letterMatrix, glm::vec3(xpos, ypos+height, 0.f));
		if(ts.italic)
		 	letterMatrix[1] += glm::vec4(-0.15,0,0,0);//change for italic
		letterMatrix = glm::scale(letterMatrix, glm::vec3(width, -height, 1.f));
		
		Mesh::rectangle().draw(shader, /*inMatrix * matrix */ letterMatrix);
	}
}
int TextUnit::getWidth(const TextSettings &ts){
	if (!ts.font)
		Log::info("No font used.");

	//displayTextureCharacterRatio
	GLfloat dtcr = ts.getDisplayTextureCharacterRatio();
	
	//draw Text
	auto &line = text;
	GLfloat offset = 0;

	for (size_t i = 0; i < text.getSize(); i++)
	{
		//tab
		if (line[i] == '\t')
		{
			auto &glyph = ts.font->getGlyph(' ',ts.bold);

			float tabsize = glyph.advance * 4;
			int tabs = (int)offset / tabsize;

			if (tabsize * tabs == offset)
				offset += tabsize;
			else
				offset = (tabs + 1) * tabsize;
			continue;
		}
		
		auto &glyph = ts.font->getGlyph(line[i],ts.bold);

		//kerning
		offset += dtcr*ts.font->getKerning(line, i);

		//offset advancing
		offset += dtcr*glyph.advance;
	}
	return offset;
}
int TextUnit::getHeight(const TextSettings &ts){
	return ts.font->getLineSpacing()*ts.getDisplayTextureCharacterRatio();
}
int TextUnit::getYOffset(const TextSettings &ts){
	return ts.font->getBaseline()*ts.getDisplayTextureCharacterRatio();
}
std::string TextUnit::getType()
{
	return "TextUnit";
}

