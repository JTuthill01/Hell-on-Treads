#include "stdafx.hpp"
#include "Level_Two.hpp"

Level_Two::Level_Two(sf::RenderWindow* window, std::stack<Level*>* level) : Level(window, level)
{
	this->pEnemyTank.setAudio(false);

	this->pWeather.stopAudio();

	this->initLevel();

	this->mFont.loadFromFile("Resources/Fonts/Anton-Regular.ttf");

	this->loadEnemyPlane();

	this->pEnemySpawnTimer.restart(sf::seconds(0.2F));

	this->loadTrees();

	this->pTrees.push_back(Trees::Trees(this->pTreeTextures, sf::Vector2f(1000.F, 1010.F)));
}

Level_Two::~Level_Two()
{
	while (!this->pLevel->empty())
	{
		delete this->pLevel->top();

		this->pLevel->pop();
	}
}

void Level_Two::update(const float& deltaTime)
{
	this->mTextTagTimer.restart(sf::seconds(0.2F));

	if (this->pEnemySpawnTimer.isExpired())
	{
		this->pEnemyPlane.push_back(Enemy(this->mEnemyPlaneTextures, Enemy::mEnemyProjectileTextures, sf::Vector2f(1700.F, (rand() % pWindow->getSize().y) - 150), sf::Vector2f(-0.8F, 0.F), PLANE));

		this->pEnemySpawnTimer.restart(sf::seconds(1.5F));
	}

	this->updateLevel(deltaTime);

	this->mPlayerPlane.update(deltaTime);

	this->mSoldier.update(deltaTime);

	this->input(deltaTime);

	this->collision(deltaTime);

	for (size_t i = 0; i < this->pEnemyPlane.size(); i++)
		this->pEnemyPlane[i].updatePlane(deltaTime);

	for (size_t i = 0; i < this->pEnemyPlane.size(); i++)
		if (this->pEnemyPlane[i].getEnemyPlanePosition().x < 0)
			this->removeEnemyPlane(i);

	this->removeProjectile();

	this->removeEnemyProjectile();
}

