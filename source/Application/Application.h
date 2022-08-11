#pragma once
#include "GL/glew.h"
#include "GL/gl.h"
#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include "ApplicationFeature.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Util/BoolTail.h"
#include "Util/ConsecutiveTickCounter.h"

class Application
{
	//thread
	std::thread 		 thread;
	std::recursive_mutex mutex;
	void 				 run(std::string title,size_t width,size_t height);

	//data
	sf::RenderWindow window;
	std::vector<ApplicationFeature*> features;
	std::queue<ApplicationFeature*> uninitializedFeatures;

	//input
	/*
		0x01	last state data
		0x02		 state data
	*/
	BoolTail keyboardData[sf::Keyboard::KeyCount];
	BoolTail mouseKeyData[sf::Mouse::ButtonCount];
	ConsecutiveTickCounter consecutiveLeftMouseClicks = 250;
	sf::String text = "";
	void updateInput();	
	sf::Clock keyboardTextClock;
	sf::Int32 KeyboardTextDelay = 1000;
	bool	  keyboardTextMove  = false;
public:
	Application(std::string title="",size_t width=1280,size_t height=720);
	~Application();

	void close();
	void registerFeature(ApplicationFeature* feature);

	void setBackgroundColor(GLclampf red,GLclampf green,GLclampf blue,GLclampf alpha=1);
	void record();

	glm::vec2 getSize();
	glm::vec4 getGLNormalizedMousePosition();

	sf::String getText();
	BoolTail::BoolTailMode getKey(sf::Mouse::Button);
	BoolTail::BoolTailMode getKey(sf::Keyboard::Key);
	unsigned int getConsecutiveLeftMouseClicks();
	bool		 isKeyboardTextMove();

	void setCursor(const sf::Cursor&);
};
Application& app();