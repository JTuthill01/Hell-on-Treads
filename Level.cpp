#include "stdafx.hpp"
#include "Level.hpp"

std::vector<sf::Texture> Level::pParticleTextures;

Level::Level(sf::RenderWindow* window, std::stack<Level*>* level) : pWindow(window), pLevel(level), pLoadLevel(false), pHasExploaded(false), pIsPlayerProjectileRemoved(false), pShouldClose(false)
{
	this->pFont.loadFromFile("Resources/Fonts/Anton-Regular.ttf");
}

Level::~Level()
{
	while (!this->pLevel->empty())
	{
		delete this->pLevel->top();

		this->pLevel->pop();
	}
}

void Level::removeTrees(unsigned index) { this->pTrees.erase(this->pTrees.begin() + index); }

void Level::loadTrees()
{
	sf::Texture temp;
	if (!temp.loadFromFile("Resources/Trees/trees.png"))
		std::cerr << "Trees failed to load" << "\n";

	this->pTree.treeTextures.push_back(temp);
}

void Level::endGameInput()
{
	if (pShouldClose)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			this->pWindow->close();
	}
}

void Level::endGame()
{
	this->mText.setFont(pFont);
	this->mText.setCharacterSize(150U);
	this->mText.setFillColor(sf::Color::Red);
	this->mText.setPosition(sf::Vector2f((this->pWindow->getSize().x / 2) - 700, (this->pWindow->getSize().y / 2) - 100));
	this->mText.setString("GAME THE FUCK OVER!!!!!");

	this->pAudio.loadSound("Resources/Sounds/Game Over/Fuck.ogg");
	this->pAudio.play();

	this->mResetText.setFont(pFont);
	this->mResetText.setCharacterSize(100U);
	this->mResetText.setFillColor(sf::Color::Red);
	this->mResetText.setPosition(sf::Vector2f((this->pWindow->getSize().x / 2.F) - 750.F, (this->pWindow->getSize().y / 2.F) - 300.F));
	this->mResetText.setString("Press Esc to quit "  " Press F1 to restart");

	this->pWindow->draw(this->mResetText);

	this->pWindow->draw(this->mText);

	this->pShouldClose = true;
}

void Level::renderEndGame(sf::RenderTarget & target)
{
	target.draw(this->mText);

	target.draw(this->mResetText);
}

void Level::removeEnemyPlane(unsigned index) { this->pEnemyPlane.erase(this->pEnemyPlane.begin() + index); }

void Level::playerProjectileCollision(const float& deltaTime)
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

void Level::enemyProjectileCollision(const float& deltaTime)
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
	}

	if (this->mExplosionTimer.isRunning())
		this->pHasExploaded = true;

	else if (this->mExplosionTimer.isExpired())
		this->pHasExploaded = false;

	if (this->pTextTagTimer.isExpired() && !this->pTextTags.empty())
		this->pTextTags.pop_back();
}

void Level::updateLevel(const float& deltaTime)
{
	for (size_t i = 0; i < this->pParticle.size(); i++)
		this->pParticle[i].update(deltaTime);

	this->pEnemyTank.update(deltaTime);

	this->playerProjectileCollision(deltaTime);

	this->enemyProjectileCollision(deltaTime);

	this->pCollision.removePlayerProjectile(this->pPlayer, this->pWindow, deltaTime);

	this->pCollision.removeEnemyProjectile(this->pWindow, this->pEnemyTank, deltaTime);

	this->pCollision.playerEnemyCollision(this->pPlayer.getPlayerSprite(), this->pEnemyTank.getEnemyTankSprite(), deltaTime);

	this->pCollision.playerProjectileCollision(this->pPlayer, this->pEnemyTank.getEnemyTankSprite(), deltaTime);

	this->pCollision.enemyProjectileCollision(this->pEnemyTank, this->pPlayer.getPlayerSprite(), deltaTime);
}

void Level::setExplosions(sf::Vector2f position, sf::Vector2f scale)
{
	if (!this->mExpolsionTexture.loadFromFile("Resources/Textures/Explosions/explosion1.png"))
		std::cerr << "Whelp you can go fuck yourself, as the fucking explosion texture didn't load";

	this->mExpolsionSprite.setTexture(this->mExpolsionTexture);
	this->mExpolsionSprite.setPosition(position);
	this->mExpolsionSprite.setScale(scale);

	this->timer();
}



void Level::timer()
{
	this->pTextTagTimer.restart(sf::seconds(0.2F));

	this->mExplosionTimer.reset(sf::seconds(0.3F));
	this->mExplosionTimer.start();

	this->mParticleTimer.reset(sf::seconds(6.F));
	this->mParticleTimer.start();
}