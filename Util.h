#pragma once
#include <SDL3/SDL.h>

class Vector2
{
public:
	int x;
	int y;
};

class FVector2
{
public:
	float x;
	float y;
};

void Util_RenderThickLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int thickness, SDL_Color color);