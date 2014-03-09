#include "pathfinder.h"
#include <list>

using namespace std;

PathFinder::PathFinder(Robot* robot, Arena* arena)
{
	_robot = robot;
	_arena = arena;
	_endX = ARENA_END_X;
	_endY = ARENA_END_Y;
}
PathFinder::PathFinder(Robot* robot, Arena* arena, Arena* fullArena)
{
	_robot = robot;
	_arena = arena;
	_fullArena = fullArena;
	_endX = ARENA_END_X;
	_endY = ARENA_END_Y;
}
PathFinder::~PathFinder()
{}

// highest level exploration, find the next step and make a move
// return false when the procedure is completed
bool PathFinder::explore(int percentage, int timeLimitInSeconds)
{   
	if (!_arena->isExploredFully(percentage) && time(0) - start < timeLimitInSeconds)
	{
#ifdef DEBUG
		cout << _robot->getPosX() << ", " << _robot->getPosY() << _robot->getDirection() << endl;
		cout <<"time elapsed: " << time(0) - start << endl;
#endif
		if (_robot->getPosX() != _endX && _robot->getPosY() != _endY)
		{
			// TODO CHANGE TO CHECK EXPLORABLE
			// if not reachable, check whether it is detectable, if not, break;
			if(!pointIsWalkable(_endX, _endY))
			{
				if(!substituteNewPoint(_endX, _endY))
					return true;
			}
			// move to new place, sense the surrounding
			vector<Grid*> result = findPathBetween(_robot->getPosX(), _robot->getPosY(), _endX, _endY);
			vector<Grid*>::reverse_iterator i = result.rbegin();
			if (result.begin() != result.end()) // list not empty
				getRobotToMoveAndSense(*i);
			else
				_robot->rotateClockwiseAndSense(90, _arena); // sense other areas
			
			return true;
		}
		else
		{
#ifdef DEBUG
			cout << "old destination: " << _endX << _endY;
#endif
			this->selectNextDestination();
#ifdef DEBUG
			cout << "new destination: " << _endX << _endY << endl;
		// debug
			for (int i = 0; i < ARENA_Y_SIZE; ++i)
				{
					for (int j = 0; j < ARENA_X_SIZE; ++j)
						cout << _arena->getGridType(j, i);
					cout << endl;
				}
		//debug
#endif
			return true;
		}
	}
	// go back to start point
	// TODO CHANGE TO FASTEST PATH RUN!
	else if (_robot->getPosX() != ARENA_START_X || _robot->getPosY() != ARENA_START_Y)
	{
		vector<Grid*> result = findPathBetween(_robot->getPosX(), _robot->getPosY(), ARENA_START_X, ARENA_START_Y);
		vector<Grid*>::reverse_iterator i = result.rbegin();
		if (result.begin() != result.end()) // list not empty
			getRobotToMoveAndSense(*i);
		return true;
	}
	else
		return false;
}
// used to avoid obstacle and provide safety distance
int PathFinder::addWeight(Grid* grid)
{
	int x = grid->getX(), y = grid->getY();
	if (!pointIsWalkable(x+1, y) ||
		!pointIsWalkable(x, y+1) ||
		!pointIsWalkable(x-1, y) ||
		!pointIsWalkable(x, y-1))
		return 10;
	else 
		return 0;
}

