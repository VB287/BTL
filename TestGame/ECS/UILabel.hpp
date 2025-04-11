#pragma once

#include "ECS.hpp"
#include "../TextureManager.hpp"
#include "../Game.hpp"

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
using namespace std;

class UILabel : public Component
{
public:
	UILabel(int xpos, int ypos, string text, const char* path, int size, SDL_Color& color) :
		labelText(text), Path(path), textSize(size), textColor(color)
	{
		position.x = xpos;
		position.y = ypos;

		loadText();
		setLabelText(labelText);
	}
	~UILabel()
	{}

	TTF_Font* font = NULL;

	void loadText()
	{
		font = TTF_OpenFont(Path, textSize);
		//if (font == NULL) {
			//cout << TTF_GetError() << endl;
		//}
	}

	void setLabelText(string text)
	{
		//cout << TTF_GetError() << endl;
		SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), textColor);
		labelTexture = SDL_CreateTextureFromSurface(Game::renderer, surface);
		SDL_FreeSurface(surface);

		SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h);
	}
	void draw() override
	{
		SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &position);
	}
private:

	SDL_Rect position;
	string labelText;
	const char* Path;
	int textSize;
	SDL_Color textColor;
	SDL_Texture* labelTexture;

};
