#include "Application.h"
#include <iostream>
#include "Util/Log.h"

Application::Application(std::string title,size_t width,size_t height)
{
	//initializing the inputvariables
	for (size_t i = 0; i < sf::Keyboard::KeyCount; i++)
		keyboardData[i]=0; 
	for (size_t i = 0; i < sf::Mouse::ButtonCount; i++)
		mouseKeyData[i]=0; 
	
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
	window.setFramerateLimit(30);

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
		updateInput();

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
void Application::record(){
	mutex.lock();
	window.setActive(true);
	mutex.unlock();
}
glm::vec2 Application::getSize(){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	auto size = window.getSize();
	return glm::vec2(size.x, size.y);
}
glm::vec2 Application::getNormalizedMousePosition(){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	auto mousepos = sf::Mouse::getPosition(window);
	auto size = window.getSize();
	return glm::vec2((float)mousepos.x/size.x,(float)mousepos.y/size.y);
}
sf::String Application::getText(){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return text; 
}
KeyState Application::getKey(sf::Keyboard::Key key){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return KeyState(keyboardData[key]);
}
KeyState Application::getKey(sf::Mouse::Button button){
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return KeyState(mouseKeyData[button]);
}
void Application::updateInput()
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	sf::Event event;
	text = "";
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
		if (event.type == sf::Event::Resized)
		{
			glViewport(0, 0, window.getSize().x, window.getSize().y);
		}
		if(event.type == sf::Event::TextEntered){
			 text += event.text.unicode;
		}
	}
	
	//keyboard
	for (size_t i = 0; i < sf::Keyboard::KeyCount; i++)
	{
		/*	update lastkey and key	*/
		keyboardData[i]=keyboardData[i]>>1; 
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key(i)))
			keyboardData[i]|=0x02;
		else 
			keyboardData[i]&=~(0x02);
		
		/*	pressed and released	*/
		if(keyboardData[i]==2){ //pressed
			Log::info("keyboard button "+std::to_string(i)+" pressed");
		}
		if(keyboardData[i]==1){ //released
			Log::info("keyboard button "+std::to_string(i)+" released");
		}
	}
	//Mouse
	for (size_t i = 0; i < sf::Mouse::ButtonCount; i++)
	{
		/*	update lastkey and key	*/
		mouseKeyData[i]=mouseKeyData[i]>>1; 
		if(sf::Mouse::isButtonPressed(sf::Mouse::Button(i)))
			mouseKeyData[i]|=0x02;
		else 
			mouseKeyData[i]&=~(0x02);

		/*	pressed and released	*/
		if(mouseKeyData[i]==2){ //pressed
			Log::info("Mouse button "+std::to_string(i)+" pressed");
		}
		if(mouseKeyData[i]==1){ //released
			Log::info("Mouse button "+std::to_string(i)+" release");
		}
	}
}
