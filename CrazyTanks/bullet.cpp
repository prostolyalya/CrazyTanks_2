#include "stdafx.h"
#include "bullet.h"
#include "wall.h"

Bullet::Bullet()
{
	m_type = GameObjectType_Bullet;
	m_ownerType = GameObjectType_None;

	setImage(kBulletImage, ConsoleColor_DarkGray, ConsoleColor_Black);
}

void Bullet::render(RenderSystem* rs)
{
	int row = int(m_y);
	int column = int(m_x);

	rs->drawChar(row, column, m_image.symbol, m_image.symbolColor, m_image.backgroundColor);
}

void Bullet::intersect(GameObject* object)
{
	// Destroy self
	setHealth(0);

	// Do damage to target
	if (m_ownerType == GameObjectType_TankPlayer && object->getType() == GameObjectType_TankEnemy)
	{
		object->doDamage(1);
		m_game->increaseDiedEnemiesCount();
	}
	if (m_ownerType == GameObjectType_TankEnemy && object->getType() == GameObjectType_TankPlayer)
		object->doDamage(1);
	if (object->getType() == GameObjectType_Wall)
		object->doDamage(1);
	if (object->getType() == GameObjectType_Bullet)
		object->doDamage(1);
	if (object->getType() == GameObjectType_Base)
		object->doDamage(1);
}

void Bullet::setImage(char symbol, ConsoleColor symbolColor, ConsoleColor bgColor)
{
	m_image.symbol = symbol;
	m_image.symbolColor = symbolColor;
	m_image.backgroundColor = bgColor;
}