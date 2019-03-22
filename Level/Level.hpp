#pragma once
#include <Entity/Player.hpp>
#include <Entity/Enemies/EnemyTank.hpp>

class Level
{
public:
	Level(sf::RenderWindow* window, std::stack<Level*>* level);
	~Level();

	virtual void update(const float& deltaTime) = 0;
	virtual void render(sf::RenderTarget& target) = 0;
	virtual void initLevel() = 0;

	void playerInput(const float& deltaTime);
	void removeProjectile();
	bool collision(const float& deltaTime);
	bool playerEnemyCollision(const float& deltaTime);
	
protected:
	std::stack<Level*>* pLevel;

	sf::RenderWindow* pWindow;

	Player pPlayer;
	std::vector<Player> player;

	EnemyTank pEnemeyTank;
	std::vector<EnemyTank> enemyTank;

	bool pLoadLevel;
};

