#include "pathfinder.h"
#include <list>

using namespace std;

PathFinder::PathFinder(Robot* robot, Arena* arena, Arena* fullArena, Connector *conn)
{
	_robot = robot;
	_arena = arena;
	_fullArena = fullArena;
	_endX = ARENA_END_X;
#ifdef GUI
	_endY = ARENA_END_Y;
#else
	_endY = ARENA_START_Y;
#endif
	_destinationCount = 0;
	_conn = conn;
}
PathFinder::~PathFinder()
{}

// highest level exploration, find the next step and make a move
// return false when the procedure is completed
bool PathFinder::explore(int percentage, int timeLimitInSeconds)
{   
	// _destinationCount < 4 for 4 corner exploration
	// (_robot->getPosX() != _endX || _robot->getPosY() != _endY) for week 8 algo
#ifdef GUI
	if ((_robot->getPosX() != _endX || _robot->getPosY() != _endY)&& time(0) - start < timeLimitInSeconds)
#else
	if (_destinationCount < 4 && time(0) - start < timeLimitInSeconds) // exploration not done
#endif
	{
		if (_robot->getPosX() != _endX || _robot->getPosY() != _endY)
		{
			// move to new place, sense the surrounding
			for (vector<Grid*>::iterator i = experiencedPath.begin(); i != experiencedPath.end(); ++i)
			{
				for (int j = -1; j < 2; ++j)
					for (int k = -1; k < 2; ++k)
						_arena->setGridType((*i)->getX() + j,(*i)->getY() + k, UNOCCUPIED);
			}
			vector<Grid*> result = findPathBetween(_robot->getPosX(), _robot->getPosY(), _endX, _endY, false);
			
			// If there is no path to current goal, find next location
			if(result.begin() == result.end())
			{
				cout << "old destination: " << _endX << _endY;
				this->selectNextDestination();
				cout << "new destination: " << _endX << _endY << endl;
				return true;
			}

			// resolve direction path problem
			if (isSamePath(prevPrev, result))
			{
				cout << "stuck, choose one path!" <<endl;
				vector<Grid*>::reverse_iterator i = result.rbegin();
				if (result.begin() != result.end())
				{
					while (i != result.rend() && !getRobotToMoveAndSense(*i))
						++i;
				}
				// if there is no path to go, it will stuck here -> loop by timer, and both is empty.
				prevPrev = prev;
				prev = result;
				return true;
			}
			prevPrev = prev;
			prev = result;
			vector<Grid*>::reverse_iterator i = result.rbegin();
			getRobotToMoveAndSense(*i);
			cout << _robot->getPosX() << ", " << _robot->getPosY() << _robot->getDirection() << endl;
			cout <<"time elapsed: " << time(0) - start << endl;
			return true;
		}
		else  // goal reached, calibrate at goal and go to next destination
		{
			cout << "old destination: " << _endX << _endY;
			switch (_destinationCount)
			{
			case 0: 
				_robot->calibrateAtUpperRight(); break;
			case 1:
				_robot->calibrateAtGoal(); break;
			case 2:
				_robot->calibrateAtBottomLeft(); break;
			default:
				break;
			}
			this->selectNextDestination();
			cout << "new destination: " << _endX << _endY << endl;
			return true;
		}
	}
	else // exploration done or time exceeded
	{
		for (vector<Grid*>::iterator i = experiencedPath.begin(); i != experiencedPath.end(); ++i)
		{
			for (int j = -1; j < 2; ++j)
				for (int k = -1; k < 2; ++k)
					_arena->setGridType((*i)->getX() + j,(*i)->getY() + k, UNOCCUPIED);
		}
		return false; 
	}
}

