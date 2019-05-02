#include "stdafx.hpp"
#include "Level_Two.hpp"

Level_Two::Level_Two(sf::RenderWindow* window, std::stack<Level*>* level) : Level(window, level), mTreeCount(5), min(1), max(1080), mYAxis(1920), mIsTreeRemoved(false), mHeight(1000)
{
	this->pEnemyTank.setAudio(false);

	this->pWeather.stopAudio();

	this->initLevel();

	this->mFont.loadFromFile("Resources/Fonts/Anton-Regular.ttf");

	this->loadEnemyPlane();

	this->pEnemySpawnTimer.restart(sf::seconds(0.2F));

	this->setGround();

	//std::cout << level->size() << "\n";
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
		this->pEnemyPlane.push_back(Enemy(this->mEnemyPlaneTextures, Enemy::mEnemyProjectileTextures, sf::Vector2f(1700.F, (thor::random(min, mYAxis)) - 150), sf::Vector2f(-0.8F, 0.F), PLANE));

		this->pEnemySpawnTimer.restart(sf::seconds(1.2F));
	}

	if (this->pLoadLevel == false)
	{
		if (this->pPlayer.getPosition().x > this->pWindow->getSize().x)
		{
			this->pLevel->emplace(new Level_Three(this->pWindow, this->pLevel));

			this->pLoadLevel = true;
		}
	}

	this->updateLevel(deltaTime);

	this->pPlayerPlane.update(deltaTime);

	this->mSoldier.update(deltaTime);

	this->input(deltaTime);

	this->pixelCollision(deltaTime);

	for (size_t i = 0; i < this->pEnemyPlane.size(); i++)
		this->pEnemyPlane[i].updatePlane(deltaTime);

	for (size_t i = 0; i < this->pEnemyPlane.size(); i++)
		if (this->pEnemyPlane[i].getEnemyPlanePosition().x < 0)
			this->removeEnemyPlane(i);

	this->removeProjectile();

	this->removeEnemyProjectile();

	this->endGameInput();
}

void Level_Two::render(sf::RenderTarget & target)
{
	target.draw(this->pBackgroundSprite);

	target.draw(this->pGroundSprite);

	this->pPlayerPlane.render(target);

	this->mSoldier.render(target);

	for (size_t i = 0; i < this->pEnemyPlane.size(); i++)
		this->pEnemyPlane[i].renderPlane(target);

	if (this->mTextTagTimer.isRunning())
		for (size_t i = 0; i < this->mTextTags.size(); i++)
			this->mTextTags[i]->render(target);

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
		this->pPlayerPlane.move(0.5F, 0.F, deltaTime);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		this->pPlayerPlane.move(-0.5F, 0.F, deltaTime);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		this->pPlayerPlane.move(0.F, -0.5F, deltaTime);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		this->pPlayerPlane.move(0.F, 0.5F, deltaTime);
}

void Level_Two::setGround()
{
	if (!this->pGroundTexture.loadFromFile("Resources/Textures/Backgrounds/road.png"))
		std::cerr << "Ground didn't load" << "\n";

	this->pGroundSprite.setTexture(this->pGroundTexture);
	this->pGroundSprite.setPosition(sf::Vector2f(0.F, 0.F));
}

