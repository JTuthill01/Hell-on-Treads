#pragma once
#include <Resources/Particle.hpp>
#include <Entity/Player/Player.hpp>
#include <Entity/Enemies/EnemyTank.hpp>
#include <Collision/Collision.hpp>
#include <Resources/AuroraLoader.hpp>
#include <Entity/Player/Player_Plane/Player_Plane.hpp>

class Level
{
public:
	Level(sf::RenderWindow* window, std::stack<Level*>* level);
	~Level();

	virtual void update(const float& deltaTime) = 0;
	virtual void render(sf::RenderTarget& target) = 0;
	virtual void initLevel() = 0;

protected:
	void playerProjectileCollision(const float& deltaTime);
	void enemyProjectileCollision(const float& deltaTime);
	void playerInput(const float& deltaTime);
	void updateLevel(const float& deltaTime);
	void setExplosions(sf::Vector2f position, sf::Vector2f scale);

	std::stack<Level*>* pLevel;

	sf::Texture pBackgroundTexture;
	sf::Texture mExpolsionTexture;
	sf::Sprite pBackgroundSprite;
	sf::Sprite mExpolsionSprite;
	sf::RenderWindow* pWindow;

	Player pPlayer;
	std::vector<Player> player;

	EnemyTank pEnemeyTank;
	std::vector<EnemyTank> enemyTank;

	Entity pEntity;
	Collision pCollision;
	AuroraLoader::AuroraLoader mAurora;

	static std::vector<sf::Texture> pParticleTextures;
	std::vector<Particle::Particle> pParticle;

	bool pLoadLevel;
	bool pHasExploaded;

private:
	thor::Timer mExplosionTimer;
	thor::Timer mParticleTimer;
	void timer();
};