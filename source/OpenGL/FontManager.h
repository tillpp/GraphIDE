#pragma once
#include "Font.h"
#include <map>
#include <mutex>
#include <string>

class FontManager
{
	static std::map<std::string,Font*> fonts;
	static std::recursive_mutex mutex;
public:
	static void clean();

	static Font* getFont(std::string path);
};
