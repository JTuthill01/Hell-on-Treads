#include "stdafx.hpp"
#include "Collision.hpp"

Collision::Collision() = default;

Collision::~Collision() = default;

bool Collision::playerEnemyCollision(sf::Sprite sprite0, sf::Sprite sprite1, const float& deltaTime)
{
	if (sprite0.getGlobalBounds().intersects(sprite1.getGlobalBounds()))
		return true;

	return false;
}

void Collision::removePlayerProjectileAfterCollision(Player player, sf::Sprite sprite, const float& deltaTime)
{
	for (int i = 0; i < player.getPlayerProjectileSize(); i++)
		if (player.getPlayerProjectile(i).getGobalBounds().intersects(sprite.getGlobalBounds()))
			player.removePlayerProjectile(i);
}

void Collision::removeEnemyProjectileAfterCollision(EnemyTank enemyTank, sf::Sprite sprite, const float& deltaTime)
{
	for (int i = 0; i < enemyTank.getEnemyTankProjectileSize(); i++)
		if (enemyTank.getEnemyTankProjectile(i).getGobalBounds().intersects(sprite.getGlobalBounds()))
			enemyTank.removeEnemyTankProjectile(i);
}

void Collision::removePlayerProjectile(Player player, sf::RenderWindow* window, const float& deltaTime)
{
	for (int i = 0; i < player.getPlayerProjectileSize(); i++)
		if (player.getPlayerProjectile(i).getProjectilePosition().x > window->getSize().x)
			player.removePlayerProjectile(i);
}

void Collision::removeEnemyProjectile(sf::RenderWindow* window, EnemyTank enemy, const float& deltaTime)
{
	for (int i = 0; i < enemy.getEnemyTankProjectileSize(); i++)
		if (enemy.getEnemyTankProjectile(i).getProjectilePosition().x > window->getSize().x)
			enemy.removeEnemyTankProjectile(i);
}


bool Collision::enemyProjectileCollision(EnemyTank enemy, sf::Sprite sprite, const float& deltaTime)
{
	for (int i = 0; i < enemy.getEnemyTankProjectileSize(); i++)
	{
		if (enemy.getEnemyTankProjectile(i).getEnemyProjectileSprite().getGlobalBounds().intersects(sprite.getGlobalBounds()))
			return true;
	}
	return false;
}

bool Collision::playerProjectileCollision(Player player, sf::Sprite sprite, const float& deltaTime)
{
	for (int i = 0; i < player.getPlayerProjectileSize(); i++)
	{
		if (player.getPlayerProjectile(i).getProjectileSprite().getGlobalBounds().intersects(sprite.getGlobalBounds()))
		{
			player.removePlayerProjectile(i);

			return true;
		}
	}
	return false;
}

