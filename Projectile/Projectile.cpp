#include "stdafx.hpp"
#include "Projectile.hpp"

Projectile::Projectile(sf::Texture* texture, sf::Vector2f position, sf::Vector2f scale, sf::Vector2f direction) : mTimer(mTimerMax), mTimerMax(2.F)
{
	this->mProjectileTexture = texture;
	this->mProjectileSprite.setTexture(*this->mProjectileTexture);

	this->mPosition = position;
	this->mScale = scale;
	this->mDirection = direction;

	this->mProjectileSprite.setScale(scale);
	this->mProjectileSprite.setPosition(position);
	this->mProjectileSprite.setRotation(atan2(this->mDirection.y, this->mDirection.x) * 180 / thor::Pi + 180);
}

Projectile::Projectile(sf::Texture* texture, float x, float y, sf::Vector2f scale, sf::Vector2f direction) : mBombTexture(texture), mBombScale(scale), mBombDirection(direction)
{
	this->mBombPosition.x = x;
	this->mBombPosition.y = y;

	this->mBombSprite.setScale(scale);
	this->mBombSprite.setPosition(x, y);
}

Projectile::~Projectile() = default;

void Projectile::render(sf::RenderTarget& target)
{
	target.draw(this->mProjectileSprite);
}

void Projectile::update(const float& deltaTime)
{
	this->movement(deltaTime);
}

void Projectile::movement(const float& deltaTime)
{
	this->mProjectileSprite.move(8.F * deltaTime * 62.5F, 0.F);

	this->mBombSprite.move(0.F, 8.F * deltaTime * 62.5F);
}

void Projectile::createMuzzleFlash(sf::Texture& texture, sf::Sprite& sprite, sf::Vector2f position, float timerMax)
{
	this->mMuzzleTexture = texture;
	this->mMuzzleSprite = sprite;
	this->mTimerMax = timerMax;
	this->mPosition = position;

	this->mMuzzleSprite.setTexture(texture);
	this->mMuzzleSprite.setPosition(position);
}

void Projectile::createBomb(sf::Texture* texture, sf::Vector2f position, sf::Vector2f scale, sf::Vector2f direction)
{
	this->mBombTexture = texture;
	this->mBombSprite.setTexture(*this->mBombTexture);

	this->mBombPosition = position;
	this->mBombDirection = direction;
	this->mBombScale = scale;

	this->mBombSprite.setScale(scale);
	this->mBombSprite.setPosition(position);
}


