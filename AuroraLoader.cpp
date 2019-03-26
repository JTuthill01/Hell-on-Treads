#include "stdafx.hpp"
#include "AuroraLoader.hpp"

namespace AuroraLoader
{
	AuroraLoader::AuroraLoader()
	{
		//this->loadAurora();
	}

	AuroraLoader::~AuroraLoader() = default;

	void AuroraLoader::loadAurora()
	{
		if (!this->mAuroraFireTexture.loadFromFile("Resources/Textures/Aurora/auroraFire.png"))
			std::cerr << "Fire texture not found" << "\n";

		if (!this->mAuroraIceTexture.loadFromFile("Resources/Textures/Aurora/auroraIce.png"))
			std::cerr << "Ice texture not found" << "\n";

		if (!this->mAuroraFireTexture.loadFromFile("Resources/Textures/Aurora/auroraCorrosion.png"))
			std::cerr << "Corrosion texture not found" << "\n";
	}

	void AuroraLoader::setAurora(const float& deltaTime)
	{
		this->loadAurora();

		if (this->mPlayer.getWeaponLevel() == ICE)
		{
			this->mAuroraSprite.setTexture(this->mAuroraFireTexture);
			this->mAuroraSprite.setPosition(sf::Vector2f(1000.F, 710.F));
			
		}
	}
	void AuroraLoader::render(sf::RenderTarget& target) { target.draw(this->mAuroraSprite); }

	//void AuroraLoader::update(const float& deltaTime) { this->setAurora(deltaTime); }
}