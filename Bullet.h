#pragma once

#include "Util.h"
#include <SDL3/SDL.h>

class Bullet {
public:
	SDL_FRect rect;
	Vector2 speed;
	SDL_Texture* texture;
	bool isShooting;
};