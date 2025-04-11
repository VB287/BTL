#pragma once

#include "Component.hpp"
#include "../Vector2D.hpp"

class TransformComponent : public Component
{
public:

	Vector2D position;
	Vector2D velocity;

	int height = 0;
	int width = 0;
	int scale = 1;

	TransformComponent()
	{
		position.x = 0.0f;
		position.y = 0.0f;
	}

	TransformComponent(float x, float y)
	{
		position.x = x;
		position.y = y;
	}

	TransformComponent(float x, float y, int h, int w, int sc)
	{
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = sc;
	}

	void boundPosition()
	{
		if (position.x < 0) { position.x = 0; }
		else if (position.x > 445) { position.x = 445; }
		if (position.y < 0) { position.y = 0; }
		else if (position.y > 625) { position.y = 625; }
	}

	void move(int speed) {
		position.x += static_cast<int>(velocity.x * speed);
		position.y += static_cast<int>(velocity.y * speed);
	}

	void init() override
	{
		velocity.x = 0;
		velocity.y = 0;
	}
};