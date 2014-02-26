#include "pathfinder.h"

PathFinder::PathFinder(Robot* robot, Arena* arena)
{
	_robot = robot;
	_arena = arena;
	_endX = 18;
	_endY = 13;
}


// highest level exploration

void PathFinder::explore()
{
	findPathBetween(0, 0, _endX, _endY);

	while (!_arena->isExploredFully())
	{
		selectNextDestination();
		findPathBetween(_robot->getPosX(), _robot->getPosY(), _endX, _endY);
	}
	findPathBetween(_robot->getPosX(), _robot->getPosY(), 0, 0);
}

// mid level exploration
// it will control robot to run
void PathFinder::findPathBetween(int startX, int startY, int endX, int endY)
{
	// check termination condition: goal or unreachable
	if (_robot->getPosX() == endX && _robot->getPosY() == endY)
		return;
	if (_arena->getGridType(endX, endY) == OBSTACLE)
		return;

	// robot sense
	_robot->senseEnvironment(_arena);
	// robot move
	_robot->exploreNextStep(_arena);
}

void PathFinder::selectNextDestination()
{
	for (int i = ARENA_X_SIZE-1; i >=0; -i)
		for (int j = 0; j < ARENA_Y_SIZE; ++j)
		{
			if (_arena->getGridType(i, j) == UNEXPLORED)
			{
				_endX = i;
				_endY = j;
			}
		}
}