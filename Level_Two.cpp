#include "stdafx.hpp"
#include "Level_Two.hpp"

Level_Two::Level_Two(sf::RenderWindow* window, std::stack<Level*>* level) : Level(window, level)
{
	this->initLevel();

	this->mFont.loadFromFile("Resources/Fonts/Anton-Regular.ttf");

	this->pEnemySpawnTimer.restart(sf::seconds(0.4F));

	this->loadEnemyPlane();

	this->loadEnemyPlaneProjectiles();
}

Level_Two::~Level_Two() = default;

void Level_Two::update(const float& deltaTime)
{
	this->updateLevel(deltaTime);

	this->mPlayerPlane.update(deltaTime);

	this->mSoldier.update(deltaTime);

	this->input(deltaTime);

	this->collision(deltaTime);

	/*if (this->pEnemySpawnTimer.isExpired())*/
	this->mEnemyPlane.push_back(Enemy(this->mEnemyPlaneTextures, this->mEnemyPlaneProjectilesTextures, pWindow->getSize(), sf::Vector2f(0.F, 0.F), sf::Vector2f(-0.7F, 0.F), PLANE));

	for (size_t i = 0; i < this->mEnemyPlane.size(); i++)
		this->mEnemyPlane[i].update(deltaTime);
}

void Level_Two::render(sf::RenderTarget & target)
{
	target.draw(this->pBackgroundSprite);

	this->mPlayerPlane.render(target);

	this->mSoldier.render(target);

	for (size_t i = 0; i < this->mEnemyPlane.size(); i++)
		this->mEnemyPlane[i].render(target);

	if (this->mTextTagTimer.isRunning())
		for (size_t i = 0; i < this->mTextTags.size(); i++)
			this->mTextTags[i]->render(target);
}

void Level_Two::initLevel()
{
	if (!this->pBackgroundTexture.loadFromFile("Resources/Textures/Backgrounds/War4/000.png"))
		std::cerr << "Fucking level, dun fucked up and failed to load" << "\n";

	this->pBackgroundSprite.setTexture(this->pBackgroundTexture);
}

void Level_Two::input(const float& deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		this->mPlayerPlane.move(0.5F, 0.F, deltaTime);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		this->mPlayerPlane.move(-0.5F, 0.F, deltaTime);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		this->mPlayerPlane.move(0.F, -0.5F, deltaTime);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		this->mPlayerPlane.move(0.F, 0.5F, deltaTime);
}

void Level_Two::collision(const float& deltaTime)
{
	for (int i = 0; i < this->mPlayerPlane.getProjectileSize(); i++)
		if (this->mPlayerPlane.getProjectile(i).getBombPosition().y > this->pWindow->getSize().y)
			this->mPlayerPlane.removeProjectile(i);

	for (int i = 0; i < this->mPlayerPlane.getProjectileSize(); i++)
	{
		if (pCollision.playerEnemyCollision(this->mPlayerPlane.getProjectile(i).getBombSprite(), this->mSoldier.getSoldierSprite(), deltaTime))
		{
			this->mPlayerPlane.setExplosionPosition(sf::Vector2f(this->mSoldier.getPosition().x - 250, this->mSoldier.getPosition().y - 200));

			this->mPlayerPlane.setExplosion(true);

			this->mExplosionTimer.restart(sf::seconds(0.3F));

			this->mPlayerPlane.removeProjectile(i);

			mTextTags.push_back(new TextTags(&this->mFont, "BOOM!!!", sf::Vector2f(this->mSoldier.getPosition().x + 100, this->mSoldier.getPosition().y - 150), sf::Vector2f(1.F, 2.F), sf::Color::Green,
				30U));

			this->mTextTagTimer.restart(sf::seconds(0.4F));

			int damage = this->mPlayerPlane.weaponDamage();

			if (mSoldier.getHp() > 0)
				this->mSoldier.takeDamage(damage);

			mTextTags.push_back(new TextTags(&this->mFont, " - " + std::to_string(damage), sf::Vector2f(this->mSoldier.getPosition().x + 100, this->mSoldier.getPosition().y - 250), sf::Vector2f(1.F, 2.F), sf::Color::Green,
				30U));
		}
	}

	if (this->mExplosionTimer.isExpired())
		this->mPlayerPlane.setExplosion(false);

	int damage = rand() % 2;

	if (pCollision.playerEnemyCollision(this->mPlayerPlane.getExplosionSprite(), this->mSoldier.getSoldierSprite(), deltaTime))
		this->mSoldier.takeDamage(damage);

	if (this->mTextTagTimer.isExpired() && !this->mTextTags.empty())
		this->mTextTags.pop_back();
}

void Level_Two::loadEnemyPlane()
{
	sf::Texture temp;
	if (!temp.loadFromFile("Resources/Textures/Enemy/Fokker.png"))
		std::cerr << "No texture for plane found" << "\n";

	this->mEnemyPlaneTextures.push_back(temp);

	this->pEnemySpawnTimer.restart(sf::seconds(0.4F));
}

void Level_Two::loadEnemyPlaneProjectiles()
{
	sf::Texture temp;
	if (!temp.loadFromFile("Resources/Textures/Bullets/missile(2).png"))
		std::cerr << "No projectile texture found" << "\n";
	this->mEnemyPlaneProjectilesTextures.push_back(temp);
}