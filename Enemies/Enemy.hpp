#pragma once
#include <Entity/Player/Player.hpp>
#include <Entity/Entity.hpp>

class Enemy : Entity
{
public:
	Enemy();
	Enemy(std::vector<sf::Texture>& textures, sf::Vector2f position, sf::Vector2f moveDirection, int type);
	~Enemy();

	const int enemyDealDamage() const;
	const int enemyPlaneDealDamage() const;

	void createTanks(std::vector<sf::Texture>& textures, sf::Vector2f position, sf::Vector2f moveDirection, int type);
	void removeEnemyTankProjectile(unsigned index);
	void removeEnemyPlaneProjectile(unsigned index);
	void takeDamage(int damage);
	void planeTakeDamage(int damage);
	void renderPlane(sf::RenderTarget& target);
	void renderTank(sf::RenderTarget& target);
	void updateTank(const float& deltaTime);
	void render(sf::RenderTarget& target);
	void update(const float& deltaTime);
	void move(const float direction_x, const float direction_y, const float& deltaTime);
	void updateAnimations(const float& deltaTime);
	void updatePlane(const float& deltaTime);

	//Getters
	const std::vector<Projectile>& getProjectiles();
	Projectile& getEnemyTankProjectile(unsigned index);
	inline int getHp() { return this->mHp; }
	inline int getEnemyType() { return this->mEnemyType; }
	inline const int getEnemyTankProjectileSize() { return this->mEnemyTankProjectile.size(); }
	inline sf::Sprite getEnemyTankSprite() { return this->mEnemyTankSprite; }
	inline sf::FloatRect getGobalBounds()const { return this->mEnemyTankSprite.getGlobalBounds(); }
	sf::Vector2f getEnemyTankSpriteCenter();
	inline sf::Vector2f getEnemyTankPosition() const { return this->mEnemyTankSprite.getPosition(); }
	inline void setEnemyTankColor(sf::Color color) { this->mEnemyTankSprite.setColor(color); }

	//Enemy plane getter(s)
	Projectile& getEnemyPlaneProjectile(unsigned index);
	inline const int getEnemyPlaneProjectileSize() { return this->mEnemyPlaneProjectile.size(); }
	inline sf::Sprite& getEnemyPlaneSprite() { return this->mEnemyPlaneSprites; }
	inline sf::FloatRect getEnemyGobalBounds()const { return this->mEnemyPlaneSprites.getGlobalBounds(); }
	inline sf::Vector2f getEnemyPlanePosition() { return this->mEnemyPlaneSprites.getPosition(); }
	inline int getPlaneHp() { return this->mPlaneHp; }
	inline const bool isPlaneAlive() { return this->mIsPlaneAlive; }

	static std::vector<sf::Texture> mEnemyPlaneWeapons;
	static std::vector<sf::Texture> mEnemyProjectileTextures;

protected:

private:
	int mHp;
	int mHpMax;
	int mDamage;
	int mDamageMax;
	int mPlaneHp;
	int mPlaneHpMax;
	int mPlaneDamage;
	int mPlaneDamageMax;
	int mEnemyPlaneType;
	int mEnemyTankType;

	bool mIsAlive;
	bool mIsAttacking;
	bool mIsFiring;
	bool mIsMuzzleOn;
	bool mIsPlaneAlive;

	float mShootTimerMax;
	float mShootTimer;

	float mMuzzleTimerMax;
	float mMuzzleTimer;

	void enemyPlaneAttack();
	void loadPlaneProjectiles();
	void hpBar();
	void enemyDead(const float& deltaTime);
	void enemyShoot();
	void initVariables();
	void createMovementComponent(const float max_velocity,
		const float acceleration, const float deceleration);
	void createAnimationComponent(sf::Texture& texture_sheet);
	void loadEnemyTanks();
	void loadProjectile();
	void updateAttack(const float& deltaTime);

	//Plane
	sf::Sprite mEnemyPlaneSprites;
	sf::Vector2u mWindowBounds;
	std::vector<sf::Texture>* mTextures;
	std::vector<sf::Texture>* mEnemyProjectiles;
	std::vector<Projectile> mEnemyPlaneProjectile;

	//Tank
	sf::Sprite mEnemyTankSprites;
	std::vector<sf::Texture>* mTankTextures;
	std::vector<Projectile> mEnemyTankProjectiles;

	//Turret

	//Hp Bars
	sf::RectangleShape mHpBar1;
	sf::RectangleShape mHpBar2;
	sf::RectangleShape mHpBar3;
	sf::RectangleShape mHpBar4;

	sf::Texture mEnemyTankTexture;
	sf::Sprite mEnemyTankSprite;

	sf::Vector2f mEnemyTankSpriteCenter;

	sf::Sound mExplosion;
	sf::SoundBuffer mBuffer;

	sf::Vector2f mPosition;
	sf::Vector2f mMoveDirection;

	std::vector<sf::Texture> mProjectileTextures;
	std::vector<Projectile> mEnemyTankProjectile;
};