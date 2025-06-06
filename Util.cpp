#include "Util.h"

void Util_RenderThickLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int thickness, SDL_Color color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	for (int i = 0; i < thickness; i++) // draw line in middle
	{
		SDL_RenderLine(renderer, x1, y1 + i, x2, y2 + i);
		SDL_RenderLine(renderer, x1 + i, y1, x2 + i, y2);
	}
}
