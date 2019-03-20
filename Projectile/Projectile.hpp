#pragma once
#include <Thor/Math.hpp>

class Projectile
{
public:
	Projectile(sf::Texture* texture, sf::Vector2f position, sf::Vector2f scale, sf::Vector2f direction);
	Projectile(sf::Texture* texture, float x, float y, sf::Vector2f scale, sf::Vector2f direction);
	~Projectile();

	void render(sf::RenderTarget& target);
	void update(const float& deltaTime);
	void movement(const float& deltaTime);
	void createMuzzleFlash(sf::Texture& texture, sf::Sprite& sprite, sf::Vector2f position, float time);
	void createBomb(sf::Texture* texture, sf::Vector2f position, sf::Vector2f scale, sf::Vector2f direction);

	inline const sf::Vector2f& getProjectilePosition() { return this->mProjectileSprite.getPosition(); }

private:
	sf::Texture* mBombTexture;
	sf::Sprite mBombSprite;
	sf::Vector2f mBombScale;
	sf::Vector2f mBombDirection;
	sf::Vector2f mBombPosition;

	sf::Texture mMuzzleTexture;
	sf::Sprite mMuzzleSprite;

	sf::Texture* mProjectileTexture;
	sf::Sprite mProjectileSprite;
	sf::Vector2f mPosition;
	sf::Vector2f mScale;
	sf::Vector2f mDirection;

	float mTimer;
	float mTimerMax;
};

