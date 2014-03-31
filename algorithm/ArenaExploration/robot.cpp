#include "robot.h"	
#include "sensor.h"
#include "Arena.h"
#include "connector.h"
#include <sstream>
#include <map>
#include <Windows.h>

using namespace std;

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

Robot::Robot(int x, int y, DIRECTION direction, Connector* conn): 
	_posX(x), _posY(y), _direction(direction), _conn(conn)
{
	movementCount = 0;
	// initialize sensor configuration
	Sensor* IRFrontL = new Sensor(IRFRONTL_ID, 0, Sensor::IR);
	Sensor* IRFrontR = new Sensor(IRFRONTR_ID, 0, Sensor::IR);
	Sensor* IRFrontC = new Sensor(IRFRONTC_ID, 0, Sensor::IR);
	Sensor* IRRight = new Sensor(IRRIGHT_ID, 270, Sensor::IR);
	Sensor* USLeft = new Sensor(USLEFT_ID, 0, Sensor::US);
	Sensor* USRight = new Sensor(USRIGHT_ID, 0, Sensor::US);
	_sensors.push_back(IRFrontL);
	_sensors.push_back(IRFrontR);
	_sensors.push_back(IRFrontC);
	_sensors.push_back(IRRight);
	_sensors.push_back(USLeft);
	_sensors.push_back(USRight);
}
Robot::~Robot(){}

