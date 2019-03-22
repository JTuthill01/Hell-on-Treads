#include "stdafx.hpp"
#include "Level.hpp"

Level::Level(sf::RenderWindow * window, std::stack<Level*>* level) : pWindow(window), pLevel(level), pLoadLevel(false) {}

Level::~Level() = default;

void Level::playerInput(const float & deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		this->pPlayer.move(0.45F, 0.F, deltaTime);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		this->pPlayer.move(-0.8F, 0.F, deltaTime);
}

void Level::removeProjectile()
{
	for (size_t i = 0; i < this->pPlayer.getProjectileSize(); i++)
		if (this->pPlayer.getProjectile(i).getProjectilePosition().x > this->pWindow->getSize().x)
			this->pPlayer.remove();
}

bool Level::collision(const float& deltaTime)
{
	for (size_t i = 0; i < this->pPlayer.getProjectileSize(); i++)
	{
		if (this->pPlayer.getProjectile(i).getGobalBounds().intersects(this->pEnemeyTank.getGobalBounds()))
			std::cout << ".." << "\n";

		return true;
	}
	return false;
}

bool Level::playerEnemyCollision(const float& deltaTime)
{
	if (pPlayer.getGobalBounds().intersects(this->pEnemeyTank.getGobalBounds()))
		return true;
	
	return false;
}



