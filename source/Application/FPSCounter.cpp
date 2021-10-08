#include "FPSCounter.h"
#include "Application.h"
#include <iostream> // just for testing [NOT THREAD SAFE]

FPSCounter::FPSCounter(/* args */)
{
	mutex.lock();
	clock.restart();
	mutex.unlock();
}
FPSCounter::~FPSCounter()
{
}

void FPSCounter::draw(Application* app){

}
void FPSCounter::update(Application* app){
	mutex.lock();
	if(clock.getElapsedTime().asSeconds()>=1){
		std::cout << "[FPS]"<<elapsedFrames << std::endl;
		clock.restart();
		elapsedFrames = 0;
	}
	elapsedFrames++;
	mutex.unlock();
}