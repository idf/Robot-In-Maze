#include "Arena.h"

int Grid::getDistanceEstimated(Grid *p)
{ 
	return (abs(p->x - x) + abs(p->y - y)) * 10; 
}

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

GRIDTYPE Arena::getGridType(int posX, int posY)
{
	if (posX < 0 || posX >= ARENA_X_SIZE || posY < 0 || posY >= ARENA_Y_SIZE)
		return OBSTACLE;
	else
		return _grid[posX][posY].type;
}
void Arena::setGridType(int posX, int posY, GRIDTYPE gridType)
{
	if (posX < 0 || posX >= ARENA_X_SIZE || posY < 0 || posY >= ARENA_Y_SIZE)
		return;
	else
	    _grid[posX][posY].type = gridType;
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
bool Arena::isExploredFully(int percentage)
{
	int count = ARENA_X_SIZE*ARENA_Y_SIZE;
	for(int i = 0; i < ARENA_X_SIZE; ++i)
	{
		for (int j = 0; j < ARENA_Y_SIZE; ++j)
		{
			if (this->getGridType(i, j) == UNEXPLORED)
				--count;
		}
	}
	return count >= percentage * 3;
}