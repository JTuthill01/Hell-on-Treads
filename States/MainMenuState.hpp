#pragma once
#include "GameState.hpp"
#include "States.hpp"
#include <Resources/Button.hpp>

class MainMenuState :
	public States
{
public:
	MainMenuState(sf::RenderWindow* window, std::stack<States*>* states);
	~MainMenuState();

	// Inherited via States
	virtual void update(const float & deltaTime) override;
	virtual void render(sf::RenderTarget * target = nullptr) override;

private:
	void initTextures();
	void initFonts();
	void createButtons();
	void renderButtons(sf::RenderTarget& target);
	void setText();

	sf::Text mText;
	sf::Font mFont;
	sf::Font mFont1;
	sf::Texture mBackgroundTexture;
	sf::Sprite mBackgroundSprite;
	sf::Event e;

	std::map<std::string, Button*> mButton;
};

