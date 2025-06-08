#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Util.h"
#include <cstdio>
#include <cstdint>
#include <string>
#include <SDL3/SDL.h>

class Target
{
public:
	SDL_FRect rect{};
	bool OnScreen = false;
	bool isHitted = false;
	void render(SDL_Renderer* renderer);
};

class Map
{
public:
	int16_t length = 0;
	int8_t data[INT16_MAX]{};
	void getDataFromFile(const char* filename);
	void setPosFromData(int index, Target& target);
};