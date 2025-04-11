//USE DEVEL-VC VERY IMPORTANT
#pragma once

#ifndef Game_hpp
#define Game_hpp

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <vector>

class ColliderComponent;

class Game {
public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	void handleEvents();
	void update();
	void render();
	void clean();
	bool running() { return isRunning; };

	static SDL_Renderer* renderer;
	static SDL_Event event;
	enum groupLabels : size_t
	{
		groupPlayers,
		groupColliders,
		groupProjectiles,
		groupEnemies,
		pBullets,
		eBullets,
		npc,
	};

private:
	int count = 0;
	bool isRunning;
	SDL_Window* window;
	//SDL_Renderer* renderer;
};

#endif /* Game_hpp */