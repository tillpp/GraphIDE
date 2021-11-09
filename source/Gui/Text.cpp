#include "Text.h"
#include "OpenGL/Mesh.h"
#include <mutex>

sf::Font Text::standartFont;
bool standartFontIsInitialzed = false;
std::recursive_mutex standartFontMutex;

Text::Text()
{
	std::lock_guard<std::recursive_mutex> lock(standartFontMutex);
	if(!standartFontIsInitialzed){
		standartFont.loadFromFile("res/font/SourceCodePro-Regular.ttf");	
		standartFontIsInitialzed = true;
	}

	font = &standartFont;
	//matrix = glm::scale(matrix,glm::vec3(30,30,1));
}

Text::~Text()
{
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
	size_t characterSize = 32;

	//Text has changed
	if (text != lastText)
 	{
		font->getGlyph('|', 32, 0, 0);
 		for (size_t i = 0; i < text.getSize(); i++)
		{

			font->getGlyph(text[i], characterSize, 0, 0);
		}

		texture.LoadFromTexture(font->getTexture(characterSize));
		lastText = text;
	}

	//text hasnt changed
	shader.use();

	//uniforms
	glUniform4f(glGetUniformLocation(shader.getOpenGLID(), "color"),color.r,color.g,color.b,color.a);
	camera.use(shader);
	texture.use(0, shader, "texture1");
	
	GLfloat offset = 0;
	GLfloat baseline = 1 - (font->getUnderlinePosition(characterSize) + font->getUnderlineThickness(characterSize));

	float one = font->getLineSpacing(characterSize);
	float debugScale = 1.f / one;

	for (size_t i = 0; i < text.getSize(); i++)
	{
		auto glyph = font->getGlyph(text[i], characterSize, 0, 0);
		//tab
		if (text[i] == '\t')
		{
			glyph = font->getGlyph(' ', characterSize, 0, 0);

			float tabsize = glyph.advance*4;
			int tabs = (int)offset/tabsize;

			if(tabsize*tabs==offset)
				 offset += tabsize;
			else offset = (tabs+1)*tabsize;
			continue;
		}
		if (i)
			offset += font->getKerning(text[i - 1], text[i], characterSize);

		GLfloat ypos = glyph.bounds.top + baseline + one;
		GLfloat xpos = glyph.bounds.left + offset;
		GLfloat height = glyph.bounds.height;
		GLfloat width = glyph.bounds.width;

		GLfloat ystop = glyph.bounds.height + ypos;
		GLfloat xstop = glyph.bounds.width + xpos;

		glm::vec4 textureRect;
	
		/*left	*/	textureRect.x = (float)glyph.textureRect.left / texture.getSize().x;
		/*width	*/	textureRect.z = (float)glyph.textureRect.width / texture.getSize().x;
		/*top	*/	textureRect.y = (float)glyph.textureRect.top / texture.getSize().y;
		/*height*/	textureRect.w = (float)glyph.textureRect.height / texture.getSize().y;

		glUniform4fv(glGetUniformLocation(shader.getOpenGLID(), "textureRect"),1,glm::value_ptr(textureRect));
 
		glm::mat4 letterMatrix = glm::mat4(1.f);
		letterMatrix = glm::translate	(letterMatrix,glm::vec3(xpos , ypos+height,0.f));
		letterMatrix = glm::scale		(letterMatrix,glm::vec3(width, -height ,1.f));
		
		Mesh::rectangle().draw(shader,inMatrix*matrix*letterMatrix);
		
		offset += glyph.advance;
	}

	GuiComponent::draw(shader,camera,inMatrix*matrix);
}

