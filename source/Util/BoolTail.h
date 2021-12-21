#pragma once

/*
	A binary value, which also stores its last value
*/

class BoolTail
{
public:
	enum BoolTailMode{
		NONE = 0,
		RELEASE = 1,
		START = 2,
		HOLD = 3
	};
protected:
	/*
		0x01	last state data
		0x02		 state data
	*/
	unsigned char state = 0;
public:
	void update(bool value);
	operator bool();
	operator BoolTailMode();
};
