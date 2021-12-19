// #include "Text.h"
// #include "OpenGL/Mesh.h"
// #include <mutex>
// #include "Util/Log.h"
// #include <math.h>
// #include <iostream>
// #include "OpenGL/FontManager.h"

// Text::Text()
// {
// 	font = FontManager::getFont("SourceCodePro-Regular.ttf");
// }
// Text::~Text()
// {
// 	font = nullptr; 
// }
// void Text::addUtf8(std::string data)
// {
// 	std::lock_guard<std::recursive_mutex> lock(mutex);
// 	addString(sf::String::fromUtf8(data.begin(), data.end()));
// }
// void Text::setUtf8(std::string data)
// {
// 	std::lock_guard<std::recursive_mutex> lock(mutex);
// 	setString(sf::String::fromUtf8(data.begin(), data.end()));
// }
// void Text::setString(sf::String data)
// {
// 	std::lock_guard<std::recursive_mutex> lock(mutex);
// 	text.clear();
// 	addString(data);
// }
// void Text::addString(sf::String data)
// {
// 	std::lock_guard<std::recursive_mutex> lock(mutex);
// 	//temporary
// 	if(selectionBoxs.size()==0){
// 		insert(data);
// 		return;
// 	}
// 	for (int i = 0; i < selectionBoxs.size(); i++)
// 	{
// 		auto& sb = selectionBoxs[i];
		
// 		auto minLine = std::min(sb.startLine,sb.endLine);
// 		auto maxLine = std::max(sb.startLine,sb.endLine);
		
// 		//delete SB if it's completly outside of Text
// 		if(minLine>=text.size()){
// 			selectionBoxs.erase(selectionBoxs.begin()+i);
// 			i--;
// 			continue;
// 		}
		
// 		//adjust SB if it is parly outside
// 		if(maxLine>=text.size()){
// 			auto& lastline = text[text.size()-1].line;
// 			sb.endChar = lastline.getSize();
// 			sb.endLine = text.size()-1;
// 			maxLine    = text.size()-1;
// 		}

// 		//adding Text
// 		auto& line = text[sb.endLine];

// 		if(line.line.getSize()<=sb.endChar)
// 			sb.endChar = line.line.getSize();
		
// 		insert(data,sb.endLine,sb.endChar);
// 	}
// }
// void Text::insert(sf::String str,size_t line,size_t pos){
// 	if(text.size()==0)
// 		text.push_back(TextLine());
// 	if(line>=text.size())
// 		line = text.size()-1;
// 	if(text[line].line.getSize()<pos)
// 		pos = text[line].line.getSize();
	
// 	for (size_t i = 0; i < str.getSize(); i++)
// 	{
// 		//Log::info(" : "+std::to_string(str[i]));
// 		if(str[i]==(sf::Uint32)'\r'||str[i]==(sf::Uint32)'\n'){
// 			text.insert(text.begin()+line,TextLine());
// 			line++;
// 			pos = 0;
// 			continue;
// 		}
// 		text[line].line.insert(pos,str[i]);
// 		pos++;
// 	}
	
// }
// void Text::drawSelectionBoxes(Shader& shader,Camera& camera,glm::mat4 inMatrix ){
// 	std::lock_guard<std::recursive_mutex> lock(mutex);
// 	shader.use();
	
// 	if(text.size()==0){
// 		selectionBoxs.clear();
// 	}
	
// 	//draw selectionboxs
// 	camera.use(shader);
// 	Texture::whiteTexture().use(0,shader,"texture1");
	
// 	for (int i = 0; i < selectionBoxs.size(); i++){
// 		auto& sb = selectionBoxs[i];

// 		size_t& minLine = sb.startLine>sb.endLine?sb.endLine:sb.startLine;
// 		size_t& minChar = sb.startLine>sb.endLine?sb.endChar:sb.startChar;
// 		size_t& maxLine = sb.startLine>sb.endLine?sb.startLine:sb.endLine;
// 		size_t& maxChar = sb.startLine>sb.endLine?sb.startChar:sb.endChar;

// 		//if selectionbox is completly outside of text
// 		if(minLine>=text.size()){
// 			selectionBoxs.erase(selectionBoxs.begin()+i);
// 			i--;
// 			continue;
// 		}
		
// 		//if selectionbox is partly outside of text
// 		if(maxLine>=text.size()){
// 			maxLine = text.size()-1;
// 			maxChar = text[text.size()-1].line.getSize();
// 		}

// 		//adjust selectionbox
// 		{
// 			auto minTextLineSize = text[minLine].line.getSize();
// 			auto maxTextLineSize = text[maxLine].line.getSize();
// 			if(minChar>minTextLineSize)
// 				minChar = minTextLineSize;
// 			if(maxChar>maxTextLineSize)
// 				maxChar = maxTextLineSize;
// 		}


// 		//drawing selection boxes
// 		glUniform4f(glGetUniformLocation(shader.getOpenGLID(), "textureRect"),0,0,1,1);
// 		glUniform4f(glGetUniformLocation(shader.getOpenGLID(), "color"),0,0,1,1);

