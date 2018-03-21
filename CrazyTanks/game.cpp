#include "stdafx.h"
#include "game.h"
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <iostream>
#include "level.h"
#include "wall.h"
#include "bullet.h"
#include "tankPlayer.h"
#include "tankEnemy.h"
#include "enemySpawner.h"
#include "utils.h"
#include "base.h"

Game::Game()
{
	m_isGameActive = true;
	m_clockLastFrame = 0;

	for (int i = 0; i < kObjectsCountMax; i++)
		m_objects[i] = 0;

	m_player1 = 0;
	m_base = 0;
	m_diedEnemiesCount = 0;
}

void Game::setupSystem()
{
	srand(time(0));

	m_renderSystem.initialize();
}

void Game::initialize()
{
	shutdown();

	m_diedEnemiesCount = 0;
	// Load level
	for (int r = 0; r < kLevelRows; r++)
	{
		for (int c = 0; c < kLevelColumns; c++)
		{
			unsigned char cellSymbol = kLevelData0[r][c];
			switch (cellSymbol)
			{
			case CellSymbol_BrickWall:
			{
				Wall* wall = (Wall*)createObject(GameObjectType_Wall, c, r);
				break;
			}
			case CellSymbol_Base:
			{
				m_base = createObject(GameObjectType_Base, c, r);
				break;
			}

			case CellSymbol_SteelWall:
			{
				Wall* wall = (Wall*)createObject(GameObjectType_Wall, c, r);
				wall->setImage(kSteelWallImage, kSteelWallSymbolColor, kSteelWallBackgroundColor);
				wall->setInvulnerable(true);
				if (r + 1 == kLevelRows && c + 1 == kLevelColumns)
				{
					int j, i, l, z,o;
					int n = 1 + rand() % 8;
					while (n != 0)
					{
						o = 0;
						l = 2 + rand() % 7;
						z = 1 + rand() % 2;
						i = 1 + rand() % 67;
						j = 1 + rand() % 21;
						if (i + l > 70)
						{
							i = 70 - l;
						}
						if (j + l > 24)
						{
							j = 24 - l;
						}
						if (z == 1)
						{
							for (int q = j; q < j + l; q++)
							{
								if (kLevelData0[i][q] == GameObjectType_TankPlayer|| kLevelData0[i][q] == GameObjectType_Wall || kLevelData0[i][q] == GameObjectType_Base)
									o++;
							}

							if (o == 0)
							{
								for (int q = j; q < j + l; q++)
								{
									Wall* wall = (Wall*)createObject(GameObjectType_Wall, i, q);
									wall->setImage(kSteelWallImage, kSteelWallSymbolColor, kSteelWallBackgroundColor);
								}
  								n--;
							}
						}
						else
						{
							for (int q = i; q < i + l; q++)
							{
								if (kLevelData0[q][j] == GameObjectType_TankPlayer || kLevelData0[q][j] == GameObjectType_Wall || kLevelData0[q][j] == GameObjectType_Base)
									o++;
							}
							if (o == 0)
							{
								for (int q = i; q < i + l; q++)
								{
									Wall* wall = (Wall*)createObject(GameObjectType_Wall, q, j);
									wall->setImage(kSteelWallImage, kSteelWallSymbolColor, kSteelWallBackgroundColor);
								}
								n--;
							}
						}


					}
				}
				break;
			}

			case CellSymbol_Player1:
			{
				TankPlayer* player1 = (TankPlayer*)createObject(GameObjectType_TankPlayer, c + 0.5, r + 0.5);
				player1->setColor(ConsoleColor_DarkRed, ConsoleColor_Red);
				player1->setKeys(VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN, VK_SPACE);
				m_player1 = player1;

				break;
			}

			case CellSymbol_EnemySpawner:
			{
				int k = 6;
				int j,i,n;
				while (k!=0)
				{
					n = 0;
					i = 2 + rand() % 67;
					j = 2 + rand() % 21;
					for (int z = i - 3; z < i + 3; z++)
					{
						for (int y = j - 3; y < j + 3; y++)
						{
							if (kLevelData0[z][y] == GameObjectType_Wall || kLevelData0[z][y] == GameObjectType_Base || kLevelData0[z][y] == GameObjectType_TankPlayer)
								n++;
						}
					}
					if (n==0)
					{

						k--;
						createObject(GameObjectType_EnemySpawner, i, j);
					}

				}
				break;
			}
			}
		}
	}
}

bool Game::loop()
{
	// Calculate delta time
	clock_t clockNow = clock();
	clock_t deltaClock = clockNow - m_clockLastFrame;
	float deltaTime = float(deltaClock) / CLOCKS_PER_SEC;
	m_clockLastFrame = clockNow;

	render();
	update(deltaTime);

	return m_isGameActive;
}

void Game::shutdown()
{
	for (int i = 0; i < kObjectsCountMax; i++)
		if (m_objects[i] != 0)
		{
			delete m_objects[i];
			m_objects[i] = 0;
		}
}

