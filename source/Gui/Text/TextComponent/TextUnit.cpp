#include "TextUnit.h"
#include "Util/Log.h"
#include "OpenGL/Mesh.h"
#include "Util/StringAlgorithm.h"

TextUnit::TextUnit(sf::String text)
{
	this->text = text;
	characterAdvances.resize(text.getSize());
	selectedChar.resize(text.getSize());
	cursorSlot.resize(text.getSize()+1);
	
	Log::debug("TextUnit::TextUnit ");
}

TextUnit::~TextUnit()
{
}
TextComponent* TextUnit::copy(){
	return new TextUnit(text);
}
void TextUnit::draw(Shader&shader,TextSettings& ts,const double x,const double y,bool drawing){
	if(updateCharacterAdvances){
		updateCharacterAdvances = false;
		draw(shader,ts,x,y,false);
	}

	yOffset = ts.getBaseline();
	characterAdvances.resize(text.getSize());
	selectedChar.resize(text.getSize());
	cursorSlot.resize(text.getSize()+1);


	auto matrix = glm::translate(glm::mat4(1.f), glm::vec3(x, y, 1));
	
	if (!ts.font)
		Log::info("No font used.");

	
	//displayTextureCharacterRatio
	GLfloat dtcr = ts.getDisplayTextureCharacterRatio();
	//draw background
	//drawBackground(shader, ts, matrix,dtcr);
	
	//	drawing selection
	if(drawing)
	{
		glUniform4f(glGetUniformLocation(shader.getOpenGLID(), "textureRect"), 0, 0, 1, 1);
		glUniform4f(glGetUniformLocation(shader.getOpenGLID(), "color"), 0, 0, 1, 0.25);
		Texture::whiteTexture().use(0, shader, "texture1");

		GLfloat xpos 	= left;
		for(int i=0;i<text.getSize();i++){
			
			if(!selectedChar[i])
				continue;
			
			GLfloat start	= i?characterAdvances[i-1]:0;
			while(i<text.getSize() && selectedChar[i]) i++;
			GLfloat end 	= i?characterAdvances[i-1]:0;
			GLfloat width 	= end-start;
			
			glm::mat4 letterMatrix = glm::mat4(1.f);
			letterMatrix = glm::translate	(letterMatrix, glm::vec3(left+start, top, 0.f));
			letterMatrix = glm::scale		(letterMatrix, glm::vec3(width, ts.getLineSpacing(), 1.f));
			
			Mesh::rectangle().draw(shader, /*inMatrix */ matrix * letterMatrix);
		}
	}
	//	drawing Cursor
	if(drawing)
	{
		glUniform4f(glGetUniformLocation(shader.getOpenGLID(), "textureRect"), 0, 0, 1, 1);
		glUniform4f(glGetUniformLocation(shader.getOpenGLID(), "color"), 0, 0, 0, 1);
		Texture::whiteTexture().use(0, shader, "texture1");

		GLfloat xpos 	= left;
		for(int i=0; i<cursorSlot.size(); i++){
			
			if(!cursorSlot[i])
				continue;
			
			GLfloat start	= i?characterAdvances[i-1]:0;
			
			glm::mat4 letterMatrix = glm::mat4(1.f);
			letterMatrix = glm::translate	(letterMatrix, glm::vec3(left+start, top, 0.f));
			letterMatrix = glm::scale		(letterMatrix, glm::vec3(cursorWidth, ts.getLineSpacing(), 1.f));

			Mesh::rectangle().draw(shader, /*inMatrix */ matrix * letterMatrix);
		}
	}

	//draw Text
	ts.font->use(shader);
	GLfloat offset = 0;
	//draw Text
	for (size_t i = 0; i < text.getSize(); i++)
	{
		//tab
		if (text[i] == '\t')
		{
			float tabsize 	= ts.font->getGlyph(' ', ts.bold).advance * 4;
			int tabs 		= (int)offset / tabsize;
			offset 			= (tabs + 1) * tabsize;
			characterAdvances[i] = offset;
			continue;
		}

		// temporaries
		bool bold 		= ts.bold;
		bool italic 	= ts.italic;
		glm::vec4 color	= ts.textColor;

		/* 	CharacterEffect:preGlyph	*/
		for (auto &e : ts.effects)
			e->preGlyph(ts,text, i, offset, color, bold, italic);

		auto &glyph = ts.font->getGlyph(text[i], bold);

		//kerning
		offset += ts.getKerning(text, i);

		glm::vec4 drawRect;
		drawRect.x = dtcr * glyph.bounds.left + offset + left;
		drawRect.y = dtcr * (glyph.bounds.top + ts.font->getBaseline() + ts.font->getLineSpacing()) + top;
		drawRect.z = dtcr * glyph.bounds.width;
		drawRect.w = dtcr * glyph.bounds.height;
		GLfloat advance = dtcr * glyph.advance;

		/* 	CharacterEffect:preDraw	*/
		for (auto &e : ts.effects)
			e->preDrawGlyph(ts, text, drawing, i, drawRect, advance);

		/* Draw glyph */
		if(drawing)
		{
			glUniform4fv(glGetUniformLocation(shader.getOpenGLID(), "textureRect"), 1, glm::value_ptr(ts.font->getRelativTextureRectGlyph(glyph)));
			glUniform4f(glGetUniformLocation(shader.getOpenGLID(), "color"), color.r, color.g, color.b, color.a);

			glm::mat4 letterMatrix = glm::mat4(1.f);
			letterMatrix = glm::translate(letterMatrix, glm::vec3(drawRect.x, drawRect.y + drawRect.w, 0.f));
			if (italic)
				letterMatrix[1] += glm::vec4(-0.15, 0, 0, 0); //change for italic
			letterMatrix = glm::scale(letterMatrix, glm::vec3(drawRect.z, -drawRect.w, 1.f));

			Mesh::rectangle().draw(shader, /*inMatrix */ matrix * letterMatrix);
		}
		/* 	CharacterEffect:postDraw	*/
		for (auto &e : ts.effects)
			e->postDrawGlyph(ts, text, drawing, drawRect, i, advance);

		//offset advancing
		offset += advance;
		characterAdvances[i] = offset;
	}
	width = offset;
	height = ts.getLineSpacing();
	//Draw Underline
	//if (ts.underline)
	//	drawUnderline		(shader, ts,matrix, dtcr,offset);
	//Draw Strikethrough
	//if (ts.strikethrough)
	//	drawStrikethrough	(shader, ts,matrix, dtcr,offset);
}

