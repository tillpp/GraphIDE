#include "Text.h"
#include "OpenGL/Mesh.h"
#include <mutex>

Font Text::standartFont("res/font/SourceCodePro-Regular.ttf");
std::recursive_mutex standartFontMutex;

Text::Text()
{
	std::lock_guard<std::recursive_mutex> lock(standartFontMutex);
	font = &standartFont;
}

Text::~Text()
{
	for(auto& x:selectionBoxs)
		delete x;
}
void Text::addUtf8(std::string data)
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	text += sf::String::fromUtf8(data.begin(), data.end());
}
void Text::addString(sf::String data)
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	text += data;
}
void Text::setUtf8(std::string data)
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	text = sf::String::fromUtf8(data.begin(), data.end());
}
void Text::setString(sf::String data)
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	text = data;
}

void Text::draw(Shader& shader,Camera& camera,glm::mat4 inMatrix){
 	std::lock_guard<std::recursive_mutex> lock(mutex);

	shader.use();
	//draw selectionboxs
		
	//draw Text
	glUniform4f(glGetUniformLocation(shader.getOpenGLID(), "color"),color.r,color.g,color.b,color.a);
	camera.use(shader);
	font->use(shader);
	GLfloat offset = 0;
	for (size_t i = 0; i < text.getSize(); i++)
	{
		auto& glyph = font->getGlyph(text[i]);
		//tab
		if (text[i] == '\t')
		{
			auto& glyph = font->getGlyph(' ');

			float tabsize = glyph.advance*4;
			int tabs = (int)offset/tabsize;

			if(tabsize*tabs==offset)
				 offset += tabsize;
			else offset = (tabs+1)*tabsize;
			continue;
		}

		//kerning
		offset += font->getKerning(text,i);

		//draw letter
		glUniform4fv(glGetUniformLocation(shader.getOpenGLID(), "textureRect"),1,glm::value_ptr(font->getRelativTextureRectGlyph(glyph)));
 
 		GLfloat ypos = glyph.bounds.top + font->getBaseline() + font->getLineSpacing();
		GLfloat xpos = glyph.bounds.left + offset;
		GLfloat height = glyph.bounds.height;
		GLfloat width = glyph.bounds.width;

		glm::mat4 letterMatrix = glm::mat4(1.f);
		letterMatrix = glm::translate	(letterMatrix,glm::vec3(xpos , ypos+height,0.f));
		letterMatrix = glm::scale		(letterMatrix,glm::vec3(width, -height ,1.f));
		
		Mesh::rectangle().draw(shader,inMatrix*matrix*letterMatrix);

		//offset advancing
		offset += glyph.advance;
	}

	GuiComponent::draw(shader,camera,inMatrix*matrix);
}

