#include "stdafx.hpp"
#include "MainMenuState.hpp"

MainMenuState::MainMenuState(sf::RenderWindow* window, std::stack<States*>* states) : States(window, states)
{
	this->initTextures();
	this->initFonts();
	this->createButtons();
	this->setText();
}

MainMenuState::~MainMenuState()
{
	auto it = this->mButton.begin();

	for (it = this->mButton.begin(); it != this->mButton.end(); ++it)
		delete it->second;
}

void MainMenuState::update(const float& deltaTime)
{
	this->updateMousePositions();

	for (auto& it : this->mButton)
		it.second->update(this->pMousePositionView);

	if (this->mButton["PLAY"]->isPressed())
		this->pStates->push(new GameState(this->pWindow, this->pStates));

	if (this->mButton["EXIT"]->isPressed())
		this->pWindow->close();
}

void MainMenuState::render(sf::RenderTarget * target)
{
	target->draw(this->mBackgroundSprite);

	target->draw(this->mText);

	this->renderButtons(*target);
}

void MainMenuState::initTextures()
{
	if (!this->mBackgroundTexture.loadFromFile("Resources/Textures/Backgrounds/war2.png"))
	{
		std::cerr << "Failed to load the fucking background texture" << "\n";

		EXIT_FAILURE;
	}

	this->mBackgroundSprite.setTexture(this->mBackgroundTexture);
}

void MainMenuState::initFonts()
{
	if (!this->mFont.loadFromFile("Resources/Fonts/Anton-Regular.ttf"))
	{
		std::cerr << "Font failed to load" << "\n";

		EXIT_FAILURE;
	}

	if (!this->mFont1.loadFromFile("Resources/Fonts/MetalMacabre.ttf"))
	{
		std::cerr << "Font failed to load" << "\n";

		EXIT_FAILURE;
	}
}

void MainMenuState::createButtons()
{
	this->mButton["PLAY"] = new Button(sf::Vector2f(400, 600), sf::Vector2f(150, 50), &this->mFont, "Play", 30U, sf::Color(150, 150, 150, 200), sf::Color(150, 150, 150, 200), sf::Color(150, 150, 150, 255),
		sf::Color::Red, sf::Color::White);

	this->mButton["EXIT"] = new Button(sf::Vector2f(400, 700), sf::Vector2f(150, 50), &this->mFont, "Pussy Out", 30U, sf::Color(150, 150, 150, 200), sf::Color(150, 150, 150, 200), sf::Color(150, 150, 150, 255),
		sf::Color::Red, sf::Color::White);
}

void MainMenuState::renderButtons(sf::RenderTarget & target)
{
	for (auto& it : this->mButton)
		it.second->render(target);
}

void MainMenuState::setText()
{
	this->mText.setFillColor(sf::Color(255, 255, 255, 190));
	this->mText.setCharacterSize(100U);
	this->mText.setFont(this->mFont1);
	this->mText.setString("Hell on Treads");
	this->mText.setPosition(sf::Vector2f(static_cast<float>(this->pWindow->getSize().x) / 2 - 450, static_cast<float>(this->pWindow->getSize().y) - 800.F));
}