#include "Game.hpp"
#include "TextureManager.hpp"
#include "Map.hpp"

#include "ECS/ECS.hpp"
#include "ECS/Component.hpp"
#include "Vector2D.hpp"
#include "Collision.hpp"

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
using namespace std;

//GameObject* player;

enum groupLabels : size_t
{
	groupPlayers,
	groupEnemies,
	groupCollider,
	groupProjectiles,
	pBullets,
	eBullets,
	npc,
};

Game::Game() {}
Game::~Game() {}

Map* map;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

Manager manager;
auto& player(manager.addEntity());
auto& labelHealth(manager.addEntity());
auto& labelScore(manager.addEntity());
//
int enemy_spawning_delay = 0;

int fire_delay = 0;

int playerhealth = 5;
const int maxPlayerHealth = playerhealth;

int score = 0;
//auto& wall(manager.addEntity());

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;

	if (fullscreen) { flags = SDL_WINDOW_FULLSCREEN; }
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		cout << "initialised" << endl;
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		//
		if (window) { cout << "window created" << endl; }
		//
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) 
		{ 
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255 );
			cout << "renderer created" << endl;
		}
		//
		isRunning = true;
	}
	else {
		isRunning = false;
	}
	TTF_Init();
	srand(time(0));

	SDL_Color white = { 255, 255, 255, 255 };
	stringstream sH, sS;
	sH << "Hitpoint: " << playerhealth << "/" << maxPlayerHealth;
	sS << "Scores: " << score;
	labelHealth.addComponent<UILabel>(10, 30, sH.str(), "Fonts/Seven-Segment.ttf", 20, white);
	labelScore.addComponent<UILabel>(10, 10, sS.str(), "Fonts/Seven-Segment.ttf", 20, white);

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//SDL_Surface* tmpSurface = IMG_Load("Assets/Sprite-0001.png");
	//texturePlayer = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	//SDL_FreeSurface(tmpSurface);
	///////////////////////////////////////////////////////////////////////////////////////////////////
	player.addComponent<TransformComponent>(215, 500, 67, 45, 1);
	player.addComponent<SpriteComponent>("Assets/Sprite-0001-sheet.png", 4, 50);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("players");
	player.addGroup(groupPlayers);

	//wall.addComponent<TransformComponent>(-20, 0, 680, 20, 1);
	//wall.addComponent<SpriteComponent>("Assets/Sprite-0003.png");
	//wall.addComponent<ColliderComponent>("wall");

	map = new Map();
	map->loadMap("Assets/Sprite-0002.png");

	//CreateEnemy(Vector2D(0, 2), 1000, 50);
	//CreateVFX("Assets/Sprite-0006.png", 2, 50, false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

auto& projectiles(manager.getGroup(groupProjectiles));
auto& enemies(manager.getGroup(groupEnemies));

auto& pbullets(manager.getGroup(pBullets));
auto& ebullets(manager.getGroup(eBullets));
auto& npcs(manager.getGroup(npc));
auto& players(manager.getGroup(groupPlayers));
auto& collider(manager.getGroup(groupCollider));

///////////////////////////////////////////////////////////////////////////////////////////////////

void CreateVFX(const char* path, int frames, int framespeed, bool oneShot)
{
	for (int i = 0; i < 2; i++) {
		int xPos = player.getComponent<TransformComponent>().position.x;
		if (i == 0) { xPos -= 1; }
		else { xPos += 37; }

		auto& vfx(manager.addEntity());
		vfx.addComponent<TransformComponent>(
			xPos,
			player.getComponent<TransformComponent>().position.y + 20,
			12, 9, 1
		);
		vfx.addComponent<SpriteComponent>(path, frames, framespeed);
		vfx.addComponent<VFX>(frames, framespeed, oneShot);
	}
}

void CreateExplosion(const char* path, int frames, int framespeed, bool oneShot, int xPos, int yPos)
{
	auto& vfx(manager.addEntity());
	vfx.addComponent<TransformComponent>(
		xPos,
		yPos,
		89, 89, 1
	);
	vfx.addComponent<SpriteComponent>(path, frames, framespeed);
	vfx.addComponent<VFX>(frames, framespeed, oneShot);
}

