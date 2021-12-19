#include "FontManager.h"

std::recursive_mutex FontManager::mutex; 
std::map<std::string,Font*> FontManager::fonts;
void FontManager::clean()
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	for(auto& pair:fonts)
		delete pair.second;
	fonts.clear();
}
Font* FontManager::getFont(std::string path){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	if(fonts.find(path)!=fonts.end())
		return fonts[path];
	auto font = new Font("res/font/"+path);
	fonts[path] = font;
	return font;
}