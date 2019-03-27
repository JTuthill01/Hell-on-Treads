#include "stdafx.hpp"
#include "Entity.hpp"

Entity::Entity()
{
	pDamage = 2;
	pDamageMax = 6;

	pIsFrozen = false;
}

Entity::~Entity() = default;

const int Entity::playerDamage() const
{
	int damage = 0;
	srand(time(0));

	switch (this->pCurrentWeapon)
	{
	case FIRE:
		damage = rand() % this->pDamage + this->pDamageMax;
		break;

	case ICE:
		damage = rand() % this->pDamage + this->pDamageMax;
		damage *= 2;

		break;

	case CORROSIVE:
		damage = rand() % this->pDamage + this->pDamageMax;
		damage *= rand() % 3;

	default:
		break;
	}

	return damage;
}

const int Entity::enemyDamage() const
{
	return 0;
}

int Entity::frozen()
{
	this->pFrozen = rand() % 10;

	if (this->pFrozen <= 5)
		this->pIsFrozen = true;

	else if(this->pFrozen > 5)
		this->pIsFrozen = false;


	return this->pIsFrozen;
}
