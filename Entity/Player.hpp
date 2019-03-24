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

	Projectile& getPlayerProjectile(unsigned index);
	void removePlayerProjectile(unsigned index);

	void render(sf::RenderTarget& target);
	void update(const float& deltaTime);
	void updateAnimations(const float& deltaTime);
	void move(const float direction_x, const float direction_y, const float  & deltaTime);
	void startContact();
	void endContact();

	inline const int getPlayerProjectileSize() { return this->mPlayerProjectile.size(); }
	inline sf::Sprite getPlayerSprite() { return this->mPlayerSprite; }
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
	bool mIsMuzzleOn;
	
	int mCurrentWeapon;

	float mMuzzleTimerMax;
	float mMuzzleTimer;

	float mKeyTimeMax;
	float mKeyTime;

	float mShootTimer;
	float mShootTimerMax;

	void initVariables();
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

	Projectile mMuzzle;
	std::vector<Projectile> mPlayerProjectile;

	static std::vector<sf::Texture> mPlayerProjectileTextures;
};

