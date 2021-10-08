#pragma once
#include <map>
#include "Texture.h"
#include <mutex>

class TextureManager
{
	static std::recursive_mutex mutex;
	static std::map<std::string,Texture*> textures;
public:
	TextureManager()=delete;

	static Texture* loadFromFile(std::string path);
};