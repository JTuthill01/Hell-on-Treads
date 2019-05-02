#include "stdafx.hpp"
#include "Level.hpp"

std::vector<sf::Texture> Level::pParticleTextures;

Level::Level(sf::RenderWindow* window, std::stack<Level*>* level) : pWindow(window), pLevel(level), pLoadLevel(false), pHasExploaded(false), pIsPlayerProjectileRemoved(false), pShouldClose(false)
{
	this->pFont.loadFromFile("Resources/Fonts/Anton-Regular.ttf");
}

Level::~Level()
{
	while (!this->pLevel->empty())
	{
		delete this->pLevel->top();

		this->pLevel->pop();
	}

	delete this->pWindow;
}

void Level::removeTrees(unsigned index) { this->pTrees.erase(this->pTrees.begin() + index); }

void Level::loadTrees()
{
	sf::Texture temp;
	if (!temp.loadFromFile("Resources/Trees/trees.png"))
		std::cerr << "Trees failed to load" << "\n";

	this->pTree.treeTextures.push_back(temp);
}

void Level::endGameInput()
{
	if (pShouldClose == true)
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			this->pWindow->close();
}

void Level::endGame()
{
	this->mText.setFont(pFont);
	this->mText.setCharacterSize(150U);
	this->mText.setFillColor(sf::Color::Red);
	this->mText.setPosition(sf::Vector2f(static_cast<float>(this->pWindow->getSize().x / 2) - 700.F, static_cast<float>(this->pWindow->getSize().y / 2) - 300.F));
	this->mText.setString("GAME THE FUCK OVER!!!!!");

	this->pAudio.loadSound("Resources/Sounds/Game Over/Fuck.ogg");
	this->pAudio.play();

	this->mResetText.setFont(pFont);
	this->mResetText.setCharacterSize(100U);
	this->mResetText.setFillColor(sf::Color::Blue);
	this->mResetText.setPosition(sf::Vector2f(static_cast<float>(this->pWindow->getSize().x / 2.F) - 750.F, static_cast<float>(this->pWindow->getSize().y / 2.F) - 100.F));
	this->mResetText.setString("Press Esc to quit "  " Press F1 to restart");

	this->pWindow->draw(this->mResetText);

	this->pWindow->draw(this->mText);

	this->pShouldClose = true;
}

void Level::renderEndGame(sf::RenderTarget & target)
{
	target.draw(this->mText);

	target.draw(this->mResetText);
}

void Level::removeEnemyPlane(unsigned index) { this->pEnemyPlane.erase(this->pEnemyPlane.begin() + index); }


void Level::updateLevel(const float& deltaTime)
{
	for (size_t i = 0; i < this->pParticle.size(); i++)
		this->pParticle[i].update(deltaTime);

	this->pEnemyTank.update(deltaTime);
}

void Level::setExplosions(sf::Vector2f position, sf::Vector2f scale)
{
	if (!this->mExpolsionTexture.loadFromFile("Resources/Textures/Explosions/explosion1.png"))
		std::cerr << "Whelp you can go fuck yourself, as the fucking explosion texture didn't load";

	this->mExpolsionSprite.setTexture(this->mExpolsionTexture);
	this->mExpolsionSprite.setPosition(position);
	this->mExpolsionSprite.setScale(scale);

	this->timer();
}

void Level::timer()
{
	this->pTextTagTimer.restart(sf::seconds(0.2F));

	this->mExplosionTimer.reset(sf::seconds(0.3F));
	this->mExplosionTimer.start();

	this->mParticleTimer.reset(sf::seconds(6.F));
	this->mParticleTimer.start();
}