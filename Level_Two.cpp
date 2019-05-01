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

	this->loadTrees();

	for (size_t i = 0; i < this->mTreeCount; i++)
		this->pTrees.push_back(Trees::Tree(this->pTree.treeTextures, sf::Vector2f(thor::random(min, max), 310.F)));
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

		this->pEnemySpawnTimer.restart(sf::seconds(1.5F));
	}

	this->updateLevel(deltaTime);

	this->pPlayerPlane.update(deltaTime);

	this->mSoldier.update(deltaTime);

	this->input(deltaTime);

	this->collision(deltaTime);

	this->treeCollision(deltaTime);

	this->treeRemoval();

	this->bombExplosion(deltaTime);

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

	this->pPlayerPlane.render(target);

	this->mSoldier.render(target);

	for (size_t i = 0; i < this->pEnemyPlane.size(); i++)
		this->pEnemyPlane[i].renderPlane(target);

	if (this->mTextTagTimer.isRunning())
		for (size_t i = 0; i < this->mTextTags.size(); i++)
			this->mTextTags[i]->render(target);

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
		this->pPlayerPlane.move(0.5F, 0.F, deltaTime);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		this->pPlayerPlane.move(-0.5F, 0.F, deltaTime);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		this->pPlayerPlane.move(0.F, -0.5F, deltaTime);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		this->pPlayerPlane.move(0.F, 0.5F, deltaTime);
}

void Level_Two::collision(const float& deltaTime)
{
	for (int i = 0; i < this->pPlayerPlane.getProjectileSize(); i++)
		if (this->pPlayerPlane.getProjectile(i).getBombPosition().y > this->pWindow->getSize().y)
			this->pPlayerPlane.removeProjectile(i);

	//Player Enemy Collisions (Bomb)
	for (int i = 0; i < this->pPlayerPlane.getProjectileSize(); i++)
	{
		if (this->pCollision.playerEnemyCollision(this->pPlayerPlane.getProjectile(i).getBombSprite(), this->mSoldier.getSoldierSprite(), deltaTime))
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
			if (this->pCollision.playerEnemyCollision(this->pPlayerPlane.getProjectile(k).getBombSprite(), this->pEnemyPlane[l].getEnemyPlaneSprite(), deltaTime))
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
		
		//Player Missile Collisions
	for (int i = 0; i < this->pPlayerPlane.getProjectileSize(); i++)
	{
		this->pPlayerPlane.getProjectile(i).update(deltaTime);

		for (size_t j = 0; j < this->pEnemyPlane.size(); j++)
		{
			if (this->pCollision.playerEnemyCollision(this->pPlayerPlane.getProjectile(i).getProjectileSprite(), this->pEnemyPlane[j].getEnemyPlaneSprite(), deltaTime))
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

					if (this->pEnemyPlane[j].getPlaneHp() == 0)
					{
						this->removeEnemyPlane(j);

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
			if (this->pCollision.playerEnemyCollision(this->pEnemyPlane[i].getEnemyPlaneProjectile(j).getMissileSprite(), this->pPlayerPlane.getPlaneSprite(), deltaTime))
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

					this->endGameInput();
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

void Level_Two::treeRemoval()
{
	for (size_t i = 0; i < this->pTrees.size(); i++)
	{
		if (this->pIsTreeRemoved)
		{
			this->removeTrees(i);

			this->pIsTreeRemoved = false;
		}
	}
}

void Level_Two::treeCollision(const float& deltaTime)
{
	//Player Missile
	for (size_t i = 0; i < this->pTrees.size(); i++)
	{
		for (int k = 0; k < this->pPlayerPlane.getProjectileSize(); k++)
		{
			if (this->pCollision.playerEnemyCollision(this->pTrees[i].getTreeSprite(), this->pPlayerPlane.getProjectile(k).getProjectileSprite(), deltaTime))
			{
				this->pPlayerPlane.setExplosion(true);

				this->pPlayerPlane.setExplosionPosition(sf::Vector2f(this->pTrees[i].getPosition().x, this->pTrees[i].getPosition().y));

				int damage = this->pPlayerPlane.weaponDamage();

				if (this->pTrees[i].getTreeHp() > 0)
				{
					this->pTrees[i].damaged(damage);

					this->pIsPlayerProjectileRemoved = true;
				}

				if (this->pTrees[i].getTreeHp() == 0)
				{
					this->pAudio.loadSound("Resources/Sounds/Explosions/treeExplosion.ogg");

					this->pAudio.setVolume(30.F);

					this->pAudio.play();

					this->pIsTreeRemoved = true;

					this->pIsPlayerProjectileRemoved = true;
				}
			}
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
	{
		if (this->pIsPlayerProjectileRemoved)
		{
			this->pPlayerPlane.removeProjectile(i);

			this->pIsPlayerProjectileRemoved = false;
		}
	}
}