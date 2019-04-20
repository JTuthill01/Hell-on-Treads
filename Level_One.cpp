#include "stdafx.hpp"
#include "Level_One.hpp"

Level_One::Level_One(sf::RenderWindow* window, std::stack<Level*>* level) : Level(window, level)
{
	this->initLevel();

	this->loadClouds();

	this->pCloudSpawnTimer.restart(sf::seconds(0.4F));
}

Level_One::~Level_One()
{
	while (!this->pLevel->empty())
	{
		delete this->pLevel->top();

		this->pLevel->pop();
	}
}

void Level_One::update(const float& deltaTime)
{
	this->updateLevel(deltaTime);

	this->pPlayer.update(deltaTime);

	this->mClouds.emplace_back(Weather(this->mCloudTextures, sf::Vector2f(1000.F, 100.F)));

	for (size_t i = 0; i < this->mClouds.size(); i++)
		std::cout << "x position: " << this->mClouds[i].getCloudPosition().x << " y position: " << this->mClouds[i].getCloudPosition().y << "\n";

	if (this->pLoadLevel == false)
	{
		if (this->pPlayer.getPosition().x > this->pWindow->getSize().x)
		{
			this->pLevel->push(new Level_Two(this->pWindow, this->pLevel));

			this->pLoadLevel = true;
		}
	}

	this->mRain.update();

	this->playerInput(deltaTime);
}

void Level_One::render(sf::RenderTarget& target)
{
	target.draw(this->mLevelOneSprite);

	if (this->pHasExploaded)
		target.draw(this->mExpolsionSprite);

	this->pPlayer.render(target);

	this->pEnemyTank.render(target);

	this->mRain.render(target);

	this->mAurora.render(target);

	for (size_t i = 0; i < this->mClouds.size(); i++)
		this->mClouds[i].renderClouds(target);

	if (this->pTextTagTimer.isRunning())
		for (size_t i = 0; i < this->pTextTags.size(); i++)
			this->pTextTags[i].render(target);
}

void Level_One::initLevel()
{
	if (!this->mLevelOneTexture.loadFromFile("Resources/Textures/Backgrounds/War3/War3.png"))
		std::cerr << "Level One failed to fucking load" << "\n";

	this->mLevelOneSprite.setTexture(this->mLevelOneTexture);
}

void Level_One::loadClouds()
{
	sf::Texture temp;
	if (!temp.loadFromFile("Resources/Textures/Weather/cloud3.png"))
		std::cerr << "Clouds not found" << "\n";

	this->mCloudTextures.emplace_back(temp);
}

void Level_One::loadTank()
{
	sf::Texture temp;
	temp.loadFromFile("Resources/Textures/Enemy/TankCombined.png");

	this->mEnemyTankTextures.push_back(temp);
}

void Level_One::playerInput(const float& deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		this->pPlayer.move(0.45F, 0.F, deltaTime);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		this->pPlayer.move(-0.8F, 0.F, deltaTime);
}

void Level_One::removeProjectile()
{
}