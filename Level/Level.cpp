#include "stdafx.hpp"
#include "Level.hpp"

std::vector<sf::Texture> Level::pParticleTextures;

Level::Level(sf::RenderWindow* window, std::stack<Level*>* level) : pWindow(window), pLevel(level), pLoadLevel(false), pHasExploaded(false)
{
}

Level::~Level()
{
	while (!this->pLevel->empty())
	{
		delete this->pLevel->top();

		this->pLevel->pop();
	}
}

void Level::playerProjectileCollision(const float& deltaTime)
{
	if (this->pCollision.playerProjectileCollision(this->pPlayer, this->pEnemeyTank.getEnemyTankSprite(), deltaTime))
	{
		this->setExplosions(sf::Vector2f(this->pEnemeyTank.getEnemyTankPosition().x, this->pEnemeyTank.getEnemyTankPosition().y), sf::Vector2f(2.F, 2.F));

		this->mAurora.setAurora(deltaTime);

		this->mAurora.setAuroraPosition(sf::Vector2f(this->pEnemeyTank.getEnemyTankPosition().x, this->pEnemeyTank.getEnemyTankPosition().y + 100));

		int damage = this->pPlayer.playerDealDamage();

		if (this->pEnemeyTank.getHp() > 0)
			this->pEnemeyTank.takeDamage(damage);
	}

	if (this->pCollision.enemyProjectileCollision(this->pEnemeyTank, this->pPlayer.getPlayerSprite(), deltaTime))
	{
		this->mAurora.setEnemyAurora(deltaTime);

		this->mAurora.setEnemyAuroraPosition(sf::Vector2f(this->pPlayer.getPosition().x - 150, this->pPlayer.getPosition().y));
	}

	if (!this->mExplosionTimer.isExpired())
		this->pHasExploaded = true;

	else if (this->mExplosionTimer.isExpired())
		this->pHasExploaded = false;
}

void Level::enemyProjectileCollision(const float& deltaTime)
{
	if (this->pCollision.enemyProjectileCollision(this->pEnemeyTank, this->pPlayer.getPlayerSprite(), deltaTime))
		this->setExplosions(sf::Vector2f(this->pPlayer.getPlayerSprite().getPosition().x, this->pPlayer.getPlayerSprite().getPosition().y), sf::Vector2f(2.F, 2.F));

	if (!this->mExplosionTimer.isExpired())
		this->pHasExploaded = true;

	else if (this->mExplosionTimer.isExpired())
		this->pHasExploaded = false;
}

void Level::playerInput(const float& deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		this->pPlayer.move(0.45F, 0.F, deltaTime);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		this->pPlayer.move(-0.8F, 0.F, deltaTime);
}

void Level::updateLevel(const float& deltaTime)
{
	for (size_t i = 0; i < this->pParticle.size(); i++)
		this->pParticle[i].update(deltaTime);

	this->pPlayer.update(deltaTime);

	this->playerInput(deltaTime);

	this->pEnemeyTank.update(deltaTime);

	this->playerProjectileCollision(deltaTime);

	this->enemyProjectileCollision(deltaTime);

	this->pCollision.removePlayerProjectile(this->pPlayer, this->pWindow, deltaTime);

	this->pCollision.playerEnemyCollision(this->pPlayer.getPlayerSprite(), this->pEnemeyTank.getEnemyTankSprite(), deltaTime);

	this->pCollision.playerProjectileCollision(this->pPlayer, this->pEnemeyTank.getEnemyTankSprite(), deltaTime);

	this->pCollision.enemyProjectileCollision(this->pEnemeyTank, this->pPlayer.getPlayerSprite(), deltaTime);

	this->pCollision.removeEnemyProjectile(this->pWindow, this->pEnemeyTank, deltaTime);

	this->pCollision.removeEnemyProjectileAfterCollision(this->pEnemeyTank, this->pPlayer.getPlayerSprite(), deltaTime);
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
	this->mExplosionTimer.reset(sf::seconds(0.3F));
	this->mExplosionTimer.start();

	this->mParticleTimer.reset(sf::seconds(6.F));
	this->mParticleTimer.start();
}