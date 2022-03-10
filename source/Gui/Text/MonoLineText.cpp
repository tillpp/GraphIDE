#include "MonoLineText.h"
#include "TextUnit.h"

MonoLineText::MonoLineText()
{
	components.push_back(new TextUnit("Hello world, how are you?"));
	components.push_back(new TextUnit("ok"));
}

MonoLineText::~MonoLineText()
{
}

void MonoLineText::draw(Shader& shader,TextSettings ts){
	ts.italic= true;
	for(auto& tc:components){
		tc->draw(shader,ts);
		ts.italic = false;
		ts.x += tc->getWidth(ts);
	}
}

int MonoLineText::getHeight(const TextSettings& ts){
	int maxUp 	= 0;
	int maxDown = 0;
	for(auto& tc:components){
		int tcUp 	= tc->getHeight(ts)+tc->getYOffset(ts);
		int tcDown 	= -tc->getYOffset(ts);

		if(tcUp>maxUp)
			maxUp = tcUp;
		if(tcDown>maxDown)
			maxDown = tcDown;
	}
	return maxUp+maxDown;
}
int MonoLineText::getWidth(const TextSettings& ts){
	int rv = 0;
	for(auto& tc:components){
		rv+=tc->getWidth(ts);
	}
	return rv;
}
