#include "stdafx.hpp"
#include "Entity.hpp"

Entity::Entity() : pIsFrozen(false), pIsEnemyAlive(true), pIsPlayerAlive(true)
{
}

Entity::~Entity() = default;

void Entity::updateAttacks(bool isAttacking)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		isAttacking = true;
}

void Entity::animations(sf::Sprite& sprite, const float& deltaTime, bool isAttacking)
{
	if (isAttacking)
	{
		if (sprite.getScale().x > 0.f) //Facing left
			sprite.setOrigin(96.f, 0.f);

		if (this->pAnimationComponent->play("ATTACK", deltaTime, true))
		{
			isAttacking = false;

			if (sprite.getScale().x > 0.f) //Facing left
				sprite.setOrigin(0.f, 0.f);
		}
	}

	else if (this->pMovementComponent->getState(MOVING))
	{
		if (sprite.getScale().x < 0.f)
		{
			sprite.setOrigin(0.f, 0.f);
			sprite.setScale(1.f, 1.f);
		}

		this->pAnimationComponent->play("MOVE", deltaTime,
			this->pMovementComponent->getVelocity().x,
			this->pMovementComponent->getMaxVelocity());
	}

	else if (this->pMovementComponent->getState(MOVING))
	{
		if (sprite.getScale().x < 0.f)
		{
			sprite.setOrigin(140.f, 0.f);
			sprite.setScale(1.f, 1.f);
		}

		this->pAnimationComponent->play("MOVE", deltaTime,
			this->pMovementComponent->getVelocity().x,
			this->pMovementComponent->getMaxVelocity());
	}
}

int Entity::frozen()
{
	this->pFrozen = rand() % 10;

	if (this->pFrozen <= 5)
		this->pIsFrozen = true;

	else if (this->pFrozen > 5)
		this->pIsFrozen = false;

	return this->pFrozen;
}