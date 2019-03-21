#pragma once
#include <Entity/Player.hpp>
#include <Entity/Enemies/Enemies.hpp>
#include <Collision/Collision.hpp>

class Level
{
public:
	Level(sf::RenderWindow* window, std::stack<Level*>* level);
	~Level();

	virtual void update(const float& deltaTime) = 0;
	virtual void render(sf::RenderTarget& target) = 0;
	virtual void initLevel() = 0;

	void playerInput(const float& deltaTime);

	void collision(const float& deltaTime);
	
protected:
	std::stack<Level*>* pLevel;

	sf::RenderWindow* pWindow;

	Player pPlayer;
	std::vector<Player> player;

	Enemies pEnemies;
	std::vector<Enemies> enemy;

	bool pLoadLevel;
};

