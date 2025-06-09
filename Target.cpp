#include "Target.h"

void Target::render(SDL_Renderer* renderer)
{
	if (this->OnScreen)
	{
		SDL_SetRenderDrawColor(renderer, 255, 0, 255, SDL_ALPHA_OPAQUE);
		if (this->isHitted)
			SDL_RenderFillRect(renderer, &this->rect);
		else
			SDL_RenderRect(renderer, &this->rect);
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
		fscanf(rawData, "%5d", &this->length);
	SDL_Log("Map length: %d", this->length);
	fseek(rawData, 2, SEEK_CUR);
	for (int i = 0; i < this->length; i++)
	{
		char cData = '\0';
		cData = getc(rawData);
		this->data[i] = cData - '0'; // convert character to integer
	}
	fclose(rawData);
}

void Map::setPosFromData(int index, Target& target, int* activeTargets)
 {
	switch (this->data[index])
	{
	case 1: // y=0
		target.OnScreen = true;
		target.rect.x = WINDOW_WIDTH + (index * 128);
		target.rect.y = 64; // basic offset
		*activeTargets += 1;
		break;
	case 2: // y=128
		target.OnScreen = true;
		target.rect.x = WINDOW_WIDTH + (index * 128);
		target.rect.y = 128 + 64;
		*activeTargets += 1;
		break;
	case 3: // y = WINDOW_HEIGHT - 256
		target.OnScreen = true;
		target.rect.x = WINDOW_WIDTH + (index * 128);
		target.rect.y = WINDOW_HEIGHT - 256 - 64;
		*activeTargets += 1;
		break;
	case 4: // y = WINDOW_HEIGHT - 128
		target.OnScreen = true;
		target.rect.x = WINDOW_WIDTH + (index * 128);
		target.rect.y = WINDOW_HEIGHT - 128 - 64;
		*activeTargets += 1;
		break;
	case 0:
	default:
		target.OnScreen = false;
		break;
	}
}
