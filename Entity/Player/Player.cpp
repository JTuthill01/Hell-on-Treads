#include "stdafx.hpp"
#include "Player.hpp"

std::vector<sf::Texture>Player::mPlayerProjectileTextures;

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

	this->pCurrentWeapon = ICE;

	this->loadProjectile();

	this->mPlayerCenter.x = this->mPlayerSprite.getPosition().x + this->mPlayerSprite.getGlobalBounds().width / 2;
	this->mPlayerCenter.y = this->mPlayerSprite.getPosition().y + this->mPlayerSprite.getGlobalBounds().height / 2;
}

Player::~Player() = default;

Projectile& Player::getPlayerProjectile(unsigned index) { return this->mPlayerProjectile[index]; }

void Player::removePlayerProjectile(unsigned index) { this->mPlayerProjectile.erase(this->mPlayerProjectile.begin() + index); }

void Player::render(sf::RenderTarget & target)
{
	target.draw(this->mPlayerSprite);

	target.draw(this->mAuroaDyingSprite);

	for (size_t i = 0; i < this->mPlayerProjectile.size(); i++)
		this->mPlayerProjectile[i].render(target);

	if (this->mIsMuzzleOn)
		this->mMuzzle.render(target);
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
}

void Player::currentWeapon(const float& deltaTime)
{
	if (this->pCurrentWeapon == REGULAR)
		this->mPlayerProjectile.push_back(Projectile(&Player::mPlayerProjectileTextures[REGULAR], sf::Vector2f(this->mPlayerSprite.getPosition().x + 300, this->mPlayerSprite.getPosition().y + 195),
			sf::Vector2f(-0.4F, 0.5F), sf::Vector2f(1.F, 0.F)));

	else if (this->pCurrentWeapon == FIRE)
		this->mPlayerProjectile.push_back(Projectile(&Player::mPlayerProjectileTextures[FIRE], sf::Vector2f(this->mPlayerSprite.getPosition().x + 300, this->mPlayerSprite.getPosition().y + 195),
			sf::Vector2f(-0.8F, 0.7F), sf::Vector2f(1.F, 0.F)));

	else if (this->pCurrentWeapon == ICE)
		this->mPlayerProjectile.push_back(Projectile(&Player::mPlayerProjectileTextures[ICE], sf::Vector2f(this->mPlayerSprite.getPosition().x + 300, this->mPlayerSprite.getPosition().y + 195),
			sf::Vector2f(-0.8F, 0.7F), sf::Vector2f(1.F, 0.F)));

	else if (this->pCurrentWeapon == CORROSIVE)
		this->mPlayerProjectile.push_back(Projectile(&Player::mPlayerProjectileTextures[CORROSIVE], sf::Vector2f(this->mPlayerSprite.getPosition().x + 300, this->mPlayerSprite.getPosition().y + 195),
			sf::Vector2f(-0.8F, 0.7F), sf::Vector2f(1.F, 0.F)));
}

void Player::loadProjectile()
{
	sf::Texture temp;
	if (!temp.loadFromFile("Resources/Textures/Bullets/Bullet_3.png"))
		std::cerr << "The fucking bullet texture failed to fucking load" << "\n";

	this->mPlayerProjectileTextures.push_back(temp);

	if (!temp.loadFromFile("Resources/Textures/Bullets/Fire1.png"))
		std::cerr << "The fucking bullet texture failed to fucking load" << "\n";

	this->mPlayerProjectileTextures.push_back(temp);

	if (!temp.loadFromFile("Resources/Textures/Bullets/Ice1.png"))
		std::cerr << "The fucking bullet texture failed to fucking load" << "\n";

	this->mPlayerProjectileTextures.push_back(temp);

	if (!temp.loadFromFile("Resources/Textures/Bullets/Corrosive1.png"))
		std::cerr << "The fucking bullet texture failed to fucking load" << "\n";

	this->mPlayerProjectileTextures.push_back(temp);
}

void Player::updateAttack(const float& deltaTime)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) | sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && this->mShootTimer >= this->mShootTimerMax && this->mKeyTime >= this->mKeyTimeMax)
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

	this->playerDamage();

	this->mKeyTime += 1.F * deltaTime * 62.5F;

	this->mShootTimer += 1.F * deltaTime * 62.5F;

	this->pMovementComponent->update(deltaTime);

	this->updateAttack(deltaTime);

	this->updateAnimations(deltaTime);

	for (size_t i = 0; i < this->mPlayerProjectile.size(); i++)
		this->mPlayerProjectile[i].update(deltaTime);

	this->mMuzzle.update(deltaTime);

	this->frozen();
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
	if (this->pHp < 4)
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
