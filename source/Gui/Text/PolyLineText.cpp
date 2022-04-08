#include "PolyLineText.h"

PolyLineText::PolyLineText(sf::String text){
	//splitting the text into blocks
	size_t pos = 0,lastpos=0;
	while(true){
		pos = text.find('\n',lastpos);
		blocks.push_back(TextBlock(text.substring(lastpos,pos-lastpos)));
		if(pos==sf::String::InvalidPos)
			break;
		lastpos = pos+1;
	}

}
 
 PolyLineText::~PolyLineText()
 {
 }
 
 void PolyLineText::draw(Shader& shader,TextSettings ts){
	for(auto& b:blocks){
		b.draw(shader,ts);
		ts.y += b.getHeight(ts);
	}
 }
int PolyLineText::getHeight(const TextSettings& ts){
	int sum = 0;
	for(auto& b:blocks){
		sum+=b.getHeight(ts);
	}
	return sum;
}
int PolyLineText::getRealWidth(const TextSettings& ts){
	int maxWidth = 0;
	for(auto& b:blocks){
		int width = b.getRealWidth(ts);
		if(maxWidth<width)
			maxWidth=width;
	}
	return maxWidth;
}int PolyLineText::getInnerWidth(const TextSettings& ts){
	int maxWidth = 0;
	for(auto& b:blocks){
		int width = b.getInnerWidth(ts);
		if(maxWidth<width)
			maxWidth=width;
	}
	return maxWidth;
}
void PolyLineText::adjust(const int width){
	for(auto& b:blocks){
		b.maxLineWidth = width;
	}
}