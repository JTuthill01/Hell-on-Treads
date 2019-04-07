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
	sf::Texture mLevelOneTexture;
	sf::Sprite mLevelOneSprite;

	Enemy enemies;
};