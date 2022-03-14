#include "TextUnit.h"
#include "OpenGL/Mesh.h"
#include "Util/Log.h"

TextUnit::TextUnit(std::string utf8text)
	: text(sf::String::fromUtf8(utf8text.begin(), utf8text.end()))
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
	drawBackground(shader, ts, dtcr);

	//draw Text
	ts.font->use(shader);
	auto &line = text;
	GLfloat offset = 0;

	for (size_t i = 0; i < text.getSize(); i++)
	{
		//tab
		if (line[i] == '\t')
		{
			auto &glyph = ts.font->getGlyph(' ', ts.bold);

			float tabsize = glyph.advance * 4;
			int tabs = (int)offset / tabsize;

			if (tabsize * tabs == offset)
				offset += tabsize;
			else
				offset = (tabs + 1) * tabsize;
			continue;
		}

		bool bold = ts.bold;
		bool italic = ts.italic;
		glm::vec4 color = ts.textColor;

		/* 	CharacterEffect:preGlyph	*/
		for (auto &e : ts.effects)
			e->preGlyph(line, i, offset, color, bold,italic, ts);
		auto &glyph = ts.font->getGlyph(line[i], bold);

		//kerning
		offset += dtcr * ts.font->getKerning(line, i);

		//draw letter
		glUniform4fv(glGetUniformLocation(shader.getOpenGLID(), "textureRect"), 1, glm::value_ptr(ts.font->getRelativTextureRectGlyph(glyph)));
		glUniform4f(glGetUniformLocation(shader.getOpenGLID(), "color"), color.r, color.g, color.b, color.a);

		glm::vec4 drawRect;
		drawRect.x = dtcr * glyph.bounds.left + offset + ts.x;
		drawRect.y = dtcr * (glyph.bounds.top + ts.font->getBaseline() + ts.font->getLineSpacing()) + ts.y;
		drawRect.z = dtcr * glyph.bounds.width;
		drawRect.w = dtcr * glyph.bounds.height;
		GLfloat advance = dtcr * glyph.advance;

		/* 	CharacterEffect:preDraw	*/
		for (auto &e : ts.effects)
			e->preDrawGlyph(line, i, drawRect, advance, ts, true);

		//drawing letter
		glm::mat4 letterMatrix = glm::mat4(1.f);
		letterMatrix = glm::translate(letterMatrix, glm::vec3(drawRect.x, drawRect.y + drawRect.w, 0.f));
		if (italic)
			letterMatrix[1] += glm::vec4(-0.15, 0, 0, 0); //change for italic
		letterMatrix = glm::scale(letterMatrix, glm::vec3(drawRect.z, -drawRect.w, 1.f));

		Mesh::rectangle().draw(shader, /*inMatrix * matrix */ letterMatrix);

		/* 	CharacterEffect:postDraw	*/
		for (auto &e : ts.effects)
			e->postDrawGlyph(line, i, drawRect, advance, ts, true);

		//offset advancing
		offset += advance;
	}

	//Draw Underline
	if (ts.underline)
		drawUnderline		(shader, ts, dtcr,offset);
	//Draw Strikethrough
	if (ts.strikethrough)
		drawStrikethrough	(shader, ts, dtcr,offset);
}
void TextUnit::drawBackground(Shader &shader, TextSettings &ts, GLfloat dtcr)
{
	Texture::whiteTexture().use(0, shader, "texture1");
	glUniform4f(glGetUniformLocation(shader.getOpenGLID(), "textureRect"), 0, 0, 1, 1);
	glUniform4f(glGetUniformLocation(shader.getOpenGLID(), "color"), ts.backgroundColor.r, ts.backgroundColor.g, ts.backgroundColor.b, ts.backgroundColor.a);

	GLfloat ypos = ts.y;
	GLfloat xpos = ts.x;
	GLfloat height = dtcr * ts.font->getLineSpacing();
	GLfloat width = getWidth(ts);

	glm::mat4 letterMatrix = glm::mat4(1.f);
	letterMatrix = glm::translate(letterMatrix, glm::vec3(xpos, ypos + height, 0.f));
	letterMatrix = glm::scale(letterMatrix, glm::vec3(width, -height, 1.f));

	Mesh::rectangle().draw(shader, /*inMatrix * matrix */ letterMatrix);
}
void TextUnit::drawUnderline(Shader &shader, TextSettings &ts, GLfloat dtcr,GLfloat offset)
{
	Texture::whiteTexture().use(0, shader, "texture1");
	glUniform4f(glGetUniformLocation(shader.getOpenGLID(), "textureRect"), 0, 0, 1, 1);

	GLfloat xpos = ts.x;
	GLfloat width = offset;

	GLfloat ypos = dtcr * (ts.font->getUnderlinePosition() + ts.font->getBaseline() + ts.font->getLineSpacing()) + ts.y;
	GLfloat height = dtcr * ts.font->getUnderlineThickness();

	glm::mat4 letterMatrix = glm::mat4(1.f);
	letterMatrix = glm::translate(letterMatrix, glm::vec3(xpos, ypos + height, 0.f));
	if (ts.italic)
		letterMatrix[1] += glm::vec4(-0.15, 0, 0, 0); //change for italic
	letterMatrix = glm::scale(letterMatrix, glm::vec3(width, -height, 1.f));

	Mesh::rectangle().draw(shader, /*inMatrix * matrix */ letterMatrix);
}
void TextUnit::drawStrikethrough(Shader &shader, TextSettings &ts,GLfloat dtcr,GLfloat offset)
{
	Texture::whiteTexture().use(0, shader, "texture1");
	glUniform4f(glGetUniformLocation(shader.getOpenGLID(), "textureRect"), 0, 0, 1, 1);

	GLfloat xpos = ts.x;
	GLfloat width = offset;

	GLfloat ypos = dtcr * (ts.font->getLineSpacing()) / 2 + ts.y;
	GLfloat height = dtcr * ts.font->getUnderlineThickness();

	glm::mat4 letterMatrix = glm::mat4(1.f);
	letterMatrix = glm::translate(letterMatrix, glm::vec3(xpos, ypos + height, 0.f));
	if (ts.italic)
		letterMatrix[1] += glm::vec4(-0.15, 0, 0, 0); //change for italic
	letterMatrix = glm::scale(letterMatrix, glm::vec3(width, -height, 1.f));

	Mesh::rectangle().draw(shader, /*inMatrix * matrix */ letterMatrix);
}

