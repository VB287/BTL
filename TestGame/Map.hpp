#include "Game.hpp"
using namespace std;

class Map
{
public:
	Map();
	~Map();

	void loadMap(const char* path);
	void updateMap(int speed);
	void drawMap();

private:
	SDL_Rect src, dest;
	SDL_Texture* background;
	int xpos = 0, ypos = 0;
};