// 		unsigned char offsetMax 	= text[maxLine].characterOffset[maxChar];
// 		unsigned char offsetMin 	= text[minLine].characterOffset[minChar];

		
// 		//CASE 1: minLine = maxLine
// 		if(minLine == maxLine){
// 			glm::mat4 letterMatrix = glm::mat4(1.f);
// 			letterMatrix = glm::translate	(letterMatrix,glm::vec3(offsetMin, minLine*font->getLineSpacing(),0.f));
// 			letterMatrix = glm::scale		(letterMatrix,glm::vec3(offsetMax-offsetMin, (minLine+1)*font->getLineSpacing() ,1.f));
// 			Mesh::rectangle().draw(shader,inMatrix*matrix*letterMatrix);
// 		}else
// 		//CASE 2: minLine != maxLine
// 		{
// 			//first line
// 			{
// 				auto& chof = text[minLine].characterOffset;
// 				if(chof.size()){
// 					unsigned char offsetMinEnd  = chof[chof.size()-1];
// 					glm::mat4 letterMatrix = glm::mat4(1.f);
// 					letterMatrix = glm::translate	(letterMatrix,glm::vec3(offsetMin, minLine*font->getLineSpacing(),0.f));
// 					letterMatrix = glm::scale		(letterMatrix,glm::vec3(offsetMinEnd-offsetMin, (minLine+1)*font->getLineSpacing() ,1.f));
// 					Mesh::rectangle().draw(shader,inMatrix*matrix*letterMatrix);
// 				}
// 			}
// 			//all lines in between
// 			for (size_t j = minLine+1; j < maxLine; j++){
// 				auto& chof = text[j].characterOffset;
// 				if(chof.size()){
// 					unsigned char offsetEnd  = chof[chof.size()-1];
// 					glm::mat4 letterMatrix = glm::mat4(1.f);
// 					letterMatrix = glm::translate	(letterMatrix,glm::vec3(0, j*font->getLineSpacing(),0.f));
// 					letterMatrix = glm::scale		(letterMatrix,glm::vec3(offsetEnd, (j+1)*font->getLineSpacing() ,1.f));
// 					Mesh::rectangle().draw(shader,inMatrix*matrix*letterMatrix);		
// 				}
// 			}
// 			//last line
// 			{
// 				glm::mat4 letterMatrix = glm::mat4(1.f);
// 				letterMatrix = glm::translate	(letterMatrix,glm::vec3(0, maxLine*font->getLineSpacing(),0.f));
// 				letterMatrix = glm::scale		(letterMatrix,glm::vec3(offsetMax, (maxLine+1)*font->getLineSpacing() ,1.f));
// 				Mesh::rectangle().draw(shader,inMatrix*matrix*letterMatrix);
// 			}
// 		}
// 		//drawing cursor
// 		auto& chof 	= text[sb.endLine].characterOffset;
// 		if(chof.size()){
// 			unsigned char cursorWidth = 2;
// 			unsigned char offsetCursor = chof[chof.size()-1];

// 			glm::mat4 letterMatrix = glm::mat4(1.f);
// 			letterMatrix = glm::translate	(letterMatrix,glm::vec3(offsetCursor, 0,0.f));
// 			letterMatrix = glm::scale		(letterMatrix,glm::vec3(cursorWidth, font->getLineSpacing() ,1.f));

// 			glUniform4f(glGetUniformLocation(shader.getOpenGLID(), "color"),1,1,1,1);	
// 			Mesh::rectangle().draw(shader,inMatrix*matrix*letterMatrix);
// 		}
// 	}
// }



// void Text::draw(Shader& shader,Camera& camera,glm::mat4 inMatrix){
//  	std::lock_guard<std::recursive_mutex> lock(mutex);

// 	//Log::info(" TEXTLINES: "+std::to_string(text.size()));
// 	shader.use();
// 	//characterOffset
// 	for(auto& tl: text){
// 		tl.characterOffset.reserve(tl.line.getSize()+1);
// 	}

// 	//draw selectionboxs
// 	drawSelectionBoxes(shader,camera,inMatrix);
	
// 	//draw Text
// 	glUniform4f(glGetUniformLocation(shader.getOpenGLID(), "color"),color.r,color.g,color.b,color.a);
// 	camera.use(shader);
// 	font->use(shader);
// 	for (size_t i = 0; i < text.size(); i++)
// 	{
// 		auto& line = text[i];
// 		GLfloat offset = 0;
// 		for (size_t j = 0; j < line.line.getSize(); j++)
// 		{
// 			line.characterOffset[j] = offset;
// 			auto& glyph = font->getGlyph(line.line[j]);
// 			//tab
// 			if (line.line[j] == '\t')
// 			{
// 				auto& glyph = font->getGlyph(' ');

// 				float tabsize = glyph.advance*4;
// 				int tabs = (int)offset/tabsize;

// 				if(tabsize*tabs==offset)
// 					offset += tabsize;
// 				else offset = (tabs+1)*tabsize;
// 				continue;
// 			}
// 			//kerning
// 			offset += font->getKerning(line.line,j);
// 			//draw letter
// 			glUniform4fv(glGetUniformLocation(shader.getOpenGLID(), "textureRect"),1,glm::value_ptr(font->getRelativTextureRectGlyph(glyph)));

