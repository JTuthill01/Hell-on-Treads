#include "stdafx.hpp"
#include "AuroraLoader.hpp"

namespace AuroraLoader
{
	AuroraLoader::AuroraLoader()
	{

	}

	AuroraLoader::~AuroraLoader() = default;

	void AuroraLoader::setAurora(const float& deltaTime)
	{
		if (this->mPlayer.getWeaponLevel() == FIRE)
		{
			if (!this->mAuroraFireTexture.loadFromFile("Resources/Textures/Aurora/auroraFire.png"))
				std::cerr << "Fire texture not found" << "\n";

			this->mAuroraSprite.setTexture(this->mAuroraFireTexture);
			this->mAuroraSprite.setPosition(sf::Vector2f(1000.F, 710.F));
		}

		if (this->mPlayer.getWeaponLevel() == ICE)
		{
			if (!this->mAuroraIceTexture.loadFromFile("Resources/Textures/Aurora/auroraIce.png"))
				std::cerr << "Ice texture not found" << "\n";

			this->mAuroraSprite.setTexture(this->mAuroraIceTexture);
			this->mAuroraSprite.setPosition(sf::Vector2f(1000.F, 710.F));
		}

		if (this->mPlayer.getWeaponLevel() == CORROSIVE)
		{

			if (!this->mAuroraCorrosionTexture.loadFromFile("Resources/Textures/Aurora/auroraCorrosion.png"))
				std::cerr << "Corrosion texture not found" << "\n";

			this->mAuroraSprite.setTexture(this->mAuroraCorrosionTexture);
			this->mAuroraSprite.setPosition(sf::Vector2f(1000.F, 710.F));

		}
	}
	void AuroraLoader::render(sf::RenderTarget& target) { target.draw(this->mAuroraSprite); }
}