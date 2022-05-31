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
 
 void PolyLineText::draw(Shader& shader,TextSettings ts,const double x,const double y){
	for(auto& b:blocks){
		b.draw(shader,ts,x,y);
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
int PolyLineText::select_selectableCount(){
	int rv = 0;
	for(auto& b:blocks){
		rv+=b.select_selectableCount();
	}
	return rv;
}
int PolyLineText::select_index(glm::vec2 mousePositionRelative2Text,TextSettings ts){
	int height = getHeight(ts);
	int width = getRealWidth(ts);

	//not in this text.
	if(not(ts.x<=mousePositionRelative2Text.x && mousePositionRelative2Text.x<ts.x+width
	     &&ts.y<=mousePositionRelative2Text.y && mousePositionRelative2Text.y<ts.y+height)){
		 return -1;
	}

	int rv = 0;

	for(auto& b:blocks){
		auto blockheight = b.getHeight(ts);
		auto blockwidth = getRealWidth(ts);
		
		if(ts.x<=mousePositionRelative2Text.x && mousePositionRelative2Text.x<ts.x+blockwidth
		 &&ts.y<=mousePositionRelative2Text.y && mousePositionRelative2Text.y<ts.y+blockheight){
			int indexInTB = b.select_index(mousePositionRelative2Text,ts);
			rv += (indexInTB==-1)?b.select_selectableCount():indexInTB;
			return rv;
		}
		rv += b.select_selectableCount();
		ts.y += b.getHeight(ts);
	}
	return rv;
}
