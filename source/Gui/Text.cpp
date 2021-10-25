// #include "Text.h"
// #include "system/Input.h"

// sf::Font Text::standartFont;

// Text::Text(/* args */)
// {
// }

// GLuint Text::VAO = -1, Text::VBO;
// void Text::init()
// {
// 	if (VAO != -1)
// 		return;

// 	standartFont.loadFromFile("res/font/arial.ttf");

// 	glGenVertexArrays(1, &VAO);
// 	glGenBuffers(1, &VBO);

// 	glBindVertexArray(VAO);
// 	glBindBuffer(GL_ARRAY_BUFFER, VBO);

// 	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 5, NULL, GL_DYNAMIC_DRAW);
// 	glEnableVertexAttribArray(0);
// 	glEnableVertexAttribArray(1);
// 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
// 	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
// 	glBindBuffer(GL_ARRAY_BUFFER, 0);

// 	glBindVertexArray(0);
// }

// Text::~Text()
// {
// }

// void Text::updateMatrix(float parent_sizex, float parent_sizey)
// {
// 	//updating size befor origin is calculated
// 	sizex(parent_sizex);
// 	sizey(parent_sizey);

// 	//updating the matrix
// 	matrix = glm::mat4(1.0f);
// 	matrix = glm::translate(matrix, glm::vec3(positionx(parent_sizex), positiony(parent_sizey), 0));
// 	matrix = glm::rotate(matrix, glm::radians(rotation), glm::vec3(0.0, 0.0, 1.0));
// 	matrix = glm::translate(matrix, -glm::vec3(originx(sizex), originy(sizey), 0));
// 	attachMatrix = matrix;
// 	matrix = glm::scale(matrix, glm::vec3(sizey(parent_sizey), sizey(parent_sizey), 1));
// 	//Text sizex is dependent by sizey
// }
// void Text::draw(
// 	Scene &scene,
// 	float parent_sizex,
// 	float parent_sizey,
// 	glm::mat4 parent_matrix)
// {

// 	size_t characterSize = 32;

// 	if (text != lastText)
// 	{
// 		font->getGlyph('|', 32, 0, 0);

// 		for (size_t i = 0; i < text.getSize(); i++)
// 		{

// 			font->getGlyph(text[i], characterSize, 0, 0);
// 		}
// 		font->getTexture(characterSize).copyToImage().saveToFile("test.png");
// 		texture.LoadFromTexture(font->getTexture(characterSize));

// 		lastText = text;
// 	}
// 	gui_shader.use();
// 	scene.use(gui_shader);
// 	texture.use(0, gui_shader, "texture1");

// 	// Activate corresponding render state
// 	glUniformMatrix4fv(glGetUniformLocation(gui_shader.getOpenGLID(), "model"), 1, GL_FALSE, glm::value_ptr(parent_matrix * matrix));
// 	glUniform4f(gui_shader_uniform_color, color.x, color.y, color.z, color.w);
// 	//glUniform2f(glGetUniformLocation(getGuiManager().textureShader.getOpenGLID(), "origin"), size*origin.x, origin.y);
// 	glBindVertexArray(VAO);

// 	GLfloat offset = 0;
// 	GLfloat baseline = 1 - (font->getUnderlinePosition(characterSize) + font->getUnderlineThickness(characterSize));

// 	GLfloat mousepos = Input::mousepos.x;

// 	float one = font->getLineSpacing(characterSize);
// 	float debugScale = 1.f / one;

// 	float cursorx = guiAspectMouse(scene, parent_matrix, attachMatrix).x/sizey(parent_sizey)/debugScale;

// 	for (size_t i = 0; i < text.getSize(); i++)
// 	{
// 		auto glyph = font->getGlyph(text[i], characterSize, 0, 0);
// 		if (text[i] == '\t')
// 		{
// 			glyph = font->getGlyph(' ', characterSize, 0, 0);

// 			//if (origin.x == 0)
// 			if (false)
// 			{
// 				int g = (offset) / (glyph.advance * 4) + 1;
// 				glyph.advance = glyph.advance * 4 * g - (offset);
// 			}
// 			else
// 				glyph.advance *= 4;
// 		}
// 		if (i)
// 			offset += font->getKerning(text[i - 1], text[i], characterSize);

// 		GLfloat ypos = glyph.bounds.top + baseline + one;
// 		GLfloat xpos = glyph.bounds.left + offset;
// 		GLfloat ystop = glyph.bounds.height + ypos;
// 		GLfloat xstop = glyph.bounds.width + xpos;

// 		sf::FloatRect textureRect;

// 		textureRect.left = (float)glyph.textureRect.left / texture.sizex;
// 		textureRect.width = (float)glyph.textureRect.width / texture.sizex;
// 		textureRect.top = (float)glyph.textureRect.top / texture.sizey;
// 		textureRect.height = (float)glyph.textureRect.height / texture.sizey;

// 		GLfloat vertices[30] = {
// 			xpos * debugScale, ypos * debugScale, 0, textureRect.left, textureRect.top,
// 			xpos * debugScale, ystop * debugScale, 0, textureRect.left, textureRect.top + textureRect.height,
// 			xstop * debugScale, ystop * debugScale, 0, textureRect.left + textureRect.width, textureRect.top + textureRect.height,

// 			xpos * debugScale, ypos * debugScale, 0, textureRect.left, textureRect.top,
// 			xstop * debugScale, ystop * debugScale, 0, textureRect.left + textureRect.width, textureRect.top + textureRect.height,
// 			xstop * debugScale, ypos * debugScale, 0, textureRect.left + textureRect.width, textureRect.top};

// 		if(cursorx>offset&&cursorx<(offset+glyph.advance)){
// 			cursorx = offset + ((cursorx-offset)/glyph.advance<0.5?0:glyph.advance);
// 		}
// 		offset += glyph.advance;

// 		// Update content of VBO memory
// 		glBindBuffer(GL_ARRAY_BUFFER, VBO);
// 		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
// 		glBindBuffer(GL_ARRAY_BUFFER, 0);
// 		// Render quad
// 		glDrawArrays(GL_TRIANGLES, 0, 6);
// 	}
// 	sizex.setValue(offset * debugScale * sizey(parent_sizey), false);
// 	glBindTexture(GL_TEXTURE_2D, 0);

// 	//cursor
// 	whiteTexture().use(0, gui_shader, "texture1");
// 	 GLfloat vertices[10] = {
// 	 		cursorx*debugScale,  	0,	0,		0, 1 ,
// 	 		cursorx*debugScale,	1,	0,		1, 1 ,
// 		};
// 	// Update content of VBO memory
// 	glBindBuffer(GL_ARRAY_BUFFER, VBO);
// 	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
// 	glBindBuffer(GL_ARRAY_BUFFER, 0);
// 	// Render quad
// 	glDrawArrays(GL_LINES, 0, 2);
// 	glBindTexture(GL_TEXTURE_2D, 0);
// 	glBindVertexArray(0);

// 	Gui::draw(scene, parent_sizex, parent_sizey, parent_matrix);
// }
// void Text::setUtf8(std::string data)
// {
// 	text = sf::String::fromUtf8(data.begin(), data.end());
// }