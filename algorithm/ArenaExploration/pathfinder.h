#ifndef PATHFINDER_H
#define PATHFINDER_H

#define MAX_SEARCH_TIME 420000   // in ms

#include "Arena.h"
#include "robot.h"
#include "sensor.h"
#include <ctime>

class PathFinder
{
public:
	PathFinder();
	~PathFinder();
	void explore(Robot* robot, Arena* arena);
	void findPathBetween(int startX, int startY, int endX, int endY, Arena* arena);
	void selectNextDestination(Arena* arena);
private:
	int _endX, _endY;
};

#endif