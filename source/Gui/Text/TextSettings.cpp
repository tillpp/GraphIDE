#include "TextSettings.h"
GLfloat TextSettings::getDisplayTextureCharacterRatio()const{
	if(!font)
		return 0;
	return font->getDisplayTextureCharacterRatio(fontSize)*(smallScript?0.5:1);
}

//Font get Informations
GLfloat TextSettings::getBaseline(){
	return getDisplayTextureCharacterRatio()*font->getBaseline();
}
GLfloat TextSettings::getLineSpacing(){
	return getDisplayTextureCharacterRatio()*font->getLineSpacing();
}
GLfloat TextSettings::getKerning(const sf::String& string,unsigned int position){
	return getDisplayTextureCharacterRatio()*font->getKerning(string,position);
}
	
//Font underline information
GLfloat TextSettings::getUnderlinePosition(){
	return getDisplayTextureCharacterRatio()*font->getUnderlinePosition();
}
GLfloat TextSettings::getUnderlineThickness(){
	return getDisplayTextureCharacterRatio()*font->getUnderlineThickness();
}