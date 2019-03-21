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

	this->mEnemySprite.setOrigin(this->mEnemySprite.getGlobalBounds().left + this->mEnemySprite.getGlobalBounds().width / 2,
		this->mEnemySprite.getGlobalBounds().top + this->mEnemySprite.getGlobalBounds().height / 2);

	this->pAudio.loadSound("Resources/Sounds/Explosions/explosion1.ogg");
	this->pAudio.setVolume(10.F);

	this->loadProjectile();

	this->loadEnemyTanks();
}

Enemies::~Enemies() = default;

void Enemies::render(sf::RenderTarget& target)
{
	target.draw(this->mEnemySprite);

	target.draw(this->mEnemyTankSprite);

	for (size_t i = 0; i < this->mProjectiles.size(); i++)
		this->mProjectiles[i].render(target);
}

void Enemies::update(const float& deltaTime)
{
	this->mEnemySprite.move(sf::Vector2f(-0.6F, 0.F));

	this->mEnemyTankSprite.move(sf::Vector2f(-0.6F, 0.F));

	for (size_t i = 0; i < this->mProjectiles.size(); i++)
		this->mProjectiles[i].update(deltaTime);
}

void Enemies::move(const float direction_x, const float direction_y, const float& deltaTime)
{
	if (this->pMovementComponent)
		this->pMovementComponent->move(direction_x, direction_y, deltaTime);
}

void Enemies::loadEnemyTanks()
{
	if (!this->mEnemyTankTexture.loadFromFile("Resources/Textures/Enemy/EnemyTank.png"))
		std::cerr << "Fucking EnemyTank failed to fucking load" << "\n";

	this->mEnemyTankSprite.setTexture(this->mEnemyTankTexture);
	this->mEnemyTankSprite.setScale(-1.7F, 1.7F);
	this->mEnemyTankSprite.setPosition(1800.F, 710.F);
	this->mEnemyTankSprite.setColor(sf::Color::Blue);
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
