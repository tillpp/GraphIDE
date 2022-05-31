#include "TextUnit.h"
#include "OpenGL/Mesh.h"
#include "Util/Log.h"

TextUnit::TextUnit(std::string utf8text)
	: text(sf::String::fromUtf8(utf8text.begin(), utf8text.end()))
{
	Log::debug("TextUnit::TextUnit ");
	// Log::debug("TextUnit::TextUnit "+text.toAnsiString());
}

TextUnit::~TextUnit()
{
}
void TextUnit::meshGenerate(Shader&shader,TextSettings& ts){
	if (!ts.font)
		Log::info("No font used.");

	//displayTextureCharacterRatio
	GLfloat dtcr = ts.getDisplayTextureCharacterRatio();

	
	int GlyphCount = 0;
	std::vector<GLfloat> vertexArray;

	//draw Text
	GLfloat offset = 0;
	goThroughGlyphs(offset,true,false,ts,dtcr,[&](const sf::Glyph& glyph,GLfloat& advance,int& i, glm::vec4& color,glm::vec4& drawRect,bool& italic)->bool{
		
		//textureRect
		auto textureRect = ts.font->getRelativTextureRectGlyph(glyph);
		
		glm::mat4 letterMatrix = glm::mat4(1.f);
		letterMatrix = glm::translate(letterMatrix, glm::vec3(drawRect.x, drawRect.y + drawRect.w, 0.f));
		if (italic)
			letterMatrix[1] += glm::vec4(-0.15, 0, 0, 0); //change for italic
		letterMatrix = glm::scale(letterMatrix, glm::vec3(drawRect.z, -drawRect.w, 1.f));

		glm::vec4 x = glm::vec4(0,0,0,1); 
		
		const std::vector<std::pair<bool,bool>> coords = {{0,0},{0,1},{1,1},{0,0},{1,0},{1,1}};
		for(auto c:coords){
			x = letterMatrix*glm::vec4(c.first,c.second,0,1);
			//cords
			vertexArray.push_back(x.x);
			vertexArray.push_back(x.y);
			vertexArray.push_back(x.z);
			//texture
			vertexArray.push_back(textureRect.x+c.first*textureRect.z);
			vertexArray.push_back(textureRect.y+(!c.second)*textureRect.w);

		}
		GlyphCount++;
		return true;
	});
	if(!mesh.isCreated())
		mesh.create();

	mesh.LoadFromVertexArray(vertexArray,GlyphCount*6);
	mesh.setSettingRead(0,3,false,5,0);
	mesh.setSettingRead(1,2,false,5,3);
	
}
/*
	returnvalue: offset
*/

