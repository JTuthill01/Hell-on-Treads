#include "stdafx.hpp"
#include "Plane.hpp"

std::vector<sf::Texture>Plane::mPlayerPlaneWeapons;

Plane::Plane() : mIsAttacking(false), mIsExploded(false)
{
	this->createMovementComponent(350.f, 16.f, 6.f);
	this->createAnimationComponent(this->mPlanePlayerTexture);

	this->pAnimationComponent->addAnimation("MOVE", 5.f, 0, 0, 1, 0, 256, 256);

	this->loadPlayerPlane();

	this->loadProjectile();

	this->mPlayerPlaneCurrentWeapon = MISSILE;

	this->startTimer();

	this->bombExplosoion();
}

Plane::~Plane() = default;

void Plane::removeProjectile(unsigned int index)
{
	this->mPlayerPlaneProjectile.erase(this->mPlayerPlaneProjectile.begin() + index);
}

void Plane::move(const float direction_x, const float direction_y, const float& deltaTime)
{
	if (this->pMovementComponent)
		this->pMovementComponent->move(direction_x, direction_y, deltaTime);
}

void Plane::render(sf::RenderTarget & target)
{
	target.draw(this->mPlanePlayerSprite);

	if (this->mIsExploded)
		target.draw(this->mBombExplosionSprite);

	for (size_t i = 0; i < this->mPlayerPlaneProjectile.size(); i++)
		this->mPlayerPlaneProjectile[i].render(target);
}

void Plane::update(const float& deltaTime)
{
	this->pMovementComponent->update(deltaTime);

	this->attack(deltaTime);

	for (size_t i = 0; i < this->mPlayerPlaneProjectile.size(); i++)
		this->mPlayerPlaneProjectile[i].update(deltaTime);
}

void Plane::setExplosionPosition(sf::Vector2f position) { this->mBombExplosionSprite.setPosition(position); }

Projectile& Plane::getBomb(unsigned index) { return this->mPlayerPlaneProjectile[index]; }

void Plane::bombExplosoion()
{
	if (!this->mBombExplosionTexture.loadFromFile("Resources/Textures/Explosions/explosion2.png"))
		std::cerr << "Fucking explosion failed to load" << "\n";

	this->mBombExplosionSprite.setTexture(this->mBombExplosionTexture);
	this->mBombExplosionSprite.setScale(1.5F, 1.5F);
}

void Plane::attack(const float& deltaTime)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && this->mKeyTimer.isExpired())
	{
		this->currentWeapon(deltaTime);

		this->mIsAttacking = true;

		this->mKeyTimer.restart(sf::seconds(0.78F));
	}
}

void Plane::loadProjectile()
{
	sf::Texture temp;
	if (!temp.loadFromFile("Resources/Textures/Bullets/missile(2).png"))
		std::cerr << "Missile failed to load" << "\n";

	this->mPlayerPlaneWeapons.push_back(temp);

	if (!temp.loadFromFile("Resources/Textures/Bullets/Bomb_2.png"))
		std::cerr << "Bomb failed to load" << "\n";

	this->mPlayerPlaneWeapons.push_back(temp);
}

void Plane::currentWeapon(const float& deltaTime)
{
	if (this->mPlayerPlaneCurrentWeapon == MISSILE)
		this->mPlayerPlaneProjectile.push_back(Projectile(&Plane::mPlayerPlaneWeapons[MISSILE], sf::Vector2f(this->mPlanePlayerSprite.getPosition().x + 300,
			this->mPlanePlayerSprite.getPosition().y + 100), sf::Vector2f(-0.1F, 0.1F), sf::Vector2f(1.F, 0.F)));

	else if (this->mPlayerPlaneCurrentWeapon == BOMB)
	{
		Projectile mTempProjectile;

		mTempProjectile.createBomb(&Plane::mPlayerPlaneWeapons[BOMB], sf::Vector2f(this->mPlanePlayerSprite.getPosition().x + 150,
			this->mPlanePlayerSprite.getPosition().y + 200), sf::Vector2f(-0.2F, 0.2F), sf::Vector2f(1.F, 0.F));

		this->mPlayerPlaneProjectile.push_back(mTempProjectile);
	}
}

void Plane::loadPlayerPlane()
{
	if (!this->mPlanePlayerTexture.loadFromFile("Resources/Textures/Player/Plane/Player_Plane.png"))
		std::cerr << "Whelp you can just fuck right off, as the plane texture does not exist" << "\n";

	this->mPlanePlayerSprite.setTexture(this->mPlanePlayerTexture);
	this->mPlanePlayerSprite.setPosition(0.F, 400.F);
	this->mPlanePlayerSprite.setScale(1.2F, 1.2F);
}

void Plane::createMovementComponent(const float max_velocity, const float acceleration, const float deceleration)
{
	this->pMovementComponent = new MovementComponent(this->mPlanePlayerSprite, max_velocity, acceleration,
		deceleration);
}

void Plane::createAnimationComponent(sf::Texture & texture_sheet)
{
	this->pAnimationComponent = new AnimationComponent(this->mPlanePlayerSprite, texture_sheet);
}

void Plane::startTimer() { this->mKeyTimer.start(); }