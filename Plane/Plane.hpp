#pragma once
#include <Entity/Entity.hpp>

class Plane :
	public Entity
{
public:
	Plane();
	Plane(std::vector<sf::Texture>& textures, std::vector<sf::Texture>& bulletTextures, sf::Vector2f position, sf::Vector2f moveDirection, int type);
	virtual ~Plane();

	static std::vector<Projectile> enemyPlaneProjectile;

	const int weaponDamage() const;
	const int enemyWeaponDamage() const;

	void playerTakeDamage(int damage);
	void enemyTakeDamage(int damage);
	void removeProjectile(unsigned int index);
	void move(const float direction_x, const float direction_y, const float& deltaTime);
	void render(sf::RenderTarget& target);
	void update(const float& deltaTime);
	void createEnemyPlane();

	//Bomb setter(s)
	void setExplosion(bool t_f) { this->mIsExploded = t_f; }
	void setExplosionPosition(sf::Vector2f position);

	//Bomb getter(s)
	Projectile& getProjectile(unsigned index);

	inline const int getProjectileSize() { return this->mPlaneProjectile.size(); }
	inline sf::Sprite& getExplosionSprite() { return this->mBombExplosionSprite; }
	inline sf::FloatRect getExplosionGobalBounds() { return this->mBombExplosionSprite.getGlobalBounds(); }
	inline sf::Vector2f getExplosionPosition() { return this->mBombExplosionSprite.getPosition(); }

	//Plane getter(s)
	inline sf::Sprite& getPlaneSprite() { return this->mPlaneSprite; }
	inline sf::FloatRect getGobalBounds()const { return this->mPlaneSprite.getGlobalBounds(); }
	inline sf::Vector2f getPlanePosition() { return this->mPlaneSprite.getPosition(); }

private:
	void loadEnemyProjectiles();
	void enemyPlaneAttack(const float& deltaTime);
	void switchWeapon(const float& deltaTime);
	void bombExplosoion();
	void attack(const float& deltaTime);
	void loadProjectile();
	void currentWeapon(const float& deltaTime);
	void loadPlane();
	void createMovementComponent(const float max_velocity,
		const float acceleration, const float deceleration);
	void createAnimationComponent(sf::Texture& texture_sheet);
	void startTimer();

	sf::Texture mPlaneTexture;
	sf::Sprite mPlaneSprite;

	sf::Texture mBombExplosionTexture;
	sf::Sprite mBombExplosionSprite;

	sf::Vector2f mPosition;
	sf::Vector2f mMoveDirection;
	sf::Vector2u mWindowBounds;

	bool mIsEnemyAttacking;
	bool mIsAttacking;
	bool mIsExploded;

	static std::vector<sf::Texture> mPlaneWeapons;
	std::vector<Projectile> mPlaneProjectile;

	int mPlayerPlaneCurrentWeapon;

	thor::Timer mKeyTimer;
	thor::Timer mShootTimer;
	thor::Timer mExplosionTimer;

	int mHp;
	int mHpMax;
	int mDamage;
	int mDamageMax;
	int mType;
};