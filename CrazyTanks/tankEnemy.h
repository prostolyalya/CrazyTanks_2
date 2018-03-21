#pragma once
#include "tank.h"

class TankEnemy
	: public Tank
{
public:
	TankEnemy();
	~TankEnemy();

	void update(float dt);

protected:

	void analize();
	void moveRandomDirection();

protected:
	float m_analizeTimer;
	float m_analizeTime;

	float m_lastAnalizeX;
	float m_lastAnalizeY;
};
