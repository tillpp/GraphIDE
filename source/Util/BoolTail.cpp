#include "BoolTail.h"

void BoolTail::update(bool value){
	state = state >> 1;
	if (value)
		state |= 0x02;
	else
		state &= ~(0x02);
}
BoolTail::operator bool(){
	return state==2||state==3; 
}
BoolTail::operator BoolTailMode(){
	return BoolTailMode(state);
}