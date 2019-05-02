#include "stdafx.hpp"
#include "Level_One.hpp"

Level_One::Level_One(sf::RenderWindow* window, std::stack<Level*>* level) : Level(window, level)
{
	this->initLevel();

	this->loadClouds();

	this->pCloudSpawnTimer.restart(sf::seconds(0.1F));

	this->pEnemySpawnTimer.restart(sf::seconds(0.1F));

	this->loadTank();
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
	this->playerEnemyCollision(deltaTime);

	this->enemyPlayerCollision(deltaTime);

	this->pCollision.removePlayerProjectile(this->pPlayer, this->pWindow, deltaTime);

	this->pCollision.removeEnemyProjectile(this->pWindow, this->pEnemyTank, deltaTime);

	this->pCollision.playerEnemyCollision(this->pPlayer.getPlayerSprite(), this->pEnemyTank.getEnemyTankSprite(), deltaTime);

	this->pCollision.playerProjectileCollision(this->pPlayer, this->pEnemyTank.getEnemyTankSprite(), deltaTime);

	this->pCollision.enemyProjectileCollision(this->pEnemyTank, this->pPlayer.getPlayerSprite(), deltaTime);

	this->updateLevel(deltaTime);

	this->pPlayer.update(deltaTime);

	if (this->pCloudSpawnTimer.isExpired())
	{
		this->mClouds.push_back(Clouds(this->mCloudTextures, sf::Vector2f(-600.F, -10.F)));

		this->pCloudSpawnTimer.restart(sf::seconds(2.F));
	}

	for (size_t i = 0; i < this->mClouds.size(); i++)
		this->mClouds[i].update(deltaTime);

	if (this->pLoadLevel == false)
	{
		if (this->pPlayer.getPosition().x > this->pWindow->getSize().x)
		{
			this->pLevel->push(new Level_Two(this->pWindow, this->pLevel));

			this->pWeather.stopAudio();

			this->pLoadLevel = true;
		}
	}

	this->pWeather.update();

	this->playerInput(deltaTime);

	for (size_t i = 0; i < this->mClouds.size(); i++)
		if (this->mClouds[i].getCloudPosition().x > this->pWindow->getSize().x)
			this->removeClouds(i);
}

void Level_One::render(sf::RenderTarget& target)
{
	target.draw(this->mLevelOneSprite);

	if (this->pHasExploaded)
		target.draw(this->mExpolsionSprite);

	this->pPlayer.render(target);

	this->pEnemyTank.render(target);

	this->pWeather.render(target);

	this->mAurora.render(target);

	for (size_t i = 0; i < this->mClouds.size(); i++)
	{
		if(this->mClouds[i].getCloudPosition().x <= this->pWindow->getSize().x)
			this->mClouds[i].render(target);
	}

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

void Level_One::removeClouds(unsigned index)
{
	this->mClouds.erase(this->mClouds.begin() + index);
}

void Level_One::loadClouds()
{
	sf::Texture temp;
	if (!temp.loadFromFile("Resources/Textures/Weather/Cloud.png"))
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

void Level_One::playerEnemyCollision(const float& deltaTime)
{
	if (this->pCollision.playerProjectileCollision(this->pPlayer, this->pEnemyTank.getEnemyTankSprite(), deltaTime))
	{
		this->setExplosions(sf::Vector2f(this->pEnemyTank.getEnemyTankPosition().x, this->pEnemyTank.getEnemyTankPosition().y), sf::Vector2f(2.F, 2.F));

		this->mAurora.setAurora(deltaTime);

		this->mAurora.setAuroraPosition(sf::Vector2f(this->pEnemyTank.getEnemyTankPosition().x, this->pEnemyTank.getEnemyTankPosition().y + 100));

		int damage = this->pPlayer.playerDealDamage();

		if (this->pEnemyTank.getHp() > 0)
			this->pEnemyTank.takeDamage(damage);

		this->pTextTags.push_back(TextTags(&this->pFont, "BOOM!!!!", sf::Vector2f(this->pEnemyTank.getEnemyTankPosition().x + 100, this->pEnemyTank.getEnemyTankPosition().y + 100), sf::Vector2f(2.F, 2.F), sf::Color::Yellow,
			40U));

		this->pTextTags.push_back(TextTags(&this->pFont, "- " + std::to_string(damage), sf::Vector2f(this->pEnemyTank.getEnemyTankPosition().x + 100, this->pEnemyTank.getEnemyTankPosition().y), sf::Vector2f(1.F, 1.F), sf::Color::Yellow,
			40U));
	}

	if (this->mExplosionTimer.isRunning())
		this->pHasExploaded = true;

	else if (this->mExplosionTimer.isExpired())
		this->pHasExploaded = false;

	if (this->pTextTagTimer.isExpired() && !this->pTextTags.empty())
		this->pTextTags.pop_back();
}

void Level_One::enemyPlayerCollision(const float& deltaTime)
{
	if (this->pCollision.enemyProjectileCollision(this->pEnemyTank, this->pPlayer.getPlayerSprite(), deltaTime))
	{
		this->mAurora.setEnemyAurora(deltaTime);

		this->mAurora.setEnemyAuroraPosition(sf::Vector2f(this->pPlayer.getPosition().x - 150, this->pPlayer.getPosition().y));

		this->setExplosions(sf::Vector2f(this->pPlayer.getPlayerSprite().getPosition().x, this->pPlayer.getPlayerSprite().getPosition().y), sf::Vector2f(2.F, 2.F));

		int damage = this->pEnemyTank.enemyDealDamage();

		if (this->pPlayer.getPlayerHp() > 0)
			this->pPlayer.takeDamage(damage);

		this->pTextTags.push_back(TextTags(&this->pFont, "BOOM!!!!", sf::Vector2f(this->pPlayer.getPosition().x + 100, this->pPlayer.getPosition().y), sf::Vector2f(1.F, 2.F), sf::Color::Yellow,
			40U));

		this->pTextTags.push_back(TextTags(&this->pFont, "- " + std::to_string(damage), sf::Vector2f(this->pPlayer.getPosition().x + 100, this->pPlayer.getPosition().y + 50), sf::Vector2f(1.F, 2.F), sf::Color(178, 34, 34, 240),
			40U));

		if (this->pPlayer.getPlayerHp() == 0)
			this->endGame();
	}

	if (this->mExplosionTimer.isRunning())
		this->pHasExploaded = true;

	else if (this->mExplosionTimer.isExpired())
		this->pHasExploaded = false;

	if (this->pTextTagTimer.isExpired() && !this->pTextTags.empty())
		this->pTextTags.pop_back();
}

void Level_One::removeProjectile()
{
}