int TextUnit::getWidth(const TextSettings &ts)
{
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
			auto &glyph = ts.font->getGlyph(' ', ts.bold);

			float tabsize = glyph.advance * 4;
			int tabs = (int)offset / tabsize;

			if (tabsize * tabs == offset)
				offset += tabsize;
			else
				offset = (tabs + 1) * tabsize;
			continue;
		}

		bool bold = ts.bold;
		bool italic = ts.italic;
		glm::vec4 color = ts.textColor;

		/* 	CharacterEffect:preGlyph	*/
		for (auto &e : ts.effects)
			e->preGlyph(line, i, offset, color, bold,italic, ts);

		auto &glyph = ts.font->getGlyph(line[i], bold);

		//kerning
		offset += dtcr * ts.font->getKerning(line, i);

		//draw letter
		glm::vec4 drawRect;
		drawRect.x = dtcr * glyph.bounds.left + offset + ts.x;
		drawRect.y = dtcr * (glyph.bounds.top + ts.font->getBaseline() + ts.font->getLineSpacing()) + ts.y;
		drawRect.z = dtcr * glyph.bounds.width;
		drawRect.w = dtcr * glyph.bounds.height;
		GLfloat advance = dtcr * glyph.advance;

		/* 	CharacterEffect:preDraw	*/
		for (auto &e : ts.effects)
			e->preDrawGlyph(line, i, drawRect, advance, ts, false);
		/* 	CharacterEffect:postDraw*/
		for (auto &e : ts.effects)
			e->postDrawGlyph(line, i, drawRect, advance, ts, false);

		//offset advancing
		offset += advance;
	}
	return offset;
}
int TextUnit::getHeight(const TextSettings &ts)
{
	return ts.font->getLineSpacing() * ts.getDisplayTextureCharacterRatio();
}
int TextUnit::getYOffset(const TextSettings &ts)
{
	return ts.font->getBaseline() * ts.getDisplayTextureCharacterRatio();
}
std::string TextUnit::getType()
{
	return "TextUnit";
}
