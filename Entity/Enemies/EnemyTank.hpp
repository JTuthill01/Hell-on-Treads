#pragma once
#include <Entity/Player/Player.hpp>
#include <Entity/Entity.hpp>

class EnemyTank : Entity
{
public:
	EnemyTank();
	~EnemyTank();

	void removeEnemyTankProjectile(unsigned index);

	void render(sf::RenderTarget& target);
	void update(const float& deltaTime);
	void move(const float direction_x, const float direction_y, const float& deltaTime);
	void updateAnimations(const float& deltaTime);

	//Getters
	Projectile& getEnemyTankProjectile(unsigned index);
	inline int getEnemyType() { return this->mEnemeyType; }
	inline const int getEnemyTankProjectileSize() { return this->mEnemyTankProjectile.size(); }
	inline sf::Sprite getEnemyTankSprite() { return this->mEnemyTankSprite; }
	inline sf::FloatRect getGobalBounds()const { return this->mEnemyTankSprite.getGlobalBounds(); }
	sf::Vector2f getEnemyTankSpriteCenter();
	inline sf::Vector2f getEnemyTankPosition() const { return this->mEnemyTankSprite.getPosition(); }
	inline void setEnemyTankColor(sf::Color color) { this->mEnemyTankSprite.setColor(color); }

protected:
	Projectile pMuzzle;
	MovementComponent* pMovementComponent;
	AnimationComponent* pAnimationComponent;
	Audio pAudio;

private:
	int mEnemeyType;

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

	sf::Vector2f mEnemySprite;

	sf::Texture mEnemyTankTexture;
	sf::Sprite mEnemyTankSprite;

	sf::Vector2f mEnemyTankSpriteCenter;

	sf::Sound mExplosion;
	sf::SoundBuffer mBuffer;

	std::vector<sf::Texture> mProjectileTextures;
	std::vector<Projectile> mEnemyTankProjectile;

	static std::vector<sf::Texture> mEnemyProjectileTextures;
};