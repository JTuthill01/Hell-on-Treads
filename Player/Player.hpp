#pragma once
#include <Entity/Entity.hpp>

class Player : Entity
{
public:
	Player();
	~Player();

	const int playerDealDamage() const;

	void createTurret();
	void updateTurret(const float& deltaTime);
	void renderTurret(sf::RenderTarget& target);
	void loadTurretProjectiles();
	void createSoldier();
	void takeDamage(int damage);
	void removePlayerProjectile(unsigned index);
	void render(sf::RenderTarget& target);
	void update(const float& deltaTime);
	void updateSoldier(const float& deltaTime);
	void renderSoldier(sf::RenderTarget& target);
	void updateAnimations(const float& deltaTime);
	void move(const float direction_x, const float direction_y, const float& deltaTime);
	void auroa();
	void updatePlayerHpBar(const float& deltaTime);

	//Tank Getter(s)
	Projectile& getPlayerProjectile(unsigned index);
	inline const int getPlayerProjectileSize() { return this->mPlayerProjectile.size(); }
	inline sf::Sprite getPlayerSprite() { return this->mPlayerSprite; }
	inline sf::FloatRect getGobalBounds()const { return this->mPlayerSprite.getGlobalBounds(); }
	inline sf::Vector2f getPlayerCenter() { return this->mPlayerCenter; }
	inline sf::Vector2f getPosition() const { return this->mPlayerSprite.getPosition(); }
	inline const int getWeaponLevel() { return this->pCurrentWeapon; }
	inline int getPlayerHp() { return this->mHp; }

	//Tank Setter(s)
	void setGunLevel(int level) { this->pCurrentWeapon = level; }

	//Soldier Getter(s)
	inline sf::Sprite getSoldierSprite() { return this->mSoldierSprite; }
	inline sf::FloatRect getGobalBounds()const { return this->mSoldierSprite.getGlobalBounds(); }
	inline sf::Vector2f getSoldierPosition() const { return this->mSoldierSprite.getPosition(); }

	//Enable weapons
	inline void enableFire() { this->pIsFireShot = true; }
	inline void enableIce() { this->pIsIceShot = true; }
	inline void enableCorrosive() { this->pIsCorrosiveShot = true; }

	//Disable weapons
	inline void disableFire() { this->pIsFireShot = false; }
	inline void disableIce() { this->pIsIceShot = false; }
	inline void disableCorrosive() { this->pIsCorrosiveShot = false; }

	bool IsAttacking() const { return this->mIsAttacking; }

private:
	//Damage
	int mDamage;
	int mDamageMax;

	//HP
	int mHp;
	int mHpMax;

	//Booleans
	bool mIsAttacking;
	bool mIsFiring;
	bool mIsMuzzleOn;
	bool mIsAlive;

	//Timers
	float mMuzzleTimerMax;
	float mMuzzleTimer;

	float mKeyTimeMax;
	float mKeyTime;

	float mShootTimer;
	float mShootTimerMax;

	//Private shit
	void hpBar();
	void initVariables();
	void currentWeapon(const float& deltaTime);
	void loadProjectile();
	void updateAttack(const float& deltaTime);
	void createMovementComponent(const float max_velocity,
		const float acceleration, const float deceleration);
	void createAnimationComponent(sf::Texture& texture_sheet);

	//Sprites & Textures
	sf::RectangleShape mHpBar1;
	sf::RectangleShape mHpBar2;
	sf::RectangleShape mHpBar3;
	sf::RectangleShape mHpBar4;

	sf::Texture mAuroaDyingTexture;
	sf::Sprite mAuroaDyingSprite;

	//Tank
	sf::Texture mPlayerTexture;
	sf::Sprite mPlayerSprite;

	//Soldier
	sf::Texture mSoldierTexture;
	sf::Sprite mSoldierSprite;

	//Turret
	sf::Texture mTurretTexture_0;
	sf::Texture mTurretTexture_1;
	sf::Sprite mTurretSprite_0;
	sf::Sprite mTurretSprite_1;

	sf::Vector2f mPlayerCenter;

	sf::Sound mExplosion;
	sf::SoundBuffer mBuffer;

	Projectile mMuzzle;
	std::vector<Projectile> mPlayerProjectile;

	static std::vector<sf::Texture> mPlayerProjectileTextures;
	static std::vector<sf::Texture> mPlayerTurretProjectileTextures;
};