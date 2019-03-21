#include "stdafx.hpp"
#include "Level_One.hpp"

Level_One::Level_One(sf::RenderWindow* window, std::stack<Level*>* level) : Level(window, level)
{
	this->initLevel();
}

Level_One::~Level_One()
{
}

void Level_One::update(const float& deltaTime)
{
	this->collision(deltaTime);

	this->pPlayer.update(deltaTime);

	this->playerInput(deltaTime);

	this->pEnemies.update(deltaTime);

	for (size_t j = 0; j < this->player.size(); j++)
		this->player[j].update(deltaTime);

	for (size_t k = 0; k < this->enemy.size(); k++)
		enemy[k].update(deltaTime);
}

void Level_One::render(sf::RenderTarget & target)
{
	target.draw(this->mLevelOneSprite);

	pPlayer.render(target);

	pEnemies.render(target);
}

void Level_One::initLevel()
{
	if (!this->mLevelOneTexture.loadFromFile("Resources/Textures/Backgrounds/War3/War3.png"))
		std::cerr << "Level One failed to fucking load" << "\n";

	this->mLevelOneSprite.setTexture(this->mLevelOneTexture);

	this->player.push_back(Player());

	this->enemy.push_back(Enemies());
}

void Level_One::collisions()
{
	if (pPlayer.getGobalBounds().intersects(pEnemies.getGobalBounds()))
		std::cout << "collision" << "\n";
}


