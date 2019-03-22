#pragma once
#include <Entity/Player.hpp>
#include <Resources/Audio.hpp>
#include <Component/AnimationComponent.hpp>
#include <Component/MovementComponent.hpp>
#include <Projectile/Projectile.hpp>

enum enemyProjectiles { BOMB = 0, MISSILE};

class EnemyTank
{
public:
	EnemyTank();
	~EnemyTank();

	void render(sf::RenderTarget& target);
	void update(const float& deltaTime);
	void move(const float direction_x, const float direction_y, const float& deltaTime);
	void updateAnimations(const float& deltaTime);

	inline sf::Sprite getSprite() { return this->mEnemySprite; }
	inline sf::FloatRect getGobalBounds()const { return this->mEnemyTankSprite.getGlobalBounds(); }
	inline sf::Vector2f getPosition() const { return this->mEnemySprite.getPosition(); }
	inline void setColor(sf::Color color) { this->mEnemySprite.setColor(color); }

protected:
	MovementComponent* pMovementComponent;
	AnimationComponent* pAnimationComponent;
	Audio pAudio;

private:
	bool mIsAttacking;
	bool mIsFiring;
	bool mIsMuzzleOn;

	float mShootTimerMax;
	float mShootTimer;

	void createMovementComponent(const float max_velocity,
		const float acceleration, const float deceleration);
	void createAnimationComponent(sf::Texture& texture_sheet);
	void loadEnemyTanks();
	void bombDrop(const float& deltaTime);
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
	std::vector<Projectile> mProjectiles;

	static std::vector<sf::Texture> mEnemyProjectileTextures;
};

