#include "stdafx.hpp"
#include "Player.hpp"

std::vector<sf::Texture>Player::mPlayerProjectileTextures;
std::vector<sf::Texture>Player::mPlayerTurretProjectileTextures;

Player::Player()
{
	this->initVariables();

	if (!this->mPlayerTexture.loadFromFile("Resources/Textures/Player/Combined.png"))
		std::cerr << "Failed to load texture" << "\n";

	this->mPlayerSprite.setTexture(this->mPlayerTexture);

	this->mPlayerSprite.setPosition(300.F, 710.F);

	this->createMovementComponent(350.f, 16.f, 6.f);
	this->createAnimationComponent(this->mPlayerTexture);

	this->pAnimationComponent->addAnimation("MOVE", 5.f, 0, 0, 7, 0, 435, 435);
	this->pAnimationComponent->addAnimation("ATTACK", 5.f, 0, 1, 7, 1, 435, 435);

	this->pAudio.loadSound("Resources/Sounds/Explosions/DeathFlash.flac");
	this->pAudio.setVolume(20.F);

	this->pCurrentWeapon = REGULAR;

	this->loadProjectile();

	this->mPlayerCenter.x = this->mPlayerSprite.getPosition().x + this->mPlayerSprite.getGlobalBounds().width / 2;
	this->mPlayerCenter.y = this->mPlayerSprite.getPosition().y + this->mPlayerSprite.getGlobalBounds().height / 2;

	this->hpBar();
}

Player::~Player() = default;

const int Player::playerDealDamage() const
{
	int damage = 0;

	switch (this->pCurrentWeapon)
	{
	case REGULAR:
		damage = rand() % this->mDamage + this->mDamageMax;
		break;

	case FIRE:
		damage = rand() % this->mDamage + this->mDamageMax + rand();
		break;

	case ICE:
		damage = rand() % this->mDamage + this->mDamageMax;
		damage *= rand() % 2;

		break;

	case CORROSIVE:
		damage = rand() % this->mDamage + this->mDamageMax;
		damage *= rand() % 3;

	default:
		break;
	}

	return damage;
}

void Player::createTurret()
{
	if (!this->mTurretTexture_0.loadFromFile("Resources/Textures/Player/Turret.png"))
		std::cerr << "Fucking turret failed to load" << "\n";

	this->mTurretSprite_0.setTexture(this->mTurretTexture_0);
	this->mTurretSprite_0.setScale(1.F, 1.F);
	this->mTurretSprite_0.setPosition(50.F, 710.F);
}

void Player::updateTurret(const float& deltaTime)
{
}

void Player::renderTurret(sf::RenderTarget& target)
{
	target.draw(this->mTurretSprite_0);
}

void Player::loadTurretProjectiles()
{
}

Projectile& Player::getPlayerProjectile(unsigned index) { return this->mPlayerProjectile[index]; }

void Player::createSoldier()
{
	this->createMovementComponent(350.f, 16.f, 6.f);
	this->createAnimationComponent(this->mSoldierTexture);

	this->pAnimationComponent->addAnimation("MOVE", 5.f, 0, 0, 7, 0, 140, 140);

	if (!this->mSoldierTexture.loadFromFile("Resources/Textures/Player/Soldier/Player_Soldier.png"))
		std::cerr << "You suh can fuck right, as the player texture failed to load" << "\n";

	this->mSoldierSprite.setPosition(0.F, 710.F);
	this->mSoldierSprite.setScale(1.F, 1.F);
}

void Player::takeDamage(int damage)
{
	this->mHp -= damage;

	if (this->mHp <= 0)
	{
		this->mHp = 0;

		this->mIsAlive = false;
	}
}

void Player::removePlayerProjectile(unsigned index) { this->mPlayerProjectile.erase(this->mPlayerProjectile.begin() + index); }

void Player::render(sf::RenderTarget & target)
{
	target.draw(this->mPlayerSprite);

	target.draw(this->mAuroaDyingSprite);

	target.draw(this->mHpBar1);

	for (size_t i = 0; i < this->mPlayerProjectile.size(); i++)
		this->mPlayerProjectile[i].render(target);

	if (this->mIsMuzzleOn)
		this->mMuzzle.render(target);
}

void Player::hpBar()
{
	//Bar one
	this->pHpBar1.setSize(sf::Vector2f(150.F, 22.F));
	this->pHpBar1.setFillColor(sf::Color::Blue);
}

