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
	PathFinder(Robot* robot, Arena* arena, Arena* fullArena);
	~PathFinder();
	bool PathFinder::explore(int percentage, int timeLimitInSeconds);
	std::vector<Grid*> findPathBetween(int startX, int startY, int endX, int endY);
	bool PathFinder::pointIsWalkable(int x, int y);
	bool PathFinder::substituteNewPoint(int x, int y);
	void PathFinder::getRobotToMove(Grid* destination);
	void PathFinder::getMovementList(std::vector<Grid*>);
	void selectNextDestination();
	time_t start;
private:
	int _endX, _endY;
	Robot* _robot;
	Arena* _arena, *_fullArena;
};

#endif