void Robot::rotateClockwise(int deg)
{
#ifdef HARDWARE
	if (!_conn->sendRotationClockwise(deg))
		return;
#endif
	for (int i = 0; i < deg/90; ++i)
		++_direction;
}
void Robot::rotateCounterClockwise(int deg)
{
#ifdef HARDWARE
	if (!_conn->sendRotationCounterClockwise(deg))
		return;
#endif
	for (int i = 0; i < deg/90; ++i)
		--_direction;
}
void Robot::moveForward(int dist)
{
#ifdef HARDWARE
	if (!_conn->sendMovement(dist))
		return;
#endif
	for (int i = 0; i < dist/10; ++i)
	{
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
}
map<Sensor*, int>* Robot::getDataFromSensor()
{
	map<int, int>* sensorData = _conn->requestForSensorInformation();
	return mapDataWithSensor(sensorData);
}
bool Robot::sendItselfAndArena(Arena* arena)
{
	ostringstream map, location;

	for(int i = 0; i < ARENA_Y_SIZE; i++)
		for(int j = 0; j < ARENA_X_SIZE; j++)
			map << arena->getGridType(j, i);
	location << this->_posX;
	location << ",";
	location << this->_posY;
	return _conn->sendMapInformationToAndroid(map.str(), location.str());
}

void Robot::rotateClockwiseAndSense(int deg, Arena* arena)
{
#ifdef HARDWARE
	map<int, int>* sensorData = _conn->sendRotationClockwiseAndSense(deg);
#endif
	++_direction;
	openArenaWithSensorData(mapDataWithSensor(sensorData), arena);
}
void Robot::rotateCounterClockwiseAndSense(int deg, Arena* arena)
{
#ifdef HARDWARE
	map<int, int>* sensorData = _conn->sendRotationCounterClockwiseAndSense(deg);
#endif
	--_direction;
	openArenaWithSensorData(mapDataWithSensor(sensorData), arena);
}
void Robot::moveForwardAndSense(int dist, Arena* arena)
{
#ifdef HARDWARE
	map<int, int>* sensorData = _conn->sendMovementAndSense(dist);
#endif
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
	openArenaWithSensorData(mapDataWithSensor(sensorData), arena);
}

void Robot::calibrateAtGoal()
{
	switch(this->_direction)
	{
	case DOWN:
		cout << endl << "calibrate: 5";
		_conn->calibrate(5);
		break;
	case RIGHT:
		cout << endl << "calibrate: 6";
		_conn->calibrate(6);
		break;
	default:
		return; // cannot calibrate
	}
}
void Robot::calibrateAtStart()
{
	switch(this->_direction)
	{
	case UP:
		cout << endl << "calibrate: 5";
		_conn->calibrate(5);
		break;
	case LEFT:
		cout << endl << "calibrate: 6";
		_conn->calibrate(6);
		break;
	default:
		return; // cannot calibrate
	}
}
void Robot::calibrateAtUpperRight()
{
	switch(this->_direction)
	{
	case RIGHT:
		cout << endl << "calibrate: 5";
		_conn->calibrate(5);
		break;
	case UP:
		cout << endl << "calibrate: 6";
		_conn->calibrate(6);
		break;
	default:
		return; // cannot calibrate
	}
}
void Robot::calibrateAtBottomLeft()
{
	switch(this->_direction)
	{
	case LEFT:
		cout << endl << "calibrate: 5";
		_conn->calibrate(5);
		break;
	case DOWN:
		cout << endl << "calibrate: 6";
		_conn->calibrate(6);
		break;
	default:
		return; // cannot calibrate
	}
}

map<Sensor*, int>* Robot::mapDataWithSensor(map<int, int>* data)
{
	map<Sensor*, int>* returnData = new map<Sensor*, int>();
	for (vector<Sensor*>::iterator iter = _sensors.begin(); iter != _sensors.end(); ++iter)
	{
		returnData->insert(pair<Sensor*, int>(*iter,  (*data)[(*iter)->getID()]));
	}
	return returnData;
}

void Robot::openArenaWithSensorData(map<Sensor*, int>* sensorData, Arena* arena)
{
	// Mark current robot location as UNOCCUPIED
	for (int i = -1; i < 2; ++i)
		for (int j = -1; j < 2; ++j)
			arena->setGridType(this->_posX + i, this->_posY + j, UNOCCUPIED);
	// determine sensor location (x,y), and sensor direction (enum)

	for (map<Sensor*, int>::iterator iter = sensorData->begin(); iter != sensorData->end(); ++iter)
	{
		DIRECTION sensorDir = this->_direction;
		int sensorX = this->_posX, sensorY = this->_posY, sensorID = iter->first->getID();
		cout << "current sensor: " << sensorX << ", " << sensorY << ", " << sensorID << ", " << sensorDir << ", " << iter->second << endl;
		// fall through cases to minimize code repetition
		switch (sensorID)
		{
		case IRFRONTL_ID: // IRFrontL
			switch(this->_direction)
			{
			case DOWN:
				sensorX = this->_posX+1; sensorY = this->_posY+1;
				break;
			case LEFT:
				sensorX = this->_posX-1; sensorY = this->_posY+1;
				break;
			case UP:
				sensorX = this->_posX-1; sensorY = this->_posY-1;
				break;
			case RIGHT:
				sensorX = this->_posX+1; sensorY = this->_posY-1;
				break;
			default:
				break;
			}
			openIRHorizon(arena, sensorX, sensorY, sensorDir, iter->second, sensorID);
			break;
		case IRFRONTC_ID:
			switch(this->_direction)
			{
			case DOWN:
				sensorX = this->_posX; sensorY = this->_posY+1;
				break;
			case LEFT:
				sensorX = this->_posX-1; sensorY = this->_posY;
				break;
			case UP:
				sensorX = this->_posX; sensorY = this->_posY-1;
				break;
			case RIGHT:
				sensorX = this->_posX+1; sensorY = this->_posY;
				break;
			default:
				break;
			}
			openIRHorizon(arena, sensorX, sensorY, sensorDir, iter->second, sensorID);
			break;
		case IRRIGHT_ID: // IRRight
			sensorDir++;
		case IRFRONTR_ID: // IRFrontR
			switch(this->_direction)
			{
			case DOWN:
				sensorX = this->_posX-1; sensorY = this->_posY+1;
				break;
			case LEFT:
				sensorX = this->_posX-1; sensorY = this->_posY-1;
				break;
			case UP:
				sensorX = this->_posX+1; sensorY = this->_posY-1;
				break;
			case RIGHT:
				sensorX = this->_posX+1; sensorY = this->_posY+1;
				break;
			default: 
				break;
			}
			openIRHorizon(arena, sensorX, sensorY, sensorDir, iter->second, sensorID);
			break;
		case USLEFT_ID:
			sensorDir--;
			switch(this->_direction)
			{
			case DOWN:
				sensorX = this->_posX+1; sensorY = this->_posY;
				break;
			case LEFT:
				sensorX = this->_posX; sensorY = this->_posY+1;
				break;
			case UP:
				sensorX = this->_posX-1; sensorY = this->_posY;
				break;
			case RIGHT:
				sensorX = this->_posX; sensorY = this->_posY-1;
				break;
			default: 
				break;
			}
			openUSHorizon(arena, sensorX, sensorY, sensorDir, iter->second);
			break;
		case USRIGHT_ID:  // right ur sensor
			sensorDir++;
			switch(this->_direction)
			{
			case DOWN:
				sensorX = this->_posX-1; sensorY = this->_posY;
				break;
			case LEFT:
				sensorX = this->_posX; sensorY = this->_posY-1;
				break;
			case UP:
				sensorX = this->_posX+1; sensorY = this->_posY;
				break;
			case RIGHT:
				sensorX = this->_posX; sensorY = this->_posY+1;
				break;
			default: 
				break;
			}
			openUSHorizon(arena, sensorX, sensorY, sensorDir, iter->second);
			break;
		default:
			break;
		}
	}
}

// collect sensor information and update the arena information
void Robot::senseEnvironment(Arena* arena, Arena* fullArena)
{
#ifdef HARDWARE
	map<Sensor*, int>* sensorData = getDataFromSensor();
	openArenaWithSensorData(sensorData, arena);
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
void Robot::openIRHorizon(Arena* arena, int x, int y, DIRECTION direction, int range, int sensorID)
{
	cout << "sensor adjusted information: " << x << ", " << y << ", " << direction << ", " << range << endl;
	int i;
	bool setObstacle = true;
	if (range == -1) // to far or too near to detect. Dont open for safety reason
	{
		range = SMALL_IR_RANGE;
		setObstacle = false;
	}
	// it will set one extra grid to free. If there is an obstacle, the later part will overwrite it.
	for (i = 0; i <= range/10; ++i)  // NOTE: DIFFERENT
	{
		switch (direction)
		{
		case DOWN: ++y; break;
		case LEFT: --x; break;
		case UP: --y; break;
		case RIGHT: ++x; break;
		}
		//cout << "setting grid: "<< x << ", " << y << " as UNOCCUPOiED" <<endl;
		// prevent overriding obstacle as free due to conflict sensor information
		if (arena->getGridType(x, y) == OBSTACLE)
			return;
		arena->setGridType(x, y, UNOCCUPIED);
	}
	if (setObstacle)
		arena->setGridType(x, y, OBSTACLE);
}

// US will only set free. It will not set obstacle.
void Robot::openUSHorizon(Arena* arena, int x, int y, DIRECTION direction, int range)
{
	cout << "sensor adjusted information: " << x << ", " << y << ", " << direction << ", " << range << endl;
	if (range == -1) // to far to detect
	{
		return;
	}
	// it will set one extra grid to free. If there is an obstacle, the later part will overwrite it.
	for (int i = 0; i < range/10; ++i)  // NOTE: DIFFERENT
	{
		switch (direction)
		{
		case DOWN: ++y; break;
		case LEFT: --x; break;
		case UP: --y; break;
		case RIGHT: ++x; break;
		}
		arena->setGridType(x, y, UNOCCUPIED);
	}
	if (range == 0)
	{
		switch (direction)
		{
		case DOWN: ++y; break;
		case LEFT: --x; break;
		case UP: --y; break;
		case RIGHT: ++x; break;
		}
		arena->setGridType(x, y, OBSTACLE);
	}
}

#ifdef GUI
bool Robot::moveForwardWithDisplay(int dist)
{
	if (movementCount == 0)
	{
		if (!_conn->sendMovementWithDisplay(dist))
			return false;
	}

	if (movementCount < dist/10)
	{
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
		cout <<movementCount;
		++movementCount;
		return false;
	}
	if (movementCount == dist/10)
	{
		char buf[1000];
		while(NetworkServices::receiveMessage(_conn->network->ConnectSocket, buf, 1000) <= 0)
			;
		movementCount = 0;
		return true;
	}
	return true;
}
#endif