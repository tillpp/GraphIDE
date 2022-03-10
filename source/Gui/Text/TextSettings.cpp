#include "TextSettings.h"

GLfloat TextSettings::getDisplayTextureCharacterRatio()const{
	if(!font)
		return 0;
	return font->getDisplayTextureCharacterRatio(fontSize)*(smallScript?0.5:1);
}