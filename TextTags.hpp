#pragma once
#include <Thor/Time/Timer.hpp>

class TextTags
{
public:
	TextTags(sf::Font* font, std::string text, sf::Vector2f position, sf::Vector2f direction, sf::Color color, unsigned size);
	~TextTags();

	void render(sf::RenderTarget& target);

private:
	sf::Font* mFont;
	sf::Text mText;
	sf::Color mColor;
	sf::Vector2f mDirection;

	thor::Timer mTimer;
};