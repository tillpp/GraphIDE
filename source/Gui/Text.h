// #pragma once

// #include "GuiComponent.h"
// #include "OpenGL/Texture.h"
// #include "OpenGL/Font.h"
// #include <SFML/Graphics.hpp>

// struct SelectionBox{
// 	size_t startLine,	startChar;
// 	size_t endLine,		endChar;
// };
// struct TextLine
// {
// 	sf::String line;
// 	std::vector<unsigned char> characterOffset;
// };

// class Text:public GuiComponent
// {
// 	//font data
// 	Font* font;
	
// 	//data
// 	glm::vec4 color = glm::vec4(1,1,1,1);
// 	std::vector<TextLine> text;

// 	//selectionBox
// 	std::vector<SelectionBox> selectionBoxs;
// public:
// 	Text();
// 	~Text();

//  	void addString(sf::String);
//  	void addUtf8(std::string);
//  	void setString(sf::String);
//  	void setUtf8(std::string);

// 	//ADJUST TO MULTIPLE LINES
// 	//*		MERGE OVERLAPPING SelectionBoxes 

// private:
// 	void insert(sf::String,size_t line = -1,size_t c = -1);
// 	void drawSelectionBoxes(Shader& shader,Camera& camera,glm::mat4 inMatrix = glm::mat4(1.f));
// public:

// 	virtual bool contain(glm::vec4& cameraMousePosition,glm::mat4 inMatrix = glm::mat4(1.f)) override;
// 	virtual void draw(Shader& shader,Camera& camera,glm::mat4 inMatrix = glm::mat4(1.f)) override;

// 	/*
// 		# TextStandartScaling
// 		As if you render the Text without any scaling
// 		in pixels 
// 	*/
// 	glm::vec4 getPositionInTextStandartScaling(glm::vec4& cameraMousePosition,glm::mat4 inMatrix = glm::mat4(1.f));
// 	/*
// 		shift: continues last selectionBox
// 		ctrl:  initiate selectionsboxes or cursors
// 	*/
// 	void startSelectionBox(glm::vec4& positionInTextStandartScaling);
// 	void moveSelectionBox(glm::vec4& positionInTextStandartScaling); 
// 	void removeAllSelectionBoxes(); 

// 	std::pair<size_t,size_t> nearestCharacter(glm::vec4& positionInTextStandartScaling);
// };