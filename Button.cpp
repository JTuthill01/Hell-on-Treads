#include "stdafx.hpp"
#include "Button.hpp"

Button::Button(sf::Vector2f position, sf::Vector2f size, sf::Font * font, std::string text, unsigned character_size, sf::Color buttonColor, sf::Color idleButtonColor, 
	sf::Color buttonHoverColor, sf::Color buttonActiveColor, sf::Color textColor) : mPosition(position), mSize(size), mFont(font), mbuttonColor(buttonColor), mIdleButtonColor(idleButtonColor),
		mHoverButtonColor(buttonHoverColor), mActiveButtonColor(buttonActiveColor)
{
	this->mShape.setFillColor(buttonColor);
	this->mShape.setSize(size);
	this->mShape.setPosition(position);
	this->mShape.setOutlineColor(buttonColor);
	this->mShape.setOutlineThickness(1U);

	this->mText.setFont(*this->mFont);
	this->mText.setCharacterSize(character_size);
	this->mText.setString(text);
	this->mText.setFillColor(sf::Color::White);
	this->mText.setPosition(sf::Vector2f(this->mShape.getPosition().x + (this->mShape.getGlobalBounds().width / 2.f)
		- this->mText.getGlobalBounds().width / 2.f,
		this->mShape.getPosition().y + (this->mShape.getGlobalBounds().height / 2.2f) -
		this->mText.getGlobalBounds().height / 2.f));
}

Button::~Button() = default;

void Button::render(sf::RenderTarget & target)
{
	target.draw(this->mShape);
	target.draw(this->mText);
}

void Button::update(const sf::Vector2f mousePosition)
{
	this->mButtonState = BUTTON_IDLE;

	if (this->mShape.getGlobalBounds().contains(mousePosition))
	{
		this->mButtonState = BUTTON_HOVER;

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			this->mButtonState = BUTTON_ACTIVE;
	}

	switch (this->mButtonState)
	{
	case BUTTON_IDLE:
		this->mShape.setFillColor(this->mIdleButtonColor);
		this->mText.setFillColor(sf::Color::White);
		break;

	case BUTTON_HOVER:
		this->mShape.setFillColor(this->mHoverButtonColor);
		this->mText.setFillColor(sf::Color::White);
		break;

	case BUTTON_ACTIVE:
		this->mShape.setFillColor(this->mActiveButtonColor);
		this->mText.setFillColor(sf::Color::White);
		break;

	default:
		break;
	}
}

const bool Button::isPressed() const
{
	if (this->mButtonState == BUTTON_ACTIVE)
		return true;

	return false;
}

