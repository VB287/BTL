#pragma once

#include "ECS.hpp"
#include "Component.hpp"
#include "../Vector2D.hpp"

class BasicEnemyComponent : public Component
{
public:
	BasicEnemyComponent(int rng, int sp, Vector2D vel, int hlth) : range(rng), speed(sp), velocity(vel)
	{}
	~BasicEnemyComponent()
	{}

	int health = 10;
	int maxHealth = health;
	int currentHealth = maxHealth;

	int attack_delay = 0;

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		transform->velocity = velocity;
	}

	void updateHitpoints(int dmg) {
		currentHealth -= dmg;
	}

	void update() override
	{
		attack_delay--;
		transform->move(speed);
		distance += speed;
		if (distance > range)
		{
			entity->destroy();
		}
	}

private:
	TransformComponent* transform;
	int range = 0;
	int speed = 0;
	int distance = 0;
	Vector2D velocity;
};