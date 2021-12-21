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

enum KeyState
{
	release = 1,
	clicked = 2,
	pressed = 3
};
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
	unsigned char keyboardData[sf::Keyboard::KeyCount];
	unsigned char mouseKeyData[sf::Mouse::ButtonCount];
	sf::String text = "";
	void updateInput();
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
	KeyState getKey(sf::Mouse::Button);
	KeyState getKey(sf::Keyboard::Key);

	void setCursor(const sf::Cursor&);
};
Application& app();