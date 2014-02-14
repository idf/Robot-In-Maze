#include "Arena.h"

Arena::Arena()
{
	init();
}

Arena::~Arena()
{
	delete _grid;
}

void Arena::init()
{
	for(int i = 0; i < ARENA_X_SIZE; ++i)
		for (int j = 0; j < ARENA_Y_SIZE; ++j)
			setGridType(i, j, UNEXPLORED);
}