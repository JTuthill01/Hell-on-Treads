#include "stdafx.hpp"
#include "EnemyTank.hpp"

std::vector<sf::Texture> EnemyTank::mEnemyProjectileTextures;

EnemyTank::EnemyTank() : mShootTimer(mShootTimerMax), mShootTimerMax(50.F)
{
	this->createMovementComponent(350.f, 16.f, 6.f);
	this->createAnimationComponent(this->mEnemyTankTexture);

	this->pAnimationComponent->addAnimation("MOVE", 5.f, 0, 0, 7, 0, 435, 435);
	this->pAnimationComponent->addAnimation("ATTACK", 5.f, 0, 1, 7, 1, 435, 435);

	this->pAudio.loadSound("Resources/Sounds/Explosions/explosion1.ogg");
	this->pAudio.setVolume(10.F);

	this->loadProjectile();

	this->loadEnemyTanks();
}

EnemyTank::~EnemyTank() = default;

void EnemyTank::render(sf::RenderTarget& target)
{
	target.draw(this->mEnemyTankSprite);

	for (size_t i = 0; i < this->mProjectiles.size(); i++)
		this->mProjectiles[i].render(target);
}

void EnemyTank::update(const float& deltaTime)
{
	this->pMovementComponent->update(deltaTime);

	this->updateAnimations(deltaTime);

	this->mEnemyTankSprite.move(sf::Vector2f(-0.6F, 0.F));

	for (size_t i = 0; i < this->mProjectiles.size(); i++)
		this->mProjectiles[i].update(deltaTime);
}

void EnemyTank::move(const float direction_x, const float direction_y, const float& deltaTime)
{
	if (this->pMovementComponent)
		this->pMovementComponent->move(direction_x, direction_y, deltaTime);
}

void EnemyTank::updateAnimations(const float& deltaTime)
{
	if (this->mIsAttacking)
	{
		if (this->mEnemyTankSprite.getScale().x > 0.f) //Facing left
			this->mEnemyTankSprite.setOrigin(96.f, 0.f);

		if (this->pAnimationComponent->play("ATTACK", deltaTime, true))
		{
			this->mIsAttacking = false;

			this->mIsMuzzleOn = false;

			if (this->mEnemyTankSprite.getScale().x > 0.f) //Facing left
				this->mEnemyTankSprite.setOrigin(0.f, 0.f);
		}
	}

	else if (this->pMovementComponent->getState(MOVING))
	{
		if (this->mEnemyTankSprite.getScale().x < 0.f)
		{
			this->mEnemyTankSprite.setOrigin(0.f, 0.f);
			this->mEnemyTankSprite.setScale(1.f, 1.f);
		}

		this->pAnimationComponent->play("MOVE", deltaTime,
			this->pMovementComponent->getVelocity().x,
			this->pMovementComponent->getMaxVelocity());
	}

	else if (this->pMovementComponent->getState(MOVING))
	{
		if (this->mEnemyTankSprite.getScale().x < 0.f)
		{
			this->mEnemyTankSprite.setOrigin(140.f, 0.f);
			this->mEnemyTankSprite.setScale(1.f, 1.f);
		}

		this->pAnimationComponent->play("MOVE", deltaTime,
			this->pMovementComponent->getVelocity().x,
			this->pMovementComponent->getMaxVelocity());
	}
}

void EnemyTank::createMovementComponent(const float max_velocity, const float acceleration, const float deceleration)
{
	this->pMovementComponent = new MovementComponent(this->mEnemyTankSprite, max_velocity, acceleration,
		deceleration);
}

void EnemyTank::createAnimationComponent(sf::Texture& texture_sheet)
{
	this->pAnimationComponent = new AnimationComponent(this->mEnemyTankSprite, texture_sheet);
}

void EnemyTank::loadEnemyTanks()
{
	if (!this->mEnemyTankTexture.loadFromFile("Resources/Textures/Enemy/TankCombined.png"))
		std::cerr << "Fucking EnemyTank failed to fucking load" << "\n";

	this->mEnemyTankSprite.setTexture(this->mEnemyTankTexture);
	this->mEnemyTankSprite.setPosition(1700.F, 710.F);
	this->mEnemyTankSprite.setColor(sf::Color::Green);
}

void EnemyTank::loadProjectile()
{
	sf::Texture temp;
	if (!temp.loadFromFile("Resources/Textures/Bullets/Bomb_2.png"))
		std::cerr << "Failed to fucking the fucking bomb texture" << "\n";

	this->mEnemyProjectileTextures.push_back(temp);
}

void EnemyTank::updateAttack(const float& deltaTime)
{
	
}
