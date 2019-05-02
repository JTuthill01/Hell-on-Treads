#include "stdafx.hpp"
#include "Enemy.hpp"

std::vector<sf::Texture> Enemy::mEnemyPlaneWeapons;
std::vector<sf::Texture> Enemy::mEnemyProjectileTextures;
std::vector<sf::Texture> Enemy::mEnemyJeepWeapons;
std::vector<sf::Texture> Enemy::mEnemyTankWeapons;

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

Enemy::Enemy(std::vector<sf::Texture>& textures, std::vector<sf::Texture>& projectileTextures, sf::Vector2f position, sf::Vector2f moveDirection, int type)
{
	this->pEnemyPlaneShootTimer.restart(sf::seconds(0.8F));

	this->mTextures = &textures;

	this->mEnemyProjectiles = &projectileTextures;

	this->mPosition = position;

	this->mMoveDirection = moveDirection;

	this->mEnemyPlaneType = type;

	this->mEnemyPlaneSprites.setTexture((*this->mTextures)[this->mEnemyPlaneType]);

	this->mEnemyPlaneSprites.setPosition(position);

	this->mEnemyPlaneSprites.setScale(-1.F, 1.F);

	this->mPlaneHp = 10;
	this->mPlaneHpMax = 15;

	this->mPlaneDamage = 2;
	this->mPlaneDamageMax = 4;

	this->mIsPlaneAlive = true;
}

Enemy::~Enemy() = default;

const std::vector<Projectile>& Enemy::getProjectiles() { return this->mEnemyPlaneProjectile; }

Projectile& Enemy::getEnemyTankProjectile(unsigned index) { return this->mEnemyTankProjectile[index]; }

sf::Vector2f Enemy::getEnemyTankSpriteCenter()
{
	this->mEnemyTankSpriteCenter.x = this->mEnemyTankSprite.getGlobalBounds().left + this->mEnemyTankSprite.getGlobalBounds().width / 2;
	this->mEnemyTankSpriteCenter.y = this->mEnemyTankSprite.getGlobalBounds().top + this->mEnemyTankSprite.getGlobalBounds().height / 2;

	return this->mEnemyTankSpriteCenter;
}

Projectile& Enemy::getEnemyPlaneProjectile(unsigned index) { return this->mEnemyPlaneProjectile[index]; }

Projectile& Enemy::getEnemyJeepProjectile(unsigned index) { return this->mEnemyJeepProjectiles[index]; }

const int Enemy::enemyDealDamage() const
{
	int damage = 0;

	switch (this->mEnemyType)
	{
	case ENEMY_REGULAR:
		damage = rand() % this->mDamage - 6;
		break;

	case ENEMY_FIRE:
		damage = rand() % this->mDamage + this->mDamageMax + 2;
		break;

	case ENEMY_ICE:
		damage = damage = rand() % this->mDamage + this->mDamageMax + 1;
		break;

	case ENEMY_CORROSIVE:
		damage = rand() % this->mDamage + this->mDamageMax;
		break;

	default:
		break;
	}

	return damage;
}

const int Enemy::enemyPlaneDealDamage() const
{
	int damage = 0;

	switch (this->mEnemyPlaneType)
	{
	case PLANE:
		damage = std::rand() % this->mPlaneDamage + this->mPlaneDamageMax - 3;
		break;

	case BOMBER:
		damage = rand() % this->mPlaneDamage + this->mPlaneDamageMax + 6;
		break;

	default:
		break;
	}

	return damage;
}

void Enemy::renderJeep(sf::RenderTarget & target)
{
	target.draw(this->mEnemyJeepSprites);

	for (size_t i = 0; i < this->mEnemyJeepProjectiles.size(); i++)
		this->mEnemyJeepProjectiles[i].render(target);
}