void Player::initVariables()
{
	this->pMovementComponent = nullptr;

	this->pMovementComponent = nullptr;

	this->mIsAttacking = false;

	this->mKeyTimeMax = 55.5F;

	this->mKeyTime = mKeyTimeMax;

	this->mShootTimerMax = 55.F;

	this->mShootTimer = this->mShootTimerMax;

	this->mIsMuzzleOn = false;

	this->mMuzzleTimerMax = 1.5F;

	this->mMuzzleTimer = this->mMuzzleTimerMax;

	this->mHp = 20;

	this->mHpMax = 30;

	this->mDamage = 2;

	this->mDamageMax = 4;

	this->mIsAlive = true;
}

void Player::currentWeapon(const float& deltaTime)
{
	if (this->pCurrentWeapon == REGULAR)
		this->mPlayerProjectile.push_back(Projectile(&Player::mPlayerProjectileTextures[REGULAR], sf::Vector2f(this->mPlayerSprite.getPosition().x + 300, this->mPlayerSprite.getPosition().y + 195),
			sf::Vector2f(-0.4F, 0.5F), sf::Vector2f(1.F, 0.F)));

	else if (this->pCurrentWeapon == FIRE)
	{
		this->mPlayerProjectile.push_back(Projectile(&Player::mPlayerProjectileTextures[FIRE], sf::Vector2f(this->mPlayerSprite.getPosition().x + 300, this->mPlayerSprite.getPosition().y + 195),
			sf::Vector2f(-0.4F, 0.5F), sf::Vector2f(1.F, 0.F)));

		for (size_t i = 0; i < this->mPlayerProjectile.size(); i++)
			this->mPlayerProjectile[i].setProjectileColor(sf::Color(139, 0, 0, 255));
	}

	else if (this->pCurrentWeapon == ICE)
	{
		this->mPlayerProjectile.push_back(Projectile(&Player::mPlayerProjectileTextures[ICE], sf::Vector2f(this->mPlayerSprite.getPosition().x + 300, this->mPlayerSprite.getPosition().y + 195),
			sf::Vector2f(-0.4F, 0.5F), sf::Vector2f(1.F, 0.F)));

		for (size_t i = 0; i < this->mPlayerProjectile.size(); i++)
			this->mPlayerProjectile[i].setProjectileColor(sf::Color::Blue);
	}

	else if (this->pCurrentWeapon == CORROSIVE)
	{
		this->mPlayerProjectile.push_back(Projectile(&Player::mPlayerProjectileTextures[CORROSIVE], sf::Vector2f(this->mPlayerSprite.getPosition().x + 300, this->mPlayerSprite.getPosition().y + 195),
			sf::Vector2f(-0.4F, 0.5F), sf::Vector2f(1.F, 0.F)));

		for (size_t i = 0; i < this->mPlayerProjectile.size(); i++)
			this->mPlayerProjectile[i].setProjectileColor(sf::Color(124, 252, 0, 255));
	}
}

void Player::loadProjectile()
{
	sf::Texture temp;
	if (!temp.loadFromFile("Resources/Textures/Bullets/Bullet_3.png"))
		std::cerr << "The fucking bullet texture failed to fucking load" << "\n";

	this->mPlayerProjectileTextures.push_back(temp);

	if (!temp.loadFromFile("Resources/Textures/Bullets/Bullet_3.png"))
		std::cerr << "The fucking bullet texture failed to fucking load" << "\n";

	this->mPlayerProjectileTextures.push_back(temp);

	if (!temp.loadFromFile("Resources/Textures/Bullets/Bullet_3.png"))
		std::cerr << "The fucking bullet texture failed to fucking load" << "\n";

	this->mPlayerProjectileTextures.push_back(temp);

	if (!temp.loadFromFile("Resources/Textures/Bullets/Bullet_3.png"))
		std::cerr << "The fucking bullet texture failed to fucking load" << "\n";

	this->mPlayerProjectileTextures.push_back(temp);
}

void Player::updateAttack(const float& deltaTime)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->mShootTimer >= this->mShootTimerMax && this->mKeyTime >= this->mKeyTimeMax)
	{
		this->currentWeapon(deltaTime);

		this->mIsAttacking = true;

		this->mIsMuzzleOn = true;

		this->mKeyTime = 0.F;

		this->mShootTimer = 0.F;

		this->pAudio.play();

		this->mMuzzle.createMuzzleFlash(sf::Vector2f(this->mPlayerSprite.getPosition().x + 150, this->mPlayerSprite.getPosition().y - 10), sf::Vector2f(0.6F, 0.5F));
	}
}

