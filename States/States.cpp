#include "stdafx.hpp"
#include "States.hpp"

States::States(sf::RenderWindow* window, std::stack<States*>* states) : pWindow(window), pStates(states)
{
}

States::~States() = default;

void States::updateMousePositions()
{
	this->pMousePositionScreen = sf::Mouse::getPosition();
	this->pMousePositionWindow = sf::Mouse::getPosition(*this->pWindow);
	this->pMousePositionView = this->pWindow->mapPixelToCoords(sf::Mouse::getPosition(*this->pWindow));
}