void Level_Two::render(sf::RenderTarget & target)
{
	target.draw(this->pBackgroundSprite);

	this->mPlayerPlane.render(target);

	this->mSoldier.render(target);

	for (size_t i = 0; i < this->pEnemyPlane.size(); i++)
		this->pEnemyPlane[i].renderPlane(target);

	if (this->mTextTagTimer.isRunning())
		for (size_t i = 0; i < this->mTextTags.size(); i++)
			this->mTextTags[i]->render(target);

	mTrees.render(target);

	for (size_t i = 0; i < this->pTrees.size(); i++)
		this->pTrees[i].render(target);

	this->renderEndGame(target);
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

	//Player Enemy Collisions (Bomb)
	for (int i = 0; i < this->mPlayerPlane.getProjectileSize(); i++)
	{
		if (this->pCollision.playerEnemyCollision(this->mPlayerPlane.getProjectile(i).getBombSprite(), this->mSoldier.getSoldierSprite(), deltaTime))
		{
			this->mPlayerPlane.setExplosionPosition(sf::Vector2f(this->mSoldier.getPosition().x - 250, this->mSoldier.getPosition().y - 200));

			this->mPlayerPlane.setExplosion(true);

			this->mExplosionTimer.restart(sf::seconds(0.2F));

			this->mTextTags.push_back(new TextTags(&this->mFont, "BOOM!!!", sf::Vector2f(this->mSoldier.getPosition().x + 100, this->mSoldier.getPosition().y - 150), sf::Vector2f(1.F, 2.F), sf::Color::Green,
				30U));

			int damage = this->mPlayerPlane.weaponDamage();

			if (this->mSoldier.getHp() > 0)
				this->mSoldier.takeDamage(damage);

			this->mTextTags.push_back(new TextTags(&this->mFont, " - " + std::to_string(damage), sf::Vector2f(this->mSoldier.getPosition().x + 100, this->mSoldier.getPosition().y - 250), sf::Vector2f(1.F, 2.F), sf::Color::Green,
				30U));
		}
	}

	//Player Plane Enemy Plane Bomb Collisions
	for (int k = 0; k < this->mPlayerPlane.getProjectileSize(); k++)
	{
		for (size_t l = 0; l < this->pEnemyPlane.size(); l++)
		{
			if (this->pCollision.playerEnemyCollision(this->mPlayerPlane.getProjectile(k).getBombSprite(), this->pEnemyPlane[l].getEnemyPlaneSprite(), deltaTime))
			{
				this->mPlayerPlane.setExplosionPosition(sf::Vector2f(this->pEnemyPlane[l].getEnemyPlanePosition().x - 350, this->pEnemyPlane[l].getEnemyPlanePosition().y - 100));

				this->mPlayerPlane.setExplosion(true);

				this->mExplosionTimer.restart(sf::seconds(0.3F));

				int damage = this->mPlayerPlane.weaponDamage();

				this->mTextTags.push_back(new TextTags(&this->mFont, " - " + std::to_string(damage), sf::Vector2f(this->pEnemyPlane[l].getEnemyPlanePosition().x + 150.F, this->pEnemyPlane[l].getEnemyPlanePosition().y
					- 200.F), sf::Vector2f(1.F, 2.F), sf::Color::Green, 30U));

				if (this->pEnemyPlane[l].getPlaneHp() > 0)
				{
					this->pEnemyPlane[l].planeTakeDamage(damage);

					if (this->pEnemyPlane[l].getPlaneHp() == 0)
					{
						this->pEnemyPlane[l].getEnemyPlaneSprite().setPosition(0.F, 1000.F);

						this->removeEnemyPlane(l);

						this->pIsRemoved = true;
					}
				}
			}
		}
	}
		
		//Player Missile Collisions
	for (int i = 0; i < this->mPlayerPlane.getProjectileSize(); i++)
	{
		this->mPlayerPlane.getProjectile(i).update(deltaTime);

		for (size_t j = 0; j < this->pEnemyPlane.size(); j++)
		{
			if (this->pCollision.playerEnemyCollision(this->mPlayerPlane.getProjectile(i).getProjectileSprite(), this->pEnemyPlane[j].getEnemyPlaneSprite(), deltaTime))
			{
				this->mPlayerPlane.setExplosionPosition(sf::Vector2f(this->pEnemyPlane[j].getEnemyPlanePosition().x - 350, this->pEnemyPlane[j].getEnemyPlanePosition().y - 200));

				this->mPlayerPlane.setExplosion(true);

				this->mExplosionTimer.restart(sf::seconds(0.3F));

				int damage = this->mPlayerPlane.weaponDamage();

				this->mTextTags.push_back(new TextTags(&this->mFont, " - " + std::to_string(damage), sf::Vector2f(this->pEnemyPlane[j].getEnemyPlanePosition().x + 150.F, this->pEnemyPlane[j].getEnemyPlanePosition().y
					- 200.F), sf::Vector2f(1.F, 2.F), sf::Color::Green, 30U));

				if (this->pEnemyPlane[j].getPlaneHp() > 0)
				{
					this->pEnemyPlane[j].planeTakeDamage(damage);

					if (this->pEnemyPlane[j].getPlaneHp() == 0)
					{
						this->removeEnemyPlane(j);

						this->pIsRemoved = true;
					}
				}
			}
		}
	}

	//Enemy Missile Collisions
	for (size_t i = 0; i < this->pEnemyPlane.size(); i++)
	{
		for (int j = 0; j < this->pEnemyPlane[i].getEnemyPlaneProjectileSize(); j++)
		{
			if (this->pCollision.playerEnemyCollision(this->pEnemyPlane[i].getEnemyPlaneProjectile(j).getMissileSprite(), this->mPlayerPlane.getPlaneSprite(), deltaTime))
			{
				this->mPlayerPlane.setExplosionPosition(sf::Vector2f(this->mPlayerPlane.getPlanePosition().x - 450, this->mPlayerPlane.getPlanePosition().y - 200));

				this->mPlayerPlane.setExplosion(true);

				this->mExplosionTimer.restart(sf::seconds(0.3F));

				int damage = this->pEnemyPlane[i].enemyPlaneDealDamage();

				if (this->mPlayerPlane.getPlayerHp() > 0)
				{
					this->mPlayerPlane.playerTakeDamage(damage);

					this->pIsEnemyProjectileRemoved = true;
				}

				if (this->mPlayerPlane.getPlayerHp() == 0)
				{
					this->mPlayerPlane.getPlaneSprite().move(0.F, 1100.F);

					this->endGame();
				}
			}
		}
	}

	if (this->mExplosionTimer.isExpired())
		this->mPlayerPlane.setExplosion(false);

	int damage = rand() % 2;

	if (this->pCollision.playerEnemyCollision(this->mPlayerPlane.getExplosionSprite(), this->mSoldier.getSoldierSprite(), deltaTime))
		this->mSoldier.takeDamage(damage);

	if (this->pTextTagTimer.isExpired() && !this->mTextTags.empty())
		this->mTextTags.pop_back();
}

void Level_Two::loadTrees()
{
	sf::Texture temp;
	if (!temp.loadFromFile("Resources/Trees/trees.png"))
		std::cerr << "Trees failed to load" << "\n";

	this->pTreeTextures.push_back(temp);
}

void Level_Two::loadEnemyPlane()
{
	sf::Texture temp;
	temp.loadFromFile("Resources/Textures/Enemy/Fokker.png");
	this->mEnemyPlaneTextures.push_back(temp);

	temp.loadFromFile("Resources/Textures/Enemy/AEG_CIV_default.png");
	this->mEnemyPlaneTextures.push_back(temp);
}

void Level_Two::removeEnemyProjectile()
{
	for (size_t i = 0; i < this->pEnemyPlane.size(); i++)
	{
		for (int j = 0; j < this->pEnemyPlane[i].getEnemyPlaneProjectileSize(); j++)
		{
			if (this->pIsEnemyProjectileRemoved)
			{
				this->pEnemyPlane[i].removeEnemyPlaneProjectile(j);

				this->pIsEnemyProjectileRemoved = false;
			}
		}
	}
}

void Level_Two::removeProjectile()
{
	for (int i = 0; i < this->mPlayerPlane.getProjectileSize(); i++)
	{
		if (this->pIsRemoved)
		{
			this->mPlayerPlane.removeProjectile(i);

			this->pIsRemoved = false;
		}
	}
}