void Enemy::updateJeep(const float& deltaTime)
{
	this->mEnemyJeepSprites.move(-1.F, 0.F);

	if (this->pEnemyJeepShootTimer.isExpired())
	{
		this->enemyJeepAttack();

		this->pEnemyJeepShootTimer.restart(sf::seconds(2.5F));
	}

	for (size_t i = 0; i < this->mEnemyJeepProjectiles.size(); i++)
		this->mEnemyJeepProjectiles[i].update(deltaTime);
}

void Enemy::createTanks(std::vector<sf::Texture> & textures, std::vector<sf::Texture>& projectileTextures, sf::Vector2f position, sf::Vector2f moveDirection, int type)
{
	this->pIsAttacking = false;

	this->createMovementComponent(350.f, 16.f, 6.f);
	this->createAnimationComponent((*this->mTankTextures)[this->mEnemyTankType]);

	this->pAnimationComponent->addAnimation("MOVE", 5.f, 0, 0, 7, 0, 435, 435);
	this->pAnimationComponent->addAnimation("ATTACK", 5.f, 0, 1, 7, 1, 435, 435);
	this->pAnimationComponent->addAnimation("DEAD", 5.f, 0, 2, 10, 2, 435, 435);

	this->pEnemyTankShootTimer.restart(sf::seconds(0.8F));

	this->mTankTextures = &textures;

	this->mEnemyProjectile = &projectileTextures;

	this->mPosition = position;

	this->mMoveDirection = moveDirection;

	this->mEnemyTankType = type;

	this->mEnemyTankSprites.setTexture((*this->mTankTextures)[this->mEnemyTankType]);

	this->mEnemyTankSprites.setPosition(position);

	this->mEnemyTankSprites.setScale(-1.F, 1.F);

	this->loadProjectile();
}

