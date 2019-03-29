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

	virtual void upDateAttacks(bool isAttacking);
	virtual void animations(sf::Sprite& sprite, const float& deltaTime, bool isAttacking = false);
	int frozen();

	inline const bool getCorrosive() { return this->pIsCorrosiveShot; }
	inline const bool getFire() { return this->pIsFireShot; }
	inline const bool getFrozen() { return this->pIsIceShot; }

protected:
	MovementComponent* pMovementComponent;
	AnimationComponent* pAnimationComponent;
	Audio pAudio;
	Projectile pMuzzle;

	//Weapons
	int pCurrentWeapon;
	bool pIsFireShot;
	bool pIsIceShot;
	bool pIsFrozen;
	bool pIsCorrosiveShot;

	//Damage
	int pFrozen;

	int mEnemeyType;
};