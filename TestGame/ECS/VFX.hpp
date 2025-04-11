#pragma once

#include "ECS.hpp"
#include "SpriteComponent.hpp"
#include "Component.hpp"

#include "SDL.h"
#include "../TextureManager.hpp"

#include <iostream>
using namespace std;


class VFX : public Component
{
public:
	VFX(int f, int fs, bool ot) :frames(f), framespeed(fs), oneShot(ot)
	{}
	~VFX()
	{}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
	}

	void update() override
	{
		lifetime += framespeed;
		if (oneShot == true && lifetime > framespeed * frames)
		{
			entity->destroy();
		}
	}

private:
	TransformComponent* transform;
	int lifetime = 0;
	int frames = 0;
	int framespeed = 0;
	bool oneShot = true;
};