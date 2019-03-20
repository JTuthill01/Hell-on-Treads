#include "stdafx.hpp"
#include "Enemies.hpp"

std::vector<sf::Texture> Enemies::mEnemyProjectileTextures;

Enemies::Enemies() : mShootTimer(mShootTimerMax), mShootTimerMax(50.F), mDropBomb(false)
{
	if (!this->mEnemyTexture.loadFromFile("Resources/Textures/Enemy/AEG_CIV_default.png"))
		std::cerr << "Failed to load the goddamn enemy, asshole" << "\n";

	this->mEnemySprite.setTexture(this->mEnemyTexture);
	this->mEnemySprite.setPosition(1800.F, 0.F);
	this->mEnemySprite.setScale(-1.5F, 1.5F);

	this->mEnemyCenter.x = this->mEnemySprite.getPosition().x + this->mEnemySprite.getGlobalBounds().width / 2;
	this->mEnemyCenter.y = this->mEnemySprite.getPosition().y + this->mEnemySprite.getGlobalBounds().height / 2;

	this->pAudio.loadSound("Resources/Sounds/Explosions/explosion1.ogg");
	this->pAudio.setVolume(10.F);

	this->loadProjectile();
}

Enemies::~Enemies() = default;

void Enemies::render(sf::RenderTarget& target)
{
	target.draw(this->mEnemySprite);

	for (size_t i = 0; i < this->mProjectiles.size(); i++)
		this->mProjectiles[i].render(target);
}

void Enemies::update(const float& deltaTime)
{
	this->mEnemySprite.move(sf::Vector2f(-0.6F, 0.F));

	this->checkPosition(deltaTime);

	if (this->checkPosition(deltaTime))
		this->bombDrop(deltaTime);

	for (size_t i = 0; i < this->mProjectiles.size(); i++)
		this->mProjectiles[i].update(deltaTime);
}

void Enemies::move(const float direction_x, const float direction_y, const float& deltaTime)
{
	if (this->pMovementComponent)
		this->pMovementComponent->move(direction_x, direction_y, deltaTime);
}

bool Enemies::checkPosition(const float& deltaTime)
{
	if (this->mEnemySprite.getPosition().y <= this->pPlayer.getPosition().y)
	{
		this->mDropBomb = true;
		return true;
	}

	else
		this->mDropBomb = false;

	return false;
}

void Enemies::bombDrop(const float& deltaTime)
{
	if (this->mDropBomb)
		this->mProjectiles.push_back(Projectile(&Enemies::mEnemyProjectileTextures[BOMB], this->mEnemySprite.getPosition().x + 300, this->mEnemySprite.getPosition().y + 195, sf::Vector2f(0.5F, 0.5F),
			sf::Vector2f(0.F, 0.5F)));
}

void Enemies::loadProjectile()
{
	sf::Texture temp;
	if (!temp.loadFromFile("Resources/Textures/Bullets/Bomb_2.png"))
		std::cerr << "Failed to fucking the fucking bomb texture" << "\n";

	this->mEnemyProjectileTextures.push_back(temp);
}

void Enemies::updateAttack(const float& deltaTime){}