bool PathFinder::isSameDirection(Grid* current, Grid* next)
{
	int xDiff, yDiff;

	// check future move direction
	xDiff = next->getX() - current->getX();
	yDiff = next->getY() - current->getY();
	DIRECTION futureDir = RIGHT, previousDir = _robot->getDirection();
	if (xDiff == 0 && yDiff == 1)
		futureDir = DOWN;
	else if (xDiff == -1 && yDiff == 0)
		futureDir = LEFT;
	else if (xDiff == 0 && yDiff == -1)
		futureDir = UP;
	else if (xDiff == 1 && yDiff == 0)
		futureDir = RIGHT;
	else
		return false;
	
	// start location case
	if (current->hasParent())
	{
		// check previous move direction
		xDiff = current->getX() - current->parent->getX();
		yDiff = current->getY() - current->parent->getY();
		if (xDiff == 0 && yDiff == 1)
			previousDir = DOWN;
		else if (xDiff == -1 && yDiff == 0)
			previousDir = LEFT;
		else if (xDiff == 0 && yDiff == -1)
			previousDir = UP;
		else
			previousDir = RIGHT;
	}

	if (futureDir != previousDir)
		return false;
	else 
		return true;
}
// used to avoid obstacle and provide safety distance
int PathFinder::addSafeWeight(Grid* grid)
{
	int x = grid->getX(), y = grid->getY();
	if (_arena->getGridType(x, y-1) == OBSTACLE ||
		_arena->getGridType(x+1, y-1) == OBSTACLE ||
		_arena->getGridType(x+2, y) == OBSTACLE ||
		_arena->getGridType(x+2, y) == OBSTACLE ||
		_arena->getGridType(x, y+2) == OBSTACLE ||
		_arena->getGridType(x+1, y+2) == OBSTACLE ||
		_arena->getGridType(x-1, y) == OBSTACLE ||
		_arena->getGridType(x-1, y+1) == OBSTACLE)
		return 10;
	else
		return 0;
}

