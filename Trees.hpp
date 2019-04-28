#pragma once

namespace Trees
{
	class Trees
	{
	public:
		Trees();
		Trees(std::vector<sf::Texture>& textures, sf::Vector2f position);
		~Trees();

		void render(sf::RenderTarget& target);
		void damaged(int damage);

		inline sf::Vector2f getPosition() { return mTreeSprite.getPosition(); }
		inline sf::Sprite getTreeSprite() { mTreeSprite; }
		inline int getTreeHp() { return mHp; }

	private:

		bool mIsAlive;

		int mHp;

		std::vector<sf::Texture>* mTextures;

		sf::Vector2f mPosition;
		sf::Texture mTreeTexture;
		sf::Sprite mTreeSprite;
	};

}