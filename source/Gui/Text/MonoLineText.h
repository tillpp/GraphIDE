#pragma once
#include <vector>
#include "TextComponent.h"

class MonoLineText
{
	std::vector<TextComponent*> components;
public:
	MonoLineText();
	~MonoLineText();

	void draw(Shader& shader,TextSettings ts);

	int getHeight(const TextSettings& ts);
	int getWidth(const TextSettings& ts);
};