// Find the path based on current map
// astar, not the shortest path
vector<Grid*> PathFinder::findPathBetween(int startX, int startY, int endX, int endY, bool oneShortestPathRun)
{
	cout << "Pathfinder: " << endX << endY << endl;
	vector<Grid*> path;
	Grid *current, *child, *start, *end;
	start = _arena->getGrid(startX, startY);
	end = _arena->getGrid(endX, endY);
	current = start;

	list<Grid*> openList;
	list<Grid*> closedList;
	list<Grid*>::iterator i;
	unsigned int n = 0;
	bool isSet = false;
	bool isCurrentPoint = true;

	// Add the start point to the openList
	openList.push_back(start);
	start->opened = true;

	while (n == 0 || current != end)
	{
		isSet = false;
		if (isCurrentPoint)
		{
			current = *openList.begin();
			isCurrentPoint = false;
		}
		else
		{
			// same dir, safe
			for (i = openList.begin();i != openList.end(); ++ i)
			{
				if (!isSameDirection(current, *i))
					continue;
				if (!isSet || (*i)->heuristic <= current->heuristic)
			    {
			        current = (*i);
					isSet = true;
			    }
			}
			if (!isSet)
			{
				for (i = openList.begin(); i != openList.end(); ++ i)
				{
				    if (!isSet || (*i)->heuristic <= current->heuristic)
				    {
				        current = (*i);
						isSet = true;
				    }
				}
			}
			
		}

		// Stop if we reached the end
		if (current == end)
			break;

		// check if there is no path to go. if yes, reset all and return a null path
		if (openList.begin() == openList.end())
		{
			for (i = openList.begin(); i != openList.end(); ++ i)
				(*i)->opened = false;

			for (i = closedList.begin(); i != closedList.end(); ++ i)
				(*i)->closed = false;

			for (int i = 0; i < ARENA_X_SIZE; ++i)
				for (int j = 0; j < ARENA_Y_SIZE; ++j)
					_arena->getGrid(i, j)->parent = NULL;
			return path;
		}

		// Remove the current point from the openList
		current->opened = false;
		openList.remove(current);

		// Add the current point to the closedList
		current->closed = true;
		closedList.push_back(current);

		// Get all current's adjacent walkable points
		// here x and y are used to iterrate through adjacent cells
		for (int x = -1; x < 2; x ++)
		{
			for (int y = -1; y < 2; y ++)
			{
				// If it's current point then pass
				if (x == 0 && y == 0)
					continue;

				// if index out of border then pass
				if (current->getX() + x < 0 ||
					current->getX() + x >= ARENA_X_SIZE ||
					current->getY() + y < 0 ||
					current->getY() + y >= ARENA_Y_SIZE)
					continue;
				// if the robot can't move in 45 degree direction
#ifdef STRAIGHT_MODE
				if (abs(x) == abs(y))
					continue;
#endif

				// Get this point
				child = _arena->getGrid(current->getX() + x, current->getY() + y);

				// If it's closed or not walkable then pass
				if (child->closed || !pointIsWalkable(child->x, child->y))
					continue;

				if (oneShortestPathRun)
				{
					if (!pointIsAlwaysSafe(child->x, child->y))
						continue;
				}
#ifndef STRAIGHT_MODE
				// If we are at a corner
				if (x != 0 && y != 0)
				{
					// if the next horizontal point is not walkable or in the closed list then pass
					if (!pointIsWalkable(current->getX(), current->getY() + y) || _arena->getGrid(current->getX(), current->getY() + y)->closed)
					{
						continue;
					}
					// if the next vertical point is not walkable or in the closed list then pass
					if (!pointIsWalkable(current->getX() + x, current->getY()) || _arena->getGrid(current->getX() + x, current->getY())->closed)
					{
						continue;
					}
				}
#endif
				// If it's already in the openList
				if (child->opened)
				{
					// If it has a wroste g score than the one that pass through the current point
					// then its path is improved when it's parent is the current point
					if (child->distanceTravelled > child->getDistanceTravelled(current))
					{
						// Change its parent and g score
						child->parent = current;
						child->computeScores(end);
					}
				}
				else
				{
					// Add it to the openList with current point as parent
					openList.push_back(child);
					child->opened = true;

					// Compute it's g, h and f score
					child->parent = current;
					child->computeScores(end);
				}

			}
		}
		++n;
	}

	// Reset
	for (i = openList.begin(); i != openList.end(); ++ i)
		(*i)->opened = false;

	for (i = closedList.begin(); i != closedList.end(); ++ i)
		(*i)->closed = false;

	// Resolve the path starting from the end point
	while (current->hasParent() && current != start)
	{
		path.push_back(current);
		current = current->parent;
		n ++;
	}
	// reset parent for next time
	for (int i = 0; i < ARENA_X_SIZE; ++i)
		for (int j = 0; j < ARENA_Y_SIZE; ++j)
			_arena->getGrid(i, j)->parent = NULL;
	return path;
}

// a point is walkable if there is no obstacle or wall within the four squares
bool PathFinder::pointIsWalkable(int x, int y)
{
	// obstacle case
	for (int i = -1; i < 2; ++i)
		for (int j = -1; j < 2; ++j)
			if (_arena->getGridType(x + i, y + j) == OBSTACLE)
				return false;
	// border case
	if (x + 1 >= ARENA_X_SIZE || y + 1 >= ARENA_Y_SIZE)
		return false;
	return true;
}

bool PathFinder::pointIsAlwaysSafe(int x, int y)
{
	for (int i = -1; i < 2; ++i)
		for (int j = -1; j < 2; ++j)
			if (_arena->getGridType(x + i, y + j) == OBSTACLE ||
				_arena->getGridType(x + i, y + j) == UNEXPLORED )
				return false;
	return true;
}

