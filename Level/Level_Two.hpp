#pragma once
#include <Level/Level.hpp>
#include <stack>

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
private:
	Player_Plane mPlayerPlane;
};