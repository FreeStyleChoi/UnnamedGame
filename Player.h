#pragma once
#include "Util.h"
#include <SDL3/SDL.h>

class Player
{
public:
	SDL_FRect rect {};
	Vector2 speed {};
	bool isLaunched;
	float movetime = 0.25;
	Uint64 Cmovetime = 0; // count moving time
};