bool PathFinder::substituteNewPoint(int x, int y)
{
	if (pointIsWalkable(_endX-2, _endY-1)) {_endX=_endX-2; _endY--; return true;}
	if (pointIsWalkable(_endX-2, _endY)) {_endX=_endX-2; return true;}
	if (pointIsWalkable(_endX-1, _endY+1)) {_endX--; _endY++; return true;}
	if (pointIsWalkable(_endX, _endY+1)) {_endY++; return true;}
	if (pointIsWalkable(_endX+1, _endY)) {_endY++; return true;}
	if (pointIsWalkable(_endX+1, _endY+1)) {_endX++; _endY++; return true;}
	if (pointIsWalkable(_endX+1, _endY-1)) {_endX++; _endY--; return true;}
	if (pointIsWalkable(_endX-1, _endY-2)) {_endX--; _endY = _endY-2; return true;}
	if (pointIsWalkable(_endX, _endY-2)) {_endY = _endY-2; return true;}
	return false;
}

// rotate or move forward
bool PathFinder::getRobotToMoveAndSense(Grid* destination)
{
	cout << "robot move sent!" << endl;
#ifdef HARDWARE
	int xDiff = destination->getX() - _robot->getPosX();
	int yDiff = destination->getY() - _robot->getPosY();
	DIRECTION dir = RIGHT, robotDir = _robot->getDirection();
	if (xDiff == 0 && yDiff == 1)
		dir = DOWN;
	else if (xDiff == -1 && yDiff == 0)
		dir = LEFT;
	else if (xDiff == 0 && yDiff == -1)
		dir = UP;
	if (dir == robotDir)
	{
		experiencedPath.push_back(_arena->getGrid(_robot->getPosX(), _robot->getPosY()));
		_robot->moveForwardAndSense(10, _arena);
		return true;
	}
	else 
		{
		if (robotDir == DOWN && dir == RIGHT)
			_robot->rotateCounterClockwiseAndSense(90, _arena);
		else if (robotDir == RIGHT && dir == DOWN)
			_robot->rotateClockwiseAndSense(90, _arena);
		else if (dir > robotDir)
			_robot->rotateClockwiseAndSense(90, _arena);
		else
			_robot->rotateCounterClockwiseAndSense(90, _arena);
		return false;
		}
#else
	this->_robot->setLocation(destination->getX(), destination->getY());
#endif
}

void PathFinder::runOnePath(vector<pair<std::string, int>*>* movementList, bool turnFirst)
{
	for (vector<pair<std::string, int>*>::iterator i = movementList->begin(); i != movementList->end(); i++)
		cout << (*i)->first << "," <<(*i)->second << endl;
	vector<pair<std::string, int>*>::iterator i = movementList->begin();

	if (turnFirst)
	{
		while((*i)->first != "moveForward")
		{
			if((*i)->first == "rotateClockwise")
				_robot->rotateClockwise((*i)->second);
			else if ((*i)->first == "rotateCounterClockwise")
				_robot->rotateCounterClockwise((*i)->second);
			++i;
		}
#ifdef ANDROID
		cout << "Wait for android run.";
		_conn->waitForAndroidRun();
#else
		cout << "Wait for button press";
		getchar();
#endif
	}

	for (; i != movementList->end(); ++i)
	{
		if((*i)->first == "rotateClockwise")
			_robot->rotateClockwise((*i)->second);
		else if ((*i)->first == "rotateCounterClockwise")
			_robot->rotateCounterClockwise((*i)->second);
		else
			_robot->moveForward((*i)->second);
	}
}