// Find the path based on current map
// astar, not the shortest path
vector<Grid*> PathFinder::findPathBetween(int startX, int startY, int endX, int endY)
{
    vector<Grid*> path;
    Grid *current, *child, *start, *end;
	start = _arena->getGrid(startX, startY);
	end = _arena->getGrid(endX, endY);
	current = start;

    list<Grid*> openList;
    list<Grid*> closedList;
    list<Grid*>::iterator i;
    unsigned int n = 0;

    // Add the start point to the openList
    openList.push_back(start);
    start->opened = true;

    while (n == 0 || current != end)
    {
		// stop if the point is unreachable
		// this part may have problem
		if (_arena->getGridType(endX, endY) == OBSTACLE)
			break;

        // Look for the smallest F value in the openList and make it the current point
        for (i = openList.begin(); i != openList.end(); ++ i)
        {
            if (i == openList.begin() || (*i)->heuristic + addWeight(*i) <= current->heuristic + addWeight(current))
            {
                current = (*i);
            }
        }

        // Stop if we reached the end
        if (current == end)
            break;

        // Remove the current point from the openList
        openList.remove(current);
        current->opened = false;

        // Add the current point to the closedList
        closedList.push_back(current);
        current->closed = true;

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
    return path;
}

// a point is walkable if there is no obstacle or wall within the four squares
bool PathFinder::pointIsWalkable(int x, int y)
{
	// obstacle case
	if (_arena->getGridType(x, y) == OBSTACLE ||
		_arena->getGridType(x + 1, y) == OBSTACLE ||
		_arena->getGridType(x, y + 1) == OBSTACLE ||
		_arena->getGridType(x + 1, y + 1) == OBSTACLE)
		return false;
	// border case
	if (x + 1 >= ARENA_X_SIZE || y + 1 >= ARENA_Y_SIZE)
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
void PathFinder::getRobotToMoveAndSense(Grid* destination)
{
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
		_robot->moveForwardAndSense(10, _arena);
	else if (robotDir == DOWN && dir == RIGHT)
		_robot->rotateCounterClockwiseAndSense(90, _arena);
	else if (robotDir == RIGHT && dir == DOWN)
		_robot->rotateClockwiseAndSense(90, _arena);
	else if (dir > robotDir)
		_robot->rotateClockwiseAndSense(90, _arena);
	else
		_robot->rotateCounterClockwiseAndSense(90, _arena);
#else
	this->_robot->setLocation(destination->getX(), destination->getY());
#endif
}

bool PathFinder::runOnePath(vector<Grid*> path)
{

}
// TODO CHANGE
vector<pair<std::string, int>*>* PathFinder::getMovementList(std::vector<Grid*> path)
{
	vector<pair<std::string, int>*>* movementList = new vector<pair<std::string, int>*>();
	pair<std::string, int>* singleMovement = new pair<std::string, int>("", 0);
	// variables for simulation of robot movement
	Grid* destination = *path.rbegin();
	int currentX = _robot->getPosX(), currentY = _robot->getPosY();
	DIRECTION currentDir = _robot->getDirection();
	int xDiff = destination->getX() - currentX;
	int yDiff = destination->getY() - currentY;
	DIRECTION dir = RIGHT;
	// set robot direction first
	if (xDiff == 0 && yDiff == 1)
		dir = DOWN;
	else if (xDiff == -1 && yDiff == 0)
		dir = LEFT;
	else if (xDiff == 0 && yDiff == -1)
		dir = UP;

	while (dir != currentDir)
	{
		if (currentDir == DOWN && dir == RIGHT)
		{
			singleMovement->first = "rotateCounterClockwise";
			singleMovement->second += 90;
		}
		else if (currentDir == RIGHT && dir == DOWN)
		{
			singleMovement->first = "rotateClockwise";
			singleMovement->second += 90;
		}
		else if (dir > currentDir)
		{
			singleMovement->first = "rotateClockwise";
			singleMovement->second += 90;
		}
		else
		{
			singleMovement->first = "rotateCounterClockwise";
			singleMovement->second += 90;
		}
	}
	if (singleMovement->second != 0)
		movementList->push_back(singleMovement);
}

void PathFinder::selectNextDestination()
{
	for (int i = 0; i < ARENA_X_SIZE; ++i)
		for (int j = 0; j < ARENA_Y_SIZE; ++j)
			if (_arena->getGridType(i, j) == UNEXPLORED)
			{
				this->_endX = i;
				this->_endY = j;
				return;
			}
}