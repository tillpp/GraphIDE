#include "TextLine.h"
#include "TextUnit.h"
#include "Util/Log.h"

TextLine::TextLine()
{
	Log::debug("TextLine::TextLine ");
}
TextLine::TextLine(sf::String text)
{
	insert(new TextUnit(text));
	insert(new TextUnit(text));
	Log::debug("TextLine::TextLine ");
}

TextLine::~TextLine()
{
	for(auto& x:children)
		delete x;
}

//>> might need update 
TextComponent* TextLine::copy(){
	auto rv = new TextLine();
	for(auto& x:children){
		rv->children.push_back(x->copy());
	}
	rv->updateChildData();
	return rv;
}
void TextLine::draw(Shader&shader,TextSettings& textSettings,const double guiComponentx,const double guiComponenty,bool draw){
	int leftOffset = 0;

	int curMaxUp = 0;
	int curMaxDown = 0;

	for(auto child:children){
		
		int up   = child->height+child->getYOffset();
		int down = -child->getYOffset();
		
		if(up>curMaxUp)
			curMaxUp = up;
		if(down>curMaxDown)
			curMaxDown = down;
		
		child->left = leftOffset + left;
		child->top = maxUp-up    + top;
		
		child->draw(shader,textSettings,guiComponentx,guiComponenty,draw);
		leftOffset += child->width;  

	}
	maxUp = curMaxUp;
	maxDown = curMaxDown;
	
	height = maxUp+maxDown;
	width = leftOffset;
}
int TextLine::getYOffset(){
	return -maxDown;
}
std::string TextLine::getType(){
	return "TextLine";
}

// ## Split & Merge ##
std::vector<TextComponent*> TextLine::split(const int TextBoxWidth,const TextSettings &ts){
	return {};//TODO
}
bool TextLine::merge(TextComponent* left){
	return false;//TODO
}

// ## CursorSlot ##
int TextLine::cursorSlotCount(){
	if(childMaxCursorSlot.size()==0)
		return 0;
	return childMaxCursorSlot[childMaxCursorSlot.size()-1]+1;
}
int TextLine::cursorSlotIndex(glm::vec2 mousePositionRelative2TextComponent){
	GLfloat xOffset = left;

	for (size_t i = 0; i < children.size(); i++)
	{
		GLfloat childWidth = children[i]->width;

		if(mousePositionRelative2TextComponent.x < xOffset+childWidth)
			return  (i?childMaxCursorSlot[i-1]:0) + children[i]->cursorSlotIndex(mousePositionRelative2TextComponent);
		
		xOffset += childWidth;
	}
	return cursorSlotCount() ? cursorSlotCount()-1 : 0;
}
glm::vec2 TextLine::cursorSlotPositition(int cursorSlot){
	for (size_t i = 0; i < children.size(); i++)
	{	
		if(cursorSlot <= childMaxCursorSlot[i])
			return  children[i]->cursorSlotPositition(cursorSlot-(i?childMaxCursorSlot[i-1]:0));	
	}
	return glm::vec2(0,0);
}

