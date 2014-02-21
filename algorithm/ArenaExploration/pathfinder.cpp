#include "pathfinder.h"

PathFinder::PathFinder()
{
	_startX = 0;
	_startY = 0;
	_endX = 18;
	_endY = 13;
}

void PathFinder::explore(Robot* robot, Arena* arena)
{
	findPathBetween(0, 0, _endX, _endY, arena);

	while (!arena->isExploredFully())
	{
		selectNextDestination(arena);
		findPathBetween(robot->getPosX(), robot->getPosY(), _endX, _endY, arena);
	}

	PathFinder* pathfinder = new PathFinder();
	pathfinder->findPathBetween(robot->getPosX, robot->getPosY, 0, 0, arena);
}

void PathFinder::findPathBetween(int startX, int startY, int endX, int endY, Arena* arena)
{
	// robot sense
	
	// robot move
}

void PathFinder::selectNextDestination(Arena* arena)
{
	for (int i = ARENA_X_SIZE - 1; i >=0; -i)
		for (int j = 0; j < ARENA_Y_SIZE; ++j)
		{
			if (arena->getGridType(i, j) == UNEXPLORED)
			{
				_endX = i;
				_endY = j;
			}
		}
}