void Level_Two::pixelCollision(const float& deltaTime)
{
	//Player Missile Collisions
	for (int i = 0; i < this->pPlayerPlane.getProjectileSize(); i++)
	{
		this->pPlayerPlane.getProjectile(i).update(deltaTime);

		for (size_t j = 0; j < this->pEnemyPlane.size(); j++)
		{
			if (PixelCollision::BoundingBoxTest(this->pPlayerPlane.getProjectile(i).getProjectileSprite(), this->pEnemyPlane[j].getEnemyPlaneSprite()))
			{
				this->pPlayerPlane.setExplosionPosition(sf::Vector2f(this->pEnemyPlane[j].getEnemyPlanePosition().x - 350, this->pEnemyPlane[j].getEnemyPlanePosition().y - 200));

				this->pPlayerPlane.setExplosion(true);

				this->mExplosionTimer.restart(sf::seconds(0.3F));

				int damage = this->pPlayerPlane.weaponDamage();

				this->mTextTags.push_back(new TextTags(&this->mFont, " - " + std::to_string(damage), sf::Vector2f(this->pEnemyPlane[j].getEnemyPlanePosition().x + 150.F, this->pEnemyPlane[j].getEnemyPlanePosition().y
					- 200.F), sf::Vector2f(1.F, 2.F), sf::Color::Green, 30U));

				if (this->pEnemyPlane[j].getPlaneHp() > 0)
				{
					this->pEnemyPlane[j].planeTakeDamage(damage);

					this->pIsPlayerProjectileRemoved = true;

					//std::cout << this->pEnemyPlane[j].getPlaneHp() << "\n";
				}

				if (this->pEnemyPlane[j].getPlaneHp() == 0)
				{
					this->removeEnemyPlane(j);

					this->pIsPlayerProjectileRemoved = true;
				}
			}
		}
	}

	//Player Enemy Collisions (Bomb)
	for (int i = 0; i < this->pPlayerPlane.getProjectileSize(); i++)
	{
		if (PixelCollision::BoundingBoxTest(this->pPlayerPlane.getProjectile(i).getBombSprite(), this->mSoldier.getSoldierSprite()))
		{
			this->pPlayerPlane.setExplosionPosition(sf::Vector2f(this->mSoldier.getPosition().x - 250, this->mSoldier.getPosition().y - 200));

			this->pPlayerPlane.setExplosion(true);

			this->mExplosionTimer.restart(sf::seconds(0.2F));

			this->mTextTags.push_back(new TextTags(&this->mFont, "BOOM!!!", sf::Vector2f(this->mSoldier.getPosition().x + 100, this->mSoldier.getPosition().y - 150), sf::Vector2f(1.F, 2.F), sf::Color::Green,
				30U));

			int damage = this->pPlayerPlane.weaponDamage();

			if (this->mSoldier.getHp() > 0)
				this->mSoldier.takeDamage(damage);

			this->mTextTags.push_back(new TextTags(&this->mFont, " - " + std::to_string(damage), sf::Vector2f(this->mSoldier.getPosition().x + 100, this->mSoldier.getPosition().y - 250), sf::Vector2f(1.F, 2.F), sf::Color::Green,
				30U));
		}
	}

	//Player Plane Enemy Plane Bomb Collisions
	for (int k = 0; k < this->pPlayerPlane.getProjectileSize(); k++)
	{
		for (size_t l = 0; l < this->pEnemyPlane.size(); l++)
		{
			if (PixelCollision::BoundingBoxTest(this->pPlayerPlane.getProjectile(k).getBombSprite(), this->pEnemyPlane[l].getEnemyPlaneSprite()))
			{
				this->pPlayerPlane.setExplosionPosition(sf::Vector2f(this->pEnemyPlane[l].getEnemyPlanePosition().x - 350, this->pEnemyPlane[l].getEnemyPlanePosition().y - 100));

				this->pPlayerPlane.setExplosion(true);

				this->mExplosionTimer.restart(sf::seconds(0.3F));

				int damage = this->pPlayerPlane.weaponDamage();

				this->mTextTags.push_back(new TextTags(&this->mFont, " - " + std::to_string(damage), sf::Vector2f(this->pEnemyPlane[l].getEnemyPlanePosition().x + 150.F, this->pEnemyPlane[l].getEnemyPlanePosition().y
					- 200.F), sf::Vector2f(1.F, 2.F), sf::Color::Green, 30U));

				if (this->pEnemyPlane[l].getPlaneHp() > 0)
				{
					this->pEnemyPlane[l].planeTakeDamage(damage);

					if (this->pEnemyPlane[l].getPlaneHp() == 0)
					{
						this->pEnemyPlane[l].getEnemyPlaneSprite().setPosition(0.F, 1000.F);

						this->removeEnemyPlane(l);

						this->pIsPlayerProjectileRemoved = true;
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
			if (PixelCollision::BoundingBoxTest(this->pEnemyPlane[i].getEnemyPlaneProjectile(j).getMissileSprite(), this->pPlayerPlane.getPlaneSprite()))
			{
				this->pPlayerPlane.setExplosionPosition(sf::Vector2f(this->pPlayerPlane.getPlanePosition().x - 450, this->pPlayerPlane.getPlanePosition().y - 200));

				this->pPlayerPlane.setExplosion(true);

				this->mExplosionTimer.restart(sf::seconds(0.3F));

				int damage = this->pEnemyPlane[i].enemyPlaneDealDamage();

				if (this->pPlayerPlane.getPlayerHp() > 0)
				{
					this->pPlayerPlane.playerTakeDamage(damage);

					this->pIsEnemyProjectileRemoved = true;
				}

				if (this->pPlayerPlane.getPlayerHp() == 0)
				{
					this->pPlayerPlane.getPlaneSprite().move(0.F, 1100.F);

					this->endGame();

					if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
						this->pLevel->push(new Level_One(pWindow, pLevel));
				}
			}
		}
	}

	if (this->mExplosionTimer.isExpired())
		this->pPlayerPlane.setExplosion(false);

	int damage = rand() % 2;

	if (this->pCollision.playerEnemyCollision(this->pPlayerPlane.getExplosionSprite(), this->mSoldier.getSoldierSprite(), deltaTime))
		this->mSoldier.takeDamage(damage);

	if (this->pTextTagTimer.isExpired() && !this->mTextTags.empty())
		this->mTextTags.pop_back();
}

void Level_Two::bombExplosion(const float& deltaTime)
{
	for (int i = 0; i < this->pPlayerPlane.getProjectileSize(); i++)
	{
		if (this->pPlayerPlane.getProjectile(i).getBombPosition().y >= this->mHeight)
		{
			this->pPlayerPlane.stop(true);

			this->pAudio.loadSound("Resources/Sounds/Explosions/rock_breaking.flac");

			this->pAudio.setVolume(30.F);

			this->pPlayerPlane.setExplosion(true);

			this->pPlayerPlane.setExplosionPosition(sf::Vector2f(this->pPlayerPlane.getProjectile(i).getBombPosition().x, this->mHeight));

			this->pAudio.play();

			this->pIsPlayerProjectileRemoved = true;
		}
	}
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
	for (int i = 0; i < this->pPlayerPlane.getProjectileSize(); i++)
		if (this->pPlayerPlane.getProjectile(i).getBombPosition().y > this->pWindow->getSize().y)
			this->pPlayerPlane.removeProjectile(i);

	for (int i = 0; i < this->pPlayerPlane.getProjectileSize(); i++)
	{
		if (this->pIsPlayerProjectileRemoved)
		{
			this->pPlayerPlane.removeProjectile(i);

			this->pIsPlayerProjectileRemoved = false;
		}
	}
}