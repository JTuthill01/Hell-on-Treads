#pragma once
#include <Entity/Entity.hpp>

class Player_Plane :
	public Entity
{
public:
	Player_Plane();
	virtual ~Player_Plane();

	void render(sf::RenderTarget& target);
	void update(const float& deltaTime);

private:
	void loadPlayerPlane();
	void createMovementComponent(const float max_velocity,
		const float acceleration, const float deceleration);
	void createAnimationComponent(sf::Texture& texture_sheet);

	sf::Texture mPlanePlayerTexture;
	sf::Sprite mPlanePlayerSprite;

	bool mIsAttacking;
};