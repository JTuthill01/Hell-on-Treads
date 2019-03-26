#pragma once
#include <Entity/Player/Player.hpp>
#include <Entity/Enemies/EnemyTank.hpp>
#include <Collision/Collision.hpp>

namespace AuroraLoader
{
	class AuroraLoader
	{
	public:
		AuroraLoader();
		~AuroraLoader();

		void setAurora(const float& deltaTime);
		void render(sf::RenderTarget& target);

	private:
		Player mPlayer;
		EnemyTank mEnemy;
		Collision mCollision;

		sf::Texture mAuroraFireTexture;
		sf::Texture mAuroraIceTexture;
		sf::Texture mAuroraCorrosionTexture;
		sf::Sprite mAuroraSprite;
	};

}