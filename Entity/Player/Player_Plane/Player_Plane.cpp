#include "stdafx.hpp"
#include "Player_Plane.hpp"

Player_Plane::Player_Plane() : mIsAttacking(false)
{
	this->createMovementComponent(350.f, 16.f, 6.f);
	this->createAnimationComponent(this->mPlanePlayerTexture);

	this->pAnimationComponent->addAnimation("MOVE", 5.f, 0, 0, 1, 0, 435, 435);
	this->pAnimationComponent->addAnimation("ATTACK", 5.f, 0, 1, 1, 1, 435, 435);
}

Player_Plane::~Player_Plane()
{
}

void Player_Plane::render(sf::RenderTarget& target)
{
	target.draw(this->mPlanePlayerSprite);
}

void Player_Plane::update(const float& deltaTime)
{
	this->animations(this->mPlanePlayerSprite, deltaTime, mIsAttacking);
}

void Player_Plane::loadPlayerPlane()
{
	if (!this->mPlanePlayerTexture.loadFromFile("Resources/Textures/Player/Plane/Player_Plane.png"))
		std::cerr << "Whelp you can just fuck right off, as the plane texture does not exist" << "\n";

	this->mPlanePlayerSprite.setTexture(this->mPlanePlayerTexture);
	this->mPlanePlayerSprite.setPosition(0.F, 400.F);
	this->mPlanePlayerSprite.setScale(1.2F, 1.2F);
}

void Player_Plane::createMovementComponent(const float max_velocity, const float acceleration, const float deceleration)
{
	this->pMovementComponent = new MovementComponent(this->mPlanePlayerSprite, max_velocity, acceleration,
		deceleration);
}

void Player_Plane::createAnimationComponent(sf::Texture& texture_sheet)
{
	this->pAnimationComponent = new AnimationComponent(this->mPlanePlayerSprite, texture_sheet);
}