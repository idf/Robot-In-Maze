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
	findPathBetween(_startX, _startY, _endX, _endY, arena);

	while (!arena->isExploredFully())
	{
		selectNextDestination(arena);
		findPathBetween(_startX, _startY, _endX, _endY, arena);
	}

	PathFinder* pathfinder = new PathFinder();
	pathfinder->findPathBetween(robot->getPosX, robot->getPosY, 0, 0, arena);
}

void findPathBetween(int startX, int startY, int endX, int endY, Arena* arena)
{

}

void selectNextDestination(Arena* arena)
{
	
}

