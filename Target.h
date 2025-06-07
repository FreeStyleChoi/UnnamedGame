#pragma once
#include "Util.h"
#include <fstream>
#include <cstdint>
#include <SDL3/SDL.h>

class Target
{
public:
	SDL_FRect rect{};
	bool OnScreen = false;
	void render(SDL_Renderer* renderer);
};

class Map
{
public:
	int16_t length = 0;
	int8_t data[INT16_MAX] {};
	void getDataFromFile(const char* filename);
};