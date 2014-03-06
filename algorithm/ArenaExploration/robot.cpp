#include "robot.h"	
#include "sensor.h"
#include "Arena.h"

#include <map>

using namespace std;

// prefix
DIRECTION& operator++(DIRECTION& orig)
{
	switch (orig)
	{
	case DOWN:
		orig = LEFT; break;
    case LEFT:
		orig = UP; break;
	case UP:
		orig = RIGHT; break;
	case RIGHT:
		orig = DOWN; break;
	}
    return orig;
}
DIRECTION& operator--(DIRECTION& orig)
{
	switch (orig)
	{
	case DOWN:
		orig = RIGHT; break;
    case LEFT:
		orig = DOWN; break;
	case UP:
		orig = LEFT; break;
	case RIGHT:
		orig = UP; break;
	}
    return orig;
}
// postfix
DIRECTION& operator++(DIRECTION& orig, int)
{
	DIRECTION &rVal = orig;
	++orig;
    return rVal;
}
DIRECTION& operator--(DIRECTION& orig, int)
{
	DIRECTION &rVal = orig;
	--orig;
    return rVal;
}

Robot::Robot(int x, int y, DIRECTION direction): 
	_posX(x), _posY(y), _direction(direction)
{
	// initialize sensor configuration
	Sensor* IRFrontL = new Sensor(IRFRONTL_ID, 0, Sensor::IR);
	Sensor* IRFrontR = new Sensor(IRFRONTR_ID, 0, Sensor::IR);
	Sensor* IRLeft = new Sensor(IRLEFT_ID, 90, Sensor::IR);
	Sensor* IRRight = new Sensor(IRRIGHT_ID, 270, Sensor::IR);
	Sensor* USFront = new Sensor(USFRONT_ID, 0, Sensor::US);
	Sensor* USSide = new Sensor(USSIDE_ID, 0, Sensor::US);
	_sensors.push_back(IRFrontL);
	_sensors.push_back(IRFrontR);
	_sensors.push_back(IRLeft);
	_sensors.push_back(IRRight);
	_sensors.push_back(USFront);
	_sensors.push_back(USSide);
	conn = new Connector();
}

Robot::~Robot(){}

void Robot::rotateClockwise(int deg)
{
#ifdef HARDWARE
	//Connector* conn = new Connector();
	if (!conn->sendRotationClockwise(90))
		return;
#endif
	++_direction;
}

void Robot::rotateCounterClockwise(int deg)
{
#ifdef HARDWARE
	//Connector* conn = new Connector();
	if (!conn->sendRotationCounterClockwise(90))
		return;
#endif
	--_direction;
}

void Robot::moveForward(int dist)
{
#ifdef HARDWARE
	// hardware control
	//Connector* conn = new Connector();
	if (!conn->sendMovement(10))
		return;
#endif
	// update location
	switch(_direction)
	{
		case DOWN:  // down
			++_posY; break;
		case LEFT: // left
			--_posX; break;
		case UP: // up
			--_posY; break;
		case RIGHT: // right
			++_posX; break;
	}
}

map<Sensor*, int>* Robot::getDataFromSensor()
{
	map<Sensor*, int>* returnData = new map<Sensor*, int>();
	map<int, int>* sensorData = conn->requestForSensorInformation();

	for (vector<Sensor*>::iterator iter = _sensors.begin(); iter != _sensors.end(); ++iter)
	{
		returnData->insert(pair<Sensor*, int>(*iter,  (*sensorData)[(*iter)->getID()]));
	}
	return returnData;
}

// collect sensor information and update the arena information
void Robot::senseEnvironment(Arena* arena, Arena* fullArena)
{
#ifndef HARDWARE
	map<Sensor*, int>* sensorData = getDataFromSensor();

	// go through each sensor
	// determine sensor location (x,y), and sensor direction (enum)
	for (map<Sensor*, int>::iterator iter = sensorData->begin(); iter != sensorData->end(); ++iter)
	{
		Sensor* current = iter->first;
		// determine sensor direction and X Y location
		DIRECTION dir = this->_direction;
		int sensorX, sensorY;
		// fall through cases to minimize code repetition
		switch (current->getID())
		{
		case 3: // IRLeft
			dir = this->_direction++;
		case 1: // IRFrontL
			dir = this->_direction;
			switch(this->_direction)
			{
			case DOWN:
				sensorX = _posX+1; sensorY = _posY+1;
				break;
			case LEFT:
				sensorX = _posX; sensorY = _posY+1;
				break;
			case UP:
				sensorX = _posX; sensorY = _posY;
				break;
			case RIGHT:
				sensorX = _posX+1; sensorY = _posY;
				break;
			}
			break;
		case 4: // IRRight
			this->_direction--;
		case 2: // IRFrontR
			dir = this->_direction;
			switch(this->_direction)
			{
			case DOWN:
				sensorX = _posX; sensorY = _posY+1;
				break;
			case LEFT:
				sensorX = _posX; sensorY = _posY;
				break;
			case UP:
				sensorX = _posX+1; sensorY = _posY;
				break;
			case RIGHT:
				sensorX = _posX+1; sensorY = _posY+1;
				break;
			}
			break;
		case 5: // USFrontR
			break;
		}
		openHorizon(arena, sensorX, sensorY, dir, iter->second);
	}
#else
	// open up surrounding areas
	int x = this->getPosX(), y = this->getPosY();
	arena->setGridType(x, y-1, fullArena->getGridType(x, y-1));
	arena->setGridType(x+1, y-1, fullArena->getGridType(x+1, y-1));
	arena->setGridType(x-1, y, fullArena->getGridType(x-1, y));
	arena->setGridType(x-1, y+1, fullArena->getGridType(x-1, y+1));
	arena->setGridType(x+2, y, fullArena->getGridType(x+2, y));
	arena->setGridType(x+2, y+1, fullArena->getGridType(x+2, y+1));
	arena->setGridType(x, y+2, fullArena->getGridType(x, y+2));
	arena->setGridType(x+1, y+2, fullArena->getGridType(x+1, y+2));
#endif
}

// open up unoccupied spaces
// whenever a range is given, there is an obstacle in front, otherwise 0 will be given
void Robot::openHorizon(Arena* arena, int x, int y, DIRECTION direction, int range)
{
	int i;
	bool isFree = false;
	if (range == -2)
	{
		// too near to detect
	}
	else if (range == -1)
	{
		// no item
		range = IR_RANGE;
		isFree = true;
	}
	for (i = 1; i <= range/10; ++i)
	{
		switch (direction)
		{
		case DOWN:
			arena->setGridType(x, y + i, UNOCCUPIED);
			break;
		case LEFT:
			arena->setGridType(x - i, y, UNOCCUPIED);
			break;
		case UP:
			arena->setGridType(x, y - i, UNOCCUPIED);
			break;
		case RIGHT:
			arena->setGridType(x + i, y, UNOCCUPIED);
			break;
		}
	}
	// obstacle case, when the range is reached.
	if (!isFree)
	{
		switch (direction)
		{
		case DOWN:
			arena->setGridType(x, y + i, OBSTACLE);
			break;
		case LEFT:
			arena->setGridType(x - i, y, OBSTACLE);
			break;
		case UP:
			arena->setGridType(x, y - i, OBSTACLE);
			break;
		case RIGHT:
			arena->setGridType(x + i, y, OBSTACLE);
			break;
		}
	}
}