#pragma once
#include <Entity/Entity.hpp>



class Player : Entity
{
public:
	Player();
	~Player();

	void removePlayerProjectile(unsigned index);
	void render(sf::RenderTarget& target);
	void update(const float& deltaTime);
	void updateAnimations(const float& deltaTime);
	void move(const float direction_x, const float direction_y, const float& deltaTime);
	void auroa();

	//Getter(s)
	Projectile& getPlayerProjectile(unsigned index);
	inline const int getPlayerProjectileSize() { return this->mPlayerProjectile.size(); }
	inline sf::Sprite getPlayerSprite() { return this->mPlayerSprite; }
	inline sf::FloatRect getGobalBounds()const { return this->mPlayerSprite.getGlobalBounds(); }
	inline sf::Vector2f getPlayerCenter() { return this->mPlayerCenter; }
	inline sf::Vector2f getPosition() const { return this->mPlayerSprite.getPosition(); }
	inline const int getWeaponLevel() { return this->pCurrentWeapon; }

	//Setter(s)
	void setGunLevel(int level) { this->pCurrentWeapon = level; }

	//Enable weapons
	inline void enableFire() { this->pIsFireShot = true; }
	inline void enableIce() { this->pIsIceShot = true; }
	inline void enableCorrosive() { this->pIsCorrosiveShot = true; }

	//Disable weapons
	inline void disableFire() { this->pIsFireShot = false; }
	inline void disableIce() { this->pIsIceShot = false; }
	inline void disableCorrosive() { this->pIsCorrosiveShot = false; }

	bool IsAttacking() const { return this->mIsAttacking; }

protected:
	MovementComponent* pMovementComponent;
	AnimationComponent* pAnimationComponent;
	Audio pAudio;

private:
	//Booleans
	bool mIsAttacking;
	bool mIsFiring;
	bool mIsMuzzleOn;

	//Timers
	float mMuzzleTimerMax;
	float mMuzzleTimer;

	float mKeyTimeMax;
	float mKeyTime;

	float mShootTimer;
	float mShootTimerMax;

	//Private shit
	void initVariables();
	void currentWeapon(const float& deltaTime);
	void loadProjectile();
	void updateAttack(const float& deltaTime);
	void createMovementComponent(const float max_velocity,
		const float acceleration, const float deceleration);
	void createAnimationComponent(sf::Texture& texture_sheet);

	//Sprites & Textures
	sf::Texture mAuroaDyingTexture;
	sf::Sprite mAuroaDyingSprite;

	sf::Texture mPlayerTexture;
	sf::Sprite mPlayerSprite;

	sf::Vector2f mPlayerCenter;

	sf::Sound mExplosion;
	sf::SoundBuffer mBuffer;

	Projectile mMuzzle;
	std::vector<Projectile> mPlayerProjectile;

	static std::vector<sf::Texture> mPlayerProjectileTextures;
};