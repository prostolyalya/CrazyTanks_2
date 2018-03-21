#pragma once
#include "gameObject.h"
#include "level.h"


class Wall
	: public GameObject
{
public:
	Wall();

	virtual void render(RenderSystem* rs);

	void setImage(unsigned char symbol, ConsoleColor symbolColor, ConsoleColor bgColor);

private:
	ConsoleSymbolData m_image;
};
