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
	PathFinder(Robot* robot, Arena* arena);
	~PathFinder();
	void explore();
	void findPathBetween(int startX, int startY, int endX, int endY);
	void selectNextDestination();
private:
	int _endX, _endY;
	Robot* _robot;
	Arena* _arena;
};

#endif