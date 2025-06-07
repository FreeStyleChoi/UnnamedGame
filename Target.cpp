#include "Target.h"

void Target::render(SDL_Renderer* renderer)
{
	if (this->OnScreen)
	{
		SDL_SetRenderDrawColor(renderer, 255, 0, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(renderer, &this->rect);
	}
}

void Map::getDataFromFile(const char* fileDirName)
{
	std::ifstream rawData(fileDirName, std::ios::in);
	if (rawData.fail())
		SDL_Log("cannot open map file: %s", fileDirName);

	char Slength[6] = { 0 }; // one byte for sign, 5 bytes for length
	if (rawData.is_open())
	{
		while (rawData.getline(Slength, 6))
		{
			this->length = std::atoi(Slength);
		}
	}

	rawData.close();
}
