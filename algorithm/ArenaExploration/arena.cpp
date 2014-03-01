#include "Arena.h"

// region grid class
void Grid::computeScores(Grid *end)
{
	distanceTravelled = getDistanceTravelled(parent);
	distanceEstimated = getDistanceEstimated(end);
	heuristic = distanceTravelled + distanceEstimated;
}

// region arena class
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
			Grid *g = getGrid(i, j);
			g->x = i;
			g->y = j;
			setGridType(i, j, UNEXPLORED);
			g->closed = false;
			g->opened = false;
			g->distanceEstimated = 0;
			g->distanceTravelled = 0;
			g->heuristic = 0;
			g->parent = NULL;
		}
	}
	// set start and end
	for(int i = ARENA_START_X; i < ARENA_START_X + 3; ++i)
		for (int j = ARENA_START_Y; j < ARENA_START_Y + 3; ++j)
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