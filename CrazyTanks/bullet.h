#pragma once
#include "gameObject.h"
#include "level.h"

class Bullet
	: public GameObject
{
public:
	Bullet();

	virtual void render(RenderSystem* rs);

	virtual void intersect(GameObject* object);

	void setImage(char symbol, ConsoleColor symbolColor, ConsoleColor bgColor);

	void setOwnerType(GameObjectType ownerType) { m_ownerType = ownerType; }
	GameObjectType getOwnerType() { return m_ownerType; }

private:
	ConsoleSymbolData m_image;
	GameObjectType m_ownerType;
};