/* sf::String::InvalidPos if cursorSlot position is in the next */
int TextLine::nextCtrlCursorSlot      (int globalCursorSlot, bool rightDirection ){

	int currentChild 	       = 0;
	int currentLocalCursorClot = 0;

	//find the currentChlid and it's localCursorSlot
	for (int i = 0; i < childMaxCursorSlot.size(); i++)
	{
		//must be in this child
		if(globalCursorSlot <= childMaxCursorSlot[i]){
			currentChild           = i;
			currentLocalCursorClot = globalCursorSlot - (i? childMaxCursorSlot[i-1] :0);
			break;
		}
	}

	//repeatitly search
	do{
		if((size_t)currentLocalCursorClot == sf::String::InvalidPos)
		{
			currentChild += rightDirection? 1 : -1;  

			//not in here
			if(currentChild >= children.size())
				return sf::String::InvalidPos;
			if(currentChild < 0)
				return 0;

			currentLocalCursorClot = rightDirection? 0 :  children[currentChild]->cursorSlotCount()-1;
		}		
		currentLocalCursorClot = children[currentChild]->nextCtrlCursorSlot(currentLocalCursorClot, rightDirection);
	
	}while((size_t)currentLocalCursorClot == sf::String::InvalidPos);

	auto& i = currentChild;
	return (i? childMaxCursorSlot[i-1] :0)+currentLocalCursorClot;
}
// >> basicly the same code as nextCtrlCursorSlot
int TextLine::nextStructuralCursorSlot(int globalCursorSlot, bool rightDirection , int layer){
	
	int currentChild 	       = 0;
	int currentLocalCursorClot = 0;

	//find the currentChlid and it's localCursorSlot
	for (int i = 0; i < childMaxCursorSlot.size(); i++)
	{
		//must be in this child
		if(globalCursorSlot <= childMaxCursorSlot[i]){
			currentChild           = i;
			currentLocalCursorClot = globalCursorSlot - (i? childMaxCursorSlot[i-1] :0);
			break;
		}
	}

	//repeatitly search
	do{
		if((size_t)currentLocalCursorClot == sf::String::InvalidPos)
		{
			currentChild += rightDirection? 1 : -1;  

			//not in here
			if(currentChild >= children.size())
				return sf::String::InvalidPos;
			if(currentChild < 0)
				return 0;

			currentLocalCursorClot = rightDirection? 0 :  children[currentChild]->cursorSlotCount()-1;
		}		
		currentLocalCursorClot = children[currentChild]->nextStructuralCursorSlot(currentLocalCursorClot, rightDirection, layer);

	}while((size_t)currentLocalCursorClot == sf::String::InvalidPos);

	auto& i = currentChild;
	return (i? childMaxCursorSlot[i-1] :0)+currentLocalCursorClot;
} 
int TextLine::nextVerticalCursorSlot  (int cursorSlot, bool downDirection ){
	return downDirection?  cursorSlotCount()-1 : 0;
}

// ## Cursor & Selection ##
void TextLine::addCursor(int cursorSlot){
	for (size_t i = 0; i < children.size(); i++)
	{
		if(cursorSlot <= childMaxCursorSlot[i])
			return  children[i]->addCursor(cursorSlot-(i?childMaxCursorSlot[i-1]:0));
	}
}
void TextLine::removeCursor(int cursorSlot){
	for (size_t i = 0; i < children.size(); i++)
	{
		if(cursorSlot <= childMaxCursorSlot[i])
			return  children[i]->removeCursor(cursorSlot-(i?childMaxCursorSlot[i-1]:0));
	}
}
void TextLine::select(int index,int amount){
	for (size_t i = 0; i < children.size(); i++)
	{
		auto start = (i?childMaxCursorSlot[i-1]:0);
		auto end   = childMaxCursorSlot[i];

		//select one child
		if(index <= childMaxCursorSlot[i]){
			auto localIndex = index-start;

			children[i]->select(localIndex, amount);
			
			// update for the next child
			amount -=  end - index;
			index = childMaxCursorSlot[i];
			if(amount < 0)
				return;
		}
	}
}
void TextLine::deselect(int index,int amount){
	for (size_t i = 0; i < children.size(); i++)
	{
		auto start = (i?childMaxCursorSlot[i-1]:0);
		auto end   = childMaxCursorSlot[i];

		//deselect one child
		if(index <= childMaxCursorSlot[i]){
			int localIndex = index-start;

			children[i]->deselect(localIndex, amount);
			
			// update for the next child
			amount -=  end - index;
			index = childMaxCursorSlot[i];
			if(amount < 0)
				return;
		}
	}
}

// ## Events ##
void TextLine::triggerEvent(const GuiEvent& event){
}
// ## Cursor Events ##
void TextLine::inputText(int globalCursorSlot, const sf::String& textInput){
	for (size_t i = 0; i < children.size(); i++)
	{
		if(globalCursorSlot <= childMaxCursorSlot[i])
		{
			auto start = (i?childMaxCursorSlot[i-1]:0);
			auto localCursorSlot = globalCursorSlot-start;

			children[i]->inputText(localCursorSlot,textInput);
			break;
		}
	}
	updateChildData();
}
void TextLine::deleteText(int startCursorSlot, int endCursorSlot){
	
	for (size_t i = 0; i < children.size(); i++)
	{
		if(startCursorSlot <= childMaxCursorSlot[i])
		{
			auto start = (i?childMaxCursorSlot[i-1]:0);
			int localStartCursorSlot = startCursorSlot-start;
			int localEndCursorSlot   = endCursorSlot  -start;

			children[i]->deleteText(localStartCursorSlot,localEndCursorSlot);
			
			/* 
				if it isn't the first:
				remove last cursor in TC, because it isn't accessable from the Textline.
			*/
			if(i){
				children[i]->removeCursor(0);
			}
			
			startCursorSlot = childMaxCursorSlot[i];
			if(endCursorSlot <= startCursorSlot)
				break;
		}
	}
	updateChildData();
}
sf::String TextLine::copyText(int startCursorSlot, int endCursorSlot){
	sf::String rv;
	for (size_t i = 0; i < children.size(); i++)
	{
		auto start = (i?childMaxCursorSlot[i-1]:0);
		auto end   = childMaxCursorSlot[i];

		if(startCursorSlot <= childMaxCursorSlot[i])
		{
			int localStartCursorSlot = startCursorSlot-start;
			int localEndCursorSlot   = endCursorSlot-start;

			rv += children[i]->copyText(localStartCursorSlot, localEndCursorSlot);

			startCursorSlot = childMaxCursorSlot[i];
			
			if(startCursorSlot >= endCursorSlot)
				break;
		}
	}
	return rv;
}

