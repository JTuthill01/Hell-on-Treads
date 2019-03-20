#pragma once
#include <States/States.hpp>
#include <States/MainMenuState.hpp>

class Game
{
public:
	Game();
	~Game();

	void run();

private:
	void initWindow();
	void render();
	void update(const float& deltaTime);
	void updateDeltaTime();
	void initStates();
	void events();

	bool isClosed;

	float mDeltaTime;

	sf::RenderWindow* mWindow;
	sf::Event e;
	sf::Clock mClock;

	std::stack<States*> mStates;
};

