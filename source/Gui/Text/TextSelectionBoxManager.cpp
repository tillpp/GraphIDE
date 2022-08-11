#include "TextSelectionBoxManager.h"
#include <algorithm>
#include "Util/Log.h"

TextSelectionBoxManager::TextSelectionBoxManager(TextComponent*& tc)
	:textComponent(tc){
}
void TextSelectionBoxManager::start(size_t index){
	auto box = new Box;
	box->start =  box->cursor = index;
	lastBox = box;
	textComponent->addCursor(index);

	boxes.push_back(box);
	sortBoxArray();
}
void TextSelectionBoxManager::end(size_t index){
	if(!lastBox){
		start(index);
	}

	if(lastBox && lastBox->cursor != index ){
		auto min = std::min(lastBox->cursor,index);
		auto max = std::max(lastBox->cursor,index);
		

		if(max <= lastBox->start){
			//both left of start
			if(lastBox->cursor < index)
				textComponent->deselect	(min, max-min); 
			else
				textComponent->select	(min, max-min); 

		}else if(lastBox->start <= min){
			//both right of start
			if(lastBox->cursor < index)
				textComponent->select	(min, max-min); 
			else
				textComponent->deselect	(min, max-min); 
		}else{
			//one left,one right of start
			if(lastBox->cursor < lastBox->start){
				textComponent->deselect	(min,			 lastBox->start-min);
				textComponent->select	(lastBox->start, max-lastBox->start);
			}else{
				textComponent->select	(min,			 lastBox->start-min);
				textComponent->deselect	(lastBox->start, max-lastBox->start);
			}
		}

		textComponent->removeCursor(lastBox->cursor);
		textComponent->addCursor(index);
		lastBox->cursor = index;
		sortBoxArray();
		
	}
}
void TextSelectionBoxManager::sortBoxArray(){
	if(!lastBox)
		return;
	//not the most optimal, but hey - the array propably won't be big
	std::sort(boxes.begin(),boxes.end(),[](Box* a, Box* b)->bool{
		auto amin = std::min(a->start,a->cursor);
		auto bmin = std::min(b->start,b->cursor);
		return (amin < bmin);
	});
	
	size_t prevMin = 0;
	size_t prevMax = 0;
	
	for(int i = 0;i<boxes.size();i++){
		auto box = boxes[i];
		
		auto min = std::min(box->cursor,box->start);
		auto max = std::max(box->cursor,box->start);

		auto totalMin = std::min(min,prevMin);
		auto totalMax = std::max(max,prevMax);

		//do the to intervalls fit in the total Intervall, 
		//no = collision:
		if(i && (max-min)+(prevMax-prevMin) >= (totalMax-totalMin))
		{
			auto prevBox = boxes[i-1];
			
			bool boxRightDirection		= box->start < box->cursor;
			bool prevBoxRightDirection 	= prevBox->start < prevBox->cursor;

			textComponent->removeCursor(prevBox->cursor);
			textComponent->removeCursor(box->cursor);

			if	(box->start == box->cursor){
				//remove box
				delete box;
				// this is the lastBox
				if(lastBox == box){
					lastBox = nullptr;
					//removing prevBox & box from Boxes
					textComponent->deselect(prevMin,prevMax-prevMin);
					delete prevBox;
					boxes.erase(std::remove_if(boxes.begin(),boxes.end(),[&](Box* c)->bool{return box==c||prevBox==c;}),boxes.end());
				}else{
					//prevBox overriding box i.e removing box
					textComponent->addCursor(prevBox->cursor);
					boxes.erase(std::remove_if(boxes.begin(),boxes.end(),[&](Box* c)->bool{return box==c;}),boxes.end());
				}
			}
			else if	(box->start != box->cursor){
				//new box
				prevBox->cursor = boxRightDirection? totalMax : totalMin;
				prevBox->start  = boxRightDirection? totalMin : totalMax;

				textComponent->addCursor(prevBox->cursor);

				//remove old box
				delete box;
				if(lastBox==box)
					lastBox = prevBox;

				boxes.erase(std::remove_if(boxes.begin(),boxes.end(),[&](Box* c)->bool{return box==c;}),boxes.end());
			}
			//	start again:
			i = -1;
			continue;

		}
		
		prevMax = max;
		prevMin = min;
	}
}
/*
############################	Old Version of sortBoxArray		############################

size_t lastMin = std::min(lastBox->cursor,lastBox->start);
size_t lastMax = std::max(lastBox->cursor,lastBox->start);
	
for(int i = 0;i<boxes.size();i++){
	auto box = boxes[i];
	if(box==lastBox)
		continue;
	auto min = std::min(box->cursor,box->start);
	auto max = std::max(box->cursor,box->start);
	auto totalMin = std::min(min,lastMin);
	auto totalMax = std::max(max,lastMax);

	//do the to intervalls fit in the total Intervall, 
	//no = collision:
	if((max-min)+(lastMax-lastMin) >= (totalMax-totalMin))
	{
		auto a = min<lastMin? box 		: lastBox;
		auto b = min<lastMin? lastBox 	: box;
		
		bool boxRightDirection		= box->start < box->cursor;
		bool lastBoxRightDirection 	= lastBox->start < lastBox->cursor;

		textComponent->removeCursor(a->cursor);
		textComponent->removeCursor(b->cursor);

		if		(lastBox->start == lastBox->cursor){
			//remove both
			textComponent->deselect(min,max-min);
			delete a,b;
			lastBox = nullptr;
			boxes.erase(std::remove_if(boxes.begin(),boxes.end(),[&](Box* c)->bool{return a==c||b==c;}),boxes.end());
			return;
		}
		else if	(lastBox->start != lastBox->cursor){
			//new box
			a->cursor = lastBoxRightDirection? totalMax : totalMin;
			a->start  = lastBoxRightDirection? totalMin : totalMax;

			textComponent->addCursor(a->cursor);

			//remove old box
			delete b;
			lastBox = a;
			boxes.erase(std::remove_if(boxes.begin(),boxes.end(),[&](Box* c)->bool{return b==c;}),boxes.end());
		}
		//	start again:
		i = -1;
		continue;
	}
	
	lastMax = max;
	lastMin = min;
}
############################	Old Version of sortBoxArray		############################
*/
void TextSelectionBoxManager::moveHorizontal(bool rightDirection, bool resetStart){
	sortBoxArray();
	//remove all cursors
	for(auto& x:boxes){
		textComponent->removeCursor(x->cursor);

		//already on the side
		if(rightDirection? x->cursor == textComponent->cursorSlotCount()-1 : x->cursor == 0)
		{
			//	deselect
			if(resetStart)
			{
				auto min = std::min(x->cursor,x->start);
				auto max = std::max(x->cursor,x->start);
				textComponent->deselect(min,max-min);
			
				x->start = x->cursor;
			}
			continue;
		}
			
		
		if(resetStart){
			//	deselect
			auto min = std::min(x->cursor,x->start);
			auto max = std::max(x->cursor,x->start);
			textComponent->deselect(min,max-min);
			
			x->cursor += rightDirection?1:-1;
			x->start = x->cursor;
		}else{
			//	select &deselect
			int position = x->cursor + (rightDirection? 0 : -1);
			if(rightDirection ? x->start <= x->cursor : x->cursor <= x->start )
				textComponent->select(position,1);
			if(rightDirection ? x->cursor < x->start : x->start < x->cursor)
				textComponent->deselect(position,1);

			x->cursor += rightDirection?1:-1;
		}	
	}
	//add all cursors
	for(auto& x:boxes){
		textComponent->addCursor(x->cursor);
	}
	sortBoxArray();
}
void TextSelectionBoxManager::moveCtrl(bool rightDirection, bool resetStart){
	sortBoxArray();
	//remove all cursors
	for(auto& x:boxes){
		textComponent->removeCursor(x->cursor);
		size_t cursorWishPosition = textComponent->nextCtrlCursorSlot(x->cursor,rightDirection);
		if(cursorWishPosition == sf::String::InvalidPos)
			cursorWishPosition = rightDirection ? textComponent->cursorSlotCount()-1: 0;

		if(resetStart){
			//	deselect
			auto min = std::min(x->cursor,x->start);
			auto max = std::max(x->cursor,x->start);
			textComponent->deselect(min,max-min);
			
			x->cursor = cursorWishPosition;
			x->start = x->cursor;
		}else{
			//	select & deselect
			auto min = std::min(x->cursor,cursorWishPosition);
			auto max = std::max(x->cursor,cursorWishPosition);
			
			if(rightDirection ? x->start <= x->cursor : x->cursor <= x->start ){
				textComponent->select(min,max-min);
			}
			if(rightDirection ? x->cursor < x->start : x->start < x->cursor){
				if(max<=x->start || x->start<=min)
					//not crossing x->start
					textComponent->deselect(min,max-min);
				else{
					//crossing x->start
					int deselectMin = std::min(x->cursor,x->start);
					int deselectMax = std::max(x->cursor,x->start);

					int selectMin = std::min(x->start,cursorWishPosition);
					int selectMax = std::max(x->start,cursorWishPosition);
					
					textComponent->deselect(deselectMin,deselectMax-deselectMin);
					textComponent->select(selectMin,selectMax-selectMin);
				}
			}
			x->cursor = cursorWishPosition;
		}
	}
	//add all cursors
	for(auto& x:boxes){
		textComponent->addCursor(x->cursor);
	}
	sortBoxArray();
}
// >> basicly the same code as moveCtrl | maybe generalize in the future.
void TextSelectionBoxManager::moveVertical(bool downDirection, bool resetStart){
	sortBoxArray();
	//remove all cursors
	for(auto& x:boxes){
		textComponent->removeCursor(x->cursor);
		size_t cursorWishPosition = textComponent->nextVerticalCursorSlot(x->cursor,downDirection);
		if(cursorWishPosition == sf::String::InvalidPos)
			cursorWishPosition = downDirection ? textComponent->cursorSlotCount()-1: 0;

		if(resetStart){
			//	deselect
			auto min = std::min(x->cursor,x->start);
			auto max = std::max(x->cursor,x->start);
			textComponent->deselect(min,max-min);
			
			x->cursor = cursorWishPosition;
			x->start = x->cursor;
		}else{
			//	select & deselect
			auto min = std::min(x->cursor,cursorWishPosition);
			auto max = std::max(x->cursor,cursorWishPosition);
			
			if(downDirection ? x->start <= x->cursor : x->cursor <= x->start ){
				textComponent->select(min,max-min);
			}
			if(downDirection ? x->cursor < x->start : x->start < x->cursor){
				if(max<=x->start || x->start<=min)
					//not crossing x->start
					textComponent->deselect(min,max-min);
				else{
					//crossing x->start
					int deselectMin = std::min(x->cursor,x->start);
					int deselectMax = std::max(x->cursor,x->start);

					int selectMin = std::min(x->start,cursorWishPosition);
					int selectMax = std::max(x->start,cursorWishPosition);
					
					textComponent->deselect(deselectMin,deselectMax-deselectMin);
					textComponent->select(selectMin,selectMax-selectMin);
				}
			}
			x->cursor = cursorWishPosition;
		}
	}
	//add all cursors
	for(auto& x:boxes){
		textComponent->addCursor(x->cursor);
	}
	sortBoxArray();
}
void TextSelectionBoxManager::removeAll(){
	for(auto& x:boxes){
		textComponent->removeCursor(x->cursor);
		auto min = std::min(x->cursor,x->start);
		auto max = std::max(x->cursor,x->start);
		
		textComponent->deselect(min,max-min);

		Log::info(" ## Cursor "+std::to_string(min));

		delete x;
	}
	lastBox = nullptr;
	boxes.clear();
}
int TextSelectionBoxManager::nextStructuralCursorSlot(int cursorSlot, bool rightDirection,unsigned int layer){
	auto rv = textComponent->nextStructuralCursorSlot(cursorSlot, rightDirection,layer);
	if((size_t)rv == sf::String::InvalidPos)
		return rightDirection ? textComponent->cursorSlotCount()-1: 0;
	return rv;
}
void TextSelectionBoxManager::expand(size_t left, size_t right){
	if(lastBox){
		bool rightDirection = lastBox->start < lastBox->cursor;

		textComponent->removeCursor(lastBox->cursor);

		auto newStart = rightDirection? std::min(left ,lastBox->start)  : std::max(right,lastBox->start);
		auto newCursor= rightDirection? std::max(right,lastBox->cursor) : std::min(left ,lastBox->cursor);
		
		if(rightDirection)
			textComponent->select(newStart, newCursor-newStart);
		else
			textComponent->select(newCursor, newStart-newCursor);

		lastBox->start = newStart;
		lastBox->cursor= newCursor;
		
		textComponent->addCursor(newCursor);

		sortBoxArray();
		return;
	}
	else {
		start(left);
		end(right);
	}
}
void TextSelectionBoxManager::inputText(const sf::String& textInput){
	bool hasSeperator = textInput.find("\x1f") != sf::String::InvalidPos;
	size_t afterLastSeperator = 0;

	int offset = 0;
	for(auto& box:boxes){
		box->cursor += offset;
		box->start  += offset;

		sf::String textChunk;
		
		if(hasSeperator){
			if(afterLastSeperator==sf::String::InvalidPos)
				textChunk = "";
			else{
				size_t seperator = textInput.find("\x1f",afterLastSeperator);
				if(seperator==sf::String::InvalidPos){
					textChunk = textInput.substring(afterLastSeperator);
					afterLastSeperator = seperator;
				}
				else{
					textChunk = textInput.substring(afterLastSeperator,seperator-afterLastSeperator);
					afterLastSeperator = seperator+1;
				}
			}
		}else{
			textChunk = textInput;
		}

		int left   = box->cursor < box->start ? box->cursor : box->start;
		int right  = box->cursor < box->start ? box->start  : box->cursor;
		textComponent->deleteText(left, right);
		textComponent->inputText(left,textChunk);
		
		box->cursor = left + textChunk.getSize();
		box->start = box->cursor;
		
		textComponent->addCursor(box->cursor);

		offset += (left - right) + textChunk.getSize();
	}
}
sf::String TextSelectionBoxManager::copyText(){
	sf::String rv;
	bool first = true;
	for(auto& box:boxes){
		auto& left   = box->cursor < box->start ? box->cursor : box->start;
		auto& right  = box->cursor < box->start ? box->start  : box->cursor;

		rv += (!first?"\x1f":"") + textComponent->copyText(left,right);
		first = false;
	}
	return rv;
}
void TextSelectionBoxManager::deleteText(bool rightDirection, bool ctrl){

	int offset = 0;
	for(auto& box:boxes){
		box->cursor += offset;
		box->start  += offset;

		int left   = box->cursor < box->start ? box->cursor : box->start;
		int right  = box->cursor < box->start ? box->start  : box->cursor;
		
		if(left==right){
			if(ctrl)
				(rightDirection? right : left) = textComponent->nextCtrlCursorSlot(rightDirection ? right : left,rightDirection);
			else 
				(rightDirection? right : left) = rightDirection ? right+1 : left-1;
		}
		if(left < 0)
			left = 0;

		textComponent->deleteText(left, right);
		
		box->cursor = left;
		box->start = box->cursor;
		
		textComponent->addCursor(box->cursor);

		offset += (left - right);
	}
}
void TextSelectionBoxManager::applyTextStyler(TextComponent& style){
	for(auto& box:boxes){
		auto& left   = box->cursor < box->start ? box->cursor : box->start;
		auto& right  = box->cursor < box->start ? box->start  : box->cursor;
 

		auto first = textComponent->findFirst(style.getType(),left,right);
		if(first)
			textComponent->explodeTextStyler(style.getType(),left,right);
		else {
			auto debris = textComponent->applyTextStyler(style,left,right);
			for(auto x: debris)
				if(x != textComponent)
					delete x;
		}
	}
}
