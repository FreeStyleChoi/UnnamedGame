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
	FILE* rawData = fopen(fileDirName, "r");
	if (rawData == NULL)
	{
		SDL_Log("Cannot open map file: %s", fileDirName);
		return;
	}
	else

		fscanf(rawData, "%d", &this->length);
	SDL_Log("Map length: %d", this->length);
	fseek(rawData, 2, SEEK_CUR);
	for (int i = 0; i < this->length; i++)
	{
		char cData = '\0';
		cData = getc(rawData);
		this->data[i] = cData - '0'; // convert character to integer
		SDL_Log("data[%d] = %d", i, this->data[i]);
	}
	fclose(rawData);
}

void Map::setPosFromData(int index, Target& target)
{
	switch (this->data[index])
	{
	case 0:
		target.OnScreen = false;
		break;
	case 1: // target is in top side of screen
		target.OnScreen = true;
		target.rect.x = WINDOW_WIDTH + (index * 64);
		target.rect.y = 128;
		break;
	case 2: // target is in bottom side of screen
		target.OnScreen = true;
		target.rect.x = WINDOW_WIDTH + (index * 64);
		target.rect.y = WINDOW_HEIGHT - 128;
		break;
	default:
		break;
	}
}
