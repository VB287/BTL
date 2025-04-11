#include "Map.hpp"
#include "TextureManager.hpp"

#include <iostream>
using namespace std;

Map::Map()
{}

Map::~Map() {
	SDL_DestroyTexture(background);
}

void Map::loadMap(const char* path) {

	background = TextureManager::LoadTexture(path);
	src.x = 0; src.y = 0;
	src.w = 480;
	src.h = 1360;
	dest.w = 480;
	dest.h = 1360;
	//
	dest.y = -680;
}

void Map::updateMap(int speed) {
	dest.y += speed;
	if (dest.y == 0) {
		dest.y = -680;
	}
}

void Map::drawMap()
{
	TextureManager::Draw(background, src, dest);
}