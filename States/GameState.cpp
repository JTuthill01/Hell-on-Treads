#include "stdafx.hpp"
#include "GameState.hpp"

GameState::GameState(sf::RenderWindow* window, std::stack<States*>* states) : States(window, states)
{
	this->initLevel();
}

GameState::~GameState()
{
	while (!this->mLevels.empty())
	{
		delete this->mLevels.top();
		this->mLevels.pop();
	}

	while (!this->pStates->empty())
	{
		this->pStates->pop();

		delete this->pStates->top();
	}
}

void GameState::update(const float& deltaTime)
{
	if (!this->mLevels.empty())
		this->mLevels.top()->update(deltaTime);
}

void GameState::render(sf::RenderTarget* target)
{
	if (!this->mLevels.empty())
		this->mLevels.top()->render(*target);
}

void GameState::initLevel()
{
	this->mLevels.push(new Level_One(this->pWindow, &this->mLevels));
}