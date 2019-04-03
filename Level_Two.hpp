#pragma once
#include <Level/Level.hpp>
#include <stack>
#include <Entity/Enemies/Soldier/EnemySoldier.hpp>

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
	Plane mPlayerPlane;
	EnemySoldier mSoldier;

	thor::Timer mExplosionTimer;
};