void Enemy::createJeep(std::vector<sf::Texture> & textures, sf::Vector2f position, sf::Vector2f moveDirection)
{
	this->pIsAttacking = false;

	this->pEnemyJeepShootTimer.restart(sf::seconds(0.2F));

	this->mJeepTextures = &textures;

	this->mJeepPosition = position;

	this->mJeepMovement = moveDirection;

	this->mEnemyJeepSprites.setTexture((*this->mJeepTextures)[0]);

	this->mEnemyJeepSprites.setPosition(position);

	this->loadJeepProjectiles();
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

void Enemy::planeTakeDamage(int damage)
{
	this->mPlaneHp -= damage;

	if (this->mPlaneHp <= 0)
	{
		this->mPlaneHp = 0;

		this->mIsPlaneAlive = false;
	}
}

void Enemy::renderPlane(sf::RenderTarget & target)
{
	target.draw(this->mEnemyPlaneSprites);

	for (size_t i = 0; i < this->mEnemyPlaneProjectile.size(); i++)
		this->mEnemyPlaneProjectile[i].render(target);
}

void Enemy::renderTank(sf::RenderTarget & target)
{
	target.draw(this->mEnemyTankSprites);

	for (size_t i = 0; i < this->mEnemyTankProjectiles.size(); i++)
		this->mEnemyTankProjectiles[i].render(target);
}

void Enemy::updateTank(const float& deltaTime)
{
	this->pMovementComponent->update(deltaTime);

	this->updateAnimations(deltaTime);

	this->animations(this->mEnemyTankSprites, deltaTime, pIsAttacking);

	if (this->pEnemyTankShootTimer.isExpired())
	{
		this->pIsAttacking = true;

		this->enemyShoot();

		this->pEnemyTankShootTimer.restart(sf::seconds(0.8F));
	}

	for (size_t i = 0; i < this->mEnemyPlaneProjectile.size(); i++)
		this->mEnemyPlaneProjectile[i].update(deltaTime);
}

void Enemy::render(sf::RenderTarget & target)
{
	target.draw(this->mEnemyTankSprite);

	if (this->mIsMuzzleOn)
		this->pMuzzle.render(target);

	for (size_t i = 0; i < this->mEnemyTankProjectile.size(); i++)
		this->mEnemyTankProjectile[i].render(target);
}

void Enemy::update(const float& deltaTime)
{
	this->mShootTimer += 1.F * deltaTime * 62.5F;

	this->updateAttack(deltaTime);

	this->pMovementComponent->update(deltaTime);

	this->updateAnimations(deltaTime);

	if (!this->pIsFrozen)
		this->mEnemyTankSprite.move(sf::Vector2f(-0.6F, 0.F));

	else if (this->pIsFrozen)
		this->mEnemyTankSprite.move(sf::Vector2f(0.F, 0.F));

	for (size_t i = 0; i < this->mEnemyTankProjectile.size(); i++)
		this->mEnemyTankProjectile[i].update(deltaTime);
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

void Enemy::updatePlane(const float& deltaTime)
{
	this->mEnemyPlaneSprites.move(-0.9F, 0.F);

	if (pEnemyPlaneShootTimer.isExpired())
	{
		this->enemyPlaneAttack();

		this->pEnemyPlaneShootTimer.restart(sf::seconds(2.5F));
	}

	for (size_t i = 0; i < this->mEnemyPlaneProjectile.size(); i++)
		this->mEnemyPlaneProjectile[i].update(deltaTime);
}

void Enemy::enemyPlaneAttack()
{
	if (this->mEnemyPlaneWeapons.empty())
		this->loadPlaneProjectiles();

	if (this->mEnemyPlaneType == PLANE)
	{
		Projectile mTempProjectile;

		mTempProjectile.createEnemyPlaneMissile(&this->mEnemyPlaneWeapons[PLANE_MISSILE], sf::Vector2f(this->mEnemyPlaneSprites.getPosition().x - 100,
			this->mEnemyPlaneSprites.getPosition().y + 100), sf::Vector2f(-0.1F, 0.1F), sf::Vector2f(-1.F, 0.F));

		this->mEnemyPlaneProjectile.push_back(mTempProjectile);
	}

	else if (this->mEnemyPlaneType == BOMBER)
	{
		Projectile mTempProjectiles;

		mTempProjectiles.createBomb(&Enemy::mEnemyPlaneWeapons[PLANE_BOMB], sf::Vector2f(this->mEnemyPlaneSprites.getPosition().x,
			this->mEnemyPlaneSprites.getPosition().y + 100), sf::Vector2f(-0.2F, 0.2F), sf::Vector2f(-1.F, 0.F));

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
void Enemy::enemyJeepAttack()
{
	Projectile tempProjectile;
	tempProjectile.createEnemyJeepMissile(&Enemy::mEnemyJeepWeapons[0], sf::Vector2f(this->mEnemyJeepSprites.getPosition().x + 100, this->mEnemyJeepSprites.getPosition().y), sf::Vector2f(-1.F, 1.F),
		sf::Vector2f(-1.F, 1.F));

	this->mEnemyJeepProjectiles.push_back(tempProjectile);
}

void Enemy::loadJeepProjectiles()
{
	sf::Texture temp;
	if (!temp.loadFromFile("Resources/Textures/Bullets/Bullet_3.png"))
		std::cerr << "Bullet failed to load" << "\n";

	this->mEnemyJeepWeapons.push_back(temp);
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
}

void Enemy::updateAttack(const float& deltaTime)
{
	if (this->mShootTimer >= this->mShootTimerMax && this->mIsAlive)
	{
		this->mIsAttacking = true;

		this->mIsMuzzleOn = true;

		this->enemyShoot();

		if (this->mIsAudioPlaying)
			this->pAudio.play();

		else
			this->pAudio.stop();

		this->pMuzzle.createMuzzleFlash(sf::Vector2f(this->mEnemyTankSprite.getPosition().x - 350, this->mEnemyTankSprite.getPosition().y), sf::Vector2f(0.6F, 0.6F));
	}
}