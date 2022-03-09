#include "MonoLineText.h"
#include "TextUnit.h"

MonoLineText::MonoLineText()
{
	components.push_back(new TextUnit("hi"));
	components.push_back(new TextUnit("ok"));
}

MonoLineText::~MonoLineText()
{
}

void MonoLineText::draw(Shader& shader,TextSettings& ts){
	ts.italic= true;
	for(auto& tc:components){
		tc->draw(shader,ts);
		ts.italic = false;
		ts.x += tc->getWidth(ts);
	}
}