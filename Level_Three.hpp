#pragma once
#include <Level\Level.hpp>

class Level_Three :
	public Level
{
public:
	Level_Three(sf::RenderWindow* window, std::stack<Level*>* level);
	~Level_Three();

	// Inherited via Level
	virtual void update(const float& deltaTime) override;
	virtual void render(sf::RenderTarget& target) override;
	virtual void initLevel() override;
	virtual void removeProjectile() override;
};