vector<pair<std::string, int>*>* PathFinder::getMovementList(std::vector<Grid*> path)
{
	vector<pair<std::string, int>*>* movementList = new vector<pair<std::string, int>*>();
	pair<std::string, int>* singleMovement;
	Grid* destination = *path.rbegin();
	Grid* currentLocation;
	DIRECTION previousDir;

	if (path.begin() == path.end())
		return movementList;

	// align robot direction first.
	int currentX = _robot->getPosX(), currentY = _robot->getPosY();
	DIRECTION currentDir = _robot->getDirection();
	int xDiff = destination->getX() - currentX;
	int yDiff = destination->getY() - currentY;
	DIRECTION dir = RIGHT;
	if (xDiff == 0 && yDiff == 1)
		dir = DOWN;
	else if (xDiff == -1 && yDiff == 0)
		dir = LEFT;
	else if (xDiff == 0 && yDiff == -1)
		dir = UP;

	while (dir != currentDir)
	{
		singleMovement = new pair<std::string, int>("", 0);
		if (currentDir == DOWN && dir == RIGHT)
		{
			singleMovement->first = "rotateCounterClockwise";
			singleMovement->second = 90;
			currentDir--;
		}
		else if (currentDir == RIGHT && dir == DOWN)
		{
			singleMovement->first = "rotateClockwise";
			singleMovement->second = 90;
			currentDir++;
		}
		else if (dir > currentDir)
		{
			singleMovement->first = "rotateClockwise";
			singleMovement->second = 90;
			currentDir++;
		}
		else
		{
			singleMovement->first = "rotateCounterClockwise";
			singleMovement->second = 90;
			currentDir--;
		}
		movementList->push_back(singleMovement);
	}
	singleMovement = new pair<string, int>("moveForward", 0);
	previousDir = dir;
	currentLocation = _arena->getGrid(_robot->getPosX(), _robot->getPosY());
	vector<Grid*>::reverse_iterator i = path.rbegin();
	while (i != path.rend())
	{
		destination = *i;
		xDiff = destination->getX() - currentLocation->getX();
		yDiff = destination->getY() - currentLocation->getY();

		if (xDiff == 0 && yDiff == 1)
			dir = DOWN;
		else if (xDiff == -1 && yDiff == 0)
			dir = LEFT;
		else if (xDiff == 0 && yDiff == -1)
			dir = UP;
		else
			dir = RIGHT;

		if(previousDir == dir)
		{
			singleMovement->second += 10;
			currentLocation = destination;
			//if ((*movementList->rbegin())->first != "moveForward")
			//{
			//	movementList->push_back(singleMovement);
			//	singleMovement = new pair<string, int>("moveForward", 0);
			//}
			++i;
		}
		else
		{
			if (previousDir == DOWN && dir == RIGHT) // turn counterclockwise
			{
				movementList->push_back(singleMovement);
				movementList->push_back(new pair<string, int>("rotateCounterClockwise", 90));
				singleMovement = new pair<string, int>("moveForward", 0);
			}
			else if (previousDir == RIGHT && dir == DOWN) // turn clockwise
			{
				movementList->push_back(singleMovement);
				movementList->push_back(new pair<string, int>("rotateClockwise", 90));
				singleMovement = new pair<string, int>("moveForward", 0);
			}
			else if (dir < previousDir)
			{
				movementList->push_back(singleMovement);
				movementList->push_back(new pair<string, int>("rotateCounterClockwise", 90));
				singleMovement = new pair<string, int>("moveForward", 0);
			}
			else
			{
				movementList->push_back(singleMovement);
				movementList->push_back(new pair<string, int>("rotateClockwise", 90));
				singleMovement = new pair<string, int>("moveForward", 0);
			}
			previousDir = dir;
		}
	}
	if (singleMovement->second != 0)
		movementList->push_back(singleMovement);
	return movementList;
}

void PathFinder::selectNextDestination()
{
	switch (_destinationCount)
	{
	case 0: // set next dest as goal
		_endY = ARENA_END_Y; break;
	case 1:
		_endX = ARENA_START_X; break;
	case 2:
		_endY = ARENA_START_Y; break;
	default:
		break;
	}
	++_destinationCount;
}

bool PathFinder::isSamePath(vector<Grid*> one, vector<Grid*> two)
{
	vector<Grid*>::reverse_iterator i,j;
	for (i = one.rbegin(), j = two.rbegin(); i != one.rend() && j != two.rend(); ++i, ++j)
	{
		if ((*i)->getX() != (*j)->getX() || (*i)->getY() != (*j)->getY())
			return false;
	}
	if (i != one.rend() || j != two.rend())
		return false;
	else
		return true;
}