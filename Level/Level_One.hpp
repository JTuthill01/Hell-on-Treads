#pragma once
#include <Entity/Player.hpp>
#include <Entity/Enemies/Enemies.hpp>
#include "Level.hpp"

class Level_One :
	public Level
{
public:
	Level_One(sf::RenderWindow * window, std::stack<Level*>* level);
	virtual ~Level_One();

	// Inherited via Level
	virtual void update(const float & deltaTime) override;
	virtual void render(sf::RenderTarget & target) override;
	virtual void initLevel() override;

private:
	sf::Texture mLevelOneTexture;
	sf::Sprite mLevelOneSprite;

	Enemies enemies;
};

