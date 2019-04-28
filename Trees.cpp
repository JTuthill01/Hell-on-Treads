#include "stdafx.hpp"
#include "Trees.hpp"

namespace Trees
{
	Trees::Trees() : mHp(2), mIsAlive(true), mTextures(nullptr)
	{
		if (!mTreeTexture.loadFromFile("Resources/Trees/trees.png"))
			std::cerr << "Texture not found" << "\n";

		mTreeSprite.setTexture(mTreeTexture);
		mTreeSprite.setPosition(sf::Vector2f(700, 210));
		mTreeSprite.setScale(2.F, 2.F);
		mTreeSprite.setColor(sf::Color::Yellow);
	}

	Trees::Trees(std::vector<sf::Texture>& textures, sf::Vector2f position) : mHp(2), mIsAlive(true)
	{
		mTextures = &textures;
		mPosition = position;

		mTreeSprite.setTexture((*mTextures)[0]);
		mTreeSprite.setPosition(position);
		mTreeSprite.setScale(4.F, 4.F);
		mTreeSprite.setColor(sf::Color::Yellow);
	}

	Trees::~Trees() = default;

	void Trees::render(sf::RenderTarget& target) { target.draw(mTreeSprite); }

	void Trees::damaged(int damage)
	{
		mHp -= damage;

		if (mHp <= 0)
		{
			mHp = 0;

			mIsAlive = false;
		}
	}
}