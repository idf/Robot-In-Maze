#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "Arena.h"
#include "robot.h"
#include "sensor.h"

class PathFinder
{
public:
	PathFinder();
	~PathFinder();
	void FindPathBetween(int startX, int startY, int endX, int endY, Arena* arena);
private:
	
};

#endif