int TextUnit::getYOffset(){
	return yOffset;
}
std::string TextUnit::getType(){
	return "TextUnit";
}

// ## Split & Merge ##
std::vector<TextComponent*> TextUnit::split(const int TextBoxWidth,const TextSettings &ts){
	return {};
}
bool TextUnit::merge(TextComponent* left){
	return false;
}
// ## CursorSlot ##
int TextUnit::cursorSlotCount(){
	return text.getSize()+1;
}
int TextUnit::cursorSlotIndex(glm::vec2 mousePositionRelative2TextComponent){
	for (size_t i = 0; i < characterAdvances.size(); i++)
	{
		GLfloat advance = characterAdvances[i]-(i?characterAdvances[i-1]:0);
		GLfloat totalLeft = left+(i?characterAdvances[i-1]:0);

		if(mousePositionRelative2TextComponent.x < totalLeft+advance){
			return (mousePositionRelative2TextComponent.x < totalLeft+advance/2) ? i : i+1;
		}
	}
	return characterAdvances.size();
}

glm::vec2 TextUnit::cursorSlotPositition(int i){
	if(i<=0)
		return glm::vec2(left,top);
	else if(0<i && i<=characterAdvances.size())
		return glm::vec2(left+characterAdvances[i-1],top);
	else return glm::vec2(left+width,top);
}
int TextUnit::nextCtrlCursorSlot(int cursorSlot, bool rightDirection){
	if(rightDirection){
		//	right
		cursorSlot = StringAlgorithm::skip(text,StringAlgorithm::whitespace,cursorSlot);
		cursorSlot = StringAlgorithm::find(text,StringAlgorithm::whitespace,cursorSlot);
	}else {
		//	left
		cursorSlot = StringAlgorithm::rskip(text,StringAlgorithm::whitespace,cursorSlot);
		cursorSlot = StringAlgorithm::rfind(text,StringAlgorithm::whitespace,cursorSlot);
	}	
	return cursorSlot;
}
int TextUnit::nextStructuralCursorSlot(int cursorSlot, bool rightDirection , int layer){
	if(layer == 0)
		return cursorSlot;
	if(layer == 1){
		if(rightDirection){
			//	right
			cursorSlot = StringAlgorithm::find(text,StringAlgorithm::whitespace,cursorSlot);
		}else {
			//	left
			cursorSlot = StringAlgorithm::rfind(text,StringAlgorithm::whitespace,cursorSlot);
		}
		return cursorSlot;
	}else 
		return sf::String::InvalidPos;
}
int TextUnit::nextVerticalCursorSlot(int cursorSlot, bool downDirection){
	return sf::String::InvalidPos;
}