// ## Styler ##
std::vector<TextComponent*>	TextLine::split(int cursorSlot, bool onlyChildren){
	
	//split children
	for (int i = 0; i < children.size(); i++)
	{
		auto start = (i?childMaxCursorSlot[i-1]:0);
		auto end   = childMaxCursorSlot[i];
		
		if(cursorSlot < end){

			int localCursorSlot = cursorSlot-start;
			
			auto current = children[i];
			auto debris = children[i]->split(localCursorSlot, false);

			//perfomance optimisation
			if(debris.size() == 1 ? debris[0] == current :0){
				//dont do anything.
			}else if(debris.size()==0){
				//remove this one
				children.erase(children.begin()+i);
				delete current;
			}
			else if(debris.size()){
				//remove this one, and place debris instead
				children.erase(children.begin()+i);
				children.insert(children.begin()+i, debris.begin(), debris.end());

				//delete current if it isn't in the debris
				bool currentIsInDebris = false;
				for(auto& x:debris)
					if(x == current)
						currentIsInDebris = true;
				if(!currentIsInDebris)
					delete current;

				//dummy placeholders
				childMaxCursorSlot.insert(childMaxCursorSlot.begin()+i+1, debris.size()-1,end);
			}
			i += (int)debris.size()-1;
			break;
		}
	}
	updateChildData();
	if(onlyChildren)
		return {this};
		
	//segment the children
	TextComponent* next = nullptr;
	size_t firstChildIndex = sf::String::InvalidPos;
	for (int i = 0; i < children.size(); i++)
	{
		auto end   = childMaxCursorSlot[i];
		
		if(cursorSlot < end){
			//only the first time
			if(firstChildIndex == sf::String::InvalidPos){
				next = copy();
				firstChildIndex = i;
			}
			next->insert(children[i]);
		}
	}
	children.erase(children.begin()+firstChildIndex,children.end());
	updateChildData();
	if(next)
		return {this,next};
	return {this};
}
#include <iostream>
std::vector<TextComponent*>	TextLine::applyTextStyler(TextComponent& style, int startCursorSlot, int endCursorSlot){
	
	//find max. 2 children that are only partily selected
	int first = -1;
	int last  = -1;
	int middle_begin = 0;
	int middle_last = children.size()-1;
	for (int i = 0; i < children.size(); i++)
	{
		auto start = (i?childMaxCursorSlot[i-1]:0);
		auto end   = childMaxCursorSlot[i];

		if(start < startCursorSlot && startCursorSlot < end){
			first = i;
		}
		if(start < endCursorSlot && endCursorSlot < end){
			last = i;
		}
		if(start < startCursorSlot && startCursorSlot <= end)
			middle_begin = i+1;
		if(start <= endCursorSlot && endCursorSlot < end)
			middle_last = i-1;
	}

	print();
	std::cout << "first:"<<first <<std::endl;
	std::cout << "last:"<<last <<std::endl;
	std::cout << "middle_begin:"<<middle_begin <<std::endl;
	std::cout << "middle_last:" <<middle_last <<std::endl;

	//offset by the children being moved
	int offsetMiddle = 0;

	//split partly selected TextComponents
	std::vector<int> partlySelected; /* reversed ordered (last to first)*/
	if(last != -1)
		partlySelected.push_back(last);
	if(first != -1 && first != last)
		partlySelected.push_back(first);
	for(auto i:partlySelected){
		auto start = (i?childMaxCursorSlot[i-1]:0);
		auto end   = childMaxCursorSlot[i];

		int localStartCursorSlot = startCursorSlot-start;
		int localEndCursorSlot   = endCursorSlot-start;

		auto current = children[i];
		auto debris = children[i]->applyTextStyler(style,localStartCursorSlot, localEndCursorSlot);

		//perfomance optimisation
		if(debris.size() == 1 ? debris[0] == current :0){
			//dont do anything.
		}else if(debris.size()==0){
			//remove this one
			children.erase(children.begin()+i);
			delete current;
		}
		else if(debris.size()){
			//remove this one, and place debris instead
			children.erase(children.begin()+i);
			children.insert(children.begin()+i, debris.begin(), debris.end());
			
			//delete current if it isn't in the debris
			bool currentIsInDebris = false;
			for(auto& x:debris)
				if(x == current)
					currentIsInDebris = true;
			if(!currentIsInDebris)
				delete current;
			
			//dummy placeholders
			childMaxCursorSlot.insert(childMaxCursorSlot.begin()+i+1, debris.size()-1,end);
		}
		if(i<middle_begin)
			offsetMiddle += (int)debris.size()-1;
	}
	updateChildData();
	
	// arent there any in the middle?
	if(middle_begin > middle_last)
		return {this};

	print();

	middle_begin += offsetMiddle;
	middle_last += offsetMiddle;

	//set styler in the middle
	auto newStilyer = style.copy();
	for (size_t i = middle_begin; i <= middle_last; i++)
	{
		newStilyer->insert(children[i]);
	}
	
	children.erase(children.begin()+middle_begin, children.begin()+middle_last+1);
	children.insert(children.begin()+middle_begin, newStilyer);

	print();

	updateChildData();
	return {this};
}
std::vector<TextComponent*>	TextLine::explodeTextStyler(std::string type,int startCursorSlot, int endCursorSlot){

	for (size_t i = 0; i < children.size(); i++)
	{
		auto start = (i?childMaxCursorSlot[i-1]:0);
		auto end   = childMaxCursorSlot[i];

		//select one child
		if(startCursorSlot <= end){
			int localStartCursorSlot = startCursorSlot-start;
			int localEndCursorSlot   = endCursorSlot-start;

			auto current = children[i];
			auto debris = current->explodeTextStyler(type, localStartCursorSlot, localEndCursorSlot);
			
			//perfomance optimisation
			if(debris.size()==1 ? debris[0]==current :0){
				//dont do anything.
			}else if(debris.size()==0){
				//remove this one
				children.erase(children.begin()+i);
				delete current;
			}
			else if(debris.size()){
				//remove this one, and place debris instead
				children.erase(children.begin()+i);
				children.insert(children.begin()+i, debris.begin(), debris.end());
				
				//delete current if it isn't in the debris
				bool currentIsInDebris = false;
				for(auto& x:debris)
					if(x == current)
						currentIsInDebris = true;
				if(!currentIsInDebris)
					delete current;

				//dummy placeholders
				childMaxCursorSlot.insert(childMaxCursorSlot.begin()+i+1, debris.size()-1,end);
			}
			i += (int)debris.size()-1;

			startCursorSlot = end;
			if(startCursorSlot >= endCursorSlot)
				break;
		}
	}
	updateChildData();

	if(type == this->getType())
		return children;
	
	return {this};
}
TextComponent* TextLine::findFirst(std::string type,		int startCursorSlot, int endCursorSlot){
	if(type==this->getType())
		return this;

	for (size_t i = 0; i < children.size(); i++)
	{
		auto start = (i?childMaxCursorSlot[i-1]:0);
		auto end   = childMaxCursorSlot[i];

		//select one child
		if(startCursorSlot <= end){
			int localStartCursorSlot = startCursorSlot-start;
			int localEndCursorSlot   = endCursorSlot-start;

			auto x = children[i]->findFirst(type, localStartCursorSlot, localEndCursorSlot);
			if(x!=nullptr)
				return x;
			startCursorSlot = end;
			if(startCursorSlot >= endCursorSlot)
				break;
		}
	}
	return nullptr;
}

// ## Children ##
void TextLine::insert(TextComponent* textComponent){
	children.push_back(textComponent);
	updateChildData();
}
void TextLine::updateChildData(){
	childMaxCursorSlot.resize(children.size());
	size_t cursorSlotOffset = 0;
	for (size_t i = 0; i < children.size(); i++){
		auto csc = children[i]->cursorSlotCount();
		childMaxCursorSlot[i] = cursorSlotOffset + (csc?csc-1:0);
		cursorSlotOffset += (csc?csc-1:0);
	}
}


void TextLine::print(std::string tabs){
	Log::info(tabs+"#type:"+getType());
	for(auto& x:children)
		x->print(tabs+"\t");
}