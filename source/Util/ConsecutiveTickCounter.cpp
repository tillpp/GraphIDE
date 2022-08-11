#include "ConsecutiveTickCounter.h"

ConsecutiveTickCounter::ConsecutiveTickCounter(unsigned int timeOut_miliseconds){
	timeoutTime = timeOut_miliseconds;
}
void ConsecutiveTickCounter::tick(){
	if(elapsedTime.getElapsedTime().asMilliseconds() < timeoutTime){
		counter++;
	}else{
		counter = 1;
	}
	elapsedTime.restart();
}
unsigned int ConsecutiveTickCounter::getCount(){
	return counter;
}