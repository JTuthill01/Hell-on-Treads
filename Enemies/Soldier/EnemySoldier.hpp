#pragma once
#include <Entity/Entity.hpp>

class EnemySoldier : Entity
{
public:
	EnemySoldier();
	~EnemySoldier();

	void update(const float& deltaTime);
	void render(sf::RenderTarget& target);
	void move(const float direction_x, const float direction_y, const float& deltaTime);
	void takeDamage(int damage);

	inline int getHp() { return this->mHp; }
	inline sf::Sprite& getSoldierSprite() { return this->mEnemySoldierSprite; }
	inline sf::FloatRect getGobalBounds()const { return this->mEnemySoldierSprite.getGlobalBounds(); }
	inline sf::Vector2f getPosition() { return this->mEnemySoldierSprite.getPosition(); }

private:
	sf::Texture mEnemySoldierTexture;
	sf::Sprite mEnemySoldierSprite;

	void load();
	void createMovementComponent(const float max_velocity,
		const float acceleration, const float deceleration);
	void createAnimationComponent(sf::Texture& texture_sheet);

	int mHp;
	int mHpMax;

	bool mIsAlive;
};