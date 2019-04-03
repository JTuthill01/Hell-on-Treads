#pragma once
#include <Entity/Entity.hpp>

class Plane :
	public Entity
{
public:
	Plane();
	virtual ~Plane();

	void removeProjectile(unsigned int index);
	void move(const float direction_x, const float direction_y, const float& deltaTime);
	void render(sf::RenderTarget& target);
	void update(const float& deltaTime);
	void setExplosion(bool t_f) { this->mIsExploded = t_f; }
	void setExplosionPosition(sf::Vector2f position);

	Projectile& getBomb(unsigned index);
	inline const int getBombSize() { return this->mPlayerPlaneProjectile.size(); }
	inline sf::Sprite getPlaneSprite() { this->mPlanePlayerSprite; }
	inline sf::FloatRect getGobalBounds()const { return this->mPlanePlayerSprite.getGlobalBounds(); }

private:
	void bombExplosoion();
	void attack(const float& deltaTime);
	void loadProjectile();
	void currentWeapon(const float& deltaTime);
	void loadPlayerPlane();
	void createMovementComponent(const float max_velocity,
		const float acceleration, const float deceleration);
	void createAnimationComponent(sf::Texture& texture_sheet);
	void startTimer();

	sf::Texture mPlanePlayerTexture;
	sf::Sprite mPlanePlayerSprite;

	sf::Texture mBombExplosionTexture;
	sf::Sprite mBombExplosionSprite;

	bool mIsAttacking;
	bool mIsExploded;

	static std::vector<sf::Texture> mPlayerPlaneWeapons;
	std::vector<Projectile> mPlayerPlaneProjectile;

	int mPlayerPlaneCurrentWeapon;

	thor::Timer mKeyTimer;
	thor::Timer mShootTimer;
	thor::Timer mExplosionTimer;
};