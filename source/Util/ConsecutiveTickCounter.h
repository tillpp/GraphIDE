#pragma once
#include "SFML/System.hpp"

class ConsecutiveTickCounter
{
	unsigned int 	timeoutTime; //miliseconds
	unsigned int 	counter = 0;
	sf::Clock 		elapsedTime;
public:
	ConsecutiveTickCounter(unsigned int timeOut_miliseconds);

	void 		 tick();
	unsigned int getCount();
};