void TextUnit::goThroughGlyphs(
	GLfloat& offset,
	bool CharacterEffectDrawingMode,
	bool callBackAfterAdvance,
	const TextSettings& ts,
	const GLfloat& dtcr,
	std::function<bool(const sf::Glyph& glyph,GLfloat& advance,int& index, glm::vec4& color,glm::vec4& drawRect,bool& italic)> F){

	if (!ts.font)
		Log::info("No font used.");

	//draw Text

	for (int i = 0; i < text.getSize(); i++)
	{
		//tab
		if (text[i] == '\t')
		{
			auto &glyph = ts.font->getGlyph(' ', ts.bold);

			float tabsize = glyph.advance * 4;
			int tabs = (int)offset / tabsize;

			if (tabsize * tabs == offset)
				offset += tabsize;
			else
				offset = (tabs + 1) * tabsize;
			continue;
		}

		bool bold = ts.bold;
		bool italic = ts.italic;
		glm::vec4 color = ts.textColor;

		/* 	CharacterEffect:preGlyph	*/
		for (auto &e : ts.effects)
			e->preGlyph(text, i, offset, color, bold,italic, ts);

		auto &glyph = ts.font->getGlyph(text[i], bold);

		//kerning
		offset += dtcr * ts.font->getKerning(text, i);

		glm::vec4 drawRect;
		drawRect.x = dtcr * glyph.bounds.left + offset + ts.x;
		drawRect.y = dtcr * (glyph.bounds.top + ts.font->getBaseline() + ts.font->getLineSpacing()) + ts.y;
		drawRect.z = dtcr * glyph.bounds.width;
		drawRect.w = dtcr * glyph.bounds.height;
		GLfloat advance = dtcr * glyph.advance;

		/* 	CharacterEffect:preDraw	*/
		for (auto &e : ts.effects)
			e->preDrawGlyph(text, i, drawRect, advance, ts, CharacterEffectDrawingMode);

		//drawing letter
		if(!callBackAfterAdvance)
			if(!F(glyph,advance,i,color,drawRect,italic))
				return;

		/* 	CharacterEffect:postDraw	*/
		for (auto &e : ts.effects)
			e->postDrawGlyph(text, i, drawRect, advance, ts, CharacterEffectDrawingMode);

		//offset advancing
		offset += advance;
		
		//drawing letter
		if(callBackAfterAdvance)
			if(!F(glyph,advance,i,color,drawRect,italic))
				return;
	}
}
void TextUnit::draw(Shader &shader, TextSettings &ts,const double x,const double y)
{
	auto matrix = glm::translate(glm::mat4(1.f), glm::vec3(x, y, 1));
	
	//performant drawing
	if(!mesh.isCreated()||needMeshRegeneration){
	 	meshGenerate(shader,ts);
	 	needMeshRegeneration = false;
	}
	glUniform4f(glGetUniformLocation(shader.getOpenGLID(), "textureRect"),0,0,1,1);
	glUniform4f(glGetUniformLocation(shader.getOpenGLID(), "color"), ts.textColor.r, ts.textColor.g, ts.textColor.b, ts.textColor.a);

	ts.font->use(shader);

	mesh.draw(shader,matrix);
	return;

	if (!ts.font)
		Log::info("No font used.");

	//displayTextureCharacterRatio
	GLfloat dtcr = ts.getDisplayTextureCharacterRatio();

	//draw background
	drawBackground(shader, ts, matrix,dtcr);

	//draw Text
	ts.font->use(shader);
	GLfloat offset = 0;
	goThroughGlyphs(offset,true,false,ts,dtcr,[&](const sf::Glyph& glyph,GLfloat& advance,int& i, glm::vec4& color,glm::vec4& drawRect,bool& italic)->bool{

		glUniform4fv(glGetUniformLocation(shader.getOpenGLID(), "textureRect"), 1, glm::value_ptr(ts.font->getRelativTextureRectGlyph(glyph)));
		glUniform4f(glGetUniformLocation(shader.getOpenGLID(), "color"), color.r, color.g, color.b, color.a);

		glm::mat4 letterMatrix = glm::mat4(1.f);
		letterMatrix = glm::translate(letterMatrix, glm::vec3(drawRect.x, drawRect.y + drawRect.w, 0.f));
		if (italic)
			letterMatrix[1] += glm::vec4(-0.15, 0, 0, 0); //change for italic
		letterMatrix = glm::scale(letterMatrix, glm::vec3(drawRect.z, -drawRect.w, 1.f));

		Mesh::rectangle().draw(shader, /*inMatrix */ matrix * letterMatrix);
		return true;
	});

	//Draw Underline
	if (ts.underline)
		drawUnderline		(shader, ts,matrix, dtcr,offset);
	//Draw Strikethrough
	if (ts.strikethrough)
		drawStrikethrough	(shader, ts,matrix, dtcr,offset);
}
void TextUnit::drawBackground(Shader &shader, TextSettings &ts,glm::mat4& matrix, GLfloat dtcr)
{
	Texture::whiteTexture().use(0, shader, "texture1");
	glUniform4f(glGetUniformLocation(shader.getOpenGLID(), "textureRect"), 0, 0, 1, 1);
	glUniform4f(glGetUniformLocation(shader.getOpenGLID(), "color"), ts.backgroundColor.r, ts.backgroundColor.g, ts.backgroundColor.b, ts.backgroundColor.a);

	GLfloat ypos = ts.y;
	GLfloat xpos = ts.x;
	GLfloat height = dtcr * ts.font->getLineSpacing();
	GLfloat width = getWidth(ts);

	glm::mat4 letterMatrix = glm::mat4(1.f);
	letterMatrix = glm::translate(letterMatrix, glm::vec3(xpos, ypos + height, 0.f));
	letterMatrix = glm::scale(letterMatrix, glm::vec3(width, -height, 1.f));

	Mesh::rectangle().draw(shader, /*inMatrix */ matrix * letterMatrix);
}
void TextUnit::drawUnderline(Shader &shader, TextSettings &ts,glm::mat4& matrix, GLfloat dtcr,GLfloat offset)
{
	Texture::whiteTexture().use(0, shader, "texture1");
	glUniform4f(glGetUniformLocation(shader.getOpenGLID(), "textureRect"), 0, 0, 1, 1);

	GLfloat xpos = ts.x;
	GLfloat width = offset;

	GLfloat ypos = dtcr * (ts.font->getUnderlinePosition() + ts.font->getBaseline() + ts.font->getLineSpacing()) + ts.y;
	GLfloat height = dtcr * ts.font->getUnderlineThickness();

	glm::mat4 letterMatrix = glm::mat4(1.f);
	letterMatrix = glm::translate(letterMatrix, glm::vec3(xpos, ypos + height, 0.f));
	if (ts.italic)
		letterMatrix[1] += glm::vec4(-0.15, 0, 0, 0); //change for italic
	letterMatrix = glm::scale(letterMatrix, glm::vec3(width, -height, 1.f));

	Mesh::rectangle().draw(shader, /*inMatrix */ matrix * letterMatrix);
}
void TextUnit::drawStrikethrough(Shader &shader, TextSettings &ts,glm::mat4& matrix,GLfloat dtcr,GLfloat offset)
{
	Texture::whiteTexture().use(0, shader, "texture1");
	glUniform4f(glGetUniformLocation(shader.getOpenGLID(), "textureRect"), 0, 0, 1, 1);

	GLfloat xpos = ts.x;
	GLfloat width = offset;

	GLfloat ypos = dtcr * (ts.font->getLineSpacing()) / 2 + ts.y;
	GLfloat height = dtcr * ts.font->getUnderlineThickness();

	glm::mat4 letterMatrix = glm::mat4(1.f);
	letterMatrix = glm::translate(letterMatrix, glm::vec3(xpos, ypos + height, 0.f));
	if (ts.italic)
		letterMatrix[1] += glm::vec4(-0.15, 0, 0, 0); //change for italic
	letterMatrix = glm::scale(letterMatrix, glm::vec3(width, -height, 1.f));

	Mesh::rectangle().draw(shader, /*inMatrix */ matrix*letterMatrix);
}

