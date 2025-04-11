#include "Game.hpp"
#include <SDL.h> 
#include <iostream>
using namespace std;

Game *game = nullptr;

const int width = 480, height = 680;

int main(int argc, char* argv[])
{

	const int framerate = 60;
	const int frameDelay = 1000 / framerate;

	Uint32 frameStart;
	int frameTime = 0;

	game = new Game();
	//init
	game->init
	(
		"Demo",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width, height,
		false
	);
	while (game->running())
	{
		frameStart = SDL_GetTicks();
		//
		game->handleEvents();
		game->update();
		game->render();
		//
		frameTime - SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);

		}
	}

	game->clean();
	//end code
	return 0;
}