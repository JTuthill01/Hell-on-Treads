#pragma once

enum button_color { BUTTON_IDLE = 0, BUTTON_HOVER, BUTTON_ACTIVE };

class Button
{
public:
	Button(sf::Vector2f position, sf::Vector2f size, sf::Font* font, std::string text,
		unsigned character_size, sf::Color buttonColor, sf::Color idleButtonColor, sf::Color buttonHoverColor, sf::Color buttonActiveColor, sf::Color textColor);
	~Button();

	void render(sf::RenderTarget& target);
	void update(const sf::Vector2f mousePosition);

	const bool isPressed() const;

private:
	sf::RectangleShape mShape;
	sf::Text mText;
	sf::Font* mFont;

	sf::Vector2f mPosition;
	sf::Vector2f mSize;

	sf::Color mbuttonColor;
	sf::Color mIdleButtonColor;
	sf::Color mHoverButtonColor;
	sf::Color mActiveButtonColor;

	unsigned short mButtonState;
};

