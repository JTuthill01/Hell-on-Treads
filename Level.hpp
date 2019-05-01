#pragma once
#include <Resources/Tree.hpp>
#include <Weather/Clouds.hpp>
#include <Resources/Audio.hpp>
#include <Weather/Weather.hpp>
#include <Projectile/Projectile.hpp>
#include <Resources/Particle.hpp>
#include <Entity/Plane/Plane.hpp>
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
	void removeTrees(unsigned index);
	void loadTrees();
	void endGameInput();
	void endGame();
	void renderEndGame(sf::RenderTarget& target);
	void removeEnemyPlane(unsigned index);
	void playerProjectileCollision(const float& deltaTime);
	void enemyProjectileCollision(const float& deltaTime);
	void updateLevel(const float& deltaTime);
	void setExplosions(sf::Vector2f position, sf::Vector2f scale);

	std::stack<Level*>* pLevel;

	sf::Event e;
	sf::Texture pBackgroundTexture;
	sf::Texture mExpolsionTexture;
	sf::Sprite pBackgroundSprite;
	sf::Sprite mExpolsionSprite;
	sf::RenderWindow* pWindow;
	sf::Font pFont;
	std::vector<sf::Texture> pTextures;
	std::vector<sf::Texture> pProjectileTextures;
	std::vector<sf::Texture> pTreeTextures;

	//Timers
	thor::Timer pEnemySpawnTimer;
	thor::Timer pTextTagTimer;
	thor::Timer pCloudSpawnTimer;

	Plane pPlayerPlane;
	Player pPlayer;
	std::vector<Player> player;

	Enemy pEnemyTank;

	std::vector<Trees::Tree> pTrees;
	std::vector<Enemy> pEnemyJeep;
	std::vector<Enemy> pEnemyTanks;
	std::vector<Enemy> pEnemyPlane;
	std::vector<Projectile>pJeepProjectiles;
	std::vector<Projectile> pProjectiles;
	std::vector<TextTags> pTextTags;

	Trees::Tree pTree; 
	Audio pAudio;
	Weather pWeather;
	Entity pEntity;
	Collision pCollision;
	AuroraLoader::AuroraLoader mAurora;

	static std::vector<sf::Texture> pParticleTextures;
	std::vector<Particle::Particle> pParticle;

	bool pShouldClose;
	bool pIsTreeRemoved;
	bool pIsEnemyProjectileRemoved;
	bool pIsPlayerProjectileRemoved;
	bool pLoadLevel;
	bool pHasExploaded;

private:
	sf::Text mText;
	sf::Text mResetText;
	
	thor::Timer mReset;
	thor::Timer mExplosionTimer;
	thor::Timer mParticleTimer;
	void timer();
};