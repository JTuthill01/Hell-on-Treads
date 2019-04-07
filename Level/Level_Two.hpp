#pragma once
#include <Level/Level.hpp>
#include <stack>
#include <Entity/Enemies/Enemy.hpp>
#include <Entity/Enemies/Soldier/EnemySoldier.hpp>
#include <Resources/TextTags.hpp>

class Level_Two :
	public Level
{
public:
	Level_Two(sf::RenderWindow* window, std::stack<Level*>* level);
	virtual ~Level_Two();

	// Inherited via Level
	virtual void update(const float& deltaTime) override;
	virtual void render(sf::RenderTarget& target) override;
	virtual void initLevel() override;

	void input(const float& deltaTime);
	void collision(const float& deltaTime);

private:
	void loadEnemyPlane();
	void loadEnemyPlaneProjectiles();

	std::vector<sf::Texture> mEnemyPlaneProjectilesTextures;
	std::vector<sf::Texture> mEnemyPlaneTextures;
	EnemySoldier mSoldier;

	Plane mPlayerPlane;

	thor::Timer mExplosionTimer;
	thor::Timer mTextTagTimer;

	std::vector<TextTags*> mTextTags;

	sf::Font mFont;
};