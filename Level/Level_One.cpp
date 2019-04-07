#include "stdafx.hpp"
#include "Level_One.hpp"

Level_One::Level_One(sf::RenderWindow* window, std::stack<Level*>* level) : Level(window, level)
{
	this->initLevel();
}

Level_One::~Level_One()
{
	while (!this->pLevel->empty())
	{
		delete this->pLevel->top();

		this->pLevel->pop();
	}
}

void Level_One::update(const float& deltaTime)
{
	this->updateLevel(deltaTime);

	if (this->pLoadLevel == false)
	{
		if (this->pPlayer.getPosition().x > this->pWindow->getSize().x)
		{
			this->pLevel->push(new Level_Two(this->pWindow, this->pLevel));

			this->pLoadLevel = true;
		}
	}
}

void Level_One::render(sf::RenderTarget& target)
{
	target.draw(this->mLevelOneSprite);

	if (this->pHasExploaded)
		target.draw(this->mExpolsionSprite);

	this->pPlayer.render(target);

	this->pEnemyTank.render(target);

	this->mAurora.render(target);

	if (this->pTextTagTimer.isRunning())
		for (size_t i = 0; i < this->pTextTags.size(); i++)
			this->pTextTags[i].render(target);
}

void Level_One::initLevel()
{
	if (!this->mLevelOneTexture.loadFromFile("Resources/Textures/Backgrounds/War3/War3.png"))
		std::cerr << "Level One failed to fucking load" << "\n";

	this->mLevelOneSprite.setTexture(this->mLevelOneTexture);
}