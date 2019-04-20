#include "stdafx.hpp"
#include "Weather.hpp"
#include <random>

Weather::Weather()
{
	if (!mTexture.loadFromFile("Resources/Textures/Weather/snow.png"))
		std::cerr << "Rain failed to load" << "\n";

	mEmitter.setEmissionRate(150.F);
	mEmitter.setParticleLifetime(sf::seconds(5.F));
	mEmitter.setParticlePosition(thor::Distributions::deflect(sf::Vector2f(100.F, 10.F), 100.F));
	mEmitter.setParticleVelocity(thor::Distributions::deflect(sf::Vector2f(500.F, 10.F), 80.F));

	mSystem.setTexture(mTexture);
	mSystem.addEmitter(mEmitter);
	
	thor::FadeAnimation mFader(0.15F, 0.15F);

	mSystem.addAffector(thor::AnimationAffector(mFader));
	mSystem.addAffector(thor::TorqueAffector(100.F));
	mSystem.addAffector(thor::ForceAffector(sf::Vector2f(0.F, 100.F)));
}

Weather::Weather(std::vector<sf::Texture>& texture, sf::Vector2f position)
{
	mClouds = &texture;
	mCloudPosition = position;

	mCloudSprite.setTexture((*mClouds)[0]);
	mCloudSprite.setPosition(position);
	mCloudSprite.setColor(sf::Color(184, 134, 11, 255));
}

Weather::Weather(const Weather& other)
{
	//Particles
	this->mTexture = other.mTexture;
	this->mWindow = other.mWindow;
	this->mTime = other.mTime;
	this->mClock = other.mClock;

	//Clouds
	this->mCloudPosition = other.mCloudPosition;
	this->mCloudsTexture = other.mCloudsTexture;
	this->mCloudSprite = other.mCloudSprite;
	this->mClouds = other.mClouds;

	//Thor stuff
	this->mGradient = other.mGradient;
	this->mEmitter = other.mEmitter;
	this->mSystem(std::move(other.mSystem));
}


Weather::~Weather()
{
}

void Weather::render(sf::RenderTarget& target)
{
	target.draw(mSystem, sf::BlendAdd);
}

void Weather::renderClouds(sf::RenderTarget& target)
{
	target.draw(mCloudSprite);
}

void Weather::update()
{
	mTime = mClock.restart();

	mSystem.update(mTime);
}

void Weather::updateClouds(const float& deltaTime) { mCloudSprite.move(0.5F, 0.F); }

