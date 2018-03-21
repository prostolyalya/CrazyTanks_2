#pragma once
#include "gameObject.h"
#include "level.h"

class Base
	: public GameObject
{
public:
	Base();

	virtual void render(RenderSystem* rs);
};