#include "Application.h"
#include <iostream>

Application::Application(std::string title,size_t width,size_t height)
{
	thread = std::thread(Application::run, this,title,width,height);

	//loop until the initalization is finished.
	mutex.lock();
	while(!window.isOpen()){
		mutex.unlock();
		sf::sleep(sf::milliseconds(1));	
		mutex.lock();
	}
	mutex.unlock();
}
Application::~Application()
{
	close();
}

void Application::run(std::string title,size_t width,size_t height)
{
	mutex.lock();
	//# init
	sf::ContextSettings settings;
	settings.depthBits 			= 24;
	settings.stencilBits 		= 8;
	settings.antialiasingLevel 	= 4;
	settings.majorVersion 		= 3;
	settings.minorVersion 		= 0;

	window.create(sf::VideoMode(width,height), title , sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);
	window.setActive(true);
	window.setFramerateLimit(60);

	auto err = glewInit();
	if (err != GLEW_OK)
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(true);

	while (window.isOpen())
	{
		//check if other threads want to do something
		window.setActive(false);
		mutex.unlock();
		sf::sleep(sf::milliseconds(1));
		mutex.lock();
		window.setActive(true);

		//init uninitiled features
		while(!uninitializedFeatures.empty()){
			uninitializedFeatures.front()->init(this);
			uninitializedFeatures.pop();
		}

		//event handling
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::Resized)
			{
				glViewport(0, 0, window.getSize().x, window.getSize().y);
			}
		}
		//process features
		for (auto &&f : features){
			f->update(this);
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		for (auto &&f : features){
			f->draw(this);
		}
		window.display();
	}
	mutex.unlock();
}
void Application::close(){
	mutex.lock();
	window.close();
	mutex.unlock();
}
void Application::registerFeature(ApplicationFeature* feature){
	mutex.lock();
	features.push_back(feature);
	uninitializedFeatures.push(feature);
	mutex.unlock();
}
void Application::setBackgroundColor(GLclampf red,GLclampf green,GLclampf blue,GLclampf alpha){
	mutex.lock();
	window.setActive(true);
	glClearColor(red,green,blue,alpha);		
	window.setActive(false);
	mutex.unlock();
}