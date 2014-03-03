#include "pathfinder.h"
#include <list>
#include <string>
#include <math.h>
#include <Windows.h>

using namespace std;

PathFinder::PathFinder(Robot* robot, Arena* arena)
{
	_robot = robot;
	_arena = arena;
	_endX = ARENA_END_X;
	_endY = ARENA_END_Y;
}

PathFinder::PathFinder(Robot* robot, Arena* arena, Arena* fullArena, MainWindow* window)
{
	_robot = robot;
	_arena = arena;
	_fullArena = fullArena;
	_window = window;
	_endX = ARENA_END_X;
	_endY = ARENA_END_Y;
}

PathFinder::~PathFinder()
{}

// highest level exploration
void PathFinder::explore()
{
	// explore the map
	while (!_arena->isExploredFully())
	{
#ifdef DEBUG
		cout << _robot->getPosX() << ", " << _robot->getPosY() << endl;
#endif
		while (_robot->getPosX() != _endX || _robot->getPosY() != _endY )
		{
			// if not reachable, check whether it is detectable, if not, break;
			if(!pointIsWalkable(_endX, _endY))
			{
				if(!substituteNewPoint(_endX, _endY))
					break;
			}
			// move to new place, sense the surrounding
			vector<Grid*> result = findPathBetween(_robot->getPosX(), _robot->getPosY(), _endX, _endY);
			vector<Grid*>::reverse_iterator i = result.rbegin();
			if (result.begin() != result.end()) // list not empty
				getRobotToMove(*i);
			_robot->senseEnvironment(_arena, _fullArena);
			Sleep(2000);
			_window->startButton.clicked();
			cout << "timeout";
		}
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
	}
	// go back to start point
	while (_robot->getPosX() != ARENA_START_X || _robot->getPosY() != ARENA_START_Y)
	{
		vector<Grid*> result = findPathBetween(_robot->getPosX(), _robot->getPosY(), ARENA_START_X, ARENA_START_Y);
		vector<Grid*>::reverse_iterator i = result.rbegin();
		if (result.begin() != result.end()) // list not empty
			getRobotToMove(*i);
	}
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

    while (n == 0 || (current != end && n < 50))
    {
		// stop if the point is unreachable
		// this part may have problem
		if (_arena->getGridType(endX, endY) == OBSTACLE)
			break;

        // Look for the smallest F value in the openList and make it the current point
        for (i = openList.begin(); i != openList.end(); ++ i)
        {
            if (i == openList.begin() || (*i)->heuristic <= current->heuristic)
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
	if (_arena->getGridType(x, y) == OBSTACLE)
		return false;
	if (_arena->getGridType(x + 1, y) == OBSTACLE)
		return false;
	if (_arena->getGridType(x, y + 1) == OBSTACLE)
		return false;
	if (_arena->getGridType(x + 1, y + 1) == OBSTACLE)
		return false;

	// border case
	if (x + 1 >= ARENA_X_SIZE || y + 1 >= ARENA_Y_SIZE)
		return false;
	
	// unexplored case
	/*if (_arena->getGridType(x, y) == UNEXPLORED)
		return false;
	if (_arena->getGridType(x + 1, y) == UNEXPLORED)
		return false;
	if (_arena->getGridType(x, y + 1) == UNEXPLORED)
		return false;
	if (_arena->getGridType(x + 1, y + 1) == UNEXPLORED)
		return false;*/

	return true;
}

bool PathFinder::substituteNewPoint(int x, int y)
{
	if (pointIsWalkable(_endX-2, _endY-1)) {_endX=_endX-2; _endY--; return true;}
	if (pointIsWalkable(_endX-2, _endY)) {_endX=_endX-2; return true;}
	if (pointIsWalkable(_endX-1, _endY+1)) {_endX--; _endY++; return true;}
	if (pointIsWalkable(_endX, _endY+1)) {_endY++; return true;}
	if (pointIsWalkable(_endX+1, _endY+1)) {_endX++; _endY++; return true;}
	if (pointIsWalkable(_endX+1, _endY-1)) {_endX++; _endY--; return true;}
	if (pointIsWalkable(_endX-1, _endY-2)) {_endX--; _endY = _endY-2; return true;}
	if (pointIsWalkable(_endX, _endY-2)) {_endY = _endY-2; return true;}
	return false;
}

// to be changed
void PathFinder::getRobotToMove(Grid* destination)
{
#ifdef HARDWARE

#else
	this->_robot->setLocation(destination->getX(), destination->getY());
#endif
}

void PathFinder::getMovementList(std::vector<Grid*>)
{

}

void PathFinder::selectNextDestination()
{
	for (int i = 0; i < ARENA_X_SIZE; ++i)
	{
		for (int j = 0; j < ARENA_Y_SIZE; ++j)
		{
			if (_arena->getGridType(i, j) == UNEXPLORED)
			{
				this->_endX = i;
				this->_endY = j;
				return;
			}
		}
	}
}