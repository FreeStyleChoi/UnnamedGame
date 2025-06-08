#define SDL_MAIN_USE_CALLBACKS 1
#include "Player.h"
#include "Bullet.h"
#include "Util.h"
#include "Target.h"
#include "easing.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <algorithm>
#include <iostream>

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

Player player = { 0 };
auto playerEasing = getEasingFunction(EaseOutExpo);

Bullet bullet = { 0 };

//SDL_FRect BackgroundRect[2] = {};
//SDL_Texture* BackgroundTexture = NULL;

const unsigned int FPS = 120;

Uint64 FrameStart = 0;
Uint64 FrameTime = 0;
Uint64 FrameCount = 0;
Uint64 FrameDelay = 1000 / FPS;

FVector2 mousePos;

Map targetMap{ 0 };
Target target[INT16_MAX]{ 0 };

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
	SDL_SetAppMetadata("Unnamed Game", "dev1", "com.FreestyleGameStudio.UnnamedGame");

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	if (!SDL_CreateWindowAndRenderer("Unnamed Game", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
		SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	//SDL_Surface* tmpSurface = IMG_Load("./asset/background.jpg");
	//BackgroundTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	//SDL_DestroySurface(tmpSurface);

	//BackgroundRect[0] = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	//BackgroundRect[1] = { WINDOW_WIDTH, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

	player.rect.w = 32;
	player.rect.h = 32;
	player.rect.x = (WINDOW_WIDTH / 2) - (player.rect.w / 2);
	player.rect.y = (WINDOW_HEIGHT / 2) - (player.rect.h / 2);
	player.speed.x = 0;
	player.speed.y = 0;
	player.movetime = 500;
	player.Cmovetime = 0;

	bullet.size.x = 1;
	bullet.size.y = 16;

	bullet.isShooting = false;

	targetMap.getDataFromFile("./asset./map1.txt");
	for (int i = 0; i < targetMap.length; i++)
	{
		target[i].OnScreen = true;
		target[i].isHitted = false;
		target[i].rect.w = 128;
		target[i].rect.h = 128;
		target[i].rect.x = 0;
		target[i].rect.y = 0;
		targetMap.setPosFromData(i, target[i]);
		if (targetMap.data[i] == 0)
		{
			target[i].OnScreen = false;
			target[i].rect.x = WINDOW_WIDTH;
		}
	}


	return SDL_APP_CONTINUE; 
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	if (event->type == SDL_EVENT_QUIT)
	{
		return SDL_APP_SUCCESS;
	}
	if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
	{
		player.Cmovetime = -1; // 누르는 동안에는 움직인다는 플래그
		player.isLaunched = true;
		bullet.isShooting = true;
	}
	if (event->type == SDL_EVENT_MOUSE_BUTTON_UP)
	{
		player.Cmovetime = 0;
		bullet.isShooting = false;
	}
	return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppIterate(void* appstate)
{
	FrameStart = SDL_GetTicks();

	// background
	//for (int i = 0; i < 2; i++)
	//{
	//	if (BackgroundRect[i].x + BackgroundRect[i].w <= 0)
	//	{
	//		if (i == 0)
	//			BackgroundRect[i].x = BackgroundRect[i + 1].x + BackgroundRect[i].w;
	//		else
	//			BackgroundRect[i].x = BackgroundRect[i - 1].x + BackgroundRect[i].w;
	//	}
	//}

	// player
	if (player.isLaunched == true && player.Cmovetime != -1)
	{
		if (player.Cmovetime >= player.movetime)
		{
			player.isLaunched = false;
			player.speed.x = 0;
		}
		else
		{
			player.speed.x = playerEasing((double)(player.Cmovetime * FrameDelay) / 1000.0) * 10;
			player.Cmovetime++;
		}
	}
	else if (player.isLaunched == true && player.Cmovetime == -1)
	{
		player.speed.x = 10; // 계속 움직임
	}

	// target
	for (int i = 0; i < targetMap.length; i++)
	{
		if (target[i].rect.x < mousePos.x && target[i].rect.x + target[i].rect.w > mousePos.x && target[i].rect.y < mousePos.y && target[i].rect.y + target[i].rect.h > mousePos.y  &&  bullet.isShooting) // first one check x pos, second check y pos, and another check mouse clicked
		{
			target[i].isHitted = true;
		}
	}

	// mouse
	SDL_GetMouseState(&mousePos.x, &mousePos.y);

	// mouse clamp
	//mousePos.x = std::clamp((int)mousePos.x, 0, (int)player.rect.x);

	// camera
	player.rect.x -= player.speed.x;
	//for (int i = 0; i < 2; i++)
	//{
	//	BackgroundRect[i].x -= player.speed.x;
	//}
	for (int i = 0; i < targetMap.length; i++)
	{
		if (target[i].OnScreen)
			target[i].rect.x -= player.speed.x;
	}

	player.rect.x += player.speed.x;
	player.rect.y += player.speed.y;

	SDL_RenderClear(renderer);
	//SDL_RenderTexture(renderer, BackgroundTexture, NULL, &BackgroundRect[0]);
	//SDL_RenderTexture(renderer, BackgroundTexture, NULL, &BackgroundRect[1]);
	for (int i = 0; i < targetMap.length; i++)
	{
		target[i].render(renderer);
	}
	if (bullet.isShooting)
	{
		Util_RenderThickLine(renderer, player.rect.x, player.rect.y + (player.rect.h / 2) - (16 / 2), mousePos.x , mousePos.y, 16, { 0, 0, 255, SDL_ALPHA_OPAQUE });
	}
	else
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderLine(renderer, player.rect.x, player.rect.y + (player.rect.h / 2), mousePos.x, mousePos.y);
	}
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &player.rect);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderPresent(renderer);

	FrameTime = SDL_GetTicks() - FrameStart;
	if (FrameTime < FrameDelay)
	{
		SDL_Delay(FrameDelay - FrameTime);
	}
	FrameCount++;
	return SDL_APP_CONTINUE;
}


void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
	//SDL_DestroyTexture(BackgroundTexture);
}

