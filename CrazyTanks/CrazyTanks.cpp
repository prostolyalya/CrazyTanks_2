#include "stdafx.h"

#include "game.h"
#include <iostream>


int main()
{

	system("pause");
	Game game;
	clock_t t0=clock();
	game.setupSystem();
	game.initialize();

	while (game.loop());

	game.shutdown();
	clock_t t1=clock();
	std::cout << "time: " << (double)(t1 - t0) / CLOCKS_PER_SEC << " ; game over ";
	return 0;
}

