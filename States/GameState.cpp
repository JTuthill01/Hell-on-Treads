#include "stdafx.hpp"
#include "GameState.hpp"

GameState::GameState(sf::RenderWindow* window, std::stack<States*>* states) : States(window, states)
{
	this->initLevel();
}

GameState::~GameState()
{
}

void GameState::update(const float& deltaTime)
{
	for (size_t i = 0; i < this->mLevels.size(); i++)
		this->mLevels[i]->update(deltaTime);
}

void GameState::render(sf::RenderTarget * target)
{
	for (size_t i = 0; i < this->mLevels.size(); i++)
		this->mLevels[i]->render(*target);
}

void GameState::initLevel()
{
	this->mLevels.push_back(new Level_One(this->pWindow, this->mLevels));
}