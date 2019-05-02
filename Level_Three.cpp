#include "stdafx.hpp"
#include "Level_Three.hpp"

Level_Three::Level_Three(sf::RenderWindow* window, std::stack<Level*>* level) : Level(window, level)
{
	

}

Level_Three::~Level_Three()
{
	/*while (!this->pLevel->empty())
	{
		delete this->pLevel->top();

		this->pLevel->pop();
	}*/
}

void Level_Three::update(const float& deltaTime)
{
}

void Level_Three::render(sf::RenderTarget& target)
{
}

void Level_Three::initLevel()
{
}

void Level_Three::removeProjectile()
{
}
