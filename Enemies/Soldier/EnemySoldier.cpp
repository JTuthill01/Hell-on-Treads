#include "stdafx.hpp"
#include "EnemySoldier.hpp"

EnemySoldier::EnemySoldier() : mHp(mHpMax), mHpMax(4)
{
	this->createMovementComponent(350.f, 16.f, 6.f);
	this->createAnimationComponent(this->mEnemySoldierTexture);

	this->pAnimationComponent->addAnimation("MOVE", 5.f, 0, 0, 7, 0, 140, 140);

	this->load();
}

EnemySoldier::~EnemySoldier() = default;

void EnemySoldier::update(const float& deltaTime)
{
	this->pMovementComponent->update(deltaTime);

	this->animations(this->mEnemySoldierSprite, deltaTime);

	this->mEnemySoldierSprite.move(-1.F, 0.F);
}

void EnemySoldier::render(sf::RenderTarget& target)
{
	target.draw(this->mEnemySoldierSprite);
}

void EnemySoldier::move(const float direction_x, const float direction_y, const float& deltaTime)
{
	if (this->pMovementComponent)
		this->pMovementComponent->move(direction_x, direction_y, deltaTime);
}

void EnemySoldier::takeDamage(int damage)
{
	this->mHp -= damage;

	if (this->mHp <= 0)
	{
		this->mHp = 0;

		this->mIsAlive = false;
	}
}

void EnemySoldier::load()
{
	if (!this->mEnemySoldierTexture.loadFromFile("Resources/Textures/Enemy/EnemySolider.png"))
		std::cerr << "Fuck enemy soldier didn't load" << "\n";

	this->mEnemySoldierSprite.setTexture(this->mEnemySoldierTexture);

	this->mEnemySoldierSprite.setPosition(1700.F, 810.F);

	this->mEnemySoldierSprite.setScale(1.6F, 1.6F);
}

void EnemySoldier::createMovementComponent(const float max_velocity, const float acceleration, const float deceleration)
{
	this->pMovementComponent = new MovementComponent(this->mEnemySoldierSprite, max_velocity, acceleration,
		deceleration);
}

void EnemySoldier::createAnimationComponent(sf::Texture& texture_sheet)
{
	this->pAnimationComponent = new AnimationComponent(this->mEnemySoldierSprite, texture_sheet);
}