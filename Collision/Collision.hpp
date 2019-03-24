#pragma once
#include <Entity/Player.hpp>
#include <Entity/Enemies/EnemyTank.hpp>

class Collision
{
public:
	Collision();
	~Collision();

	bool enemyProjectileCollision(EnemyTank enemy, sf::Sprite sprite, const float& deltaTime);
	bool playerProjectileCollision(Player player, sf::Sprite sprite, const float& deltaTime);
	bool playerEnemyCollision(sf::Sprite sprite0, sf::Sprite sprite1, const float& deltaTime);

	void removePlayerProjectileAfterCollision(Player player, sf::Sprite sprite, const float& deltaTime);
	void removeEnemyProjectileAfterCollision(EnemyTank enemyTank, sf::Sprite sprite, const float& deltaTime);
	void removePlayerProjectile(Player player, sf::RenderWindow* window, const float& deltaTime);
	void removeEnemyProjectile(sf::RenderWindow* window, EnemyTank enemy, const float& deltaTime);
};

