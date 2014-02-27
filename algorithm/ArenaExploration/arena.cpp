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
	{
		for (int j = 0; j < ARENA_Y_SIZE; ++j)
		{
			setGridType(i, j, UNEXPLORED);
			setIsOnPath(i, j, false);
			setIsClosed(i, j, false);
			setIsOpen(i, j, false);
			setDistanceEstimated(i, j, 0);
			setDistanceTravelled(i, j, 0);
			getGrid(i, j)->x = i;
			getGrid(i, j)->y = j;
		}
	}
	// set start and end
	for(int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			setGridType(i, j, START);

	for(int i = ARENA_X_SIZE - 3; i < ARENA_X_SIZE; ++i)
		for (int j = ARENA_Y_SIZE - 3; j < ARENA_Y_SIZE; ++j)
			setGridType(i, j, GOAL);
}

GRIDTYPE Arena::getRealGridType(int posX, int posY)
{
	return UNEXPLORED;
}


// to be updated to cater to unexplorable area;
bool Arena::isExploredFully()
{
	for(int i = 0; i < ARENA_X_SIZE; ++i)
	{
		for (int j = 0; j < ARENA_Y_SIZE; ++j)
		{
			if (this->getGridType(i, j) == UNEXPLORED)
				return false;
		}
	}
	return true;
}