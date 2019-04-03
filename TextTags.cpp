#include "stdafx.hpp"
#include "TextTags.hpp"

TextTags::TextTags(sf::Font* font, std::string text, sf::Vector2f position, sf::Vector2f direction, sf::Color color, unsigned size) : mDirection(direction)
{
	this->mText.setCharacterSize(size);
	this->mText.setFont(*font);
	this->mText.setPosition(position);
	this->mText.setFillColor(color);
	this->mText.setString(text);
}

TextTags::~TextTags() = default;

void TextTags::render(sf::RenderTarget& target)
{
	target.draw(this->mText);
}