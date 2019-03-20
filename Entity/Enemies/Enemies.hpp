#pragma once
#include <Entity/Player.hpp>
#include <Resources/Audio.hpp>
#include <Component/AnimationComponent.hpp>
#include <Component/MovementComponent.hpp>
#include <Projectile/Projectile.hpp>

enum enemyProjectiles { BOMB = 0, MISSILE};

class Enemies
{
public:
	Enemies();
	~Enemies();

	void render(sf::RenderTarget& target);
	void update(const float& deltaTime);
	void move(const float direction_x, const float direction_y, const float& deltaTime);

	inline sf::FloatRect getGobalBounds()const { return this->mEnemySprite.getGlobalBounds(); }
	inline sf::Vector2f getPosition() const { return this->mEnemySprite.getPosition(); }
	inline void setColor(sf::Color color) { this->mEnemySprite.setColor(color); }

protected:
	MovementComponent* pMovementComponent;
	AnimationComponent* pAnimationComponent;
	Audio pAudio;
	Player pPlayer;

private:
	bool mDropBomb;

	float mShootTimerMax;
	float mShootTimer;

	bool checkPosition(const float& deltaTime);

	void bombDrop(const float& deltaTime);
	void loadProjectile();
	void updateAttack(const float& deltaTime);

	sf::Texture mEnemyTexture;
	sf::Sprite mEnemySprite;
	sf::Vector2f mEnemyCenter;
	sf::Sound mExplosion;
	sf::SoundBuffer mBuffer;

	std::vector<sf::Texture> mProjectileTextures;
	std::vector<Projectile> mProjectiles;

	static std::vector<sf::Texture> mEnemyProjectileTextures;
};