// ## Cursor & Selection ##
void TextUnit::addCursor	 (int i){
	if(i<0 || i>=cursorSlot.size())
		return;
	cursorSlot[i] =  true;
}
void TextUnit::removeCursor (int i){
	if(i<0 || i>=cursorSlot.size())
		return;
	cursorSlot[i] =  false;

}
void TextUnit::select(int index,int amount){
	if(index<0)
		return;
	for (size_t i = 0; i < amount && i < selectedChar.size(); i++)
	{
		if(index+i < cursorSlot.size())
			selectedChar[index+i] = true;
	}
	
}
void TextUnit::deselect(int index,int amount){
	if(index<0)
		return;
	for (size_t i = 0; i < amount && i < selectedChar.size(); i++)
	{
		if(index+i<cursorSlot.size())
			selectedChar[index+i] = false;
	}
}
	
// ## Events ##
void TextUnit::triggerEvent(const GuiEvent& event){
	
}
// ## Cursor Events ##
void TextUnit::inputText(int cSlot, const sf::String& textInput){
	selectedChar.insert			( selectedChar.begin()		+cSlot, textInput.getSize(), false);
	cursorSlot.insert  			( cursorSlot.begin()  		+cSlot, textInput.getSize(), false);
	characterAdvances.insert  	( characterAdvances.begin() +cSlot, textInput.getSize(), 0);
	text.insert(cSlot,textInput);

	updateCharacterAdvances = true;
}
void TextUnit::deleteText(int startCursorSlot, int endCursorSlot){
	if(startCursorSlot < 0)
		startCursorSlot = 0;
	if(text.getSize()<endCursorSlot)
		endCursorSlot = text.getSize();
	if(startCursorSlot > endCursorSlot)
		startCursorSlot = endCursorSlot;

	size_t left = startCursorSlot;
	size_t right = endCursorSlot;

	text.erase				(left, right-left);
	characterAdvances.erase (characterAdvances.begin() + left, characterAdvances.begin() + right);
	selectedChar.erase 		(selectedChar.begin()	   + left, selectedChar.begin()		 + right);
	cursorSlot.erase 		(cursorSlot.begin()		   + left, cursorSlot.begin()		 + right);	
}
sf::String TextUnit::copyText(int startCursorSlot, int endCursorSlot){
	return text.substring(startCursorSlot,endCursorSlot-startCursorSlot);
}

// ## Styler ##
std::vector<TextComponent*>	TextUnit::split(int cursorSlot, bool onlyChildren){
	std::vector<TextComponent*> rv;
	rv.push_back(this);
	rv.push_back(new TextUnit(text.substring(cursorSlot)));
	text.erase(cursorSlot,text.getSize()-cursorSlot);
	return rv;
}
std::vector<TextComponent*>	TextUnit::applyTextStyler(TextComponent& style, int startCursorSlot, int endCursorSlot){
	if(startCursorSlot < 0)
		startCursorSlot = 0;
	if(startCursorSlot >= text.getSize())
		startCursorSlot = text.getSize()?text.getSize():0;	
	if(endCursorSlot < 0)
		endCursorSlot = 0;
	if(endCursorSlot >= text.getSize())	
		endCursorSlot = text.getSize()?text.getSize():0;	


	auto textStyler = style.copy();
	std::vector<TextComponent*> rv;
	
	sf::String prefix = text.substring(0			  ,startCursorSlot);
	sf::String middle = text.substring(startCursorSlot,endCursorSlot - startCursorSlot);
	sf::String suffix = text.substring(endCursorSlot  ,text.getSize()- startCursorSlot);

	this->text = prefix;
	auto middleTextUnit = new TextUnit(middle);
	textStyler->insert(middleTextUnit);
	auto suffixTextUnit = new TextUnit(suffix);

	rv.push_back(this);
	rv.push_back(textStyler);
	rv.push_back(suffixTextUnit);

	// set selected
	for (size_t i = 0; i < middle.getSize(); i++)
	{
		if(selectedChar[prefix.getSize()+i])
		 	middleTextUnit->select(i,1);
		else 
			middleTextUnit->deselect(i,1);
	}
	for (size_t i = 0; i < suffix.getSize(); i++)
	{
		if(selectedChar[prefix.getSize()+middle.getSize()+i])
		 	suffixTextUnit->select(i,1);
		else
			suffixTextUnit->deselect(i,1);
	}
	

	return rv;
}
std::vector<TextComponent*> TextUnit::explodeTextStyler(std::string type,int startCursorSlot, int endCursorSlot){
	return {this};
}
TextComponent* TextUnit::findFirst(std::string type,int startCursorSlot, int endCursorSlot){
	if(type==this->getType())
		return this;
	return nullptr;
}
void TextUnit::insert(TextComponent* textComponent){
	delete textComponent;
}
void TextUnit::print(std::string tabs){
	Log::info(tabs+"Text:"+text.toAnsiString());
}
