#include "TextBlock.h"
#include "TextUnit.h"

TextBlock::TextBlock(sf::String text)
{
	components.push_back(new TextUnit(text));
}

TextBlock::~TextBlock()
{
}

void TextBlock::goThroughLine(
	TextSettings& ts,
	std::function<void(int& i,int& j,int lineHeight)> F){


	double offsetx = 0;
	for (int i = 0; i < components.size();)
	{
		/*
			split the components if they are to big for the line.
			Determine start/end of line:

			i: start of line
			j: last of line
		*/
		int j = i;
		offsetx = 0; // the x offset
		while (j < components.size())
		{
			//does it exceed the line width? yes: split
			int tcWidth = components[j]->getWidth(ts);
			if (offsetx + tcWidth > maxLineWidth)
			{
				auto pieces = components[j]->split(offsetx, maxLineWidth, ts);
				
				if(pieces.size()>1){
					components.erase(components.begin()+j);
					components.insert(components.begin()+j,pieces.begin(),pieces.end());
					continue;
				}else if(offsetx>0){
					break;
				}
			}
			offsetx += tcWidth;
			j++;
		}
		/*
			Try to merge stuff.
		*/
		for (int k = i; k < j-1; k++)
		{
			if(components[k]->merge(components[k+1])){
				delete components[k+1];
				components.erase(components.begin()+k+1);
				j--;
				k--;
			}
		}
		/*
			calculate the line Height first,befor drawing.
		*/
		double lineHeight = calculateLineHeight(ts, i, j);
		
		

		/*
			do something with the line (draw? etc.)
		*/
		offsetx = 0; // the x offset

		auto startts = ts.x;
		F(i,j,lineHeight);
		ts.x = startts;

		//next line
		i=j;
		ts.y += lineHeight;
	}
}
void TextBlock::draw(Shader &shader, TextSettings ts)
{
	int count = 0;
	goThroughLine(ts,[&](int& i,int& j,int lineHeight){		
		while (/*i < components.size() &&*/ i < j)
		{
			ts.textColor.g = count++%2;
			auto &tc = components[i];
			tc->draw(shader, ts);
			ts.x += tc->getWidth(ts);
			i++;
		}
	});
}
int TextBlock::getHeight(const TextSettings& ets)
{
	TextSettings ts = ets;
	
	int maxHeight = 0;
	goThroughLine(ts,[&](int& i,int& j,int lineHeight){
		maxHeight += lineHeight;
	});
	return maxHeight;
}
int TextBlock::getRealWidth(const TextSettings& ets)
{	
	TextSettings ts = ets;
	
	int maxWidth = 0;
	goThroughLine(ts,[&](int& i,int& j,int lineHeight){
		int width = 0;
		while (/*i < components.size() &&*/ i < j)
		{
			auto &tc = components[i];
			width += tc->getWidth(ts);
			i++;
		}
		if(width>maxWidth)
			maxWidth = width;
	});
	return maxWidth;
}

int TextBlock::getInnerWidth(const TextSettings &ts)
{
	int maxWidth = 0;
	for (auto &c : components)
	{
		maxWidth += c->getWidth(ts); 
	}
	return maxWidth;
}
int TextBlock::calculateLineHeight(TextSettings ts, int i, int j)
{
	int maxUp = 0;
	int maxDown = 0;
	for (; i < j && i < components.size(); i++)
	{
		auto &tc = components[i];

		int tcUp = tc->getHeight(ts) + tc->getYOffset(ts);
		int tcDown = -tc->getYOffset(ts);

		if (tcUp > maxUp)
			maxUp = tcUp;
		if (tcDown > maxDown)
			maxDown = tcDown;
	}
	return maxUp + maxDown;
}