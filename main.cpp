/* rectangles.c ... */

/*
 * This example creates an SDL window and renderer, and then draws some
 * rectangles to it every frame.
 *
 * This code is public domain. Feel free to use it for any purpose!
 */

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include "Player.h"
#include "Util.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

 /* We will use this renderer to draw into this window every frame. */
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define CAMERASPEED 3

Player player = { 0 };

const unsigned int FPS = 120;  /* frames per second */

Uint64 FrameStart = 0;
Uint64 FrameTime = 0;
Uint64 FrameCount = 0;
Uint64 FrameDelay = 1000 / FPS;

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

	player.rect.w = 32;
	player.rect.h = 32;
	player.rect.x = 128;
	player.rect.y = (WINDOW_HEIGHT / 2) - (player.rect.h / 32);
	player.speed.x = 0;
	player.speed.y = 0;
	player.movetime = 500;
	player.Cmovetime = 0;

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
		player.Cmovetime = SDL_GetTicks();
		player.isLaunched = true;
	}
	return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppIterate(void* appstate)
{
	FrameStart = SDL_GetTicks(); 

	if (player.isLaunched == true)
	{
		if (player.Cmovetime + player.movetime <= SDL_GetTicks())
		{
			player.isLaunched = false;
			player.speed.x = 0;
		}
		else
		{
			player.speed.x = CAMERASPEED;
		}
	}


	// camera



	player.rect.x += player.speed.x;
	player.rect.y += player.speed.y;


	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &player.rect);
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

}

