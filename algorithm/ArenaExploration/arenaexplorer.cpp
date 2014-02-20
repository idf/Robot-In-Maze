#include <vector>

#include "arenaexplorer.h"
#include "robot.h"
#include "sensor.h"
#include "pathfinder.h"

using namespace std;

ArenaExplorer::ArenaExplorer()
{
}

ArenaExplorer::~ArenaExplorer()
{
}


// robot core algorithm
void ArenaExplorer::explore(Robot* robot, Arena* arena)
{
	while (!arena->isExploredFully())
	{
		robot->senseEnvironment(arena);
		robot->exploreNextStep(arena);
	}

	PathFinder* pathfinder = new PathFinder();
	pathfinder->findPathBetween(robot->getPosX, robot->getPosY, 0, 0, arena);
}