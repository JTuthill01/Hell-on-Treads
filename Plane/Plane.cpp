#include "stdafx.hpp"
#include "Plane.hpp"

std::vector<sf::Texture>Plane::mPlaneWeapons;

Plane::Plane() : mIsAttacking(false), mIsExploded(false), mHp(mHpMax), mHpMax(10), mDamage(5), mDamageMax(12)
{
	this->createMovementComponent(350.f, 16.f, 6.f);
	this->createAnimationComponent(this->mPlaneTexture);

	this->pAnimationComponent->addAnimation("MOVE", 5.f, 0, 0, 1, 0, 256, 256);

	this->loadPlane();

	this->loadProjectile();

	this->mPlayerPlaneCurrentWeapon = MISSILE;

	this->startTimer();

	this->bombExplosoion();
}

Plane::Plane(std::vector<sf::Texture>& textures, std::vector<sf::Texture>& bulletTextures, sf::Vector2f position, sf::Vector2f moveDirection, int type)
{
}

Plane::~Plane() = default;

void Plane::removeProjectile(unsigned int index)
{
	this->mPlaneProjectile.erase(this->mPlaneProjectile.begin() + index);
}

void Plane::move(const float direction_x, const float direction_y, const float& deltaTime)
{
	if (this->pMovementComponent)
		this->pMovementComponent->move(direction_x, direction_y, deltaTime);
}

void Plane::render(sf::RenderTarget & target)
{
	target.draw(this->mPlaneSprite);

	if (this->mIsExploded)
		target.draw(this->mBombExplosionSprite);

	for (size_t i = 0; i < this->mPlaneProjectile.size(); i++)
		this->mPlaneProjectile[i].render(target);
}

void Plane::update(const float& deltaTime)
{
	this->switchWeapon(deltaTime);

	this->pMovementComponent->update(deltaTime);

	this->attack(deltaTime);

	for (size_t i = 0; i < this->mPlaneProjectile.size(); i++)
		this->mPlaneProjectile[i].update(deltaTime);
}

void Plane::setExplosionPosition(sf::Vector2f position) { this->mBombExplosionSprite.setPosition(position); }

Projectile& Plane::getProjectile(unsigned index) { return this->mPlaneProjectile[index]; }

const int Plane::weaponDamage() const
{
	int damage = 0;

	switch (mPlayerPlaneCurrentWeapon)
	{
	case MISSILE:
		damage = rand() % this->mDamage + this->mDamageMax;
		break;

	case BOMB:
		damage = rand() % this->mDamage + mDamageMax + 2;
		break;

	default:
		break;
	}

	return damage;
}

const int Plane::enemyWeaponDamage() const
{
	int damage = 0;

	switch (this->mType)
	{
	case MISSILE:
		damage = rand() % this->mDamage + this->mDamageMax;
		break;

	case BOMB:
		damage = rand() % this->mDamage + mDamageMax + 2;
		break;

	default:
		break;
	}

	return damage;
}

void Plane::playerTakeDamage(int damage)
{
	this->mHp -= damage;

	if (this->mHp <= 0)
	{
		this->mHp = 0;

		this->setIsPlayerAlive(false);
	}
}

void Plane::enemyTakeDamage(int damage)
{
	this->mHp -= damage;

	if (this->mHp <= 0)
	{
		this->mHp = 0;

		this->setIsEnemyAlive(false);
	}
}

void Plane::enemyPlaneAttack(const float& deltaTime)
{
}

void Plane::switchWeapon(const float& deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
		this->mPlayerPlaneCurrentWeapon = MISSILE;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
		this->mPlayerPlaneCurrentWeapon = BOMB;
}

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

	if (this->pEnemyPlaneShootTimer.isExpired())
	{
		this->enemyPlaneAttack(deltaTime);

		this->mIsEnemyAttacking = true;

		this->pEnemyPlaneShootTimer.restart(sf::seconds(0.78F));
	}
}

void Plane::loadProjectile()
{
	sf::Texture temp;
	if (!temp.loadFromFile("Resources/Textures/Bullets/missile(2).png"))
		std::cerr << "Missile failed to load" << "\n";

	this->mPlaneWeapons.push_back(temp);

	if (!temp.loadFromFile("Resources/Textures/Bullets/Bomb_2.png"))
		std::cerr << "Bomb failed to load" << "\n";

	this->mPlaneWeapons.push_back(temp);
}

void Plane::currentWeapon(const float& deltaTime)
{
	if (this->mPlayerPlaneCurrentWeapon == MISSILE)
		this->mPlaneProjectile.push_back(Projectile(&Plane::mPlaneWeapons[MISSILE], sf::Vector2f(this->mPlaneSprite.getPosition().x + 300,
			this->mPlaneSprite.getPosition().y + 150), sf::Vector2f(-0.1F, 0.1F), sf::Vector2f(1.F, 0.F)));

	else if (this->mPlayerPlaneCurrentWeapon == BOMB)
	{
		Projectile mTempProjectile;

		mTempProjectile.createBomb(&Plane::mPlaneWeapons[BOMB], sf::Vector2f(this->mPlaneSprite.getPosition().x + 150,
			this->mPlaneSprite.getPosition().y + 200), sf::Vector2f(-0.2F, 0.2F), sf::Vector2f(1.F, 0.F));

		this->mPlaneProjectile.push_back(mTempProjectile);
	}
}

void Plane::loadPlane()
{
	if (!this->mPlaneTexture.loadFromFile("Resources/Textures/Player/Plane/Player_Plane.png"))
		std::cerr << "Whelp you can just fuck right off, as the plane texture does not exist" << "\n";

	this->mPlaneSprite.setTexture(this->mPlaneTexture);
	this->mPlaneSprite.setPosition(0.F, 400.F);
	this->mPlaneSprite.setScale(1.2F, 1.2F);
}

void Plane::createMovementComponent(const float max_velocity, const float acceleration, const float deceleration)
{
	this->pMovementComponent = new MovementComponent(this->mPlaneSprite, max_velocity, acceleration,
		deceleration);
}

void Plane::createAnimationComponent(sf::Texture & texture_sheet)
{
	this->pAnimationComponent = new AnimationComponent(this->mPlaneSprite, texture_sheet);
}

void Plane::startTimer()
{
	this->mKeyTimer.start();

	this->pEnemyPlaneShootTimer.start();
}