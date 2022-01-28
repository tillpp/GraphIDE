#include "Sprite.h"
#include "OpenGL/Mesh.h"
#include "OpenGL/Texture.h"

Sprite::Sprite(/* args */) : texBoundXpos(this, true,"texture.x"),
							 texBoundYpos(this, false,"texture.y"),
							 texBoundWidth(this, true,"texture.width"),
							 texBoundHeight(this, false,"texture.height"),
							 texWidth(this, true,"texture.full.width"),
							 texHeight(this, false,"texture.full.height")
{
	texWidth.setEquation(GuiEqPixel(0));
	texHeight.setEquation(GuiEqPixel(0));
	texBoundWidth.setEquation(GuiEqTexturePercent(100));
	texBoundHeight.setEquation(GuiEqTexturePercent(100));
}

Sprite::~Sprite()
{
}
void Sprite::draw(Shader &shader, Camera &camera, const double& x, const double& y)
{
	std::lock_guard<std::recursive_mutex> lock(mutex);

	//update textureWidth if nessary
	if (texture)
	{
		texWidth.overrideCachedValue(texture->getSize().x); 
		texHeight.overrideCachedValue(texture->getSize().y);
	}

	shader.use();

	//uniforms
	glUniform4f(glGetUniformLocation(shader.getOpenGLID(), "color"), color.r, color.g, color.b, color.a);

	glUniform4f(glGetUniformLocation(shader.getOpenGLID(), "textureRect"),
				(texBoundXpos + (flipX ? 1 : 0)*texBoundWidth)/ texWidth,
				(texBoundYpos - (flipY ? 0 : 1)*texBoundHeight) / texHeight,
				((flipX ? -1 : 1) * texBoundWidth) / texWidth,
				((flipY ? -1 : 1) * texBoundHeight) / texHeight);

	camera.use(shader);
	if (texture)
		texture->use(0, shader, "texture1");
	else
		Texture::whiteTexture().use(0, shader, "texture1");

	auto matrix = glm::translate(glm::mat4(1.f), glm::vec3(x + xpos, y + ypos, 1));
	matrix = glm::scale(matrix, glm::vec3((double)width, (double)height, 1));

	Mesh::rectangle().draw(shader, matrix);
	GuiComponent::draw(shader, camera, x, y);
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
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return "Sprite";
}
void Sprite::setFlipX(bool x)
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	flipX = x;
}
bool Sprite::getFlipX()
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return flipX;
}
void Sprite::setFlipY(bool x)
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	flipY = x;
}
bool Sprite::getFlipY()
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return flipY;
}
std::vector<GuiAttribute*> Sprite::getGuiAttributes(){
	auto rv = GuiComponent::getGuiAttributes();
	rv.push_back(&texBoundHeight);
	rv.push_back(&texBoundWidth);
	rv.push_back(&texBoundXpos);
	rv.push_back(&texBoundYpos);
	rv.push_back(&texHeight);
	rv.push_back(&texWidth);
	return rv;
}