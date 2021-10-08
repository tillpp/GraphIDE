#include "TextureManager.h"
#include <fstream>

std::map<std::string,Texture*> 	TextureManager::textures;
std::recursive_mutex 			TextureManager::mutex;

bool fileExist(std::string path){
	std::ifstream file(path);
	return (bool)file;
}

Texture * TextureManager::loadFromFile(std::string path)
{
	mutex.lock();
	if (textures.find(path) == textures.end())
	{
		Texture* texture;
		if(fileExist(path)){
			texture = new Texture;
			texture->LoadFromFile(path);
		}
		else texture = &Texture::whiteTexture();
		textures[path] = texture;
		return texture;
	}
	return textures[path];
	mutex.unlock();
}
