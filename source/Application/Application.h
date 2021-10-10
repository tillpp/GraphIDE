#pragma once
#include "GL/glew.h"
#include "GL/gl.h"
#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include "ApplicationFeature.h"

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
public:
	Application(std::string title="",size_t width=1280,size_t height=720);
	~Application();

	void close();
	void registerFeature(ApplicationFeature* feature);

	void setBackgroundColor(GLclampf red,GLclampf green,GLclampf blue,GLclampf alpha=1);
	void record();
};
