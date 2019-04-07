#include "stdafx.hpp"
#include "Enemy.hpp"

std::vector<sf::Texture>Enemy::mEnemyPlaneWeapons;
std::vector<sf::Texture> Enemy::mEnemyProjectileTextures;

Enemy::Enemy() : mShootTimer(mShootTimerMax), mShootTimerMax(50.F), mIsAlive(true)
{
	this->initVariables();

	this->createMovementComponent(350.f, 16.f, 6.f);
	this->createAnimationComponent(this->mEnemyTankTexture);

	this->pAnimationComponent->addAnimation("MOVE", 5.f, 0, 0, 7, 0, 435, 435);
	this->pAnimationComponent->addAnimation("ATTACK", 5.f, 0, 1, 7, 1, 435, 435);
	this->pAnimationComponent->addAnimation("DEAD", 5.f, 0, 2, 10, 2, 435, 435);

	this->pAudio.loadSound("Resources/Sounds/Explosions/explosion1.ogg");
	this->pAudio.setVolume(10.F);

	this->loadProjectile();

	this->loadEnemyTanks();

	this->pAudio.loadSound("Resources/Sounds/Explosions/DeathFlash.flac");
	this->pAudio.setVolume(20.F);

	this->mEnemyType = ENEMY_REGULAR;

	this->mHp = 20;
	this->mHpMax = 20;

	this->mDamageMax = 4;
	this->mDamage = 2;
}

Enemy::Enemy(std::vector<sf::Texture>& textures, std::vector<sf::Texture>& bulletTextures, sf::Vector2u windowBounds, sf::Vector2f position, sf::Vector2f moveDirection, int type)
{
	this->mTextures = &textures;

	this->mEnemyProjectiles = &bulletTextures;

	this->mPosition = position;

	this->mMoveDirection = moveDirection;

	this->mEnemyPlaneType = type;

	this->mWindowBounds = windowBounds;

	this->mEnemyPlaneSprites.setTexture((*this->mTextures)[this->mEnemyPlaneType]);

	this->mEnemyPlaneSprites.setPosition(windowBounds.x, (rand() % windowBounds.y) - this->mEnemyPlaneSprites.getGlobalBounds().height);

	this->loadPlaneProjectiles();
}

Enemy::~Enemy() = default;

Projectile& Enemy::getEnemyTankProjectile(unsigned index) { return this->mEnemyTankProjectile[index]; }

sf::Vector2f Enemy::getEnemyTankSpriteCenter()
{
	this->mEnemyTankSpriteCenter.x = this->mEnemyTankSprite.getGlobalBounds().left + this->mEnemyTankSprite.getGlobalBounds().width / 2;
	this->mEnemyTankSpriteCenter.y = this->mEnemyTankSprite.getGlobalBounds().top + this->mEnemyTankSprite.getGlobalBounds().height / 2;

	return this->mEnemyTankSpriteCenter;
}

Projectile& Enemy::getEnemyPlaneProjectile(unsigned index) { return this->mEnemyPlaneProjectile[index]; }

const int Enemy::enemyDealDamage() const
{
	int damage = 0;

	switch (this->mEnemyType)
	{
	case ENEMY_REGULAR:
		damage = rand() % this->mDamage + this->mDamageMax;
		break;

	case ENEMY_FIRE:
		damage = rand() % this->mDamage + this->mDamageMax - 2;
		break;

	case ENEMY_ICE:
		damage = damage = rand() % this->mDamage + this->mDamageMax - 1;
		break;

	case ENEMY_CORROSIVE:
		damage = rand() % this->mDamage + this->mDamageMax;
		break;

	default:
		break;
	}

	return damage;
}

void Enemy::removeEnemyTankProjectile(unsigned index) { this->mEnemyTankProjectile.erase(this->mEnemyTankProjectile.begin() + index); }

void Enemy::removeEnemyPlaneProjectile(unsigned index) { this->mEnemyPlaneProjectile.erase(this->mEnemyPlaneProjectile.begin() + index); }

void Enemy::takeDamage(int damage)
{
	this->mHp -= damage;

	if (this->mHp <= 0)
	{
		this->mHp = 0;

		this->mIsAlive = false;
	}
}

