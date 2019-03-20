#pragma once
#include <Projectile/Projectile.hpp>
#include <Component/AnimationComponent.hpp>
#include <Component/MovementComponent.hpp>
#include <Resources/Audio.hpp>

enum projectileType { REGULAR = 0, FIRE, ICE, CORROSIVE };

class Player
{
public:
	Player();
	~Player();

	Projectile& getProjectile(unsigned index);

	void render(sf::RenderTarget& target);
	void update(const float& deltaTime);
	void updateAnimations(const float& deltaTime);
	void move(const float direction_x, const float direction_y, const float  & deltaTime);
	void startContact();
	void endContact();

	inline sf::FloatRect getGobalBounds()const { return this->mPlayerSprite.getGlobalBounds(); }
	inline sf::Vector2f getPosition() const { return this->mPlayerSprite.getPosition(); }

	bool IsAttacking() const { return mIsAttacking; }

protected:
	MovementComponent* pMovementComponent;
	AnimationComponent* pAnimationComponent;
	Audio pAudio;

private:
	bool mIsContact;
	bool mIsAttacking;
	bool mIsFiring;

	float mKeyTimeMax;
	float mKeyTime;

	int mCurrentWeapon;

	float mShootTimer;
	float mShootTimerMax;

	void currentWeapon(const float& deltaTime);
	void loadProjectile();
	void updateAttack(const float& deltaTime);
	void createMovementComponent(const float max_velocity,
		const float acceleration, const float deceleration);
	void createAnimationComponent(sf::Texture& texture_sheet);

	sf::Texture mPlayerTexture;
	sf::Sprite mPlayerSprite;
	sf::Vector2f mPlayerCenter;
	sf::Sound mExplosion;
	sf::SoundBuffer mBuffer;

	std::vector<Projectile> mProjectile;

	static std::vector<sf::Texture> mPlayerProjectileTextures;
};

