#pragma once
#include <Entity/Player/Player.hpp>
#include <Entity/Enemies/Enemy.hpp>
#include <Weather/Weather.hpp>
#include "Level.hpp"
#include "Level_Two.hpp"

class Level_One :
	public Level
{
public:
	Level_One(sf::RenderWindow* window, std::stack<Level*>* level);
	virtual ~Level_One();

	// Inherited via Level
	virtual void update(const float& deltaTime) override;
	virtual void render(sf::RenderTarget& target) override;
	virtual void initLevel() override;

private:
	void loadClouds();
	void loadTank();
	void playerInput(const float& deltaTime);

	sf::Texture mLevelOneTexture;
	sf::Sprite mLevelOneSprite;

	std::vector<sf::Texture> mEnemyTankTextures;
	std::vector<sf::Texture> mEnemyTankProjectiles;
	std::vector<Enemy> mEnemyTank;

	Weather mRain;
	Enemy enemies;

	std::vector<sf::Texture> mCloudTextures;
	std::vector<Weather> mClouds;

	// Inherited via Level
	virtual void removeProjectile() override;
};