void Enemy::render(sf::RenderTarget & target)
{
	target.draw(this->mEnemyTankSprite);

	target.draw(this->mEnemyPlaneSprites);

	if (this->mIsMuzzleOn)
		this->pMuzzle.render(target);

	for (size_t i = 0; i < this->mEnemyTankProjectile.size(); i++)
		this->mEnemyTankProjectile[i].render(target);
}

void Enemy::update(const float& deltaTime)
{
	this->mShootTimer += 1.F * deltaTime * 62.5F;

	this->pMovementComponent->update(deltaTime);

	this->updateAnimations(deltaTime);

	this->updateAttack(deltaTime);

	this->mEnemyPlaneSprites.move(-0.8F, 0.F);

	if (!this->pIsFrozen)
		this->mEnemyTankSprite.move(sf::Vector2f(-0.6F, 0.F));

	else if (this->pIsFrozen)
		this->mEnemyTankSprite.move(sf::Vector2f(0.F, 0.F));

	for (size_t i = 0; i < this->mEnemyTankProjectile.size(); i++)
		this->mEnemyTankProjectile[i].update(deltaTime);

	for (size_t i = 0; i < this->mEnemyPlaneProjectile.size(); i++)
		this->mEnemyPlaneProjectile[i].update(deltaTime);
}

void Enemy::move(const float direction_x, const float direction_y, const float& deltaTime)
{
	if (this->pMovementComponent)
		this->pMovementComponent->move(direction_x, direction_y, deltaTime);
}

void Enemy::updateAnimations(const float& deltaTime)
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

	this->enemyDead(deltaTime);
}

void Enemy::enemyPlaneAttack()
{
	if (this->mEnemyPlaneType == PLANE)
		this->mEnemyPlaneProjectile.push_back(Projectile(&Enemy::mEnemyPlaneWeapons[PLANE_MISSILE], this->mEnemyPlaneSprites.getPosition().x - 200,
			this->mEnemyPlaneSprites.getPosition().y + 100, sf::Vector2f(-0.1F, 0.1F), sf::Vector2f(-1.F, 0.F)));

	else if (this->mEnemyPlaneType == BOMBER)
	{
		Projectile mTempProjectiles;

		mTempProjectiles.createBomb(&Enemy::mEnemyPlaneWeapons[PLANE_BOMB], sf::Vector2f(this->mEnemyPlaneSprites.getPosition().x - 150,
			this->mEnemyPlaneSprites.getPosition().y + 200), sf::Vector2f(-0.2F, 0.2F), sf::Vector2f(-1.F, 0.F));

		this->mEnemyPlaneProjectile.push_back(mTempProjectiles);
	}
}

void Enemy::loadPlaneProjectiles()
{
	sf::Texture temp;
	if (!temp.loadFromFile("Resources/Textures/Bullets/missile(2).png"))
		std::cerr << "Missile failed to load" << "\n";

	this->mEnemyPlaneWeapons.push_back(temp);

	if (!temp.loadFromFile("Resources/Textures/Bullets/Bomb_2.png"))
		std::cerr << "Bomb failed to load" << "\n";

	this->mEnemyPlaneWeapons.push_back(temp);
}

void Enemy::hpBar()
{
}

void Enemy::enemyDead(const float& deltaTime)
{
	if (!this->mIsAlive)
	{
		this->createMovementComponent(0.F, 0.F, 0.F);

		this->pAnimationComponent->play("DEAD", deltaTime, true);

		this->mIsAttacking = false;

		this->mIsMuzzleOn = false;

		this->pAudio.stop();

		this->mEnemyTankSprite.move(0.F, 0.F);
	}
}

