#include "stdafx.hpp"
#include "Level_One.hpp"

Level_One::Level_One(sf::RenderWindow * window, std::stack<Level*>* level) : Level(window, level)
{
	this->initLevel();
}

Level_One::~Level_One()
{
}

void Level_One::update(const float & deltaTime)
{
	this->pPlayer.update(deltaTime);

	this->playerInput(deltaTime);

	this->pEnemies.update(deltaTime);
}

void Level_One::render(sf::RenderTarget & target)
{
	target.draw(this->mLevelOneSprite);

	this->pPlayer.render(target);

	this->pEnemies.render(target);
}

void Level_One::initLevel()
{
	if (!this->mLevelOneTexture.loadFromFile("Resources/Textures/Backgrounds/War3/War3.png"))
		std::cerr << "Level One failed to fucking load" << "\n";

	this->mLevelOneSprite.setTexture(this->mLevelOneTexture);
}