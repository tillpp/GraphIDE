#include "GuiHandler.h"
#include <iostream>
#include "OpenGL/TextureManager.h"

GuiHandler::GuiHandler()
{
}

GuiHandler::~GuiHandler()
{
}

static time_t start;
void GuiHandler::init(Application* app){
	mutex.lock();

	shader.create();
	shader.addVertexShaderFromFile("res/shader/gui.ver");
	shader.addFragmentShaderFromFile("res/shader/gui.fra");
	shader.generate();

	mesh.create();
	mesh.LoadFromVertexArray({
		1,1,0, 1,1,
		0,1,0, 0,1,
		1,0,0, 1,0,
	},3);
	mesh.setSettingRead(0,3,false,5,0);
	mesh.setSettingRead(1,2,false,5,3);
	mutex.unlock();

	//sprite.setTexture(TextureManager::loadFromFile("res/texture/background.png"));
	sprite.width.setEquation(GuiEqPercent(100));
	sprite.height.setEquation(GuiEqPercent(100));
	sprite.texBoundHeight.setEquation(GuiEqSize(100));
	sprite.texBoundWidth.setEquation(GuiEqSize(100));
	sprite.texBoundXpos.setEquation(GuiEqSize(-50));
	sprite.setColor(glm::vec4(227.f/255, 192.f/255, 104.f/255,1));

	// sprite.setFlipY(true);
	// sprite.setFlipX(true);
	
	sprite2 = new Sprite;
	sprite2->width.setEquation(GuiEqPixel(50));
	sprite2->height.setEquation(GuiEqPixel(50));
	sprite2->setColor(glm::vec4(1,0,0,1));

	sprite2->xpos.setEquation(GuiEqSub(GuiEqPercent(100),GuiEqPixel(50)));
	sprite2->ypos.setEquation(GuiEqPixel(0));

	scene.add(&sprite);
	sprite.add(sprite2);

	sprite.addFeature<GuiFeatureResize>();
	sprite.addFeature<GuiFeatureMove>();
	sprite2->addFeature<GuiFeatureResize>();
	sprite2->addFeature<GuiFeatureMove>();

	auto animation = sprite.createAnimation("test");
	
	animation->addKeyFrame(new KeyFrame(5,{
		new KeyFrameAttributeInformation(GuiEqPixel(50),[](double x)->double{return x;},"height")}));
	animation->addKeyFrame(new KeyFrame(0.1,{
	 	new KeyFrameAttributeInformation(GuiEqPixel(720),[](double x)->double{return x;},"height")}));
    sprite.useAnimation("test");

	time(&start);
	//text.setUtf8(	  u8"w		ok");
	//text2.setUtf8(	  u8"wass	hmm");
}
void GuiHandler::draw(Application* app){
	mutex.lock();

    scene.setSize(app->getSize());

	app->record();
	scene.draw(shader);
	
	time_t t;
	time(&t);
   	//sprite.width.overrideCachedValue(500);
	
	// text.addString(app->getText());
	// text.draw(shader,camera);
	// glm::mat4 mat(1.f);
	// mat = glm::translate(mat,glm::vec3(0,50,0));
	// text2.draw(shader,camera,mat);
	// mat = glm::translate(mat,glm::vec3(0,50,0));
	
	//sprite.draw(shader,camera,0,100);
	mutex.unlock();
}
void GuiHandler::update(Application* app){
	mutex.lock();
	//glm::vec4 screenMousePosition = scene.getInverseViewProjection()*app->getGLNormalizedMousePosition();
	
	
	scene.update();
	
	
	// glm::mat4 mat(1.f);
	// auto pit = text.getPositionInTextStandartScaling(screenMousePosition,mat);
	// if(app->getKey(sf::Mouse::Left)==KeyState::clicked){
	// 	text.startSelectionBox(pit);
	// }
	// if(app->getKey(sf::Mouse::Left)==KeyState::pressed){
	// 	text.moveSelectionBox(pit);
	// }
	
	// mat = glm::translate(mat,glm::vec3(0,50,0));	
	// pit = text2.getPositionInTextStandartScaling(screenMousePosition,mat);
	// if(app->getKey(sf::Mouse::Left)==KeyState::clicked){
	// 	text2.startSelectionBox(pit);
	// }
	// if(app->getKey(sf::Mouse::Left)==KeyState::pressed){
	// 	text2.moveSelectionBox(pit);
	// }
	//std::cout << "Positon:\t"<< text2.nearestCharacter(pit)<<std::endl;

	mutex.unlock();
}