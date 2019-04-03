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

	this->mSoldier.update(deltaTime);

	this->input(deltaTime);

	this->collision(deltaTime);
}

void Level_Two::render(sf::RenderTarget& target)
{
	target.draw(this->pBackgroundSprite);

	this->mPlayerPlane.render(target);

	this->mSoldier.render(target);
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

void Level_Two::collision(const float& deltaTime)
{
	for (int i = 0; i < this->mPlayerPlane.getBombSize(); i++)
		if (this->mPlayerPlane.getBomb(i).getBombPosition().y > this->pWindow->getSize().y)
			this->mPlayerPlane.removeProjectile(i);

	for (int i = 0; i < this->mPlayerPlane.getBombSize(); i++)
	{
		if (pCollision.playerEnemyCollision(this->mPlayerPlane.getBomb(i).getBombSprite(), this->mSoldier.getSoldierSprite(), deltaTime))
		{
			this->mPlayerPlane.setExplosionPosition(sf::Vector2f(this->mSoldier.getPosition().x - 250, this->mSoldier.getPosition().y - 200));

			this->mPlayerPlane.setExplosion(true);

			this->mExplosionTimer.restart(sf::seconds(0.3F));

			this->mPlayerPlane.removeProjectile(i);
		}
	}

	if (this->mExplosionTimer.isExpired())
		this->mPlayerPlane.setExplosion(false);
}