void Game::render()
{
	// Start frame
	m_renderSystem.clear();

	// Draw all game objects
	int objectsCount = 0;
	for (int i = 0; i < kObjectsCountMax; i++)
		if (m_objects[i] != 0)
		{
			m_objects[i]->render(&m_renderSystem);
			objectsCount++;
		}
	char buff[64];
	sprintf_s(buff, "Time: %d , HP: %d , Kills: %d", m_clockLastFrame/CLOCKS_PER_SEC, m_player1->getHealth(),m_diedEnemiesCount);
	m_renderSystem.drawText(0, 0, buff, ConsoleColor_Gray, ConsoleColor_Black);


	// End frame
	m_renderSystem.flush();
}

void Game::update(float dt)
{
	for (int i = 0; i < kObjectsCountMax; i++)
		if (m_objects[i] != 0)
		{
			m_objects[i]->update(dt);

			if (m_objects[i]->getHealth() <= 0 && m_objects[i]->getDestroyAfterDeath())
				destroyObject(m_objects[i]);
		}
	if (m_base && m_base->getHealth() <= 0)
	{
		destroyObject(m_base);
		m_base = NULL;
		m_isGameActive = false;
		std::cout << "Lose, base destroyed! your score: " << m_diedEnemiesCount << std::endl;
	}
	// Player1 destroyed
	if (m_player1 && m_player1->getHealth() <= 0)
	{
		destroyObject(m_player1);
		m_player1 = NULL;
		m_isGameActive = false;
		std::cout << "Lose! your score: " << m_diedEnemiesCount << std::endl;
	}

	// All enemies destroyed
	if (m_diedEnemiesCount == kEnemiesPerLevel)
	{
		m_isGameActive = false;
		std::cout << "Win! your score: " << m_diedEnemiesCount << std::endl;
	}
}

GameObject* Game::checkIntersects(float x, float y, float width, float height, GameObject* exceptObject)
{
	int r00 = int(y);
	int c00 = int(x);
	int r01 = r00 + height - 1;
	int c01 = c00 + width - 1;

	for (int i = 0; i < kObjectsCountMax; i++)
		if (m_objects[i] != 0 && m_objects[i]->getPhysical() && m_objects[i] != exceptObject)
		{
			int r10 = int(m_objects[i]->getY());
			int c10 = int(m_objects[i]->getX());
			int r11 = r10 + m_objects[i]->getHeight() - 1;
			int c11 = c10 + m_objects[i]->getWidth() - 1;

			if (r00 <= r11 && r01 >= r10 && c00 <= c11 && c01 >= c10)
				return m_objects[i];
		}

	return 0;
}

bool Game::moveObjectTo(GameObject* object, float x, float y)
{
	int r0 = int(y);
	int c0 = int(x);
	int r1 = r0 + object->getHeight() - 1;
	int c1 = c0 + object->getWidth() - 1;

	if (r0 < 0 || c0 < 0 || r1 >= kLevelRows || c1 >= kLevelColumns)
		return false;


	bool canMoveToCell = false;

	GameObject* otherObject = checkIntersects(x, y, object->getWidth(), object->getHeight(), object);

	if (otherObject)
		object->intersect(otherObject);
	else
		canMoveToCell = true;


	if (canMoveToCell)
	{
		object->setX(x);
		object->setY(y);
	}

	return canMoveToCell;
}

int Game::getObjectsCount(GameObjectType type)
{
	int totalCount = 0;

	for (int i = 0; i < kObjectsCountMax; i++)
	{
		if (m_objects[i] == 0)
			continue;

		if (m_objects[i]->getType() == type)
			totalCount++;
	}

	return totalCount;
}

GameObject* Game::createObject(GameObjectType type, float x, float y)
{
	// Find free pointer and create object
	for (int i = 0; i < kObjectsCountMax; i++)
	{
		if (m_objects[i] == 0)
		{
			GameObject* object = 0;

			switch (type)
			{
			case GameObjectType_Wall:			object = new Wall();			break;
			case GameObjectType_Base:			object = new Base();			break;
			case GameObjectType_Bullet:			object = new Bullet();			break;
			case GameObjectType_TankPlayer:		object = new TankPlayer();		break;
			case GameObjectType_TankEnemy:		object = new TankEnemy();		break;
			case GameObjectType_EnemySpawner:	object = new EnemySpawner();	break;
			}

			if (object == 0)
				return 0;

			object->setGame(this);

			if (moveObjectTo(object, x, y) == false)
			{
				delete object;
				return 0;
			}

			m_objects[i] = object;

			return object;
		}
	}

	return 0;
}

void Game::destroyObject(GameObject* object)
{
	for (int i = 0; i < kObjectsCountMax; i++)
	{
		if (m_objects[i] == object)
		{
			delete m_objects[i];
			m_objects[i] = 0;

			return;
		}
	}
}

void Game::increaseDiedEnemiesCount()
{
	m_diedEnemiesCount++;
}