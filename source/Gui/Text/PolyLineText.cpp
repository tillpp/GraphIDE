#include "PolyLineText.h"

PolyLineText::PolyLineText(/* args */){
	lines.push_back(MonoLineText());
	lines.push_back(MonoLineText());
}
 
 PolyLineText::~PolyLineText()
 {
 }
 
 void PolyLineText::draw(Shader& shader,TextSettings ts){
	for(auto& mlt:lines){
		mlt.draw(shader,ts);
		ts.y += mlt.getHeight(ts);
	}
 }
int PolyLineText::getHeight(const TextSettings& ts){
	int sum = 0;
	for(auto& mlt:lines){
		sum+=mlt.getHeight(ts);
	}
	return sum;
}
int PolyLineText::getWidth(const TextSettings& ts){
	int maxWidth = 0;
	for(auto& mlt:lines){
		int width = mlt.getWidth(ts);
		if(maxWidth>width)
			maxWidth=width;
	}
	return maxWidth;
}
