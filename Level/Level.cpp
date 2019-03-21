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

void Level::collision(const float& deltaTime)
{
	if (this->pPlayer.getGobalBounds().intersects(this->pEnemies.getGobalBounds()))
		std::cout << "Collision" << "\n";
}



