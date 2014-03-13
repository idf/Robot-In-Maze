#ifndef PATHFINDER_H
#define PATHFINDER_H

#define MAX_SEARCH_TIME 420000   // in ms

#include "Arena.h"
#include "robot.h"
#include "sensor.h"
#include "connector.h"
#include <ctime>
#include <vector>
#include <string>
#include <math.h>

class PathFinder
{
public:
	PathFinder(Robot* robot, Arena* arena);
	PathFinder(Robot* robot, Arena* arena, Arena* fullArena, Connector* conn);
	~PathFinder();
	bool PathFinder::explore(int percentage, int timeLimitInSeconds);
	bool PathFinder::isSameDirection(Grid* current, Grid* next);
	int PathFinder::addSafeWeight(Grid* grid);
	std::vector<Grid*> findPathBetween(int startX, int startY, int endX, int endY, bool oneShortestPathRun);
	bool PathFinder::pointIsAlwaysSafe(int x, int y);
	bool PathFinder::pointIsWalkable(int x, int y);
	bool PathFinder::substituteNewPoint(int x, int y);
	void PathFinder::getRobotToMoveAndSense(Grid* destination);
	bool PathFinder::runOnePath(std::vector<Grid*> path);
	std::vector<std::pair<std::string, int>*>* PathFinder::getMovementList(std::vector<Grid*> path);
	void selectNextDestination();
	time_t start;
private:
	int _endX, _endY;
	Robot* _robot;
	Arena* _arena, *_fullArena;
	Connector* _conn;
};

#endif