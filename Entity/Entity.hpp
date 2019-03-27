#pragma once
#include <Resources/Audio.hpp>
#include <Component/AnimationComponent.hpp>
#include <Component/MovementComponent.hpp>
#include <Projectile/Projectile.hpp>

enum projectileType { REGULAR = 0, FIRE, ICE, CORROSIVE };

enum enemyType { ENEMY_REGULAR = 0, ENEMY_FIRE, ENEMY_ICE, ENEMY_CORROSIVE };

class Entity
{
public:
	Entity();
	~Entity();

	const int playerDamage() const;
	const int enemyDamage() const;
	int frozen();

	inline const bool getCorrosive() { return this->pIsCorrosiveShot; }
	inline const bool getFire() { return this->pIsFireShot; }
	inline const bool getFrozen() { return this->pIsIceShot; }

protected:
	//Weapons
	int pCurrentWeapon;
	bool pIsFireShot;
	bool pIsIceShot;
	bool pIsFrozen;
	bool pIsCorrosiveShot;

	//Damage
	int pHp;
	int pHpMax;
	int pDamage;
	int pDamageMax;
	int pFrozen;
};