int TextUnit::getWidth(const TextSettings &ts)
{
	if (!ts.font)
		Log::info("No font used.");

	//displayTextureCharacterRatio
	GLfloat dtcr = ts.getDisplayTextureCharacterRatio();

	//draw Text
	GLfloat offset = 0;
	goThroughGlyphs(offset,false,false,ts,dtcr,[](const sf::Glyph& glyph,GLfloat& advance,int& i, glm::vec4& color,glm::vec4& drawRect,bool& italic)->bool{return true;});
	return std::ceil(offset);
}
int TextUnit::getHeight(const TextSettings &ts)
{
	return ts.font->getLineSpacing() * ts.getDisplayTextureCharacterRatio();
}
int TextUnit::getYOffset(const TextSettings &ts)
{
	return ts.font->getBaseline() * ts.getDisplayTextureCharacterRatio();
}
std::string TextUnit::getType()
{
	return "TextUnit";
}

std::vector<TextComponent*> TextUnit::split(int startoffset,const int maxLineWidth,const TextSettings &ts){
	if (!ts.font)
		Log::info("No font used.");

	//displayTextureCharacterRatio
	GLfloat dtcr = ts.getDisplayTextureCharacterRatio();

	//draw Text
	GLfloat offset = startoffset;
	std::vector<TextComponent*> rv;
	int potentialSoftBreakPoint = 0;

	goThroughGlyphs(offset,false,true,ts,dtcr,[&](const sf::Glyph& glyph,GLfloat& advance,int& i, glm::vec4& color,glm::vec4& drawRect,bool& italic)->bool{
		if(i>0)
			if(sf::String(" ").find(text[i-1])!=sf::String::InvalidPos){
				potentialSoftBreakPoint = i;
			}
		
		
		if(offset>maxLineWidth){
			//soft split [Fits in this line]
			if(potentialSoftBreakPoint!=0){
				//is this the last bit?
				if(text.getSize()<=potentialSoftBreakPoint){
					rv.push_back(this);
					return false;
				}
				//no
				rv.push_back(new TextUnit(text.substring(0,potentialSoftBreakPoint)));
				text.erase(0,potentialSoftBreakPoint);
			}//soft split [doesnt fit this line,but in the next line]
			else if(offset-startoffset<maxLineWidth){
				
			}//hard split
			else{
				//have atleast one letter at one line.
				if(i==0)
					i=1;
				//is this the last bit?
				if(text.getSize()<=i){
					rv.push_back(this);
					return false;
				}
				//this is not the last bit
				rv.push_back(new TextUnit(text.substring(0,i)));	
				text.erase(0,i);
			}
			i = -1;
			offset = 0;
			startoffset = 0;
			potentialSoftBreakPoint = 0;
		}
		return true;
	});
	if(rv.size())
		if(rv[rv.size()-1]!=this)
			rv.push_back(this);
	return rv;
}
bool TextUnit::merge(TextComponent* left){
	if(left->getType()!=getType())
		return false;
	auto tu = (TextUnit*)left;
	text+=tu->text;
	// Log::debug("TextUnit::merge "+tu->text.toAnsiString()+" -> "+text.toAnsiString());
	return true;
}
int TextUnit::select_selectableCount(){
	return text.getSize();
}
int TextUnit::select_index(glm::vec2 mousePositionRelative2TC,const TextSettings& ts){
	GLfloat dtcr = ts.getDisplayTextureCharacterRatio();
	GLfloat ypos = ts.y;
	GLfloat height = dtcr * ts.font->getLineSpacing();

	if(not(ypos<=mousePositionRelative2TC.y&&mousePositionRelative2TC.y<ypos+height))
		return -1;
	int rv = text.getSize();

	GLfloat offset = 0;
	goThroughGlyphs(offset,false,true,ts,dtcr,[&](const sf::Glyph& glyph,GLfloat& advance,int& i, glm::vec4& color,glm::vec4& drawRect,bool& italic)->bool{
		
		GLfloat xpos = drawRect.x+ts.x;
		GLfloat width = drawRect.z;
		
		if(mousePositionRelative2TC.x<xpos+width){
			rv=(mousePositionRelative2TC.x<xpos+width/2)?i:i+1;
			return false;
		}
		return true;
	});

	return rv;
} 