void Enemy::enemyShoot()
{
	if (this->mEnemyType == ENEMY_REGULAR)
	{
		this->mEnemyTankProjectile.push_back(Projectile(&Enemy::mEnemyProjectileTextures[ENEMY_REGULAR], this->mEnemyTankSprite.getPosition().x + 150, this->mEnemyTankSprite.getPosition().y + 180,
			sf::Vector2f(0.4F, 0.5F), sf::Vector2f(1.F, 0.F)));

		this->mEnemyTankSprite.setColor(sf::Color(222, 184, 135, 255));
	}

	else if (this->mEnemyType == ENEMY_FIRE)
	{
		this->mEnemyTankProjectile.push_back(Projectile(&Enemy::mEnemyProjectileTextures[ENEMY_FIRE], this->mEnemyTankSprite.getPosition().x + 150, this->mEnemyTankSprite.getPosition().y + 180,
			sf::Vector2f(0.4F, 0.5F), sf::Vector2f(1.F, 0.F)));

		this->mEnemyTankSprite.setColor(sf::Color(178, 34, 34, 255));
	}

	else if (this->mEnemyType == ENEMY_ICE)
	{
		this->mEnemyTankProjectile.push_back(Projectile(&Enemy::mEnemyProjectileTextures[ENEMY_ICE], this->mEnemyTankSprite.getPosition().x + 150, this->mEnemyTankSprite.getPosition().y + 180,
			sf::Vector2f(0.4F, 0.5F), sf::Vector2f(1.F, 0.F)));

		this->mEnemyTankSprite.setColor(sf::Color(0, 191, 255, 255));
	}

	else if (this->mEnemyType == ENEMY_CORROSIVE)
	{
		this->mEnemyTankProjectile.push_back(Projectile(&Enemy::mEnemyProjectileTextures[ENEMY_CORROSIVE], this->mEnemyTankSprite.getPosition().x + 150, this->mEnemyTankSprite.getPosition().y + 180,
			sf::Vector2f(0.4F, 0.5F), sf::Vector2f(1.F, 0.F)));

		this->mEnemyTankSprite.setColor(sf::Color(0, 255, 0, 255));
	}

	this->mShootTimer = 0.F;
}

void Enemy::initVariables()
{
	this->mIsAttacking = false;

	this->mShootTimerMax = 250.F;

	this->mShootTimer = this->mShootTimerMax;

	this->mIsMuzzleOn = false;

	this->mMuzzleTimerMax = 1.5F;

	this->mMuzzleTimer = this->mMuzzleTimerMax;
}

void Enemy::createMovementComponent(const float max_velocity, const float acceleration, const float deceleration)
{
	this->pMovementComponent = new MovementComponent(this->mEnemyTankSprite, max_velocity, acceleration,
		deceleration);
}

void Enemy::createAnimationComponent(sf::Texture & texture_sheet)
{
	this->pAnimationComponent = new AnimationComponent(this->mEnemyTankSprite, texture_sheet);
}

void Enemy::loadEnemyTanks()
{
	if (!this->mEnemyTankTexture.loadFromFile("Resources/Textures/Enemy/TankCombined.png"))
		std::cerr << "Fucking EnemyTank failed to fucking load" << "\n";

	this->mEnemyTankSprite.setTexture(this->mEnemyTankTexture);
	this->mEnemyTankSprite.setPosition(1700.F, 710.F);
}

void Enemy::loadProjectile()
{
	sf::Texture temp;
	if (!temp.loadFromFile("Resources/Textures/Bullets/Bullet_3.png"))
		std::cerr << "Failed to fucking the fucking bomb texture" << "\n";

	this->mEnemyProjectileTextures.push_back(temp);

	//if (!temp.loadFromFile("Resources/Textures/Bullets/Fire1.png"))
	//	std::cerr << "The fucking bullet texture failed to fucking load" << "\n";

	//this->mEnemyProjectileTextures.push_back(temp);

	//if (!temp.loadFromFile("Resources/Textures/Bullets/Ice1.png"))
	//	std::cerr << "The fucking bullet texture failed to fucking load" << "\n";

	//this->mEnemyProjectileTextures.push_back(temp);

	//if (!temp.loadFromFile("Resources/Textures/Bullets/Corrosive1.png"))
	//	std::cerr << "The fucking bullet texture failed to fucking load" << "\n";

	//this->mEnemyProjectileTextures.push_back(temp);
}

void Enemy::updateAttack(const float& deltaTime)
{
	if (this->mShootTimer >= this->mShootTimerMax && this->mIsAlive)
	{
		this->mIsAttacking = true;

		this->mIsMuzzleOn = true;

		this->enemyShoot();

		this->pAudio.play();

		this->pMuzzle.createMuzzleFlash(sf::Vector2f(this->mEnemyTankSprite.getPosition().x - 350, this->mEnemyTankSprite.getPosition().y), sf::Vector2f(0.6F, 0.6F));
	}
}