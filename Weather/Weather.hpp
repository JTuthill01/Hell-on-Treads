#pragma once
class Weather
{
public:
	Weather();
	Weather(std::vector<sf::Texture>& texture, sf::Vector2f position);
	Weather(const Weather& other);
	~Weather();

	void render(sf::RenderTarget& target);
	void renderClouds(sf::RenderTarget& target);
	void update();
	void updateClouds(const float& deltaTime);

	inline sf::Vector2f getCloudPosition() { return mCloudSprite.getPosition(); }

private:
	//Particles
	sf::Texture mTexture;
	sf::RenderWindow* mWindow;
	sf::Time mTime;
	sf::Clock mClock;

	//Clouds
	sf::Vector2f mCloudPosition;
	sf::Texture mCloudsTexture;
	sf::Sprite mCloudSprite;
	std::vector<sf::Texture>* mClouds;

	//Thor stuff
	thor::ColorGradient mGradient;
	thor::UniversalEmitter mEmitter;
	thor::ParticleSystem mSystem;

	
};

