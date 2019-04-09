#pragma once
#include <Projectile/Projectile.hpp>
#include <Resources/Particle.hpp>
#include <Entity/Player/Player.hpp>
#include <Entity/Enemies/Enemy.hpp>
#include <Collision/Collision.hpp>
#include <Resources/AuroraLoader.hpp>
#include <Entity/Plane/Plane.hpp>
#include <Resources/TextTags.hpp>

class Level
{
public:
	Level(sf::RenderWindow* window, std::stack<Level*>* level);
	~Level();

	virtual void update(const float& deltaTime) = 0;
	virtual void render(sf::RenderTarget& target) = 0;
	virtual void initLevel() = 0;
	virtual void removeProjectile() = 0;

protected:
	void removeEnemyPlane(unsigned index);
	void playerProjectileCollision(const float& deltaTime);
	void enemyProjectileCollision(const float& deltaTime);
	void updateLevel(const float& deltaTime);
	void setExplosions(sf::Vector2f position, sf::Vector2f scale);

	std::stack<Level*>* pLevel;

	sf::Texture pBackgroundTexture;
	sf::Texture mExpolsionTexture;
	sf::Sprite pBackgroundSprite;
	sf::Sprite mExpolsionSprite;
	sf::RenderWindow* pWindow;
	sf::Font pFont;

	//Timers
	thor::Timer pEnemySpawnTimer;
	thor::Timer pTextTagTimer;

	Player pPlayer;
	std::vector<Player> player;

	Enemy pEnemyTank;
	std::vector<Enemy> pEnemyTanks;
	std::vector<Enemy> pEnemyPlane;
	std::vector<Projectile> pProjectiles;

	Entity pEntity;
	Collision pCollision;
	AuroraLoader::AuroraLoader mAurora;

	static std::vector<sf::Texture> pParticleTextures;
	std::vector<Particle::Particle> pParticle;

	bool pIsRemoved;
	bool pLoadLevel;
	bool pHasExploaded;

	std::vector<TextTags> pTextTags;

private:
	thor::Timer mExplosionTimer;
	thor::Timer mParticleTimer;
	void timer();
};