// 			GLfloat ypos = glyph.bounds.top + font->getBaseline() + font->getLineSpacing();
// 			GLfloat xpos = glyph.bounds.left + offset+i*font->getLineSpacing();
// 			GLfloat height = glyph.bounds.height;
// 			GLfloat width = glyph.bounds.width;

// 			glm::mat4 letterMatrix = glm::mat4(1.f);
// 			letterMatrix = glm::translate	(letterMatrix,glm::vec3(xpos, ypos+height,0.f));
// 			letterMatrix = glm::scale		(letterMatrix,glm::vec3(width, -height ,1.f));
			
// 			Mesh::rectangle().draw(shader,inMatrix*matrix*letterMatrix);
// 			//offset advancing
// 			offset += glyph.advance;
// 		}
// 		line.characterOffset[line.line.getSize()] = offset;		
// 	}
// 	GuiComponent::draw(shader,camera,inMatrix*matrix);
// }

// bool Text::contain(glm::vec4& cameraMousePosition,glm::mat4 inMatrix){
//  	std::lock_guard<std::recursive_mutex> lock(mutex);
// 	auto t = glm::inverse(matrix)*glm::inverse(inMatrix)*cameraMousePosition;
// 	//check y
// 	if(text.size()<=t.y/font->getLineSpacing()||0>t.y)
// 		return false;
	
// 	//check if line has an width
// 	size_t lineNumber = t.y/font->getLineSpacing();
// 	auto& chof = text[lineNumber].characterOffset;
// 	if(!chof.size())
// 		return false;

// 	//check x
// 	float textWidth = chof[chof.size()-1];
// 	if(0<=t.x&&t.x<=textWidth){
// 		Log::info("hey");
// 		return true;
// 	}else Log::info("stop");
// 	return false;
// }
// glm::vec4 Text::getPositionInTextStandartScaling(glm::vec4& cameraMousePosition,glm::mat4 inMatrix ){
// 	std::lock_guard<std::recursive_mutex> lock(mutex);
// 	return glm::inverse(matrix)*glm::inverse(inMatrix)*cameraMousePosition;
// }

// void Text::startSelectionBox(glm::vec4& positionInTextStandartScaling){
// 	std::lock_guard<std::recursive_mutex> lock(mutex);
// 	return;
// 	auto nc = nearestCharacter(positionInTextStandartScaling);
// 	SelectionBox sb;
// 	sb.startLine = nc.first;
// 	sb.endLine = nc.first;

// 	sb.startChar = nc.second;
// 	sb.endChar = nc.second;
	
// 	selectionBoxs.push_back(sb);
// }
// void Text::moveSelectionBox(glm::vec4& positionInTextStandartScaling){
// 	std::lock_guard<std::recursive_mutex> lock(mutex);
// 	if(selectionBoxs.size()==0)
// 		return;
// 	auto nc = nearestCharacter(positionInTextStandartScaling);
// 	auto& sb = selectionBoxs[selectionBoxs.size()-1]; 
// 	sb.endLine = nc.first;
// 	sb.endChar = nc.second;
// }

// void Text::removeAllSelectionBoxes(){
// 	std::lock_guard<std::recursive_mutex> lock(mutex);
// 	selectionBoxs.clear();
// } 
	
// std::pair<size_t,size_t> Text::nearestCharacter(glm::vec4& positionInTextStandartScaling){
// 	std::lock_guard<std::recursive_mutex> lock(mutex);
// 	std::pair<size_t,size_t> rv = {0,0};
// 	//check y
// 	if(positionInTextStandartScaling.x <0||text.size()==0){
// 		return {0,0};
// 	}
// 	size_t lineNumber = positionInTextStandartScaling.y/font->getLineSpacing();
// 	if(lineNumber>=text.size()){
// 		auto& lastlineChof = text[text.size()-1].characterOffset;
// 		if(lastlineChof.size())
// 			return {text.size()-1,lastlineChof.size()-1};
// 		return {text.size()-1,0};
// 	}

// 	//check x
// 	rv.first = lineNumber;
// 	auto& chof = text[lineNumber].characterOffset;
// 	if(!chof.size())
// 		return {lineNumber,0};

// 	//check x
// 	for (size_t i = 0; i < chof.size(); i++)
// 	{
// 		if(chof[i]==positionInTextStandartScaling.x){
// 			rv.second = i;
// 			return rv;
// 		}
// 		if(chof.size()<=i+1){
// 			rv.second = chof.size()-1;
// 			return rv;	
// 		}
// 		if(chof[i]<positionInTextStandartScaling.x&&chof[i+1]>positionInTextStandartScaling.x){
// 			auto x1 = positionInTextStandartScaling.x-chof[i];
// 			auto x2 = chof[i+1]-chof[i];
// 			auto percent = x1/x2;
// 			if(percent < 0.5)
// 				rv.second = i;
// 			else rv.second = i+1;
// 			return rv;
// 		}
// 	}	
// 	rv.second = chof.size()-1;
// 	return rv;
// }