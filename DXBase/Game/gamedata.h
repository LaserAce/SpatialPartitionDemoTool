#ifndef _GAME_DATA_H_
#define _GAME_DATA_H_

#include "dinput0x0800.h"

struct GameData
{
	float dt; //duration of previous frame
	unsigned char* keyboard; //current keyboard state
	unsigned char* prevKeyboard; //previous frame's keyboard state
	DIMOUSESTATE* mouse; //current mouse state
	DIMOUSESTATE* prevMouse; //previous frame's state
};

#endif