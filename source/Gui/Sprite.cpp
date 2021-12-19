#include "Sprite.h"
#include "OpenGL/Mesh.h"
#include "OpenGL/Texture.h"

Sprite::Sprite(/* args */) : texBoundXpos(this, true),
							 texBoundYpos(this, false),
							 texBoundWidth(this, true),
							 texBoundHeight(this, false),
							 texWidth(this, true),
							 texHeight(this, false)
{
	texWidth.setEquation(GuiEqPixel(0));
	texHeight.setEquation(GuiEqPixel(0));
	texBoundWidth.setEquation(GuiEqTexturePercent(100));
	texBoundHeight.setEquation(GuiEqTexturePercent(100));
}

Sprite::~Sprite()
{
}
void Sprite::draw(Shader &shader, Camera &camera, double x, double y)
{
	std::lock_guard<std::recursive_mutex> lock(mutex);

	//update textureWidth if nessary
	if (texture)
	{
		int textureWidth = (int)texture->getSize().x;
		int textureHeight = (int)texture->getSize().y;
		if (textureWidth != (int)texWidth)
			texWidth.adjustEquation([&](GuiEquation *ge) -> void
									{ ((GuiEqPixel *)ge)->setValue(textureWidth); });
		if (textureHeight != (int)texHeight)
			texHeight.adjustEquation([&](GuiEquation *ge) -> void
									 { ((GuiEqPixel *)ge)->setValue(textureHeight); });
	}

	shader.use();

	//uniforms
	glUniform4f(glGetUniformLocation(shader.getOpenGLID(), "color"), color.r, color.g, color.b, color.a);
	glUniform4f(glGetUniformLocation(shader.getOpenGLID(), "textureRect"),
		texBoundXpos/texWidth,
		texBoundYpos/texHeight,
		(texBoundXpos+texBoundWidth)/texWidth,
		(texBoundYpos+texBoundHeight)/texHeight);

	camera.use(shader);
	if (texture)
		texture->use(0, shader, "texture1");
	else
		Texture::whiteTexture().use(0, shader, "texture1");

	auto matrix = glm::translate(glm::mat4(1.f), glm::vec3(x + xpos, y + ypos, 1));
	matrix = glm::scale(matrix, glm::vec3((double)width, (double)height, 1));

	Mesh::rectangle().draw(shader, matrix);
	GuiComponent::draw(shader, camera, xpos, ypos);
}

void Sprite::setColor(glm::vec4 inColor)
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	color = inColor;
}
glm::vec4 Sprite::getColor()
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return color;
}
void Sprite::setTexture(Texture *inTexture)
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	this->texture = inTexture;
}
Texture *Sprite::getTexture()
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return texture;
}

std::string Sprite::getType()
{
	return "Sprite";
}