void Player::createMovementComponent(const float max_velocity, const float acceleration, const float deceleration)
{
	this->pMovementComponent = new MovementComponent(this->mPlayerSprite, max_velocity, acceleration,
		deceleration);
}

void Player::createAnimationComponent(sf::Texture & texture_sheet)
{
	this->pAnimationComponent = new AnimationComponent(this->mPlayerSprite, texture_sheet);
}

void Player::update(const float& deltaTime)
{
	this->auroa();

	this->playerDealDamage();

	this->mKeyTime += 1.F * deltaTime * 62.5F;

	this->mShootTimer += 1.F * deltaTime * 62.5F;

	this->pMovementComponent->update(deltaTime);

	this->updateAttack(deltaTime);

	this->updateAnimations(deltaTime);

	for (size_t i = 0; i < this->mPlayerProjectile.size(); i++)
		this->mPlayerProjectile[i].update(deltaTime);

	this->mMuzzle.update(deltaTime);

	this->frozen();

	this->updatePlayerHpBar(deltaTime);
}

void Player::updateSoldier(const float& deltaTime)
{
	this->pMovementComponent->update(deltaTime);

	this->animations(this->mSoldierSprite, deltaTime, pIsAttacking);

	this->mSoldierSprite.move(1.F, 0.F);
}

void Player::renderSoldier(sf::RenderTarget & target)
{
	target.draw(this->mSoldierSprite);
}

void Player::updateAnimations(const float& deltaTime)
{
	if (this->IsAttacking())
	{
		if (this->mPlayerSprite.getScale().x > 0.f) //Facing left
			this->mPlayerSprite.setOrigin(96.f, 0.f);

		else //Facing right
			this->mPlayerSprite.setOrigin(350.f, 0.f);

		if (this->pAnimationComponent->play("ATTACK", deltaTime, true))
		{
			this->mIsAttacking = false;

			this->mIsMuzzleOn = false;

			if (this->mPlayerSprite.getScale().x > 0.f) //Facing left
				this->mPlayerSprite.setOrigin(0.f, 0.f);

			else //Facing right
				this->mPlayerSprite.setOrigin(258.f, 0.f);
		}
	}

	else if (this->pMovementComponent->getState(MOVING_RIGHT))
	{
		if (this->mPlayerSprite.getScale().x < 0.f)
		{
			this->mPlayerSprite.setOrigin(0.f, 0.f);
			this->mPlayerSprite.setScale(1.f, 1.f);
		}

		this->pAnimationComponent->play("MOVE", deltaTime,
			this->pMovementComponent->getVelocity().x,
			this->pMovementComponent->getMaxVelocity());
	}

	else if (this->pMovementComponent->getState(MOVING_LEFT))
	{
		if (this->mPlayerSprite.getScale().x < 0.f)
		{
			this->mPlayerSprite.setOrigin(140.f, 0.f);
			this->mPlayerSprite.setScale(1.f, 1.f);
		}

		this->pAnimationComponent->play("MOVE", deltaTime,
			this->pMovementComponent->getVelocity().x,
			this->pMovementComponent->getMaxVelocity());
	}
}

void Player::move(const float direction_x, const float direction_y, const float& deltaTime)
{
	if (this->pMovementComponent)
		this->pMovementComponent->move(direction_x, direction_y, deltaTime);
}

void Player::auroa()
{
	if (this->mHp <= 4)
	{
		if (!this->mAuroaDyingTexture.loadFromFile("Resources/Textures/Aurora/auroaDying.png"))
			std::cerr << "The fucking Aurora failed to fucking load" << "\n";

		this->mAuroaDyingSprite.setTexture(this->mAuroaDyingTexture);
		this->mAuroaDyingSprite.setPosition(this->mPlayerSprite.getPosition().x + 150, this->mPlayerSprite.getPosition().y - 10);
		this->mAuroaDyingSprite.setRotation(45.F);
		this->mAuroaDyingSprite.setScale(2.7F, 2.7F);
		this->mAuroaDyingSprite.setColor(sf::Color::Black);
	}
}

void Player::updatePlayerHpBar(const float& deltaTime)
{
	this->mHpBar1.setPosition(this->mPlayerSprite.getPosition().x + 100, this->mPlayerSprite.getPosition().y + 250);

	if (this->mHp <= 9)
		this->mHpBar1.setFillColor(sf::Color(220, 20, 60, 255));

	if (this->mHp <= 12)
		this->mHpBar1.setFillColor(sf::Color::Yellow);

	if (this->mHp < 3)
		this->mHpBar4.setFillColor(sf::Color::Black);
}