void CreatePlayerProjectile(Vector2D vel, int range, int speed)
{
	for (int i = 0; i < 2; i++) {
		int xPos = player.getComponent<TransformComponent>().position.x;
		if (i == 0) { xPos += 1; }
		else { xPos += 41; }

		auto& projectile(manager.addEntity());

		CreateVFX("Assets/Sprite-0006.png", 2, 25, true);

		projectile.addComponent<TransformComponent>(
			xPos,
			player.getComponent<TransformComponent>().position.y + 25,
			10, 3, 1
		);
		projectile.addComponent<SpriteComponent>("Assets/Sprite-0004.png");
		projectile.addComponent<ProjectileComponent>(range, speed, vel);
		projectile.addComponent<ColliderComponent>("pBullets");
		projectile.addGroup(Game::pBullets);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void CreateEnemy(Vector2D vel, int range, int speed, int health)
{

	int randomX = (rand() % 401);

	auto& enemy(manager.addEntity());
	enemy.addComponent<TransformComponent>(randomX, -50, 67, 45, 1);
	enemy.addComponent<SpriteComponent>("Assets/Sprite-0005-sheet.png", 4, 50);
	enemy.addComponent<BasicEnemyComponent>(range, speed, vel, health);
	enemy.addComponent<ColliderComponent>("npc");
	enemy.addGroup(Game::npc);
}

void CreateEnemyProjectile(Vector2D vel, int range, int speed, int xPos, int yPos)
{
	auto& projectile(manager.addEntity());
	projectile.addComponent<TransformComponent>(
		xPos,
		yPos,
		6, 6, 1
	);
	projectile.addComponent<SpriteComponent>("Assets/Sprite-0007.png");
	projectile.addComponent<ProjectileComponent>(range, speed, vel);
	projectile.addComponent<ColliderComponent>("eBullets");
	projectile.addGroup(Game::eBullets);
}

void updateUI()
{
	stringstream sH, sS;
	sH << "Hitpoint: " << playerhealth << "/" << maxPlayerHealth;
	sS << "Scores: " << score;
	labelHealth.getComponent<UILabel>().setLabelText(sH.str());
	labelScore.getComponent<UILabel>().setLabelText(sS.str());
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Game::handleEvents() 
{
	//SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Game::update()
{
	manager.refresh();
	manager.update();

	enemy_spawning_delay--;
	if (enemy_spawning_delay <= 0) {
		CreateEnemy(Vector2D(0, 2), 500, 1, 0);
		enemy_spawning_delay = 50;
	}

	//cout << player.getComponent<TransformComponent>().position.x << ", " <<
		//player.getComponent<TransformComponent>().position.y << endl;

	player.getComponent<TransformComponent>().boundPosition();
	player.getComponent<TransformComponent>().move(7);

	fire_delay--;
	if (fire_delay <= 0) {
		CreatePlayerProjectile(Vector2D(0, -2), 400, 10);
		fire_delay = 5;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////
	for (auto& p : pbullets)
	{
		for (auto& e : npcs) {
			if (Collision::AABB(e->getComponent<ColliderComponent>().collider,
				p->getComponent<ColliderComponent>().collider)) 
			{
				//cout << "enemy hit!" << endl;
				//CreateExplosion("Assets/explosion.png", 11, 50, true,p->getComponent<TransformComponent>().position.x - 45,
					//p->getComponent<TransformComponent>().position.y);
				p->destroy();
				e->getComponent<BasicEnemyComponent>().updateHitpoints(1);
			}
		}
	}

	for (auto& e : npcs) {
		if (e->getComponent<BasicEnemyComponent>().currentHealth <= 0) {
			//e->getComponent<TransformComponent>().velocity = Vector2D(0, 0);
			CreateExplosion("Assets/Explosion.png", 10, 50, true, e->getComponent<TransformComponent>().position.x - 25,
				e->getComponent<TransformComponent>().position.y);
			//cout << e->getComponent<TransformComponent>().position.x <<
				//e->getComponent<TransformComponent>().position.y << endl;
			e->destroy();
			score += 10; updateUI();
		}
		if (e->getComponent<BasicEnemyComponent>().attack_delay <= 0) {
			CreateEnemyProjectile(Vector2D(0, 2), 400, 2,
				e->getComponent<TransformComponent>().position.x + 20,
				e->getComponent<TransformComponent>().position.y + 60);
			e->getComponent<BasicEnemyComponent>().attack_delay = 200;
		}
		if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
			e->getComponent<ColliderComponent>().collider))
		{
			cout << "player hit!" << endl;
			e->getComponent<BasicEnemyComponent>().updateHitpoints(10000);
			playerhealth--;
		}
	}

	for (auto& e : ebullets) {
		if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
			e->getComponent<ColliderComponent>().collider))
		{
			cout << "player hit!" << endl;
			e->destroy();
			playerhealth--; updateUI();
		}
	}
	//cout << projectiles.size() << endl;
	//cout << enemies.size() << endl;

	//if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		//wall.getComponent<ColliderComponent>().collider))
	//{
		//cout << "hit" << endl;
	//}

	map->updateMap(5);
	//
	//count++;
	//cout << "the game has been updated " << count << " times!" << endl;

	if (playerhealth <= 0) {
		Game::isRunning = false;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////

void Game::render()
{
	//background -> objects -> NPC -> players
	SDL_RenderClear(renderer);
	///////////////////////////////////////////////////////////////////////////////////////////////////
	map->drawMap();
	manager.draw();
	labelHealth.draw();
	labelScore.draw();
	///////////////////////////////////////////////////////////////////////////////////////////////////
	SDL_RenderPresent(renderer);
	//add things to render//
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	cout << "quit game" << endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

//bool Game::running(){}

///////////////////////////////////////////////////////////////////////////////////////////////////

