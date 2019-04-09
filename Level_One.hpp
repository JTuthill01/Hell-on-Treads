#pragma once
#include <Entity/Player/Player.hpp>
#include <Entity/Enemies/Enemy.hpp>
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
	void loadTank();
	void playerInput(const float& deltaTime);

	sf::Texture mLevelOneTexture;
	sf::Sprite mLevelOneSprite;

	std::vector<sf::Texture> mEnemyTankTextures;
	std::vector<sf::Texture> mEnemyTankProjectiles;
	std::vector<Enemy> mEnemyTank;

	Enemy enemies;

	// Inherited via Level
	virtual void removeProjectile() override;
};