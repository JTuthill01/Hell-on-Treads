#pragma once
#include <Entity/Player.hpp>
#include <Resources/Audio.hpp>
#include <Component/AnimationComponent.hpp>
#include <Component/MovementComponent.hpp>
#include <Projectile/Projectile.hpp>

class EnemyTank
{
public:
	EnemyTank();
	~EnemyTank();

	Projectile& getEnemyTankProjectile(unsigned index);
	void removeEnemyTankProjectile(unsigned index);

	void render(sf::RenderTarget& target);
	void update(const float& deltaTime);
	void move(const float direction_x, const float direction_y, const float& deltaTime);
	void updateAnimations(const float& deltaTime);

	inline const int getEnemyTankProjectileSize() { return this->mEnemyTankProjectile.size(); }
	inline sf::Sprite getEnemyTankSprite() { return this->mEnemyTankSprite; }
	inline sf::FloatRect getGobalBounds()const { return this->mEnemyTankSprite.getGlobalBounds(); }
	inline sf::Vector2f getEnemyTankPosition() const { return this->mEnemyTankSprite.getPosition(); }
	inline void setEnemyTankColor(sf::Color color) { this->mEnemyTankSprite.setColor(color); }

protected:
	Projectile pMuzzle;
	MovementComponent* pMovementComponent;
	AnimationComponent* pAnimationComponent;
	Audio pAudio;

private:
	bool mIsAttacking;
	bool mIsFiring;
	bool mIsMuzzleOn;

	float mShootTimerMax;
	float mShootTimer;

	float mMuzzleTimerMax;
	float mMuzzleTimer;

	void enemyShoot();
	void initVariables();
	void createMovementComponent(const float max_velocity,
		const float acceleration, const float deceleration);
	void createAnimationComponent(sf::Texture& texture_sheet);
	void loadEnemyTanks();
	void loadProjectile();
	void updateAttack(const float& deltaTime);

	sf::Texture mEnemyTankTexture;
	sf::Sprite mEnemyTankSprite;

	sf::Texture mEnemyTexture;
	sf::Sprite mEnemySprite;

	sf::Vector2f mEnemyCenter;

	sf::Sound mExplosion;
	sf::SoundBuffer mBuffer;

	std::vector<sf::Texture> mProjectileTextures;
	std::vector<Projectile> mEnemyTankProjectile;

	static std::vector<sf::Texture> mEnemyProjectileTextures;
};

