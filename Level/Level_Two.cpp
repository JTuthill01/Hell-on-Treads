#include "stdafx.hpp"
#include "Level_Two.hpp"

Level_Two::Level_Two(sf::RenderWindow* window, std::stack<Level*>* level) : Level(window, level)
{
	this->initLevel();
}

Level_Two::~Level_Two() = default;

void Level_Two::update(const float& deltaTime)
{
	this->updateLevel(deltaTime);

	this->mPlayerPlane.update(deltaTime);

	this->input(deltaTime);
}

void Level_Two::render(sf::RenderTarget& target)
{
	target.draw(this->pBackgroundSprite);

	this->mPlayerPlane.render(target);
}

void Level_Two::initLevel()
{
	if (!this->pBackgroundTexture.loadFromFile("Resources/Textures/Backgrounds/War4/000.png"))
		std::cerr << "Fucking level, dun fucked up and failed to load" << "\n";

	this->pBackgroundSprite.setTexture(this->pBackgroundTexture);
}

void Level_Two::input(const float& deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		this->mPlayerPlane.move(0.5F, 0.F, deltaTime);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		this->mPlayerPlane.move(-0.5F, 0.F, deltaTime);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		this->mPlayerPlane.move(0.F, -0.5F, deltaTime);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		this->mPlayerPlane.move(0.F, 0.5F, deltaTime);
}